#include "bsp_init.h"

#include "bsp_dwt.h"
#define LOG_TAG "bsp_init"
#define LOG_LVL LOG_LVL_INFO
#include "ulog_def.h"


void BSP_Init()
{
    BSP_DWT_Init(168);
    
}