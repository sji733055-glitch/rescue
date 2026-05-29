#ifndef __PS2_H__
#define __PS2_H__

#include "stdint.h"
#include "module_remote.h"

int8_t PS2_Device_Init();
void PS2_decode(Remote_data_t *data);

#endif /* __MODULE_PS2_H__ */