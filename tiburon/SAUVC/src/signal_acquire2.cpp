#include <ros/ros.h>
#include <fstream>
/*	gcc adc.c -lbcm2835       */
#include <stdio.h> //time library
#include <bcm2835.h>
#include<std_msgs/Float32MultiArray.h>
#include<std_msgs/Float32.h>
#include <stdlib.h>
#include <cstdlib>
using namespace std;
#define ADC_CONVST RPI_BPLUS_GPIO_J8_35
#define ADC_DIN RPI_BPLUS_GPIO_J8_37

#define initial_time 2 //(in secs);
#define time_delay 0.0//(this needs to be changed as per peak detection)
#define batch_time 0.5// the sample time for the batching
bool initialize = true;
bool delay = false;
ros::Publisher pub;

void get(){
    if (!bcm2835_init()){  printf("bcm2835_init failed.\n");
        	exit(0);}
    	if (!bcm2835_spi_begin()){ printf("bcm2835_spi_begin failed.\n");
		exit(0);}
    	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
    	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4);
    	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    	bcm2835_gpio_fsel(ADC_DIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(ADC_CONVST, BCM2835_GPIO_FSEL_OUTP);
	uint8_t read_data;
	uint8_t read_data1;
	uint8_t read_data2;
	long 	adcbinarydata;
	float 	adcvoltage;

    	bcm2835_gpio_write(ADC_DIN, LOW);
    	bcm2835_gpio_write(ADC_CONVST, LOW);
	double MAX_ADC_VAL = (1 << 18) - 1;

    ofstream test;
	test.open("Output_07_11_18_sine53kHzfg.txt"); //change this to store commandline
	printf("test initialized");
	clock_t begin_t = clock();
	// long len = 1e5;// a random declaration of the max limit
	// long adcdata[len];
    std_msgs::Float32MultiArray array;
    array.data.clear();
 	do// reason for the code???
    {
		bcm2835_gpio_write(ADC_DIN, HIGH);
    }while((clock() - begin_t)/CLOCKS_PER_SEC < batch_time);
           //bcm2835_spi_transfer(0);
//    		bcm2835_delayMicroseconds(1);
	// cout << "Time for one spi byte transfer in microseconds " << time_us/len  <<endl;
    // double time_us = (clock() - begin_t)*1e6/CLOCKS_PER_SEC;

begin_t = clock();
 	do //change this as a function of time then len
	{

    		bcm2835_gpio_write(ADC_DIN, HIGH);
    		bcm2835_gpio_write(ADC_CONVST, HIGH);
    		bcm2835_spi_transfer(0);
//    		bcm2835_delayMicroseconds(1);
    		bcm2835_gpio_write(ADC_DIN, LOW);
    		read_data  = bcm2835_spi_transfer(0);
		read_data1 = bcm2835_spi_transfer(0);
		read_data2 = bcm2835_spi_transfer(0);
		read_data = read_data & 0x7F;
		adcbinarydata = (read_data << 11) + (read_data1 << 3) + (read_data2 >> 5);
		array.data.push_back(adcbinarydata); //storing read_data
	//	printf("%ld \n",adcbinarydata);
	//	adcvoltage = (double)adcbinarydata*4.83/MAX_ADC_VAL;

    		bcm2835_gpio_write(ADC_CONVST, LOW);
//    		bcm2835_spi_transfer(0);
		//test<<adcbinarydata<<endl;
    		//bcm2835_delayMicroseconds(1);
	}while((clock() - begin_t)/CLOCKS_PER_SEC < batch_time);

    pub.publish(array);
	cout << CLOCKS_PER_SEC*(float)batch_time/(clock() - begin_t) << "Hz" << endl;
    for(std::vector<float>::const_iterator it = array.data.begin(); it != array.data.end(); ++it)
    {
    test<<*it<<endl;
    }
	test.close();
    	bcm2835_spi_end();
    	bcm2835_close();

}

 int main(int argc, char **argv)
 {
     ros::init(argc, argv, "signal_acquire");
     ros::NodeHandle nh;
     ros::Publisher pub = nh.advertise<std_msgs::Float32MultiArray>("signal_acquire", 1000);

     while (ros::ok())
 	{

 		get();
 		ROS_INFO("I published something!");
 		ros::spinOnce();
        sleep(2);
    }

     exit(0);
     return 0;
 }
