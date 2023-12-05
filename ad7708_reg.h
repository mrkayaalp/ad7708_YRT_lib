#ifndef __AD7708_REG_H__
#define __AD7708_REG_H__

#include "stdint.h"

typedef union CommReg{  
    uint8_t byte;
    struct 
    {
        uint8_t A0    :1;
        uint8_t A1    :1;    
        uint8_t A2    :1;    
        uint8_t A3    :1;    
        uint8_t z1    :1; //   
        uint8_t z2    :1; // must be 0   
        uint8_t RW    :1; // 0: write, 1: read   
        uint8_t WEN   :1; // 0: enable, 1: disable; must be 0 for upload communication reg  
    }bits;
    struct merged
    {
        uint8_t addr  :4; // Which register to access
        uint8_t       :4;
    };
};

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

typedef union statusReg{  
    uint8_t byte;
    struct bits
    {
        uint8_t lock   :1; // PLL Lock Status Bit
        uint8_t none1  :1;    
        uint8_t none2  :1;    
        uint8_t err    :1; // Set to indicate that the result written to the ADC data register has been clamped to all zeros or all ones   
        uint8_t none3  :1; 
        uint8_t cal    :1; // Set to indicate completion of calibration   
        uint8_t none4  :1;   
        uint8_t rdy    :1; // Set when data is ready to be read 
    };
};

typedef union modeReg{  
    uint8_t byte;
    struct bits
    {
        uint8_t md0   :1; 
        uint8_t md1   :1;    
        uint8_t md2   :1;    
        uint8_t oscpd  :1; // 0: Ossilator not shut off in stnadby mode, 1: Ossilator shut off in stnadby mode
        uint8_t chcon  :1; // 0: two ref with 8 channels, 1: one ref with 10 channels
        uint8_t refsel :1; // 0: REFIN1 active, 1: REFIN2 active  
        uint8_t negbuf :1; // 0: pseudo-differential mod (connect AINCOM to AGND), 1: Not using AINCOM
        uint8_t chop   :1; // 0: chop disable 1:chop enable
    };
    struct merged
    {
        uint8_t mode   :3; // Mode bits
        uint8_t        :5;
    };
    
};

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

typedef union controlReg
{
    uint8_t byte;
    struct bits
    {
        uint8_t rn0             :1; 
        uint8_t rn1             :1;
        uint8_t rn2             :1;
        uint8_t ub              :1; // 0: bipolar, 1: unipolar
        uint8_t ch0             :1;        
        uint8_t ch1             :1;        
        uint8_t ch2             :1;        
        uint8_t ch3             :1;        
    };
    struct merged
    {
        uint8_t channelConfig   :3; // Channel configuration bits
        uint8_t                 :1;
        uint8_t range           :4; // Range bits
    };   
};

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

typedef union filterReg
{
    uint8_t byte;
    struct bits
    {
        uint8_t sf0     :1;
        uint8_t sf1     :1;
        uint8_t sf2     :1;
        uint8_t sf3     :1;
        uint8_t sf4     :1;
        uint8_t sf5     :1;
        uint8_t sf6     :1;
        uint8_t sf7     :1;
    };
};

typedef union ioControlReg
{
    uint8_t byte;
    struct bits
    {
        uint8_t p1dat   :1; // Digital I/O Port (P1) Data Bit--The value written-> output of pin(in output mode)
        uint8_t p2dat   :1; // Digital I/O Port (P2) Data Bit--The value written-> output of pin(in output mode)
        uint8_t z3      :1; // must be 0 
        uint8_t z4      :1; // must be 0 
        uint8_t p1dir   :1; // P1 pin-- 0: input 1: output
        uint8_t p2dir   :1; // P2 pin-- 0: input 1: output
        uint8_t z5      :1; // must be 0 
        uint8_t z6      :1; // must be 0 
    };
    
};



#endif