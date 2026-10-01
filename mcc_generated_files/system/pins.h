/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.4.1
 *
 * @skipline  Device : dsPIC33CK256MP508
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD7 GPIO Pin which has a custom name of IO_RD7 to High
 * @pre      The RD7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RD7_SetHigh()          (_LATD7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD7 GPIO Pin which has a custom name of IO_RD7 to Low
 * @pre      The RD7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RD7_SetLow()           (_LATD7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD7 GPIO Pin which has a custom name of IO_RD7
 * @pre      The RD7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RD7_Toggle()           (_LATD7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD7 GPIO Pin which has a custom name of IO_RD7
 * @param    none
 * @return   none  
 */
#define IO_RD7_GetValue()         _RD7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD7 GPIO Pin which has a custom name of IO_RD7 as Input
 * @param    none
 * @return   none  
 */
#define IO_RD7_SetDigitalInput()  (_TRISD7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD7 GPIO Pin which has a custom name of IO_RD7 as Output
 * @param    none
 * @return   none  
 */
#define IO_RD7_SetDigitalOutput() (_TRISD7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of IO_RD9 to High
 * @pre      The RD9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RD9_SetHigh()          (_LATD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of IO_RD9 to Low
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RD9_SetLow()           (_LATD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD9 GPIO Pin which has a custom name of IO_RD9
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RD9_Toggle()           (_LATD9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD9 GPIO Pin which has a custom name of IO_RD9
 * @param    none
 * @return   none  
 */
#define IO_RD9_GetValue()         _RD9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of IO_RD9 as Input
 * @param    none
 * @return   none  
 */
#define IO_RD9_SetDigitalInput()  (_TRISD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of IO_RD9 as Output
 * @param    none
 * @return   none  
 */
#define IO_RD9_SetDigitalOutput() (_TRISD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE6 GPIO Pin which has a custom name of IO_RE6 to High
 * @pre      The RE6 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE6_SetHigh()          (_LATE6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE6 GPIO Pin which has a custom name of IO_RE6 to Low
 * @pre      The RE6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE6_SetLow()           (_LATE6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE6 GPIO Pin which has a custom name of IO_RE6
 * @pre      The RE6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE6_Toggle()           (_LATE6 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE6 GPIO Pin which has a custom name of IO_RE6
 * @param    none
 * @return   none  
 */
#define IO_RE6_GetValue()         _RE6

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE6 GPIO Pin which has a custom name of IO_RE6 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE6_SetDigitalInput()  (_TRISE6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE6 GPIO Pin which has a custom name of IO_RE6 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE6_SetDigitalOutput() (_TRISE6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE7 GPIO Pin which has a custom name of IO_RE7 to High
 * @pre      The RE7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE7_SetHigh()          (_LATE7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE7 GPIO Pin which has a custom name of IO_RE7 to Low
 * @pre      The RE7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE7_SetLow()           (_LATE7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE7 GPIO Pin which has a custom name of IO_RE7
 * @pre      The RE7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE7_Toggle()           (_LATE7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE7 GPIO Pin which has a custom name of IO_RE7
 * @param    none
 * @return   none  
 */
#define IO_RE7_GetValue()         _RE7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE7 GPIO Pin which has a custom name of IO_RE7 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE7_SetDigitalInput()  (_TRISE7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE7 GPIO Pin which has a custom name of IO_RE7 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE7_SetDigitalOutput() (_TRISE7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE8 GPIO Pin which has a custom name of IO_RE8 to High
 * @pre      The RE8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE8_SetHigh()          (_LATE8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE8 GPIO Pin which has a custom name of IO_RE8 to Low
 * @pre      The RE8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE8_SetLow()           (_LATE8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE8 GPIO Pin which has a custom name of IO_RE8
 * @pre      The RE8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE8_Toggle()           (_LATE8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE8 GPIO Pin which has a custom name of IO_RE8
 * @param    none
 * @return   none  
 */
#define IO_RE8_GetValue()         _RE8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE8 GPIO Pin which has a custom name of IO_RE8 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE8_SetDigitalInput()  (_TRISE8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE8 GPIO Pin which has a custom name of IO_RE8 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE8_SetDigitalOutput() (_TRISE8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE9 GPIO Pin which has a custom name of IO_RE9 to High
 * @pre      The RE9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE9_SetHigh()          (_LATE9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE9 GPIO Pin which has a custom name of IO_RE9 to Low
 * @pre      The RE9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE9_SetLow()           (_LATE9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE9 GPIO Pin which has a custom name of IO_RE9
 * @pre      The RE9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE9_Toggle()           (_LATE9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE9 GPIO Pin which has a custom name of IO_RE9
 * @param    none
 * @return   none  
 */
#define IO_RE9_GetValue()         _RE9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE9 GPIO Pin which has a custom name of IO_RE9 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE9_SetDigitalInput()  (_TRISE9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE9 GPIO Pin which has a custom name of IO_RE9 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE9_SetDigitalOutput() (_TRISE9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE12 GPIO Pin which has a custom name of IO_RE12 to High
 * @pre      The RE12 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE12_SetHigh()          (_LATE12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE12 GPIO Pin which has a custom name of IO_RE12 to Low
 * @pre      The RE12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE12_SetLow()           (_LATE12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE12 GPIO Pin which has a custom name of IO_RE12
 * @pre      The RE12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE12_Toggle()           (_LATE12 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE12 GPIO Pin which has a custom name of IO_RE12
 * @param    none
 * @return   none  
 */
#define IO_RE12_GetValue()         _RE12

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE12 GPIO Pin which has a custom name of IO_RE12 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE12_SetDigitalInput()  (_TRISE12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE12 GPIO Pin which has a custom name of IO_RE12 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE12_SetDigitalOutput() (_TRISE12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE13 GPIO Pin which has a custom name of IO_RE13 to High
 * @pre      The RE13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE13_SetHigh()          (_LATE13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE13 GPIO Pin which has a custom name of IO_RE13 to Low
 * @pre      The RE13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE13_SetLow()           (_LATE13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE13 GPIO Pin which has a custom name of IO_RE13
 * @pre      The RE13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE13_Toggle()           (_LATE13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE13 GPIO Pin which has a custom name of IO_RE13
 * @param    none
 * @return   none  
 */
#define IO_RE13_GetValue()         _RE13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE13 GPIO Pin which has a custom name of IO_RE13 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE13_SetDigitalInput()  (_TRISE13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE13 GPIO Pin which has a custom name of IO_RE13 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE13_SetDigitalOutput() (_TRISE13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE14 GPIO Pin which has a custom name of IO_RE14 to High
 * @pre      The RE14 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE14_SetHigh()          (_LATE14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE14 GPIO Pin which has a custom name of IO_RE14 to Low
 * @pre      The RE14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE14_SetLow()           (_LATE14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE14 GPIO Pin which has a custom name of IO_RE14
 * @pre      The RE14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE14_Toggle()           (_LATE14 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE14 GPIO Pin which has a custom name of IO_RE14
 * @param    none
 * @return   none  
 */
#define IO_RE14_GetValue()         _RE14

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE14 GPIO Pin which has a custom name of IO_RE14 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE14_SetDigitalInput()  (_TRISE14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE14 GPIO Pin which has a custom name of IO_RE14 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE14_SetDigitalOutput() (_TRISE14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE15 GPIO Pin which has a custom name of IO_RE15 to High
 * @pre      The RE15 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RE15_SetHigh()          (_LATE15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RE15 GPIO Pin which has a custom name of IO_RE15 to Low
 * @pre      The RE15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE15_SetLow()           (_LATE15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RE15 GPIO Pin which has a custom name of IO_RE15
 * @pre      The RE15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RE15_Toggle()           (_LATE15 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RE15 GPIO Pin which has a custom name of IO_RE15
 * @param    none
 * @return   none  
 */
#define IO_RE15_GetValue()         _RE15

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE15 GPIO Pin which has a custom name of IO_RE15 as Input
 * @param    none
 * @return   none  
 */
#define IO_RE15_SetDigitalInput()  (_TRISE15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RE15 GPIO Pin which has a custom name of IO_RE15 as Output
 * @param    none
 * @return   none  
 */
#define IO_RE15_SetDigitalOutput() (_TRISE15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for IO_RD9 Pin
 * @param    none
 * @return   none   
 */
void IO_RD9_CallBack(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for IO_RE12 Pin
 * @param    none
 * @return   none   
 */
void IO_RE12_CallBack(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for IO_RE7 Pin
 * @param    none
 * @return   none   
 */
void IO_RE7_CallBack(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for IO_RE8 Pin
 * @param    none
 * @return   none   
 */
void IO_RE8_CallBack(void);

/**
 * @ingroup  pinsdriver
 * @brief    This function is callback for IO_RE9 Pin
 * @param    none
 * @return   none   
 */
void IO_RE9_CallBack(void);


/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void IO_RD9_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void IO_RE12_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void IO_RE7_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void IO_RE8_SetInterruptHandler(void (* InterruptHandler)(void));

/**
 * @ingroup    pinsdriver
 * @brief      This function assigns a function pointer with a callback address
 * @param[in]  InterruptHandler - Address of the callback function 
 * @return     none  
 */
void IO_RE9_SetInterruptHandler(void (* InterruptHandler)(void));


#endif
