

#ifndef F_CPU
    #define F_CPU 1000000UL     // CPU Frequency
#endif


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcdpcf8574/lcdpcf8574.h"

#include "nrf24/nrf24.h"

#define UART_BAUD_RATE 2400



#ifndef F_CPU
    #define F_CPU 3000000UL     // CPU Frequency
#endif





static uint8_t tx_address[5] = { 0x0, 0x0, 0x0, 0x17, 0x0 }; //0x17 = 23 in dec   19=25
static uint8_t deviceAddress[5] = { 0x0, 0x0, 0x0, 0x18, 0x0 };


int receive(uint8_t* buff) {

	if (nrf24_dataReady()) {
		nrf24_getData(buff);
		return 1;
	}
	return 0;

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

void message(const char *s){
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts(s);
	_delay_ms(500);
	lcd_clrscr();
}

unsigned int normalize(unsigned int i){
	if (i>999){
		return 0;
	}else if (i<0){
		return 0;
	}else{
		return i;
	}
}


int main(void)
{



    lcd_init(LCD_DISP_ON);

    //lcd go home
    lcd_home();
	lcd_led(0); //set led


	message("Witam krolika.");
	_delay_ms(500);
	initRadio();

	message("radio zainicjalizowane");

	_delay_ms(500);

	unsigned int data[16];




	int i=0;
    while(1) {



    	while (receive(data)) {//something receive



       		lcd_gotoxy(0,0);
       		lcd_put_int3(normalize(data[0]));

       		lcd_gotoxy(5,0);
       		lcd_put_int3(normalize(data[1]));


       		lcd_gotoxy(0,1);
       		lcd_put_int3(normalize(data[2]));

       		lcd_gotoxy(15,0);
       		lcd_put_int(i++%10);

       	}



       	 _delay_ms(10);


    }
}


