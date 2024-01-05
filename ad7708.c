#include "ad7708.h"

/********************** Static function declarations ************************/

/*!
 * @brief Set the CS pin to the desired state
 * @param[in] state - Desired state of the CS pin
 * @return void
 */
static void setCS(uint8_t state);

/*!
 * @brief Transmit data to the AD7708
 * @param[in] dev - Pointer to the device structure
 * @param[in] data - Pointer to the data to be transmitted
 * @return 0: case of success, error code otherwise.
 */
static StatusTypeDef spiTransmit(ad7708_dev* dev, uint8_t* data, uint16_t len);

/*!
 * @brief Set the next operation to be performed by the AD7708
 *
 * @param[in] dev - Pointer to the device structure
 * @param[in] reg - Register to be accessed
 * @param[in] rw - Read or Write operation
 * @param[in] len - Length of the data to be transmitted or received
 *
 * @return 0: case of success, 1: error code otherwise.
 */
static StatusTypeDef setNextOperation(ad7708_dev* dev, SelectedReg reg, uint8_t rw, uint16_t len);

/*!
 * @brief Recieve data from the AD7708
 * @param[in] dev - Pointer to the device structure
 * @param[in] data - Pointer to the data to be recieved
 * @return 0: case of success, error code otherwise.
 */
static StatusTypeDef spiRecieve(ad7708_dev* dev, uint16_t* data, uint16_t len);

/*!
 * @brief Read data from the AD7708 registers
 * @param[in] dev - Pointer to the device structure
 * @param[in] reg - Register to be accessed
 * @param[in] data - Pointer to the data to be recieved
 * @param[in] len - Length of the data to be recieved
 * @return 0: case of success, error code otherwise.
 */
static StatusTypeDef ad7708_readReg(ad7708_dev* dev, SelectedReg reg, uint16_t* data, uint16_t len);

/****************** User Function Definitions *******************************/

/*!
 * @brief Are you there AD7708?
 */
uint8_t ad7708_areYouThere(ad7708_dev* dev) {
    uint8_t status;
    uint8_t id;

    status = ad7708_readReg(dev, ID_REG, &id, 1);

    if ((id >> 4) == AD7708_ID) { return 0; }
    else { return 1; }
}


StatusTypeDef ad7780_init(ad7708_dev* dev)
{
    StatusTypeDef status = AD7708_OK;
    dev->id = AD7708_ID;
    dev->intf = AD7708_INTF;
    dev->delay_ms = HAL_Delay; // delayOS if used in freeRTOS

    status = ioConfig(dev, AD7708_IOPIN_Input, AD7708_IOPIN_Output);
    status = sfRateConfig(dev, AD7708_SF_Rate);
    status = channelConfig(dev, AD7708_Channel_2, AD7708_Range_20mV, AD7708_Unipolar);
    status = modeConfig(dev, AD7708_SingleConversion);
}

/*!
 * @brief Configure the AD7708 modes
 */
StatusTypeDef ad7708_modeConfig(ad7708_dev* dev, AD7708_Mode mode, uint8_t chcon, uint8_t refsel, uint8_t chop, uint8_t negbuf, uint8_t oscpd)
{
    StatusTypeDef status = AD7708_OK;

    dev->modeReg.merged.mode = mode;
    dev->modeReg.bits.chcon = chcon;
    dev->modeReg.bits.refsel = refsel;
    dev->modeReg.bits.chop = chop;
    dev->modeReg.bits.negbuf = negbuf;
    dev->modeReg.bits.oscpd = oscpd;

    status = setNextOperation(dev, CONTROL_REG, AD7708_Write, 1);

    setCS(0);
    status = spiTransmit(dev, &dev->controlReg.byte, 1);
    setCS(1);

    return status;
}

/*!
 * @brief Configure the AD7708 channel, range and polarity
 */
StatusTypeDef ad7708_channelConfig(ad7708_dev* dev, AD7708_Channel channel, AD7708_Range range, AD7708_Polarity polarity)
{
    StatusTypeDef status = AD7708_OK;

    dev->controlReg.merged.channelConfig = channel;
    dev->controlReg.merged.range = range;
    dev->controlReg.bits.ub = polarity;

    status = setNextOperation(dev, CONTROL_REG, AD7708_Write, 1);

    setCS(0);
    status = spiTransmit(dev, &dev->controlReg.byte, 1);
    setCS(1);

    return status;
}

/*!
 * @brief Configure the AD7708 SF rate
 */
StatusTypeDef ad7708_sfRateConfig(ad7708_dev* dev, uint8_t sfRate)
{
    StatusTypeDef status = AD7708_OK;

    dev->filterReg.byte = sfRate;

    status = setNextOperation(dev, FILTER_REG, AD7708_Write, 1);

    setCS(0);
    status = spiTransmit(dev, &dev->filterReg.byte, 1);
    setCS(1);

    return status;
}

/*!
 * @brief Configure the AD7708 IO pins
 */
