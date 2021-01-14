            /*	gcc adc.c -lbcm2835       */
#include<iostream>
#include<fstream>
#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <ctime>

#define ADC_CONVST RPI_BPLUS_GPIO_J8_35 
#define ADC_DIN_1 RPI_BPLUS_GPIO_J8_33
#define ADC_DIN_2 RPI_BPLUS_GPIO_J8_37
using namespace std;

int main(){
	//cout<<BCM2835_SPI_CLOCK_DIVIDER_4<<endl;
	//cout<<CLOCKS_PER_SEC<<endl;
	
	if (!bcm2835_init()){  printf("bcm2835_init failed.\n");
        	exit(0);}
    	if (!bcm2835_spi_begin()){ printf("bcm2835_spi_begin failed.\n");
		exit(0);}
    	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);  
    	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4);
    	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);         
    	bcm2835_gpio_fsel(ADC_DIN_1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(ADC_DIN_2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(ADC_CONVST, BCM2835_GPIO_FSEL_OUTP);
	uint8_t read_data;
	uint8_t read_data1;
	uint8_t read_data2;
	long 	adcbinarydata;
	float 	adcvoltage;
        
	
    	bcm2835_gpio_write(ADC_DIN_1, LOW);
	bcm2835_gpio_write(ADC_DIN_2, LOW);
    	bcm2835_gpio_write(ADC_CONVST, LOW);
	double MAX_ADC_VAL = (1 << 18) - 1;

	ofstream test;
	test.open("Output_record_29_10_2018_take_4_sinewave_50k.txt");
	clock_t begin_t = clock();
	long len = 1e5;  //changed from 1e5 to 1e6
	long adcdata[len];
 	for(long i=0; i<len; i++)
		bcm2835_gpio_write(ADC_DIN_1, HIGH);
    		//bcm2835_spi_transfer(0);
//    		bcm2835_delayMicroseconds(1);
	double time_us = (clock() - begin_t)*1e6/CLOCKS_PER_SEC;
	cout << "Time for one spi byte transfer in microseconds " << time_us/len  << endl;
	
	begin_t = clock();
 	for(long i=0; i<len; i++)
  	//while(1)      
	{

    		bcm2835_gpio_write(ADC_DIN_1, HIGH);
    		bcm2835_gpio_write(ADC_CONVST, HIGH);
    		bcm2835_spi_transfer(0);
//    		bcm2835_delayMicroseconds(1);
    		bcm2835_gpio_write(ADC_DIN_1, LOW);
    		read_data  = bcm2835_spi_transfer(0);
		read_data1 = bcm2835_spi_transfer(0);
		read_data2 = bcm2835_spi_transfer(0);
		read_data = read_data & 0x7F;
		adcbinarydata = (read_data << 11) + (read_data1 << 3) + (read_data2 >> 5);
		adcvoltage = (double)adcbinarydata*4.77/MAX_ADC_VAL;
		adcdata[i] = adcbinarydata;
		printf("%f \n", adcvoltage);


		bcm2835_gpio_write(ADC_DIN_2, HIGH);
    		bcm2835_gpio_write(ADC_CONVST, HIGH);
    		bcm2835_spi_transfer(0);
//    		bcm2835_delayMicroseconds(1);
    		bcm2835_gpio_write(ADC_DIN_2, LOW);
    		read_data  = bcm2835_spi_transfer(0);
		read_data1 = bcm2835_spi_transfer(0);
		read_data2 = bcm2835_spi_transfer(0);

		read_data = read_data & 0x7F;
		adcbinarydata = (read_data << 11) + (read_data1 << 3) + (read_data2 >> 5);
		adcvoltage = (double)adcbinarydata*4.77/MAX_ADC_VAL;
		adcdata[i+1] = adcbinarydata;
		printf("%f \n", adcvoltage);




	//	printf("%ld \n",adcbinarydata);
	//	adcvoltage = (double)adcbinarydata*4.83/MAX_ADC_VAL;

    		bcm2835_gpio_write(ADC_CONVST, LOW);
//    		bcm2835_spi_transfer(0);
		//test<<adcbinarydata<<endl;
    		//bcm2835_delayMicroseconds(1);
	}	
	cout << CLOCKS_PER_SEC*(float)len/(clock() - begin_t) << "Hz" << endl;
    	for(long i=0; i<len; i++)
	{
		test<<adcdata[i]<<endl;
	}
	test.close();
    	bcm2835_spi_end();
    	bcm2835_close();
	return 0;
}
