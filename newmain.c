// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, BOREN = OFF, LVP = OFF, CPD = OFF, WRT = OFF, CP = OFF

#include <xc.h>

#define _XTAL_FREQ 20000000     // 20 MHz Crystal

#define SPI_CS PORTCbits.RC0

// Software I2C Pins (PORTB)
#define SDA_PIN PORTBbits.RB0
#define SCL_PIN PORTBbits.RB1
#define SDA_TRIS TRISBbits.TRISB0
#define SCL_TRIS TRISBbits.TRISB1

// ==================== ADC MODULE ====================
void ADC_Init(void) {
    ADCON0 = 0x01;              // ADC ON, AN0 channel, Fosc/32
    ADCON1 = 0x80;              // Right justified, Vref = VDD
}

unsigned int ADC_Read(unsigned char ch) {
    ADCON0bits.CHS = ch;
    __delay_us(25);
    GO_nDONE = 1;
    while (GO_nDONE);
    return (unsigned int)(((unsigned int)ADRESH << 8) | ADRESL);
}

// ==================== UART MODULE ====================
void UART_Init(void) {
    TRISC6 = 0;                 // TX Pin = Output
    TRISC7 = 1;                 // RX Pin = Input
    SPBRG = 31;                 // 9600 Baud @ 20MHz crystal
    TXSTA = 0x24;               // TX Enabled, High Speed Baud
    RCSTA = 0x90;               // Serial Port Enabled
}

void UART_Write_Char(char data) {
    while (!TXIF);
    TXREG = data;
}

void UART_Write_String(const char *buffer) {
    while (*buffer) {
        UART_Write_Char(*buffer++);
    }
}

void UART_Write_Temp(float temp) {
    unsigned int whole = (unsigned int)temp;
    unsigned int decimal = (unsigned int)((temp - (float)whole) * 10.0f);

    UART_Write_String("Temperature: ");
    UART_Write_Char((char)((whole / 10) + '0'));
    UART_Write_Char((char)((whole % 10) + '0'));
    UART_Write_Char('.');
    UART_Write_Char((char)(decimal + '0'));
    UART_Write_String(" C\r\n");
}

// ==================== HARDWARE SPI MODULE ====================
void SPI_Init(void) {
    TRISC0 = 0;                 // CS Output
    TRISC3 = 1;                 // SCK Input in Master Mode per datasheet
    TRISC4 = 1;                 // SDI Input
    TRISC5 = 0;                 // SDO Output
    SPI_CS = 1;

    SSPSTAT = 0x40;             // CKE = 1
    SSPCON  = 0x20;             // Enable SPI Master Mode (Fosc/4)
}

void SPI_Write(unsigned char data) {
    SPI_CS = 0;
    SSPBUF = data;
    while (!SSPSTATbits.BF);
    SPI_CS = 1;
}

// ==================== PUSH-PULL SOFTWARE I2C ====================
void SW_I2C_Init(void) {
    SDA_TRIS = 0; SDA_PIN = 1;
    SCL_TRIS = 0; SCL_PIN = 1;
}

void SW_I2C_Start(void) {
    SDA_PIN = 1; SCL_PIN = 1;
    __delay_us(5);
    SDA_PIN = 0;
    __delay_us(5);
    SCL_PIN = 0;
}

void SW_I2C_Stop(void) {
    SDA_PIN = 0;
    __delay_us(5);
    SCL_PIN = 1;
    __delay_us(5);
    SDA_PIN = 1;
    __delay_us(5);
}

void SW_I2C_Write(unsigned char data) {
    for (unsigned char i = 0; i < 8; i++) {
        SDA_PIN = (data & 0x80) ? 1 : 0;
        __delay_us(2);
        SCL_PIN = 1;
        __delay_us(5);
        SCL_PIN = 0;
        data <<= 1;
    }
    SDA_TRIS = 1;
    SCL_PIN = 1;
    __delay_us(5);
    SCL_PIN = 0;
    SDA_TRIS = 0;
}

// ==================== MAIN ROUTINE ====================
void main(void) {
    unsigned int raw;
    float temp_c;

    ADC_Init();
    UART_Init();
    SPI_Init();
    SW_I2C_Init();

    UART_Write_String("System Initialized...\r\n");

    while (1) {
        raw = ADC_Read(0);
        temp_c = ((float)raw * 500.0f) / 1023.0f;

        UART_Write_Temp(temp_c);
        SPI_Write((unsigned char)temp_c);

        SW_I2C_Start();
        SW_I2C_Write(0xA0);
        SW_I2C_Write((unsigned char)temp_c);
        SW_I2C_Stop();

        __delay_ms(1000);
    }
}