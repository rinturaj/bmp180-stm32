# bmp180-stm32

bmp180 library for stm32f103 devices . 
temperature calculation not included into this files 

if you need those code you can use the below code as an example

float temperature(void)
{
  bmp.B5  = (((int32_t)ucomp.ut - (int32_t)bmp.AC6) * (int32_t)bmp.AC5) >> 15;
	bmp.B5 += ((int32_t)bmp.MC << 11) / (bmp.B5 + bmp.MD);

	return (bmp.B5 + 8) >> 4
}
