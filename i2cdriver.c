

#include "i2cdriver.h"



void init_i2c()
{
    I2C_InitTypeDef I2C_Structure;
    GPIO_InitTypeDef GPIO_Structure;
  RCC_APB1PeriphResetCmd(I2C_RCC_Periph, ENABLE);
  RCC_APB1PeriphResetCmd(I2C_RCC_Periph, DISABLE);
    /* Enable I2C and GPIO clocks */
    RCC_APB1PeriphClockCmd(I2C_RCC_Periph, ENABLE);
    RCC_APB2PeriphClockCmd(I2C_RCC_Port, ENABLE);

    /* Configure I2C pins: SCL and SDA */
    GPIO_Structure.GPIO_Pin = SCL_Pin | SDA_Pin;
    GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_OD;
    

    /* I2C configuration */
    I2C_Structure.I2C_Mode = I2C_Mode_I2C;
    I2C_Structure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_Structure.I2C_OwnAddress1 =  1;
    I2C_Structure.I2C_Ack = I2C_Ack_Enable;
    I2C_Structure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Structure.I2C_ClockSpeed = I2C_Speed;
    
    GPIO_Init(GPIOB, &GPIO_Structure);
    /* Apply I2C configuration after enabling it */
    I2C_Init(I2C, &I2C_Structure);
    /* I2C Peripheral Enable */
    I2C_Cmd(I2C, ENABLE);
    
  //option use led 
}

/**
 * @brief  Writes one byte to the  i2c device.
 * @param  slaveAddr : slave address i2c device_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer  containing the data to be written to the i2c device.
 * @param  writeAddr : address of the register in which the data will be written
 * @return None
 */
void I2C_ByteWrite(u8 slaveAddr, u8* pBuffer, u8 writeAddr)
{
  
    /* Send START condition */
    I2C_GenerateSTART(I2C, ENABLE);
   
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));
    
     /* Send  address for write */
    I2C_Send7bitAddress(I2C, slaveAddr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send the internal address to write to */
    I2C_SendData(I2C, writeAddr);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send the byte to be written */
    I2C_SendData(I2C, *pBuffer);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C, ENABLE);


}

void I2C_BufferRead(u8 slaveAddr, u8* pBuffer, u8 readAddr, u16 NumByteToRead)
{

    /* While the bus is busy */
    while (I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2C, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send address for write */
    I2C_Send7bitAddress(I2C, slaveAddr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C, ENABLE);

    /* Send the  internal address to write to */
    I2C_SendData(I2C, readAddr);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send  address for read */
    I2C_Send7bitAddress(I2C, slaveAddr, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while (NumByteToRead)
    {
        if (NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the MPU6050 */
            *pBuffer = I2C_ReceiveData(I2C);

            /* Point to the next location where the byte read will be saved */
            pBuffer++;

            /* Decrement the read bytes counter */
            NumByteToRead--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C, ENABLE);
   
}
void I2C_WriteBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t tmp;uint8_t mask;
    I2C_BufferRead(slaveAddr,&tmp, regAddr,1);
    mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    tmp &= ~(mask); // zero all important bits in existing byte
    tmp |= data; // combine data with existing byte
    I2C_ByteWrite(slaveAddr, &tmp, regAddr);
}
