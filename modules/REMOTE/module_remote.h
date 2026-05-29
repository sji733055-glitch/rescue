#ifndef __MODULE_REMOTE_H__
#define __MODULE_REMOTE_H__
#include "stdint.h"
typedef struct {
    uint8_t channels[4];
}Remote_data_t;
void Module_Remote_init(void);

#endif /* __MODULE_REMOTE_H__ */