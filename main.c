#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd_lib.h"

#define BITRATE 9600
#define BITTIMER ((F_CPU / BITRATE / 16) - 1)

#define output_low(port, pin) port &= ~(1 << pin)
#define output_high(port, pin) port |= (1 << pin)
#define set_input(portdir, pin) portdir &= ~(1 << pin)
#define set_output(portdir, pin) portdir |= (1 << pin)

#define LED_PIN PB6

volatile uint8_t BUTTONS = 0;


static void serial_init(unsigned int bittimer) {
    // set baud rate
    UBRR0H = (unsigned char) (bittimer >> 8);
    UBRR0L = (unsigned char) bittimer;

    // 8N1
    UCSR0C = (3 << UCSZ00);

    // make it so
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

static void serial_putchar(unsigned char input) {
    while (!(UCSR0A & (1 < UDRE0)));
    UDR0 = input;
}

static unsigned char serial_getchar(void) {
    // block until data available at UART
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

static int serial_readline(unsigned char* buff) {
    int i = 0;
    for (i = 0; i < 255; i++) {
        buff[i] = serial_getchar();
        if (((buff[i-1] == '\r') && buff[i] == '\n')
            || (buff[i] == '\0')) {
            break;
        }
    }
    if (buff[i] != '\0') {
        i++;
        buff[i] = '\0';
    }
    return i;
}

void setup(void) {
    set_output(DDRB, LED_PIN);

    set_input(DDRC, PC0);
    PORTC |= (1 << PORTC0);

    set_input(DDRC, PC1);
    PORTC |= (1 << PORTC1);

    set_input(DDRC, PC2);
    PORTC |= (1 << PORTC2);

    set_input(DDRC, PC3);
    PORTC |= (1 << PORTC3);

    LCDinit();
    LCDclr();
    LCDhome();
}

int main(void) {
  setup();

  uint8_t message[] = "Hello, world!";
  LCDstring(message, 12);
  LCDcursorOFF();

  while (1) {
      if (BUTTONS != PINC) {
          LCDGotoXY(0, 1);
          LCDsendChar(((PINC >> 2) & 0x1) + 48);
          LCDsendChar(((PINC >> 3) & 0x1) + 48);
          LCDsendChar(((PINC >> 0) & 0x1) + 48);
          LCDsendChar(((PINC >> 1) & 0x1) + 48);
          _delay_ms(100);
          BUTTONS = PINC;
      }
  }

  return 0;
}
