/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-29 10:29:53
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-29 14:08:06
 * @FilePath: \rescue\modules\module_init.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "module_init.h"


#include "module_remote.h"
#include "module_motor.h"



#define LOG_LVL LOG_LVL_INFO
#define LOG_TAG "Car Init"
#include "ulog_def.h"


void MODULE_Init()
{
    Module_Remote_init();
    Module_Motor_init();
    LOG_I("Modules init Finished ");
}