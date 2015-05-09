/* 
 * File:   main.c
 * Author: rz0df9
 *
 * Created on 3 avril 2015, 09:13
 *
 * Sur base control des moteurs Stepper
 */

#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>

//#define FOSC 64000000L
#define FOSC 32000000L
//#define FOSC 8000000L
#define FCAN 125000L
#include "../common/CAN.h"

INT8 sin[] = {0, 4, 8, 13, 17, 22, 26, 31, 35, 40, 44, 48, 53, 57, 61, 66, 70, 74, 79, 83, 87, 91, 95, 100, 104, 108, 112, 116, 120, 124, 127, 131, 135, 139, 143, 146, 150, 154, 157, 161, 164, 167, 171, 174, 177, 181, 184, 187, 190, 193, 196, 198, 201, 204, 207, 209, 212, 214, 217, 219, 221, 223, 226, 228, 230, 232, 233, 235, 237, 238, 240, 242, 243, 244, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 252, 252, 251, 250, 249, 248, 247, 246, 244, 243, 242, 240, 238, 237, 235, 233, 232, 230, 228, 226, 223, 221, 219, 217, 214, 212, 209, 207, 204, 201, 198, 196, 193, 190, 187, 184, 181, 177, 174, 171, 167, 164, 161, 157, 154, 150, 146, 143, 139, 135, 131, 128, 124, 120, 116, 112, 108, 104, 100, 95, 91, 87, 83, 79, 74, 70, 66, 61, 57, 53, 48, 44, 40, 35, 31, 26, 22, 17, 13, 8, 4, 0, -4, -8, -13, -17, -22, -26, -31, -35, -40, -44, -48, -53, -57, -61, -66, -70, -74, -79, -83, -87, -91, -95, -100, -104, -108, -112, -116, -120, -124, -127, -131, -135, -139, -143, -146, -150, -154, -157, -161, -164, -167, -171, -174, -177, -181, -184, -187, -190, -193, -196, -198, -201, -204, -207, -209, -212, -214, -217, -219, -221, -223, -226, -228, -230, -232, -233, -235, -237, -238, -240, -242, -243, -244, -246, -247, -248, -249, -250, -251, -252, -252, -253, -254, -254, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -255, -254, -254, -253, -252, -252, -251, -250, -249, -248, -247, -246, -244, -243, -242, -240, -238, -237, -235, -233, -232, -230, -228, -226, -223, -221, -219, -217, -214, -212, -209, -207, -204, -201, -198, -196, -193, -190, -187, -184, -181, -177, -174, -171, -167, -164, -161, -157, -154, -150, -146, -143, -139, -135, -131, -128, -124, -120, -116, -112, -108, -104, -100, -95, -91, -87, -83, -79, -74, -70, -66, -61, -57, -53, -48, -44, -40, -35, -31, -26, -22, -17, -13, -8, -4};

#include <delays.h>

#define max 180
int ir = 0, ig = max / 3, ib = 2 * max / 3;
UINT32_VAL t32;

void setRGB(char R, char G, char B) {
    char i;

    TXB1D2 = t32.byte.LB = B; // red
    TXB1D1 = t32.byte.HB = G; // green
    TXB1D0 = t32.byte.UB = R; // blue
    TXB1D3 = COMSTAT; // -> 05
    TXB1D4 = TXERRCNT; // Errors...
    TXB1D5 = RXERRCNT;
    TXB1DLC = 6;
    //if (!TXB1CONbits.TXREQ)
      //TXB1CONbits.TXREQ = 1; // Send CAN message

    // 32MHz -> 0.125us
    // Reset PL9823: 50us low
    PORTAbits.RA1 = 0;
    PORTAbits.RA1 = 1;
    PORTAbits.RA1 = 0;
    Delay100TCYx(5); // 400*0.125 = 50us
    //PORTAbits.RA1 = 1;

    for (i = 0; i <= 24; i++) {
        if (t32.bits.b23) {
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
        } else { // G B
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 1;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA1 = 0;
        }
        t32.Val *= 2;
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    char i;
    // For the INTIOx modes (HF-INTOSC):
    // ? Only the PLLEN(OSCTUNE<6>) can enable the PLL (PLLCFG is ignored)
#ifdef   __18F25K80
    //OSCCON |= 0b01100010; // Int Osc = 8MHz
    //OSCCONbits.IRCF = 0b110; // Int Osc = 8MHz
    OSCCON = 0b110 << 4; // Int Osc = 8MHz
    //OSCCON = 0b111 << 4; // Int Osc = 16MHz
    OSCTUNEbits.PLLEN = 1; // PLL is enabled
    // => 8*4 = 32MHz
#else
#endif
    OSCCON2 = 0b00000000; // for SOSC tbc
    //REFOCON = 0x00; // as default

    init_CAN(0x20, BRP); // Id et speed
    //init_CAN(0x20, 1); // 8MHz
    //init_CAN(0x20, 7); // 32MHz
    IPR5bits.RXB0IP = 0; // Low prio
    IPR5bits.RXB1IP = 0; // Low prio

    // RA1: out blue LED

    ANCON0 = 0b11111100; // AN0: digital 0: Analog, 1:Digital
    ANCON1 = 0b11111111; //

    PORTAbits.RA1 = 1;
    TRISAbits.TRISA1 = 0;

    // PORTC: stepper motor output
    PORTC = 0x00; //0x00;
    TRISC = 0x00; // output

    // Timer0
    //T0CON = 0b10000100; // Fosc/4, 1:32 prescaler [FFFF -> 1.05s] Fosc = 8MHz
    //T0CON = 0b10000010; // Fosc/4, 1:8 prescaler [FFFF -> 0.25s] Fosc = 8MHz
    T0CON = 0b10000000; // Fosc/4, 1:2 prescaler [FFFF -> 0.XXs] Fosc = 8MHz
    //#define T0FREQ (FOSC/(4*32))
    INTCONbits.TMR0IE = 1;

    TXB1D0 = 0x55;
    cmd = 0x01;
    TXB1DLC = 4;
    //sendMessage(1);

    PORTAbits.RA1 = 1;

    //setRGB(0,10,0);

    RCONbits.IPEN = 1; // Interrupt Priority Enable bit
    INTCONbits.GIEH = 1; //enable high interrupts
    INTCONbits.GIEL = 1; //enable low interrupts
    OSCCONbits.IDLEN = 1; // IDLE select

    while (1) {
        Sleep();
    }
}

// high interrut

void interrupt high_isr(void) {
    char i;
    // Timer0: every 1.05 sec
    if (TMR0IE) if (/*INTCONbits.*/TMR0IF) {
            //PORTAbits.RA1 ^= 1; // toggle debug blue LED

            //setRGB(sin[ir++], sin[ig++], sin[ig++]);
            if (ir>=max) ir -=max;
            if (ig>=max) ig -=max;
            if (ib>=max) ib -=max;

            TMR0IF = 0;
        }
}

// low interrut

void low_priority interrupt low_isr(void) {
    // CCP2: stepper
    if (CCP2IE) if (CCP2IF) {
            CCP2IF = 0;
        }
    // **********  RXB0IF  **********
    // RXB0: command request on idCAN
    if (RXB0IF) {
        RXB0IF = 0;
        setRGB(RXB0D0, RXB0D1, RXB0D2);
        RXB0CONbits.RXFUL = 0;
    }

    // **********  RXB1IF  **********
    // RXB1: debug 777h
    if (RXB1IF) {
        RXB1IF = 0;
        RXB1CONbits.RXFUL = 0;
    }

}
