#include "pwm.h"

void pwm1_init(void)
{
	CLK_ModuleSrc(CLK_PORTE, CLK_SRC_OSC40M); // ����GPIOE��ʱ��ΪCLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTE); // ʹ��GPIOE
	
	CLK_ModuleSrc(CLK_TIM0, CLK_SRC_OSC40M); // ���ö�ʱ��0��ʱ��ΪCLK_SRC_OSC40M
	CLK_SetClkDivider(CLK_TIM0, CLK_DIV_1); // �Զ�ʱ����ʱ�ӽ��з�Ƶ
	
	SYSCTRL_ResetModule(SYSCTRL_TIM0); // ��λ��ʱ��0
	SYSCTRL_EnableModule(SYSCTRL_TIM0); // ʹ�ܶ�ʱ��0
	
	TIM_PwmChannelConfig_t pwmChConfig; // ����pwm���ýṹ��
	
	pwmChConfig.channelId = TIM_CHANNEL_6; // ��ʱ����ͨ��0���pwm ���ö�ʱ���ĸ�ͨ�����pwm
	pwmChConfig.pwmModeConfig = TIM_PWM_LOW_TRUE_PULSE; // pwmģʽ ��stm32��pwm��ͬ
	pwmChConfig.polarity = TIM_POL_LOW; // ��Ч��ƽΪ�� TIM_PWM_LOW_TRUE_PULSE ģʽ�� ����ֵС�ڱȽ�ֵ�����Ч��ƽ ��С��ʱΪ��,����ʱΪ�� 
	pwmChConfig.compareValue = 0; // ����pwm�ļ��Է�תʱ��ֵ ���Ƚ�ֵ
	pwmChConfig.faultCtrlChannelEnable = DISABLE; // ������ͨ��Ϊ��ʱ���Ĺ���ͨ��
	pwmChConfig.ccvUpdateEnable = ENABLE; // ������ccv���� ccv�жϼ�����ʱ������ֵ����compareValueʱ��������
	
	TIM_PwmConfig_t pwmConfig;
	
	pwmConfig.numChannels = 1; // pwm��ͨ���� ��TIM_PwmChannelConfig_t������
	pwmConfig.minCounterVal = 0; // ��ʱ���ļ����ĳ�ʼֵ ����Ϊ0����0��ʼ����
	pwmConfig.maxCounterVal = UINT16_MAX; // ����Ϊ16λ��ʱ�������ֵ
	pwmConfig.pwmChConfig = &pwmChConfig; // ��pwmChConfigָ��ָ��ʱ������pwm���õĵ�ַ,��ʼ������ѭ��������ʼ�� numChannels��ֵ��pwmChConfig��������һֱ
	
	TIM_OutputEdgeAlignedPwmConfig(TIM0_ID,&pwmConfig); // pwm��ʼ�� �����ʼ����ʱ��0��ͨ��0
	
	pwmChConfig.channelId = TIM_CHANNEL_7; // ��ʱ����ͨ��0���pwm ���ö�ʱ���ĸ�ͨ�����pwm
	pwmChConfig.pwmModeConfig = TIM_PWM_LOW_TRUE_PULSE; // pwmģʽ ��stm32��pwm��ͬ
	pwmChConfig.polarity = TIM_POL_LOW; // ��Ч��ƽΪ�� TIM_PWM_LOW_TRUE_PULSE ģʽ�� ����ֵС�ڱȽ�ֵ�����Ч��ƽ ��С��ʱΪ��,����ʱΪ�� 
	pwmChConfig.compareValue = 0; // ����pwm�ļ��Է�תʱ��ֵ ���Ƚ�ֵ
	pwmChConfig.faultCtrlChannelEnable = DISABLE; // ������ͨ��Ϊ��ʱ���Ĺ���ͨ��
	pwmChConfig.ccvUpdateEnable = ENABLE; // ������ccv���� ccv�жϼ�����ʱ������ֵ����compareValueʱ��������
	
	pwmConfig.numChannels = 1; // pwm��ͨ���� ��TIM_PwmChannelConfig_t������
	pwmConfig.minCounterVal = 0; // ��ʱ���ļ����ĳ�ʼֵ ����Ϊ0����0��ʼ����
	pwmConfig.maxCounterVal = UINT16_MAX; // ����Ϊ16λ��ʱ�������ֵ
	pwmConfig.pwmChConfig = &pwmChConfig; // ��pwmChConfigָ��ָ��ʱ������pwm���õĵ�ַ,��ʼ������ѭ��������ʼ�� numChannels��ֵ��pwmChConfig��������һֱ
	
	TIM_OutputEdgeAlignedPwmConfig(TIM0_ID,&pwmConfig); // pwm��ʼ�� �����ʼ����ʱ��0��ͨ��0
	
	TIM_ReloadConfig_t reloadConfig; // ��ʱ���������������ýṹ��
	
	reloadConfig.reloadMode = TIM_RELOAD_FULL_CYCLE; // ��������ģʽΪmaxCounterValʱ����
	reloadConfig.loadFrequency = 0; // �����ʾ����n+1��(����������ֵʱ)�Ŵ��������ж� ��������0����һ�μ���������ֵ�������ж�
	reloadConfig.numChannels = 0; // �����ʾchannelMatchConfig����Ϊ0
	reloadConfig.channelMatchConfig = NULL; // ����ccv�����ж�
	
	TIM_PwmSyncConfig_t pwmSyncConfig; // pwmͬ������
	
	pwmSyncConfig.cntinitSync = TIM_UPDATE_PWM_SYN; // 
	pwmSyncConfig.outswcSync = TIM_UPDATE_PWM_SYN; // 
	pwmSyncConfig.syncSWTriggerEnable = DISABLE; // 
	pwmSyncConfig.syncReloadEnable = ENABLE; // 
	pwmSyncConfig.reloadConfig = &reloadConfig; // 
	
	TIM_SyncConfig(TIM0_ID,&pwmSyncConfig); // ��������
	
	TIM_CountingModeConfig(TIM0_ID,TIM_COUNTING_UP); // ����ģʽ���� ���ϼ���
	
	PORT_PinmuxConfig(PORT_E, GPIO_8, PTE8_TIM0_CH6); // ��GPIOE��pin8 ��λΪ��ʱ��0��ͨ��6
	PORT_PinmuxConfig(PORT_E, GPIO_9, PTE9_TIM0_CH7); // ��GPIOE��pin9 ��λΪ��ʱ��0��ͨ��7
	
	TIM_ChannelOutputEnable(TIM0_ID,  TIM_CHANNEL_6); // ��ʼ��ʱ��0��ͨ��6���pwm
	TIM_ChannelOutputEnable(TIM0_ID,  TIM_CHANNEL_7); // ��ʼ��ʱ��0��ͨ��7���pwm
	
	TIM_StartCounter(TIM0_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1); // ���ü�������ʱ���䶨ʱ��ʱ�Ӳ����䣬40M/UINT16_MAX = 610Hz pwmƵ�� ���ҿ�ʼ����
	//
}


