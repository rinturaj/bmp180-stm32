
#include<stdio.h>
#include<math.h>
#include<stdbool.h>

#include "bmp180.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

Bmpdata bmp;

void BMP180_init() {
  u8 temp = 0xb6;
  I2C_ByteWrite(BMP180_ADDR ,&temp ,BMP180_SOFT_RESET_REG);
  osDelay(10);
  bmp = BMP_data_value();
}


Bmpdata BMP_data_value(void){
  
  
  uint8_t buffer[BMP180_PROM_DATA_LEN];
	
  I2C_BufferRead(BMP180_ADDR , buffer, BMP180_PROM_START_ADDR, BMP180_PROM_DATA_LEN); 
  
  bmp.AC1 = (buffer[0]  << 8) | buffer[1];
	bmp.AC2 = (buffer[2]  << 8) | buffer[3];
	bmp.AC3 = (buffer[4]  << 8) | buffer[5];
	bmp.AC4 = (buffer[6]  << 8) | buffer[7];
	bmp.AC5 = (buffer[8]  << 8) | buffer[9];
	bmp.AC6 = (buffer[10] << 8) | buffer[11];
	bmp.B1  = (buffer[12] << 8) | buffer[13];
	bmp.B2  = (buffer[14] << 8) | buffer[15];
	bmp.MB  = (buffer[16] << 8) | buffer[17];
	bmp.MC  = (buffer[18] << 8) | buffer[19];
	bmp.MD  = (buffer[20] << 8) | buffer[21];  
  
return bmp;
}
uncompressed ucomp;

uncompressed ut_and_up(void){
  //up 
  uint8_t buffer1[3];
  u8 temp; 
  // to measure p with 4.3ms delay
  temp = BMP180_P0_MEASURE;
  I2C_ByteWrite(BMP180_ADDR, &temp, BMP180_CTRL_MEAS_REG);
  delay(5);
  I2C_BufferRead(BMP180_ADDR , buffer1, BMP180_ADC_OUT_MSB_REG, 2); 
  
  ucomp.up = ((buffer1[0]<<16)  | buffer1[1]<<8 | buffer1[2])>>8;
  return  ucomp;
}

float altitude(void){
  
  int32_t B3,B6,X3,p,pre;
	uint32_t B4,B7;
    // uncompressed ut and up 
  ucomp =  ut_and_up();
  
  B6 = bmp.B5 - 4000;
  X3 = ((bmp.B2 * ((B6 * B6) >> 12)) >> 11) + ((bmp.AC2 * B6) >> 11);
  B3 = (((((int32_t)bmp.AC1) * 4 + X3) << 0) + 2) >> 2;
	X3 = (((bmp.AC3 * B6) >> 13) + ((bmp.B1 * ((B6 * B6) >> 12)) >> 16) + 2) >> 2;
	B4 = (bmp.AC4 * (uint32_t)(X3 + 32768)) >> 15;
	B7 = ((uint32_t)ucomp.up - B3) * (50000 >> 0);
	if (B7 < 0x80000000){ p = (B7 << 1) / B4;} else {p = (B7 / B4) << 1;}
	pre= p +(((((p >> 8) * (p >> 8) * BMP180_PARAM_MG) >> 16) + ((BMP180_PARAM_MH * p) >> 16) + BMP180_PARAM_MI) >> 4);
  //return p;
  pre=pre/100;
  return (44330 * (1 - pow(( pre / 1013.25),1/5.255)));
  
  
}
void delay(int ms){
  int i,j;
  for(i=0;i<=1000*ms;i++)
     {
}}
