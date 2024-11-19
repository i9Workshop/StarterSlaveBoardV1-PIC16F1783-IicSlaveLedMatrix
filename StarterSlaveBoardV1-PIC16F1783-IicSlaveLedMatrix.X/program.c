#include "program.h"


// Delay x1.5us
void delay_x1o5us(uint8_t delay) {
    for(uint8_t i=0; i<delay; i++) NOP();
}

// Delay x24.25us
void delay_x24o25us(uint16_t delay) {
    for(uint16_t i=0; i<delay; i++) delay_x1o5us(15);
}

// Delay x1ms
void delay_ms(uint32_t delay) {
    for(uint32_t i=0; i<delay; i++) delay_x24o25us(41);
}


uint8_t i2c_RxData = 0; // Variable to store received data
uint8_t i2c_TxData = 0; // Variable to store transmit data


void programInitialize(void) {
    TRISA = 0;
    ANSELA = 0;
    LATA = 0;
    
    TRISB = 0x3F;
    ANSELB = 0;
    
    // SCl pin
    TRISCbits.TRISC3 = 1;
    APFCONbits.SCKSEL = 0;
    
    // SDa pin
    TRISCbits.TRISC4 = 1;
    APFCONbits.SDISEL = 0;
    
    i2c_Initialize(0x10); // Change address to 0x20 for the second device
}

void programLoop(void) {
    ledMatrix_SetDisplay(i2c_RxData); // Display led matrix from  variable i2c_RxData
}

void ledMatrix_DelayScanInput(uint16_t delay) {
    for(uint16_t i=0; i<delay; i++) {
        i2c_ScanMaster(); // Call I2C polling function
        
        i2c_TxData = (uint8_t)((pb_No1 << 5) | (pb_No2 << 4) | (sw_No1 << 3) | // Write switches status to variable i2c_TxData
                     (sw_No2 << 2) | (sw_No3 << 1) | sw_No4);
    }
}

void ledMatrix_AllOff(void) {
    led_VccColumn1 = 0;
    led_VccColumn2 = 0;
    led_VccColumn3 = 0;
    led_GndRow1 = 0;
    led_GndRow2 = 0;
    led_GndRow3 = 0;
}

void ledMatrix_SetDisplay(uint16_t ledWord) {
    uint16_t delay = 200;
    
    // First row
    
    ledMatrix_AllOff();
    
    led_VccColumn1 = (bool)(ledWord & 0b100000000);
    led_VccColumn2 = (bool)(ledWord & 0b010000000);
    led_VccColumn3 = (bool)(ledWord & 0b001000000);
    led_GndRow1 = 0;
    led_GndRow2 = 1;
    led_GndRow3 = 1;
    
    ledMatrix_DelayScanInput(delay);
    
    // Second row
    
    ledMatrix_AllOff();
    
    led_VccColumn1 = (bool)(ledWord & 0b000100000);
    led_VccColumn2 = (bool)(ledWord & 0b000010000);
    led_VccColumn3 = (bool)(ledWord & 0b000001000);
    led_GndRow1 = 1;
    led_GndRow2 = 0;
    led_GndRow3 = 1;
    
    ledMatrix_DelayScanInput(delay);
    
    // Third row
    
    ledMatrix_AllOff();
    
    led_VccColumn1 = (bool)(ledWord & 0b000000100);
    led_VccColumn2 = (bool)(ledWord & 0b000000010);
    led_VccColumn3 = (bool)(ledWord & 0b000000001);
    led_GndRow1 = 1;
    led_GndRow2 = 1;
    led_GndRow3 = 0;
    
    ledMatrix_DelayScanInput(delay);
}

void i2c_Initialize(uint8_t address) {
    SSPCON1bits.SSPM = 6; // I2C slave mode 7bit address - Page 306
    
    SSPCON2bits.SEN = 1; // Set enable clock stretching for both slave transmit and slave receive - Page 307
    
    SSPCON3bits.SDAHT = 1; // Set minimum of hold time on SDA after the falling edge of SCL to 300 ns - Page 308
    
    SSPADD = (uint8_t)(address << 1); // Set MSSP module address from the function argument - Page 309
    
    SSPCON1bits.SSPEN = 1; // Set enable MSSP module - Page 306
    
    delay_x1o5us(2); // Wait for MSSP module configuration - Page 373 from I/O pin timing
                     // Tioz = 2us
}

void i2c_ScanMaster(void) {
    // Single byte data receiver - Page 272
    // Single byte data transmit - Page 277
    if(PIR1bits.SSP1IF) { // Polling for MSSP module interrupt - Page 266
        if(SSPSTATbits.R_nW) { // Condition to select between read from or write to master device - Page 304
            SSPBUF = i2c_TxData; // Write to MSSP module data register - Page 266
        }
        
        else {
            i2c_RxData = SSPBUF; // Read from MSSP module data register
        }
        
        SSPCON1bits.CKP = 1; // Release SCL line - Page 306
        
        PIR1bits.SSP1IF = 0; // Clear MSSP module interrupt
    }
}
