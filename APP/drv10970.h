#ifndef __DRV10970_H_
#define __DRV10970_H_

#include "Z20K118_conf.h"
#include "pwm.h"
#include "uart.h"
#include "enconder.h"

// FREQ = 72000000/(TIM_PER*PULSE_MAX)
#define TIM_PER			5
#define PULSE_MAX		7200

// �������תһȦ��������(������Ҫ������ĵ������ȥ��)
#define MOTOR_PULSE	11

// �ٶȲ������(ȡֵΪ0-65535 �Ե�ʱ��Ϊx*0.0001 ����ȡֵ2000 ��Ӧ��2000*0.0001=0.2 (s) ����ȡֵ�� 500-10000 )
#define SPEED_INTERVAL	2000

// ���ֵ�����ٱ�9.6 ���ת��620 ���տ���ת��620/9.6=64.6 ��������һȦ11������



typedef enum{
	motor1,
	motor2

}motor_type;

//FR
 typedef enum {
	reverse,
	forward

}fr_dir;
 
void fr_control_dir(fr_dir dir1,fr_dir dir2);
void drv10970_init(void);

//FG
float get_motor_speed(motor_type motor);



#endif
