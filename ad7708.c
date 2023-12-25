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
static StatusTypeDef spiTransmit(ad7708_dev *dev, uint8_t *data, uint16_t len);

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
static StatusTypeDef setNextOperation(ad7708_dev *dev, SelectedReg reg, uint8_t rw, uint16_t len);









/****************** User Function Definitions *******************************/

StatusTypeDef ad7780_init(ad7708_dev *dev)
{
    StatusTypeDef status = AD7708_OK;
    dev->intf = AD7708_INTF;
    dev->delay_ms = HAL_Delay; // delayOS if used in freeRTOS

    status = ioConfig(dev, AD7708_IOPIN_Input, AD7708_IOPIN_Output);
    status = sfRateConfig(dev, AD7708_SF_Rate);
    status = channelConfig(dev, AD7708_Channel_2, AD7708_Range_20mV, AD7708_Unipolar);
    status = modeConfig(dev, AD7708_SingleConversion);
}

/*!
 * @brief Configure the AD7708 mode
 */
StatusTypeDef modeConfig(ad7708_dev *dev, AD7708_Mode mode)
{
    StatusTypeDef status = AD7708_OK;

    dev->modeReg.merged.mode = mode;
    dev->modeReg.bits.chcon = AD7708_CHCON;
    dev->modeReg.bits.refsel = AD7708_REFSEL;
    dev->modeReg.bits.chop = AD7708_CHOP;
    dev->modeReg.bits.negbuf = AD7708_NEGBUF; // TO:DO define these
    dev->modeReg.bits.oscpd = AD7708_OSCPD;

    status = setNextOperation(dev, CONTROL_REG, AD7708_Write, 1);

    setCS(0);
    status = spiTransmit(dev, &dev->controlReg.byte, 1);
    setCS(1);

    return status;
}

/*!
 * @brief Configure the AD7708 channel, range and polarity
 */
StatusTypeDef channelConfig(ad7708_dev *dev, AD7708_Channel channel, AD7708_Range range, AD7708_Polarity polarity)
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
StatusTypeDef sfRateConfig(ad7708_dev *dev, uint8_t sfRate)
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
StatusTypeDef ioConfig(ad7708_dev *dev, uint8_t pin1State, uint8_t pin2State)
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




/****************** Static Function Definitions *******************************/

/*!
 * @brief Set the next operation to be performed by the AD7708
 */
static StatusTypeDef setNextOperation(ad7708_dev *dev, SelectedReg reg, uint8_t rw, uint16_t len)
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
static StatusTypeDef spiTransmit(ad7708_dev *dev, uint8_t *data, uint16_t len)
{
    StatusTypeDef status = AD7708_OK;

    if (HAL_SPI_Transmit(&dev->intf, data, len, AD7708_TIMEOUT) != AD7708_OK)
    {
        status = AD7708_ERROR;
    }

    return status;
}






