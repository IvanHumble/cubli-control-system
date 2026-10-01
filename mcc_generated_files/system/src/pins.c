 /**
 * PINS Generated Driver Source File 
 * 
 * @file      pins.c
 *            
 * @ingroup   pinsdriver
 *            
 * @brief     This is the generated driver source file for PINS driver.
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

// Section: Includes
#include <xc.h>
#include <stddef.h>
#include "../pins.h"

// Section: File specific functions
static void (*IO_RD9_InterruptHandler)(void) = NULL;
static void (*IO_RE12_InterruptHandler)(void) = NULL;
static void (*IO_RE7_InterruptHandler)(void) = NULL;
static void (*IO_RE8_InterruptHandler)(void) = NULL;
static void (*IO_RE9_InterruptHandler)(void) = NULL;

// Section: Driver Interface Function Definitions
void PINS_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000U;
    LATB = 0x0000U;
    LATC = 0x0000U;
    LATD = 0x1000U;
    LATE = 0x0000U;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001FU;
    TRISB = 0xFFFFU;
    TRISC = 0xBFFFU;
    TRISD = 0xEF7FU;
    TRISE = 0x1FBFU;


    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPUA = 0x0000U;
    CNPUB = 0x0000U;
    CNPUC = 0x0000U;
    CNPUD = 0x0000U;
    CNPUE = 0x0380U;
    CNPDA = 0x0000U;
    CNPDB = 0x0000U;
    CNPDC = 0x0000U;
    CNPDD = 0x0000U;
    CNPDE = 0x0000U;


    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000U;
    ODCB = 0x0000U;
    ODCC = 0x0000U;
    ODCD = 0x0000U;
    ODCE = 0x0000U;


    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001FU;
    ANSELB = 0x009FU;
    ANSELC = 0x00CFU;
    ANSELD = 0x2C00U;
    ANSELE = 0x000FU;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
     __builtin_write_RPCON(0x0000); // unlock PPS

        RPINR14bits.QEIA1R = 0x0038U; //RC8->QEI1:QEA1;
        RPINR14bits.QEIB1R = 0x0039U; //RC9->QEI1:QEB1;
        RPOR15bits.RP62R = 0x0010U;  //RC14->SCCP2:OCM2;
        RPOR22bits.RP76R = 0x0001U;  //RD12->UART1:U1TX;

     __builtin_write_RPCON(0x0800); // lock PPS

    /*******************************************************************************
    * Interrupt On Change: any
    *******************************************************************************/
    CNEN0Dbits.CNEN0D9 = 1; //Pin : RD9U; 
    CNEN1Dbits.CNEN1D9 = 1; //Pin : RD9U; 
    /*******************************************************************************
    * Interrupt On Change: any
    *******************************************************************************/
    CNEN0Ebits.CNEN0E12 = 1; //Pin : RE12U; 
    CNEN1Ebits.CNEN1E12 = 1; //Pin : RE12U; 
    /*******************************************************************************
    * Interrupt On Change: positive
    *******************************************************************************/
    CNEN0Ebits.CNEN0E7 = 1; //Pin : RE7U; 
    /*******************************************************************************
    * Interrupt On Change: positive
    *******************************************************************************/
    CNEN0Ebits.CNEN0E8 = 1; //Pin : RE8U; 
    /*******************************************************************************
    * Interrupt On Change: positive
    *******************************************************************************/
    CNEN0Ebits.CNEN0E9 = 1; //Pin : RE9U; 

    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFDbits.CNFD9 = 0;    //Pin : IO_RD9
    CNFEbits.CNFE12 = 0;    //Pin : IO_RE12
    CNFEbits.CNFE7 = 0;    //Pin : IO_RE7
    CNFEbits.CNFE8 = 0;    //Pin : IO_RE8
    CNFEbits.CNFE9 = 0;    //Pin : IO_RE9

    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONDbits.CNSTYLE = 1; //Config for PORTD
    CNCONDbits.ON = 1; //Config for PORTD
    CNCONEbits.CNSTYLE = 1; //Config for PORTE
    CNCONEbits.ON = 1; //Config for PORTE

    /* Initialize IOC Interrupt Handler*/
    IO_RD9_SetInterruptHandler(&IO_RD9_CallBack);
    IO_RE12_SetInterruptHandler(&IO_RE12_CallBack);
    IO_RE7_SetInterruptHandler(&IO_RE7_CallBack);
    IO_RE8_SetInterruptHandler(&IO_RE8_CallBack);
    IO_RE9_SetInterruptHandler(&IO_RE9_CallBack);

    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS4bits.CNDIF = 0; //Clear CNDI interrupt flag
    IEC4bits.CNDIE = 1; //Enable CNDI interrupt
    IFS4bits.CNEIF = 0; //Clear CNEI interrupt flag
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void __attribute__ ((weak)) IO_RD9_CallBack(void)
{

}