StatusTypeDef ad7708_ioConfig(ad7708_dev* dev, uint8_t pin1State, uint8_t pin2State)
{
    StatusTypeDef status = AD7708_OK;

    dev->ioControlReg.bits.p1dir = pin1State;
    dev->ioControlReg.bits.p2dir = pin2State; // TODO: Define io read and write functions
    dev->ioControlReg.merged.zeros1 = 0;      // TODO: These bits are default 0, but should be set to 0 anyway?????
    dev->ioControlReg.merged.zeros2 = 0;

    status = setNextOperation(dev, IO_CONTROL_REG, AD7708_Write, 1);

    setCS(0);
    status = spiTransmit(dev, &dev->ioControlReg.byte, 1); // 1 byte ???
    setCS(1);

    return status;
}

/*!
 * @brief Calibrate the ad7708 selected channel
 */
StatusTypeDef ad7708_calibrate(ad7708_dev* dev, AD7708_Channel channel)
{
    StatusTypeDef status = AD7708_OK;
    status = ad7708_channelConfig(dev, channel, AD7708_Range_20mV, AD7708_Unipolar);
    status = ad7708_modeConfig(dev, AD7708_InternalZeroCalibration); // TODO: Sytem or Internal calibration??

    if (ad7708_waitForIdle(dev, 200) == AD7708_OK)
    {
        status = ad7708_modeConfig(dev, AD7708_InternalFullCalibration);
        if (ad7708_waitForIdle(dev, 200) == AD7708_OK) { return AD7708_OK; }
        else { return AD7708_TIMEOUT; }

    }
    else
    {
        return AD7708_TIMEOUT;
    }

    return status;
}

/*!
* @brief AD7708 start continuous conversion mode
*/
StatusTypeDef ad7708_startContinuousConversion(ad7708_dev* dev) {
    uint8_t status;
    status = ad7708_modeConfig(dev, AD7708_ContinuousConversion, AD7708_CHCON, AD7708_REFSEL, AD7708_CHOP, AD7708_NEGBUF, AD7708_OSCPD);
    //status = ad7708_channelConfig(dev, channel, AD7708_Range_20mV, AD7708_Unipolar); --->> channel config should be done before mode config ??

    return status;
}

/*!
* @brief Read 16 bit data from the AD7708 data register
*/
uint16_t ad7708_readData(ad7708_dev* dev, uint16_t* data) {
    uint8_t status;
    status = setNextOperation(dev, DATA_REG, AD7708_Read, 1);

    setCS(0);
    status = spiRecieve(dev, data, 2);
    setCS(1);

    return data;
}
/****************** Static Function Definitions *******************************/

/*!
 * @brief Set the next operation to be performed by the AD7708
 */
static StatusTypeDef setNextOperation(ad7708_dev* dev, SelectedReg reg, uint8_t rw, uint16_t len)
{
    StatusTypeDef status;
    dev->commReg.bits.WEN = 0;
    dev->commReg.bits.RW = rw;
    dev->commReg.merged.zeros = 0; // must be 0 !!!!!!!!
    dev->commReg.merged.addr = reg;

    setCS(0);
    status = spiTransmit(dev, &dev->commReg.byte, len);
    setCS(1);
    dev->commReg.bits.WEN = 1;

    return status;
}

/*!
 * @brief Set the CS pin to the desired state
 */
static void setCS(uint8_t state)
{
    HAL_GPIO_WritePin(AD7708_CS_GPIO_Port, AD7708_CS_Pin, state);
}

/*!
 * @brief Transmit data to the AD7708
 */
static StatusTypeDef spiTransmit(ad7708_dev* dev, uint8_t* data, uint16_t len)
{
    StatusTypeDef status = AD7708_OK;

    if (HAL_SPI_Transmit(&dev->intf, data, len, AD7708_TIMEOUT) != AD7708_OK)
    {
        status = AD7708_ERROR;
    }

    return status;
}

/*!
 * @brief Recieve data from the AD7708
 */
static StatusTypeDef spiRecieve(ad7708_dev* dev, uint16_t* data, uint16_t len) // is there any problem that i pass 8 bit data to 16 bit pointer?
{
    StatusTypeDef status = AD7708_OK;

    if (HAL_SPI_Receive(&dev->intf, data, len, AD7708_TIMEOUT) != AD7708_OK)
    {
        status = AD7708_ERROR;
    }

    return status;
}

/*!
 * @brief Read data from the AD7708 registers
 */
static StatusTypeDef ad7708_readReg(ad7708_dev* dev, SelectedReg reg, uint16_t* data, uint16_t len)
{
    StatusTypeDef status = AD7708_OK;

    status = setNextOperation(dev, reg, AD7708_Read, 1);

    setCS(0);
    status = spiRecieve(dev, data, len);
    setCS(1);

    return status;
}

/*!
 * @brief Wait for the AD7708 to be idle mode
 */
static StatusTypeDef ad7708_waitForIdle(ad7708_dev* dev, uint16_t timeout_ms)
{
    uint32_t tickstart = HAL_GetTick();

    while (1)
    {
        ad7708_readReg(dev, MODE_REG, &dev->modeReg.byte, 1);

        if (dev->modeReg.merged.mode == AD7708_Idle)
        {
            return AD7708_OK;
        }
        if ((HAL_GetTick() - tickstart) >= timeout_ms || (HAL_GetTick() - tickstart) >= AD7708_MAX_TIMEOUT) // OR kısmını eklemek mantıklı mı??
        {
            return AD7708_TIMEOUT;
        }
    }

}