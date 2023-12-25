#ifndef __AD7708_DEFS_H__
#define __AD7708_DEFS_H__

#include "stdint.h"

/****************** Device Specifications *******************************/
#define AD7708_CS_Pin GPIO_PIN_4
#define AD7708_CS_GPIO_Port GPIOC
#define AD7708_INTF SPI1 // SPI'i büyük harfle yazmam problem yaratır mı?

#define AD7708_IMEOUT 500  // ms TODO: timeout değerini belirle
#define AD7708_SF_Rate 150 // Hz TODO: sample rate'i belirle

#define AD7708_CHCON 1 // Channel configuration !! IMPORTANT !! 0->8 or 1->10

/****************** Device Commands *******************************/
#define AD7708_Read 0x01U
#define AD7708_Write 0x00U

// pin-- 0: input 1: output
#define AD7708_IOPIN_Input 0x00U
#define AD7708_IOPIN_Output 0x01U

typedef void (*ad7708_delay_fptr_t)(uint32_t period);

/****************** Device Enums*******************************/

/*Register Selection Table*/
/*  A3 A2 A1 A0
 *   0  0  0  0  Communications Register during a Write Operation
 *   0  0  0  0  Status Register during a Read Operation
 *   0  0  0  1  Mode Register
 *   0  0  1  0  ADC Control Register
 *   0  0  1  1  Filter Register
 *   0  1  0  0  ADC Data Register
 *   0  1  0  1  ADC Offset Register
 *   0  1  1  0  ADC Gain Register
 *   0  1  1  1  I/O Control Register
 *   1  0  0  0  Undefined
 *   1  0  0  1  Undefined
 *   1  0  1  0  Undefined
 *   1  0  1  1  Undefined
 *   1  1  0  0  Test 1 Register
 *   1  1  0  1  Test 2 Register
 *   1  1  1  0  Undefined
 *   1  1  1  1  ID Register
 */
typedef enum
{
    COMM_REG = 0x00U,
    STATUS_REG = 0x00U, // Is this a problem?
    MODE_REG = 0x01U,
    CONTROL_REG = 0x02U,
    FILTER_REG = 0x03U,
    DATA_REG = 0x04U,
    OFFSET_REG = 0x05U,
    GAIN_REG = 0x06U,
    IO_CONTROL_REG = 0x07U,
    Undefined = 0x08U,
    Undefined1 = 0x09U,
    Undefined2 = 0x0AU,
    Undefined3 = 0x0BU,
    TEST1_REG = 0x0CU,
    TEST2_REG = 0x0DU,
    Undefined4 = 0x0EU,
    ID_REG = 0x0FU
} SelectedReg;

typedef enum
{
    AD7708_OK = 0x00U,
    AD7708_ERROR = 0x01U,
    AD7708_BUSY = 0x02U,
    AD7708_TIMEOUT = 0x03U
} StatusTypeDef;

/*ADC Input Range Table
 *
 *RN2 RN1 RN0 ADC Input Range (VREF = 2.5 V)
 *0   0   0   ±20 mV
 *0   0   1   ±40 mV
 *0   1   0   ±80 mV
 *0   1   1   ±160 mV
 *1   0   0   ±320 mV
 *1   0   1   ±640 mV
 *1   1   0   ±1.28 V
 *1   1   1   ±2.56 V
 */

typedef enum
{
    AD7708_Range_20mV = 0x00U,
    AD7708_Range_40mV = 0x01U,
    AD7708_Range_80mV = 0x02U,
    AD7708_Range_160mV = 0x03U,
    AD7708_Range_320mV = 0x04U,
    AD7708_Range_640mV = 0x05U,
    AD7708_Range_1p28V = 0x06U,
    AD7708_Range_2p56V = 0x07U
} AD7708_Range;

