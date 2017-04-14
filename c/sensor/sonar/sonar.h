/*!
 * ********************************************************************************************************
 * \file sonar.h
 * \brief Interfacing HC-SR04 Ultrasonic Sensor Module (Sonar)
 *
 * \author      :   Praveen Kumar
 * \date        :   Mar 24, 2014
 * Copyright(c) :   Praveen Kumar - www.veerobot.com
 * Description  :   Interfacing HC-SR04 Ultrasonic Sensor Module (Sonar). Program is tested on
 *                  Draco - AVR Development board available at www.veerobot.com/store which has an
 *                  ATmega328P microcontroller. If you replace that with any other 28 pin AVR
 *                  microcontroller, be sure to modify registers accordingly.
 *
 * LICENSE      :   Redistribution and use in source and/or binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain this copyright notice, list of conditions and disclaimer.
 * - Redistributions in binary form must reproduce this copyright notice, list of conditions and disclaimer in
 *      documentation and/or other materials provided with the distribution.
 *
 * DISCLAIMER   :   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" WITHOUT ANY
 * KIND OF WARRANTIES. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE
 *
 * ********************************************************************************************************
 */
#ifndef SONAR_H_
#define SONAR_H_



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*...- . . .-. --- -... --- -
 * Define Ports and Pins as required
 * Modify Maximum response time and delay as required
 * MAX_RESP_TIME : default: 300
 * DELAY_BETWEEN_TESTS : default: 50
 */
#define SONAR_DDR    DDRB            // Trigger Port
#define SONAR_PORT   PORTB
#define SONAR_PIN    PINB



#define TRIG_BIT1    PB0             // Trigger Pin
#define ECHO_BIT1    PB2             // Echo Pin

#define TRIG_BIT2    PB1            // Trigger Pin
#define ECHO_BIT2    PB5             // Echo Pin

#define TRIG_BIT3    PB4            // Trigger Pin
#define ECHO_BIT3    PB3             // Echo Pin
// Speed of sound
// Default: 343 meters per second in dry air at room temperature (~20C)
#define SPEED_OF_SOUND  343
#define MAX_SONAR_RANGE 10          // This is trigger + echo range (in meters) for SR04
#define DELAY_BETWEEN_TESTS 500     // Echo canceling time between sampling. Default: 500us
#define TIMER_MAX 65535             // 65535 for 16 bit timer and 255 for 8 bit timer

/* ...- . . .-. --- -... --- -
 * Do not change anything further unless you know what you're doing
 * */
#define TRIG_ERROR -1000
#define ECHO_ERROR -2000

#define CYCLES_PER_US (F_CPU/1000000)// instructions per microsecond
#define CYCLES_PER_MS (F_CPU/1000)      // instructions per millisecond
// Timeout. Decreasing this decreases measuring distance
// but gives faster sampling
#define SONAR_TIMEOUT ((F_CPU*MAX_SONAR_RANGE)/SPEED_OF_SOUND)

#define TRIG_INPUT_MODE1() SONAR_DDR &= ~(1<<TRIG_BIT1)
#define TRIG_OUTPUT_MODE1() SONAR_DDR |= (1<<TRIG_BIT1)

#define TRIG_INPUT_MODE2() SONAR_DDR &= ~(1<<TRIG_BIT2)
#define TRIG_OUTPUT_MODE2() SONAR_DDR |= (1<<TRIG_BIT2)

#define TRIG_INPUT_MODE3() SONAR_DDR &= ~(1<<TRIG_BIT3)
#define TRIG_OUTPUT_MODE3() SONAR_DDR |= (1<<TRIG_BIT3)

#define TRIG_LOW1() SONAR_PORT &= ~(1<<TRIG_BIT1)
#define TRIG_HIGH1() SONAR_PORT |=(1<<TRIG_BIT1)

#define TRIG_LOW2() SONAR_PORT &= ~(1<<TRIG_BIT2)
#define TRIG_HIGH2() SONAR_PORT |=(1<<TRIG_BIT2)

#define TRIG_LOW3() SONAR_PORT &= ~(1<<TRIG_BIT3)
#define TRIG_HIGH3() SONAR_PORT |=(1<<TRIG_BIT3)

#define ECHO_INPUT_MODE1() SONAR_DDR &= ~(1<<ECHO_BIT1)
#define ECHO_OUTPUT_MODE1() SONAR_DDR |= (1<<ECHO_BIT1)

#define ECHO_INPUT_MODE2() SONAR_DDR &= ~(1<<ECHO_BIT2)
#define ECHO_OUTPUT_MODE2() SONAR_DDR |= (1<<ECHO_BIT2)

#define ECHO_INPUT_MODE3() SONAR_DDR &= ~(1<<ECHO_BIT3)
#define ECHO_OUTPUT_MODE3() SONAR_DDR |= (1<<ECHO_BIT3)


#define ECHO_LOW() SONAR_PORT &= ~(1<<ECHO_BIT)
#define ECHO_HIGH() SONAR_PORT |=(1<<ECHO_BIT)

#define ECHO_LOW() SONAR_PORT &= ~(1<<ECHO_BIT)
#define ECHO_HIGH() SONAR_PORT |=(1<<ECHO_BIT)

#define CONVERT_TO_CM ((10000*2)/SPEED_OF_SOUND)    // or simply 58

/** ...- . . .-. --- -... --- -
 * @brief   Initiate Ports for Trigger and Echo pins
 * @param   void
 * @return  none
*/
void init_sonar1();
void init_sonar2();
void init_sonar3();
/**  ...- . . .-. --- -... --- -
 * @brief   Send 10us pulse on Ultrasonic Trigger pin
 * @param   void
 * @return  none
*/
void trigger_sonar1();
void trigger_sonar2();
void trigger_sonar3();
/**  ...- . . .-. --- -... --- -
 * @brief   Calculate and store echo time and return distance
 * @param   void
 * @return  unsigned int
 * Usage    int foo = read_sonar();
*/
unsigned int read_sonar1();
unsigned int read_sonar2();
unsigned int read_sonar3();

unsigned int sonar1AVG();
unsigned int sonar2AVG();
unsigned int sonar3AVG();
#endif /* SONAR_H_ */
