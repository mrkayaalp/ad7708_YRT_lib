#ifndef __AD7708_H__
#define __AD7708_H__

#include "ad7708_defs.h"


/*!
* @brief Set the next operation to be performed by the AD7708
*
* @param[in] dev - Pointer to the device structure
* @param[in] reg - Register to be accessed
* @param[in] rw - Read or Write operation
*
* @return 0: case of success, 1: error code otherwise.
*/
StatusTypeDef ad7708setNextOperation(ad7708_dev *dev,  SelectedReg reg, uint8_t rw);

#endif