#if AD7708_CHCON == 0
/*8 Channel Configuration Table [CHCON = 0]
 *
 * CH3 CH2 CH1 CH0 (+)      (-)
 * 0   0   0   0   AIN1     AINCOM
 * 0   0   0   1   AIN2     AINCOM
 * 0   0   1   0   AIN3     AINCOM
 * 0   0   1   1   AIN4     AINCOM
 * 0   1   0   0   AIN5     AINCOM
 * 0   1   0   1   AIN6     AINCOM
 * 0   1   1   0   AIN7     AINCOM
 * 0   1   1   1   AIN8     AINCOM
 * 1   0   0   0   AIN1     AIN2
 * 1   0   0   1   AIN3     AIN4
 * 1   0   1   0   AIN5     AIN6
 * 1   0   1   1   AIN7     AIN8
 * 1   1   0   0   AIN2     AIN2
 * 1   1   0   1   AINCOM   AINCOM
 * 1   1   1   0   REFIN(+) REFIN(–)
 * 1   1   1   1   OPEN     OPEN
 */

typedef enum
{
    ADC_Channel_1 = 0x00U,
    ADC_Channel_2 = 0x01U,
    ADC_Channel_3 = 0x02U,
    ADC_Channel_4 = 0x03U,
    ADC_Channel_5 = 0x04U,
    ADC_Channel_6 = 0x05U,
    ADC_Channel_7 = 0x06U,
    ADC_Channel_8 = 0x07U,
    ADC_Channel_1_2 = 0x08U,
    ADC_Channel_3_4 = 0x09U,
    ADC_Channel_5_6 = 0x0AU,
    ADC_Channel_7_8 = 0x0BU,
    ADC_Channel_2_2 = 0x0CU,
    ADC_Channel_COM = 0x0DU,
    ADC_Channel_REF = 0x0EU,
    ADC_Channel_OPEN = 0x0FU
} AD7708_Channel;

#elif AD7708_CHCON == 1
/*10 Channel Configuration Table [CHCON = 1]
 *
 * CH3 CH2 CH1 CH0 (+)      (-)
 * 0   0   0   0   AIN1     AINCOM
 * 0   0   0   1   AIN2     AINCOM
 * 0   0   1   0   AIN3     AINCOM
 * 0   0   1   1   AIN4     AINCOM
 * 0   1   0   0   AIN5     AINCOM
 * 0   1   0   1   AIN6     AINCOM
 * 0   1   1   0   AIN7     AINCOM
 * 0   1   1   1   AIN8     AINCOM
 * 1   0   0   0   AIN1     AIN2
 * 1   0   0   1   AIN3     AIN4
 * 1   0   1   0   AIN5     AIN6
 * 1   0   1   1   AIN7     AIN8
 * 1   1   0   0   AIN9     AIN10
 * 1   1   0   1   AINCOM   AINCOM
 * 1   1   1   0   AIN9     AINCOM
 * 1   1   1   1   AIN10    AINCOM
 */

typedef enum
{
    AD7708_Channel_1 = 0x00U,
    AD7708_Channel_2 = 0x01U,
    AD7708_Channel_3 = 0x02U,
    AD7708_Channel_4 = 0x03U,
    AD7708_Channel_5 = 0x04U,
    AD7708_Channel_6 = 0x05U,
    AD7708_Channel_7 = 0x06U,
    AD7708_Channel_8 = 0x07U,
    AD7708_Channel_1_2 = 0x08U,
    AD7708_Channel_3_4 = 0x09U,
    AD7708_Channel_5_6 = 0x0AU,
    AD7708_Channel_7_8 = 0x0BU,
    AD7708_Channel_9_10 = 0x0CU,
    AD7708_Channel_COM = 0x0DU,
    AD7708_Channel_9_COM = 0x0EU,
    AD7708_Channel_10_COM = 0x0FU
} AD7708_Channel;
#endif

typedef enum
{
    AD7708_Bipolar = 0x00U,
    AD7708_Unipolar = 0x01U
} AD7708_Polarity;

/*
 * MD2 MD1 MD0
 * 0   0   0    Power-Down Mode (Power-On Default)
 * 0   0   1   Idle Mode
 * 0   1   0   Single Conversion Mode
 * 0   1   1   Continuous Conversion
 * 1   0   0   Internal Zero-Scale Calibration
 * 1   0   1   Internal Full-Scale Calibration
 * 1   1   0   System Zero-Scale Calibration
 * 1   1   1   System Full-Scale Calibration
 */

