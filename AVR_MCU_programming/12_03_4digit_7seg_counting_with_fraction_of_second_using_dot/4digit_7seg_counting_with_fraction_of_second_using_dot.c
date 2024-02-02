#define F_CPU 1000000UL  // set MK frequency to 1MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>  // size_t

void TurnOnDigit(int index_of_common_cathode_pin);
void TurnOnDot();
int GetDigitsFromNumberWithDotForTenthsOfSecond(int number);

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
static int MAX_COUNTS = 80;     // MAX_COUNTS_increments * T_compare_interrupt = 80 * 0.1s = 8 seconds
int cc_pins[4] = {0, 1, 2, 3};  // common cathodes pins for selected port (PB0, PB1, PB2, PB3)
int cc_idx = 0;  // index of currently active common cathode (GROUND will be applied to that pin: [0,1,2,3])

int digits_of_number[] = {0, 0, 0, 0};
const size_t d_length = sizeof(digits_of_number) / sizeof(int); // length == 4

int seconds_counter = 0;
int start_pos = 0; // position of first nonzero digit in number (i.e. skipping leading zeros in number) BEFORE dot
// =======================================================

//interrupt handler (on TCNT0 overflow) will be called each 2 milliseconds for each digit, so period T = 2ms*4digits = 8ms
ISR(TIMER0_OVF_vect) {
	// we don't want to display leading zeros in the number, so instead of "0089" the number "89" will be displayed
	if (cc_idx < start_pos) {
		cc_idx = start_pos;
	}

	TurnOnDigit(cc_idx);
	
	if (cc_idx == (d_length - 2)) {  // when cc_idx == 2 in case of 4-digit 7-segment indicator
		TurnOnDot();
	}
	
	cc_idx = (cc_idx + 1) % 4;
}

// interrupt handler (when TCNT1 becomes equal to OCR1A) will be called each 0.1 second
ISR(TIMER1_COMPA_vect) {
	if (++seconds_counter > MAX_COUNTS) {
		seconds_counter = 0;
		PORTC |= 1<<0;  // turn ON LED on PC0
		
		// prevent MCU resource consumption - stop stop all unnecessary timer/counters
		TCCR1B &= ~(1<<CS12 | 1<<CS11 | 1<<CS10); // stop Timer/Counter1 (works in Clear Timer on Compare (CTC) mode)
		
		// we can't stop Timer/Counter0 because it is responsible for dynamic displaying digits
		//TCCR0  &= ~(1<<CS02 | 1<<CS01 | 1<<CS00); // stop Timer/Counter0, so we prevent MCU resource consumption
	} else {
		start_pos = GetDigitsFromNumberWithDotForTenthsOfSecond(seconds_counter);
	}
	
}

// returns the position of the first non-zero digit before dot in a "d_length"-digit number
int GetDigitsFromNumberWithDotForTenthsOfSecond(int number) {
	if (number == 0) {
		digits_of_number[d_length - 1] = 0;
		digits_of_number[d_length - 2] = 0;
		return d_length - 2;
	}

	int i = d_length - 1; // initial value of i == 3
	
	while (number) {
		digits_of_number[i] = number % 10;
		number /= 10;
		
		if (number) {
			--i;
		}
	}
	
	return (i == (d_length - 1)) ? i-1 : i;
}

void TurnOnDigit(int cc_pin_index) {
	// supplied all common cathodes with HIGH potential, i.e. disable all digits on 4_digit_7seg_indicator
	CC_PORTx |= (1 << cc_pins[0]) | (1 << cc_pins[1])   | (1 << cc_pins[2])  | (1 << cc_pins[3]);

	// cc_pin_index specifies which position in number should be turned on
	int digit = digits_of_number[cc_pin_index];
	SEG7_PORTx = digits_7seg_CC[digit];

	// apply GROUND to desired common cathode pin (set '0' in corresponding bit in PORTx)
	CC_PORTx &= ~(1<<cc_pins[cc_pin_index]);
}

void TurnOnDot() {
	SEG7_PORTx |= 1<<7;
	
	// apply GROUND to 2-nd common cathode pin
	CC_PORTx &= ~(1<<cc_pins[2]);
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
	//// ====================== Configure Timer/Counter #0 =====================
	// Timer/Counter0 Control Register: enable running TCNT0 and set prescaler = clk/8
	// T_overflow = prescaler * max_TCNT0_val / freq_MCU = 8 * 255 / 10**6 = 0.00204 seconds ~ 2 miliseconds
	TCCR0 |= 1<<CS01;
	TCCR0 &= ~(1<<CS00);

	TCNT0 = 0;          // initialize counter with 0
	TIMSK |= 1<<TOIE0;  // Bit 0 – TOIE0: Timer/Counter0 Overflow Interrupt Enable
	//// ================ END of Timer/Counter TCNT0 configuration ================

	// ======================= Configure Timer/Counter #1 ======================
	// set Prescaler = clk/8 for Timer/Counter1; 1'000'000 / 8 = 125'000 Hz (125000 clocks(ticks) per second)
	TCCR1B |= 1<<CS11;

	TCNT1 = 0;

	// Output Compare Register, when TCNT1 becomes equal to OCR1A, interrupt will be generated
	// 125'000 clocks(ticks) correspond to 1 second
	// if OCR1A==125'000/10 = 12500, then interrupt on compare match will be triggered every 0.1 second
	// if OCR1A==125'000/100 = 1250, then interrupt on compare match will be triggered every 0.01 second
	// if OCR1A==125'000/10 = 125,   then interrupt on compare match will be triggered every 0.001 second (1 millisecond)

	OCR1A = 125000 / 10;  // interrupt on compare match (TIMER1_COMPA_vect) will be triggered every 0.1 second
	TIMSK |= 1<<OCIE1A;   // Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
	TCCR1B |= 1<<WGM12;   // reset TCNT1 immediately when match occured; mode #4: Clear Timer on Compare Match (CTC)
	// ================ END of Timer/Counter TCNT1 configuration ================

	sei();  // Status Register Bit 7 – I: Global Interrupt Enable
}

int main() {
	ConfigureSegmentsPins();
	ConfigureCommonCathodesPins();
	ConfigureTimerCounter();
	ConfigureStandaloneLED();

	start_pos = GetDigitsFromNumberWithDotForTenthsOfSecond(seconds_counter);

	while (true) {
		// all useful stuff is implemented in Interrupt Service Routines (ISR), or Interrupt Handlers
	}

	return 0;
}
