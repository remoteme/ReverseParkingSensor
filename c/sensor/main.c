
#ifndef F_CPU
#define F_CPU 1600000UL     // CPU Frequency
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "nrf24/nrf24.h"
#include "sonar/sonar.h"

static uint8_t tx_address[5] = { 0x0, 0x0, 0x0, 0x18, 0x0 };
static uint8_t deviceAddress[5] = { 0x0, 0x0, 0x0, 0x17, 0x0 }; //0x17 = 23 in dec   19=25




int receive(uint8_t* buff) {

	if (nrf24_dataReady()) {
		nrf24_getData(buff);
		return 1;
	}
	return 0;

}

void diodeOnOff(int on) {
	if (on){
		PORTD |= (1 << 7);
	}else{
		PORTD &= ~(1 << 7);
	}
}


void blink(int times,int onDelay, int offDelay) {
	for(int i=0;i<times;i++){
		diodeOnOff(1);
		_delay_ms(onDelay);
		diodeOnOff(0);
		_delay_ms(offDelay);
	}
}

int send(uint8_t data_array[32]){
	nrf24_send(data_array);
	while(nrf24_isSending());


	uint8_t temp = nrf24_lastMessageStatus();

	int ret=0;
	if(temp == NRF24_TRANSMISSON_OK)
	{
		ret=1;
	}
	else if(temp == NRF24_MESSAGE_LOST)
	{
		ret=0;
	}
	nrf24_retransmissionCount();

	_delay_ms(50);
	nrf24_powerUpRx();

	return ret;
}

void initRadio() {

	nrf24_init();

	/* Set the device addresses */

	nrf24_tx_address(tx_address);
	nrf24_rx_address(deviceAddress);

	/* Channel #2 , payload length: 4 */
	nrf24_config(1, 32);

}

void initKeepAlivePorts() {
	DDRD = 0xFF; //Nakes PORTC as Output
	PORTD &= ~(1 << 6); //uphold low for keeping ON
}

void turnOffKeepAlive() {
	DDRD &= ~(1 << 6); //6 as input
	PORTD |= (1 << 6); //high impedance
}



int main(void) {
	initKeepAlivePorts();



	init_sonar1();

	init_sonar2();
	init_sonar3();



	initRadio();

	blink(1,500,500);

	unsigned int data[16];
	data[0]=0;
	data[1]=0;


	int i=20;


	while (1) {

		data[0]=sonar1AVG();
		data[1]=sonar2AVG();
		data[2]=sonar3AVG();



		 if (send(data)){
			 blink(1,40,40);
			 i=20;
		 }

		 i--;
		 if (i==0){
			 blink(3,100,10);
			 nrf24_powerDown();
			 turnOffKeepAlive();
		 }


	}


}

