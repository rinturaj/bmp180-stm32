
#ifndef _I2CDRIVER_H_
#define _I2CDRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x.h"                  // Device header
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework

  
#define I2C                  I2C1
#define I2C_RCC_Periph       RCC_APB1Periph_I2C1
#define I2C_Port             GPIOB
#define SCL_Pin          GPIO_Pin_7
#define SDA_Pin          GPIO_Pin_6
#define I2C_RCC_Port         RCC_APB2Periph_GPIOB
#define I2C_Speed            100000 // 100kHz standard mode

  void init_i2c(void);
  void I2C_BufferRead(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead); 
  void I2C_WriteBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
  void I2C_ByteWrite(u8 slaveAddr, u8* pBuffer, u8 writeAddr);
  #ifdef __cplusplus
}
#endif

#endif /* _I2CDRIVER_H_ */
/**********END OF FILE****/
