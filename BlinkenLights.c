#include <msp430.h> 

void handle_LEDS(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	// Port1 Setup
    P1SEL = 0x00;
    P1DIR = 0xFF;

    // Port2 Setup
    P2SEL = 0x00;
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0xFF; // Also prevents unused pins from wrecking havoc

    handle_LEDS();

	return 0;
}


void handle_LEDS(void) {
    const char read_pattern = (BIT3 | BIT4 | BIT5);
    char display_pattern = ~P2IN & read_pattern;

    for (;;) {
        if ((P2IN & BIT0) == 0x00) { // read mode
            display_pattern = ~P2IN & read_pattern;
        } else { // rotate mode
            if ((P2IN & BIT7) == 0x00) {
                display_pattern = display_pattern << 1 | display_pattern >> 7;
            } else {
                display_pattern = display_pattern >> 1 | display_pattern << 7;
            }
        }

        display_pattern &= 0xFF;
        P1OUT = display_pattern;

        // speed handling

        if ((P2IN & BIT6) == 0x00) {
            __delay_cycles(400000);
        }

        __delay_cycles(100000);
     }
}
