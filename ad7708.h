#ifndef __AD7708_H__
#define __AD7708_H__

#include "ad7708_defs.h"

/*!
*
*/
StatusTypeDef ad7780_init(ad7708_dev *dev);

/*!
 * @brief Configure the AD7708 mode
 * @param[in] dev - Pointer to the device structure
 * @param[in] mode -  desired mode
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_modeConfig(ad7708_dev *dev, AD7708_Mode mode);

/*!
 * @brief Configure the AD7708 channel, range and polarity
 * @param[in] dev - Pointer to the device structure
 * @param[in] channel - Desired channel
 * @param[in] range - Desired range
 * @param[in] polarity - Desired polarity
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_channelConfig(ad7708_dev *dev, AD7708_Channel channel, AD7708_Range range, AD7708_Polarity polarity);

/*!
 * @brief Configure the AD7708 SF rate
 * @param[in] dev - Pointer to the device structure
 * @param[in] sfRate - Desired SF rate
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_sfRateConfig(ad7708_dev *dev, uint8_t sfRate);

/*!
 * @brief Configure the AD7708 IO pins
 * @param[in] dev - Pointer to the device structure
 * @param[in] pin1State - Direction of IO pin 1
 * @param[in] pin2State - Direction of IO pin 2
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_ioConfig(ad7708_dev *dev, uint8_t pin1State, uint8_t pin2State);

/*!
* @brief Calibrate the ad7708 selected channel
* @param[in] dev - Pointer to the device structure
* @param[in] channel - Desired channel
* @return 0: case of success, error code otherwise.
* @note Use this function after configurate the selected channel
*/
StatusTypeDef ad7708_calibrate(ad7708_dev *dev, AD7708_Channel channel);

#endif