typedef enum
{
    AD7708_PowerDown = 0x00U,
    AD7708_Idle = 0x01U,
    AD7708_SingleConversion = 0x02U,
    AD7708_ContinuousConversion = 0x03U,
    AD7708_InternalZeroCalibration = 0x04U,
    AD7708_InternalFullCalibration = 0x05U,
    AD7708_SystemZeroCalibration = 0x06U,
    AD7708_SystemFullCalibration = 0x07U
} AD7708_Mode;

/****************** Device Bit Fields*******************************/
typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t A0 : 1;
        uint8_t A1 : 1;
        uint8_t A2 : 1;
        uint8_t A3 : 1;
        uint8_t z1 : 1;  //
        uint8_t z2 : 1;  // must be 0
        uint8_t RW : 1;  // 0: write, 1: read
        uint8_t WEN : 1; // 0: enable, 1: disable; must be 0 for upload communication reg
    } bits;
    struct
    {
        uint8_t addr : 4; // Which register to access
        uint8_t zeros : 2;
        uint8_t : 2;
    } merged;
} CommReg;

typedef union
{
    uint8_t byte;
    struct bits
    {
        uint8_t lock : 1; // PLL Lock Status Bit
        uint8_t none1 : 1;
        uint8_t none2 : 1;
        uint8_t err : 1; // Set to indicate that the result written to the ADC data register has been clamped to all zeros or all ones
        uint8_t none3 : 1;
        uint8_t cal : 1; // Set to indicate completion of calibration
        uint8_t none4 : 1;
        uint8_t rdy : 1; // Set when data is ready to be read
    };
} statusReg;

typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t md0 : 1;
        uint8_t md1 : 1;
        uint8_t md2 : 1;
        uint8_t oscpd : 1;  // 0: Ossilator not shut off in stnadby mode, 1: Ossilator shut off in stnadby mode
        uint8_t chcon : 1;  // 0: two ref with 8 channels, 1: one ref with 10 channels
        uint8_t refsel : 1; // 0: REFIN1 active, 1: REFIN2 active
        uint8_t negbuf : 1; // 0: pseudo-differential mod (connect AINCOM to AGND), 1: Not using AINCOM
        uint8_t chop : 1;   // 0: chop disable 1:chop enable
    } bits;
    struct
    {
        uint8_t mode : 3; // Mode bits
        uint8_t : 5;
    } merged;

} ModeReg;

typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t rn0 : 1;
        uint8_t rn1 : 1;
        uint8_t rn2 : 1;
        uint8_t ub : 1; // 0: bipolar, 1: unipolar
        uint8_t ch0 : 1;
        uint8_t ch1 : 1;
        uint8_t ch2 : 1;
        uint8_t ch3 : 1;
    } bits;
    struct
    {
        uint8_t range : 3; // Range bits
        uint8_t : 1;
        uint8_t channelConfig : 4; // Channel configuration bits
    } merged;
} ControlReg;

typedef union
{
    uint8_t byte;
    struct bits
    {
        uint8_t sf0 : 1;
        uint8_t sf1 : 1;
        uint8_t sf2 : 1;
        uint8_t sf3 : 1;
        uint8_t sf4 : 1;
        uint8_t sf5 : 1;
        uint8_t sf6 : 1;
        uint8_t sf7 : 1;
    };
} FilterReg;

typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t p1dat : 1; // Digital I/O Port (P1) Data Bit--The value written-> output of pin(in output mode)
        uint8_t p2dat : 1; // Digital I/O Port (P2) Data Bit--The value written-> output of pin(in output mode)
        uint8_t z3 : 1;    // must be 0
        uint8_t z4 : 1;    // must be 0
        uint8_t p1dir : 1; // P1 pin-- 0: input 1: output
        uint8_t p2dir : 1; // P2 pin-- 0: input 1: output
        uint8_t z5 : 1;    // must be 0
        uint8_t z6 : 1;    // must be 0
    } bits;
    struct
    {
        uint8_t : 2;
        uint8_t zeros1 : 2;
        uint8_t : 2;
        uint8_t zeros2 : 2;
    } merged;

} IOControlReg;

/*! @name API device structure */
typedef struct
{
    uint8_t intf;
    CommReg commReg;
    IOControlReg ioControlReg;
    FilterReg filterReg;
    ControlReg controlReg;
    ModeReg modeReg;
    ad7708_delay_fptr_t delay_ms;
} ad7708_dev;

#endif