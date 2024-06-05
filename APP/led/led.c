#include "led.h"

void led_init(void)
{
	CLK_ModuleSrc(CLK_PORTB, CLK_SRC_OSC40M); // ����GPIOB��ʱ��ΪCLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTB); // ʹ��GPIOB
	
	CLK_ModuleSrc(CLK_PORTD, CLK_SRC_OSC40M); // ����GPIOD��ʱ��ΪCLK_SRC_OSC40M
	SYSCTRL_EnableModule(SYSCTRL_PORTD); // ʹ��GPIOD
	
	PORT_PinmuxConfig(PORT_B, GPIO_5, PTB5_GPIO); // R
	
	PORT_PinmuxConfig(PORT_B, GPIO_4, PTB4_GPIO); // G
	
	PORT_PinmuxConfig(PORT_D, GPIO_16, PTD16_GPIO); // B
	
	PORT_PinConfig_t pinConfig;
	
	pinConfig.pullConfig = PORT_PULL_DISABLED; // ��������
	pinConfig.slewRateConfig = PORT_FAST_RATE; // ��ƽ�л��ٶ� ����
	pinConfig.passiveFilterConfig = DISABLE; //
	pinConfig.openDrainConfig = DISABLE; // ��ʹ�ÿ�©���
	pinConfig.driveStrengthConfig = PORT_HIGH_DRIVE_STRENGTH; // ����������
	pinConfig.filterEnable = DISABLE; //
	pinConfig.filterConfig = 0; //
	
	// ��ʼ��IO
	PORT_PinInit(PORT_B,GPIO_5,&pinConfig);
	
	PORT_PinInit(PORT_B,GPIO_4,&pinConfig);
	
	PORT_PinInit(PORT_D,GPIO_16,&pinConfig);
	
	// ���� IO ��� ģʽ
	GPIO_SetPinDir(PORT_B,GPIO_5,GPIO_OUTPUT);
	
	GPIO_SetPinDir(PORT_B,GPIO_4,GPIO_OUTPUT);
	
	GPIO_SetPinDir(PORT_D,GPIO_16,GPIO_OUTPUT);
	
	// ��ʼ��IO��ƽ
	GPIO_WritePinOutput(PORT_B,GPIO_5,GPIO_LOW);
	
	GPIO_WritePinOutput(PORT_B,GPIO_4,GPIO_LOW);
	
	GPIO_WritePinOutput(PORT_D,GPIO_16,GPIO_LOW);
}

// color xxxx xrgb
void RGB_Set_Color(LED_COLOR color)
{
	color & 0x04 ? GPIO_WritePinOutput(PORT_B,GPIO_5,GPIO_HIGH) : GPIO_WritePinOutput(PORT_B,GPIO_5,GPIO_LOW);
	
	color & 0x02 ? GPIO_WritePinOutput(PORT_B,GPIO_4,GPIO_HIGH) : GPIO_WritePinOutput(PORT_B,GPIO_4,GPIO_LOW);
	
	color & 0x01 ? GPIO_WritePinOutput(PORT_D,GPIO_16,GPIO_HIGH) : GPIO_WritePinOutput(PORT_D,GPIO_16,GPIO_LOW);
}
