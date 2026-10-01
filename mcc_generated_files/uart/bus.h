/* 
 * File:   bus.h
 * Author: ivanm
 *
 * Created on June 11, 2026, 9:10 PM
 */

#ifndef BUS_H
#define	BUS_H

typedef struct{
    union{
        uint8_t status;
        struct{
            unsigned enable         : 1;
            unsigned active         : 1;
            unsigned done_wait      : 1;
        }bits;
    }msg_tx_status;
}rs485_t;

extern volatile rs485_t buscomm;

#endif	/* BUS_H */

