/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-27 19:56:08
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-30 15:26:48
 * @FilePath: \rescue\modules\REMOTE\module_remote.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MODULE_REMOTE_H__
#define __MODULE_REMOTE_H__
#include "stdint.h"
typedef struct {
    uint8_t channels[2];
}Remote_data_t;
void Module_Remote_init(void);
Remote_data_t *Module_Get_Data(void);
uint16_t Module_Get_Channels(uint8_t ch);

#endif /* __MODULE_REMOTE_H__ */