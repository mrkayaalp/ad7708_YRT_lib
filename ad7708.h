#ifndef __AD7708_H__
#define __AD7708_H__

#include "ad7708_defs.h"

/*!
*
*/
StatusTypeDef ad7780_init(ad7708_dev* dev);

/*!
 * @brief Configure the AD7708 modes
 * @param[in] dev - Pointer to the device structure
 * @param[in] mode -  desired mode
 * @param[in] chcon - Channel configuration 0:8ch or 1:10ch
 * @param[in] refsel - 0: REFIN1 active, 1: REFIN2 active
 * @param[in] chop - 0: chop disable 1:chop enable
 * @param[in] negbuf - 0: pseudo-differential mod, 1: Not using AINCOM
 * @param[in] oscpd -  0: Ossilator not shut off in stnadby mode, 1: Ossilator shut off in stnadby mode
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_modeConfig(ad7708_dev* dev, AD7708_Mode mode, uint8_t chcon, uint8_t refsel, uint8_t chop, uint8_t negbuf, uint8_t oscpd);

/*!
 * @brief Configure the AD7708 channel, range and polarity
 * @param[in] dev - Pointer to the device structure
 * @param[in] channel - Desired channel
 * @param[in] range - Desired range
 * @param[in] polarity - Desired polarity
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_channelConfig(ad7708_dev* dev, AD7708_Channel channel, AD7708_Range range, AD7708_Polarity polarity);

/*!
 * @brief Configure the AD7708 SF rate
 * @param[in] dev - Pointer to the device structure
 * @param[in] sfRate - Desired SF rate
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_sfRateConfig(ad7708_dev* dev, uint8_t sfRate);

/*!
 * @brief Configure the AD7708 IO pins
 * @param[in] dev - Pointer to the device structure
 * @param[in] pin1State - Direction of IO pin 1
 * @param[in] pin2State - Direction of IO pin 2
 * @return 0: case of success, error code otherwise.
 */
StatusTypeDef ad7708_ioConfig(ad7708_dev* dev, uint8_t pin1State, uint8_t pin2State);

/*!
* @brief Calibrate the ad7708 selected channel
* @param[in] dev - Pointer to the device structure
* @param[in] channel - Desired channel
* @return 0: case of success, error code otherwise.
* @note Use this function after configurate the selected channel
*/
StatusTypeDef ad7708_calibrate(ad7708_dev* dev, AD7708_Channel channel);

/*!
* @brief AD7708 start continuous conversion mode
* @param[in] dev - Pointer to the device structure
* @return 0: case of success, error code otherwise.
*/
StatusTypeDef ad7708_startContinuousConversion(ad7708_dev* dev);

/*!
* @brief Read 16 bit data from the AD7708 data register
* @param[in] dev - Pointer to the device structure
* @param[out] data - Pointer to the data buffer
* @return 0: case of success, error code otherwise.
*/
uint16_t ad7708_readData(ad7708_dev* dev, uint16_t* data);

/*!
* @brief Are you there AD7708?
* @param[in] dev - Pointer to the device structure
* @return 0: yes im here 1: silence
*/
uint8_t ad7708_areYouThere(ad7708_dev* dev);

#endif