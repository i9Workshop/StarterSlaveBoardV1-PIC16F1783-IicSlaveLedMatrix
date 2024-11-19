#ifndef program_H
#define	program_H

#include <xc.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#define _User_FOSC      32000000 // CPU Clock Frequency
    
#define led_VccColumn1      LATAbits.LATA0
#define led_VccColumn2      LATAbits.LATA1
#define led_VccColumn3      LATAbits.LATA2
#define led_GndRow1         LATAbits.LATA3
#define led_GndRow2         LATAbits.LATA4
#define led_GndRow3         LATAbits.LATA5
    
#define pb_No1          PORTBbits.RB1
#define pb_No2          PORTBbits.RB0
    
#define sw_No1          PORTBbits.RB5
#define sw_No2          PORTBbits.RB4
#define sw_No3          PORTBbits.RB3
#define sw_No4          PORTBbits.RB2

    
    void delay_x1o5us(uint8_t delay);
    void delay_x24o25us(uint16_t delay);
    void delay_ms(uint32_t delay);
    
    void programInitialize(void);
    void programLoop(void);
    
    void ledMatrix_AllOff(void);
    void ledMatrix_SetDisplay(uint16_t ledWord);
    void ledMatrix_DelayScanInput(uint16_t delay);
    
    void i2c_Initialize(uint8_t address);
    void i2c_ScanMaster(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* program_H */
