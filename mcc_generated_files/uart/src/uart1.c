    /**
     * UART1 Generated Driver Source File
     * 
 * @file      uart1.c
     *  
 * @ingroup   uartdriver
     *  
 * @brief     This is the generated driver source file for the UART1 driver.
     *            
 * @skipline @version   Firmware Driver Version 1.7.0
     *
 * @skipline @version   PLIB Version 1.5.4
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

    // Section: Included Files
    #include <stdbool.h>
    #include <stdint.h>
    #include <stddef.h>
    #include <xc.h>
    #include "../uart1.h"
    #include "../bus.h"

    const struct UART_INTERFACE UART1_Drv = {
        .Initialize = &UART1_Initialize,
        .Deinitialize = &UART1_Deinitialize,
        .Write = &UART1_Write,
        .IsTxReady = &UART1_IsTxReady,
        .IsTxDone = &UART1_IsTxDone,
        .TransmitEnable = &UART1_TransmitEnable,
        .TransmitDisable = &UART1_TransmitDisable,
        .TransmitInterruptEnable = NULL,
        .TransmitInterruptDisable = NULL
    };

    // Section: Private Variable Definitions
    // Section: Data Type Definitions

    /**
     @ingroup  uartdriver
     @static   UART Driver Queue Status
     @brief    Defines the object required for the status of the queue
    */

    static volatile uint8_t txBuffIdx = 0;
    static volatile uint8_t txBuffDataCnt = 0;

    /**
     @ingroup  uartdriver
     @brief    Defines the length of the Transmit and Receive Buffers
    */

    /* We add one extra byte than requested so that we don't have to have a separate
     * bit to determine the difference between buffer full and buffer empty, but
     * still be able to hold the amount of data requested by the user.  Empty is
     * when head == tail.  So full will result in head/tail being off by one due to
     * the extra byte.
     */
    #define UART1_TX_BUFFER_SIZE        64U

    /**
     @ingroup  uartdriver
     @static   UART Driver Queue
     @brief    Defines the Transmit and Receive Buffers
    */
    volatile static uint8_t txBuffer[UART1_TX_BUFFER_SIZE];

    // Section: Driver Interface

    void UART1_Initialize(void)
    {
        buscomm.msg_tx_status.bits.done_wait = true;
        IEC0bits.U1TXIE = 0;
        IFS0bits.U1TXIF = 0;

        // URXEN ; RXBIMD ; UARTEN disabled; MOD Asynchronous 8-bit UART; UTXBRK ; BRKOVR ; UTXEN ; USIDL ; WAKE ; ABAUD ; BRGH ; 
        U1MODE = 0x0U;
        // STSEL 1 Stop bit sent, 1 checked at RX; BCLKMOD enabled; SLPEN ; FLO ; BCLKSEL FOSC; C0EN ; RUNOVF ; UTXINV ; URXINV ; HALFDPLX ; 
        U1MODEH = 0xC00U;
        // OERIE ; RXBKIF ; RXBKIE ; ABDOVF ; OERR ; TXCIE ; TXCIF ; FERIE ; TXMTIE ; ABDOVE ; CERIE ; CERIF ; PERIE ; 
        U1STA = 0x80U;
        // URXISEL ; UTXBE ; UTXISEL TX_BUF_EMPTY; URXBE ; STPMD ; TXWRE ; 
        U1STAH = 0x2EU;
    // BaudRate 100000.00; Frequency 8000000 Hz; BRG 80; 
        U1BRG = 0x50U;
        // BRG 0; 
        U1BRGH = 0x0U;
        
        txBuffIdx = 0;
        txBuffDataCnt = 0;

        // UART Transmit collision interrupt
        U1STAbits.TXCIE = 1;

        //Make sure to set LAT bit corresponding to TxPin as high before UART initialization
        U1MODEbits.UARTEN = 1;   // enabling UART ON bit
        U1MODEbits.UTXEN = 1;
        U1MODEbits.URXEN = 0;
    }

    void UART1_Deinitialize(void)
    {
        // UART Transmit interrupt
        IFS0bits.U1TXIF = 0;
        IEC0bits.U1TXIE = 0;

        // UART Receive Interrupt
        IFS0bits.U1RXIF = 0;
        IEC0bits.U1RXIE = 0;

        // UART Event interrupt
        IFS11bits.U1EVTIF = 0;
        IEC11bits.U1EVTIE = 0;

        // UART Error interrupt
        IFS3bits.U1EIF = 0;
        IEC3bits.U1EIE = 0;

        U1MODE = 0x0U;
        U1MODEH = 0x0U;
        U1STA = 0x80U;
        U1STAH = 0x2EU;
        U1BRG = 0x0U;
        U1BRGH = 0x0U;
    }

    void UART1_Write(uint8_t byte)
    {
        if(txBuffDataCnt < UART1_TX_BUFFER_SIZE)
        {
            txBuffer[txBuffDataCnt++] = byte;
        }
    }

    void UART1_TxStart(void)
    {
        IEC0bits.U1RXIE = 0;
        if(txBuffDataCnt == 0U){
            return;
        }   
        txBuffIdx = 0;

         // ako je TXREG prazan, rucno posalji prvi byte
        if(U1STAHbits.UTXBE)
        {
            U1TXREG = txBuffer[txBuffIdx++];
        }

        IFS0bits.U1TXIF = 0;
        IEC0bits.U1TXIE = 1;
    }
    
    void UART1_TxResetBufferIndex(void)
    {
        txBuffIdx = 0;
        txBuffDataCnt = 0;
    }

    bool UART1_IsTxReady(void)
    {
        return (txBuffDataCnt < UART1_TX_BUFFER_SIZE);
    }

    bool UART1_IsTxDone(void)
    {
        return (txBuffDataCnt == 0U) && U1STAbits.TRMT && U1STAHbits.UTXBE;
    }

    void UART1_TransmitEnable(void)
    {
        U1MODEbits.UTXEN = 1;
    }

    void UART1_TransmitDisable(void)
    {
        U1MODEbits.UTXEN = 0;
    }
    
    void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1TXInterrupt(void)
    {
        IFS0bits.U1TXIF = 0;
        
        while((U1STAHbits.UTXBF == 0U) && (txBuffIdx < txBuffDataCnt))
        {
            U1TXREG = txBuffer[txBuffIdx++];
        }

        if(txBuffIdx >= txBuffDataCnt)
        {
            IEC0bits.U1TXIE = 0;
            txBuffIdx = 0;
            txBuffDataCnt = 0;
            buscomm.msg_tx_status.bits.done_wait = true;
        }
    }