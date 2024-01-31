#define F_CPU 1000000UL  // set MK frequency to 1MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>  // size_t

void TurnOnDigit(int index_of_common_cathode_pin);
int GetDigitsFromNumber(int number);

// use port D to control all of 7 segments: a, b, c, d, e, f, g
#define SEG7_DDRx  DDRD
#define SEG7_PORTx PORTD

// use port B to control 4 common cathodes in case of 4-digit 7-segment indicator
#define CC_DDRx DDRB
#define CC_PORTx PORTB

int digits_7seg_CC[] = {  // PORTX values to highlight digits in 7-segment indicator, Common Cathode (CC)
	0b00111111,  // highlight "0"  Common Cathode (CC)
	0b00000110,  // highlight "1"  Common Cathode (CC)
	0b01011011,  // highlight "2"  Common Cathode (CC)
	0b01001111,  // highlight "3"  Common Cathode (CC)
	0b01100110,  // highlight "4"  Common Cathode (CC)
	0b01101101,  // highlight "5"  Common Cathode (CC)
	0b01111101,  // highlight "6"  Common Cathode (CC)
	0b00000111,  // highlight "7"  Common Cathode (CC)
	0b01111111,  // highlight "8"  Common Cathode (CC)
	0b01101111,  // highlight "9"  Common Cathode (CC)
	0b10000000   // highlight "dp/colon" Common Cathode (CC)
};

// ================== Global variables ==================
static int MAX_COUNTS = 100;
int cc_pins[4] = {0, 1, 2, 3};  // common cathodes pins for selected port (PB0, PB1, PB2, PB3)
int cc_idx = 0;  // index of currently active common cathode (GROUND will be applied to that pin: [0,1,2,3])

int digits_of_number[] = {0, 0, 0, 0};
const size_t d_length = sizeof(digits_of_number) / sizeof(int); // length == 4

int seconds_counter = 0;
int start_pos = 0; // position of first nonzero digit in number (i.e. skipping leading zeros in number)

// =======================================================

//interrupt handler will be called each 2 milliseconds for each digit, so period T = 2ms*4digits = 8ms
ISR(TIMER0_OVF_vect) {
	if (cc_idx < start_pos) {
		cc_idx = start_pos;
	}
	
	TurnOnDigit(cc_idx);
	cc_idx = (cc_idx + 1) % 4;
}

// interrupt handler will be called each 1 second
ISR(TIMER1_COMPA_vect) {
	seconds_counter = (seconds_counter + 1) % (MAX_COUNTS + 1);
	start_pos = GetDigitsFromNumber(seconds_counter);
	PORTC ^= 1<<0;  // toggle LED connected to PC0
}

// returns the position of the first non-zero digit (or pos = d_length-1 if number == 0) in a "d_length"-digit number
int GetDigitsFromNumber(int number) {
	if (!number) {
		return d_length - 1;
	}

	int i = d_length - 1; // initial value of i == 3 in case of 4-digit 7-segment indicator
	int pos = i;

	while (number) {
		digits_of_number[i--] = number % 10;
		number /= 10;
		
		if (number) {
			--pos;
		}
	}

	return pos;
}

void TurnOnDigit(int cc_pin_index) {
	// supplied all common cathodes with HIGH potential, i.e. disable all digit on 4_digit_7seg_indicator
	CC_PORTx |= (1 << cc_pins[0]) | (1 << cc_pins[1])   | (1 << cc_pins[2])  | (1 << cc_pins[3]);
	
	// cc_pin_index specifies which position in number should be turned on
	int digit = digits_of_number[cc_pin_index];
	SEG7_PORTx = digits_7seg_CC[digit];
	
	// apply GROUND to desired common cathode pin (set '0' in corresponding bit in PORTx)
	CC_PORTx &= ~(1<<cc_pins[cc_pin_index]);
}

void ConfigureSegmentsPins() {
	SEG7_DDRx = 0xFF;  // all pins of port D set to OUTPUT
	SEG7_PORTx = 0x00; // all pins of port D set to LOW potential: by default all segments are turned OFF
}

void ConfigureCommonCathodesPins() {
	// configure all 4 pins corresponding to common cathodes (arr_cc[i]) as OUTPUT
	CC_DDRx |= (1 << cc_pins[0]) | (1 << cc_pins[1])   | (1 << cc_pins[2])  | (1 << cc_pins[3]);

	// configure all 4 common cathodes pins(cc[i]) as HIGH potential ==> so digits will be turned OFF by default
	// i.e. to turn ON desired digit we should apply GROUND ("-") to corresponding common cathode pin
	CC_PORTx |= (1 << cc_pins[0]) | (1 << cc_pins[1])   | (1 << cc_pins[2])  | (1 << cc_pins[3]);
}

void ConfigureStandaloneLED() {
	DDRC |= 1<<0;      // set PC0 as OUTPUT
	PORTC &= ~(1<<0);  // set PC0 as LOW level
}

void ConfigureTimerCounter() {
	//// ====================== Configure Timer/Counter TCNT0 =====================
	// Timer/Counter0 Control Register: enable running TCNT0 and set prescaler = clk/8
	// T_overflow = prescaler * max_TCNT0_val / freq_MCU = 8 * 255 / 10**6 = 0.00204 seconds ~ 2 miliseconds
	TCCR0 |= 1<<CS01;
	TCCR0 &= ~(1<<CS00);
	
	TCNT0 = 0;          // initialize counter with 0
	TIMSK |= 1<<TOIE0;  // Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable
	//// ================ END of Timer/Counter TCNT0 configuration ================
	
	// ======================= Configure Timer/Counter TCNT1 ======================
	// set Prescaler (clk/64) for Timer/Counter1; 1'000'000 / 64 = 15'625 Hz ()
	TCCR1B |= 1<<CS11 | 1<<CS10;
	TCCR1B &= ~(1<<CS12);
	
	TCNT1 = 0;
	OCR1A = 15625;       // Output Compare Register, when TCNT1 becomes equal to OCR1AL, interrupt will occur
	TIMSK |= 1<<OCIE1A;  // Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TCCR1B |= 1<<WGM12;  // reset TCNT1 immediately when match occured; mode #4: Clear Timer on Compare Match (CTC)
	
	// ================ END of Timer/Counter TCNT1 configuration ================
	
	sei();  // Status Register Bit 7 – I: Global Interrupt Enable
}



int main() {
	ConfigureSegmentsPins();
	ConfigureCommonCathodesPins();
	ConfigureTimerCounter();
	ConfigureStandaloneLED();

	start_pos = GetDigitsFromNumber(seconds_counter);
	
	while (true) {
		// all useful stuff is implemented in Interrupt Service Routines (ISR), or Interrupt Handlers
	}

	return 0;
}

