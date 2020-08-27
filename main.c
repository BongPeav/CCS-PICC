/*
 * i2c_ds3232.c
 *
 * Created: 7/28/2020 3:57:57 PM
 * Author : bonTha
 */ 

#include <avr/io.h>

#define F_CPU 4000000UL
#include <util/delay.h>

void i2cInit(void){
	TWSR=0x00; //Set The Prescaler Bit To Zero
	TWBR=0x47; //SCL Frequency Is 50k @ 8MHz
	TWCR=0x04; //Enable The TWI Module
}

void i2cStart(void){
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}

void i2cWrite(unsigned char data){
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}

unsigned char i2cRead(unsigned char isLast){
	if(isLast==0)
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	else
	TWCR=(1<<TWINT)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
	return TWDR;
}

void i2cStop(){
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

int main(void)
{
	DDRA=0xFF;
	DDRB=0xFF;
	DDRD=0xFF;
	i2cInit();

	i2cStart();
	i2cWrite(0b11010000);
	i2cWrite(0x0E);
	i2cWrite(0b00100000);
	i2cStop();

	while (1)
	{
		i2cStart();
		i2cWrite(0b11010000);
		i2cWrite(0x00);
		i2cStop();

		
		i2cStart();
		i2cWrite(0b11010001);
		PORTA=i2cRead(1);
		i2cStop();
		
		i2cStart();
		i2cWrite(0b11010001);
		PORTB=i2cRead(1);
		i2cStop();

		i2cStart();
		i2cWrite(0b11010000);
		i2cWrite(0x11);
		i2cStop();

		i2cStart();
		i2cWrite(0b11010001);
		PORTD=i2cRead(1);
		i2cStop();

		_delay_ms(1000);
	}
}
