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
static StatusTypeDef spiTransmit(ad7708_dev *dev, uint8_t *data);

/****************** User Function Definitions *******************************/

StatusTypeDef ad7708_setNextOperation(ad7708_dev *dev,  SelectedReg reg, uint8_t rw)
{
    StatusTypeDef status;
    dev->commReg.bits.WEN = 0;
    dev->commReg.bits.RW = rw;
    dev->commReg.merged.addr = reg;

    setCS(0);
    status = spiTransmit(dev, &dev->commReg.byte); 
    setCS(1);

    return status;  
}



/****************** Static Function Definitions *******************************/

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
static StatusTypeDef spiTransmit(ad7708_dev *dev, uint8_t *data)
{
    StatusTypeDef status = AD7708_OK;

    if(HAL_SPI_Transmit(&dev->intf, data, 1, 1000) != AD7708_OK)
    {
        status = AD7708_ERROR;
    }

    return status;
}
