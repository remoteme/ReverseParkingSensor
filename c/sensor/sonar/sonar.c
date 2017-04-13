/*!
 * **********************************************************************************************
 * \file sonar.c
 * \brief Interfacing HC-SR04 Ultrasonic Sensor Module (Sonar)
 *
 * \author      :   Praveen Kumar
 * \date        :   Mar 24, 2014
 * Copyright(c)         :   Praveen Kumar - www.veerobot.com
 * Description          :   refer sonar.h
 *
 * LICENSE      :   Refer sonar.h
 *
 * **********************************************************************************************
 */

#include "sonar.h"


 #define MAX(a,b) ({a > b ? a : b; })
 #define MIN(a,b) ({a < b ? a : b; })

volatile uint32_t overFlowCounter = 0;
volatile uint32_t trig_counter = 0;
volatile uint32_t no_of_ticks = 0;

/********** ...- . . .-. --- -... --- - *********************************
 * Initiate Ultrasonic Module Ports and Pins
 * Input:   none
 * Returns: none
*********** ...- . . .-. --- -... --- - *********************************/
void init_sonar1(){
    TRIG_OUTPUT_MODE1();     // Set Trigger pin as output
    ECHO_INPUT_MODE1();      // Set Echo pin as input
}

void init_sonar2(){
    TRIG_OUTPUT_MODE2();     // Set Trigger pin as output
    ECHO_INPUT_MODE2();      // Set Echo pin as input
}

/********** ...- . . .-. --- -... --- - *********************************
 * Send 10us pulse on Sonar Trigger pin
 * 1.   Clear trigger pin before sending a pulse
 * 2.   Send high pulse to trigger pin for 10us
 * 3.   Clear trigger pin to pull it trigger pin low
 *  Input:   none
 *  Returns: none
********** ...- . . .-. --- -... --- - *********************************/
void trigger_sonar1(){
    TRIG_LOW1();             // Clear pin before setting it high
    _delay_us(1);           // Clear to zero and give time for electronics to set
    TRIG_HIGH1();            // Set pin high
    _delay_us(12);          // Send high pulse for minimum 10us
    TRIG_LOW1();             // Clear pin
    _delay_us(1);           // Delay not required, but just in case...
}

void trigger_sonar2(){
    TRIG_LOW2();             // Clear pin before setting it high
    _delay_us(1);           // Clear to zero and give time for electronics to set
    TRIG_HIGH2();            // Set pin high
    _delay_us(12);          // Send high pulse for minimum 10us
    TRIG_LOW2();             // Clear pin
    _delay_us(1);           // Delay not required, but just in case...
}

/********** ...- . . .-. --- -... --- - *********************************
 * Increment timer on each overflow
 * Input:   none
 * Returns: none
********** ...- . . .-. --- -... --- - *********************************/
ISR(TIMER1_OVF_vect){   // Timer1 overflow interrupt
    overFlowCounter++;
    TCNT1=0;
}

/********** ...- . . .-. --- -... --- - *********************************
 * Calculate and store echo time and return distance
 * Input:   none
 * Returns: 1. -1       :   Indicates trigger error. Could not pull trigger high
 *          2. -2       :   Indicates echo error. No echo received within range
 *          3. Distance :   Sonar calculated distance in cm.
********** ...- . . .-. --- -... --- - *********************************/
unsigned int read_sonar1(){
    int dist_in_cm = 0;
    overFlowCounter=0;
    trig_counter=0;
    init_sonar1();                       // Setup pins and ports
    trigger_sonar1();                    // send a 10us high pulse

    while(!(SONAR_PIN & (1<<ECHO_BIT1))){   // while echo pin is still low
        trig_counter++;
         uint32_t max_response_time = SONAR_TIMEOUT;
        if (trig_counter > max_response_time){   // SONAR_TIMEOUT
            return TRIG_ERROR;
        }
    }

    TCNT1=0;                            // reset timer
    TCCR1B |= (1<<CS10);              // start 16 bit timer with no prescaler
    TIMSK1 |= (1<<TOIE1);             // enable overflow interrupt on timer1
    overFlowCounter=0;                  // reset overflow counter
    sei();                              // enable global interrupts

    while((SONAR_PIN & (1<<ECHO_BIT1))){    // while echo pin is still high
        if (((overFlowCounter*TIMER_MAX)+TCNT1) > SONAR_TIMEOUT){
            return ECHO_ERROR;          // No echo within sonar range
        }
    };

    TCCR1B = 0x00;                      // stop 16 bit timer with no prescaler
    cli();                              // disable global interrupts
    no_of_ticks = ((overFlowCounter*TIMER_MAX)+TCNT1);  // counter count
    dist_in_cm = (no_of_ticks/(CONVERT_TO_CM*CYCLES_PER_US));   // distance in cm
    return (dist_in_cm );
}


unsigned int read_sonar2(){
    int dist_in_cm = 0;
    overFlowCounter=0;
      trig_counter=0;
    init_sonar2();                       // Setup pins and ports
    trigger_sonar2();                    // send a 10us high pulse

    while(!(SONAR_PIN & (1<<ECHO_BIT2))){   // while echo pin is still low
        trig_counter++;
         uint32_t max_response_time = SONAR_TIMEOUT;
        if (trig_counter > max_response_time){   // SONAR_TIMEOUT
            return TRIG_ERROR;
        }
    }

    TCNT1=0;                            // reset timer
    TCCR1B |= (1<<CS10);              // start 16 bit timer with no prescaler
    TIMSK1 |= (1<<TOIE1);             // enable overflow interrupt on timer1
    overFlowCounter=0;                  // reset overflow counter
    sei();                              // enable global interrupts

    while((SONAR_PIN & (1<<ECHO_BIT2))){    // while echo pin is still high
        if (((overFlowCounter*TIMER_MAX)+TCNT1) > SONAR_TIMEOUT){
            return ECHO_ERROR;          // No echo within sonar range
        }
    };

    TCCR1B = 0x00;                      // stop 16 bit timer with no prescaler
    cli();                              // disable global interrupts
    no_of_ticks = ((overFlowCounter*TIMER_MAX)+TCNT1);  // counter count
    dist_in_cm = (no_of_ticks/(CONVERT_TO_CM*CYCLES_PER_US));   // distance in cm
    return (dist_in_cm );
}


#define ARRAY_COUNT 14


int compare( const void* a, const void* b )
{
    if( *(unsigned int*)a == *(unsigned int*)b ) return 0;
    return *(unsigned int*)a < *(unsigned int*)b ? -1 : 1;
}


unsigned int countAvg2(unsigned int (*readPntr)()){
	unsigned int array[ARRAY_COUNT];



	for(int i=0;i<ARRAY_COUNT;i++){
		array[i]=readPntr();
		_delay_ms(20);
	}

	qsort(array,ARRAY_COUNT,sizeof(unsigned int),compare);

	int sum=0;
	for(int i=ARRAY_COUNT/2-2;i<ARRAY_COUNT/2+2;i++){
		sum+=array[i];

	}

	return (sum/(4));

}




unsigned int sonar1AVG(){
	return countAvg2(read_sonar1 );
}

unsigned int sonar2AVG(){
	return countAvg2(read_sonar2 );
}