void __attribute__ ((weak)) IO_RE12_CallBack(void)
{

}

void __attribute__ ((weak)) IO_RE7_CallBack(void)
{

}

void __attribute__ ((weak)) IO_RE8_CallBack(void)
{

}

void __attribute__ ((weak)) IO_RE9_CallBack(void)
{

}

void IO_RD9_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNDIE = 0; //Disable CNDI interrupt
    IO_RD9_InterruptHandler = InterruptHandler; 
    IEC4bits.CNDIE = 1; //Enable CNDI interrupt
}

void IO_RE12_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    IO_RE12_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void IO_RE7_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    IO_RE7_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void IO_RE8_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    IO_RE8_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

void IO_RE9_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC4bits.CNEIE = 0; //Disable CNEI interrupt
    IO_RE9_InterruptHandler = InterruptHandler; 
    IEC4bits.CNEIE = 1; //Enable CNEI interrupt
}

/* Interrupt service function for the CNDI interrupt. */
/* cppcheck-suppress misra-c2012-8.4
*
* (Rule 8.4) REQUIRED: A compatible declaration shall be visible when an object or 
* function with external linkage is defined
*
* Reasoning: Interrupt declaration are provided by compiler and are available
* outside the driver folder
*/
void __attribute__ (( interrupt, no_auto_psv )) _CNDInterrupt (void)
{
    if(CNFDbits.CNFD9 == 1)
    {
        if(IO_RD9_InterruptHandler != NULL) 
        { 
            IO_RD9_InterruptHandler(); 
        }
        
        CNFDbits.CNFD9 = 0;  //Clear flag for Pin - IO_RD9
    }
    
    // Clear the flag
    IFS4bits.CNDIF = 0;
}

/* Interrupt service function for the CNEI interrupt. */
/* cppcheck-suppress misra-c2012-8.4
*
* (Rule 8.4) REQUIRED: A compatible declaration shall be visible when an object or 
* function with external linkage is defined
*
* Reasoning: Interrupt declaration are provided by compiler and are available
* outside the driver folder
*/
void __attribute__ (( interrupt, no_auto_psv )) _CNEInterrupt (void)
{
    if(CNFEbits.CNFE12 == 1)
    {
        if(IO_RE12_InterruptHandler != NULL) 
        { 
            IO_RE12_InterruptHandler(); 
        }
        
        CNFEbits.CNFE12 = 0;  //Clear flag for Pin - IO_RE12
    }
    
    if(CNFEbits.CNFE7 == 1)
    {
        if(IO_RE7_InterruptHandler != NULL) 
        { 
            IO_RE7_InterruptHandler(); 
        }
        
        CNFEbits.CNFE7 = 0;  //Clear flag for Pin - IO_RE7
    }
    
    if(CNFEbits.CNFE8 == 1)
    {
        if(IO_RE8_InterruptHandler != NULL) 
        { 
            IO_RE8_InterruptHandler(); 
        }
        
        CNFEbits.CNFE8 = 0;  //Clear flag for Pin - IO_RE8
    }
    
    if(CNFEbits.CNFE9 == 1)
    {
        if(IO_RE9_InterruptHandler != NULL) 
        { 
            IO_RE9_InterruptHandler(); 
        }
        
        CNFEbits.CNFE9 = 0;  //Clear flag for Pin - IO_RE9
    }
    
    // Clear the flag
    IFS4bits.CNEIF = 0;
}

