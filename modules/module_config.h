/*
 * @Author: sji733055-glitch sji733055@gmail.com
 * @Date: 2026-05-27 19:19:16
 * @LastEditors: sji733055-glitch sji733055@gmail.com
 * @LastEditTime: 2026-05-29 10:02:43
 * @FilePath: \rescue\modules\module_config.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MODULE_CONFIG_H__
#define __MODULE_CONFIG_H__

/*REMOTE 参数*/
#define REMOTE_DEAD_ZONE        10
#define REMOTE_TASK_STACK_SIZE  1024
#define REMOTE_TASK_PRIORITY    9

/* MOTOR 参数 */
#define MOTOR_TASK_STACK_SIZE   1024
#define MOTOR_TASK_PRIORITY     12

#endif /* __MODULE_CONFIG_H__ */