#ifndef MLX90614_H_
#define MLX90614_H_

#include "stm32f10x.h"
#include "io_bit.h"
#include"sys.h"
#include"Delay.h"

#define MLX90614_SDA_in  PAin(2)
#define MLX90614_SDA_out  PAout(2)
#define MLX90614_SCL	PBout(9)
void MLX90614_IO_Init(void);
float MLX90614_read_value(void);
u8 MLX90614_SDA_receive_bit(void);





#endif
