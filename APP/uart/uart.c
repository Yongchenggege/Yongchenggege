#include "uart.h"
#include "stdarg.h"
#include "math.h"
#include "string.h"

static int uart_printf(UART_ID_t uartId,const char* format,va_list pArgs);
usart_pack pack;
uint8_t p_flag;

#ifdef UART0_EN
#if UART0_EN

void uart0_init(uint32_t baudRate)
{
	CLK_ModuleSrc(CLK_UART0, CLK_SRC_OSC40M); // ���ô���0ʱ��
	CLK_SetClkDivider(CLK_UART0, CLK_DIV_1);  // ����0ʱ�Ӳ���Ƶ
	SYSCTRL_ResetModule(SYSCTRL_UART0); // ��λ����0
	SYSCTRL_EnableModule(SYSCTRL_UART0); // ʱ�Ӵ���0����
	
	CLK_ModuleSrc(CLK_PORTC, CLK_SRC_OSC40M); // ���ô���0��GPIOʱ��
	SYSCTRL_EnableModule(SYSCTRL_PORTC); // ʱ�Ӵ���0��GPIO
	CLK_ModuleSrc(CLK_PORTA, CLK_SRC_OSC40M); // ���ô���0��GPIOʱ��
	SYSCTRL_EnableModule(SYSCTRL_PORTA); // ʱ�Ӵ���0��GPIO
	
	
	PORT_PinmuxConfig(PORT_A, GPIO_3, PTA3_UART0_TX); // ��PTC3 ����Ϊuart0_tx
	
	PORT_PinmuxConfig(PORT_A, GPIO_2, PTA2_UART0_RX); // ��PTC2 ����Ϊuart0_rx
	
	UART_Config_t tUartCfgStruct;
	
	tUartCfgStruct.baudRate = baudRate;
	tUartCfgStruct.dataBits = UART_DATABITS_8;
	tUartCfgStruct.parity = UART_PARITY_NONE;
	tUartCfgStruct.stopBits = UART_STOPBITS_1;
	tUartCfgStruct.autoFlowControl = DISABLE;
	tUartCfgStruct.oscFreq = 40000000;
	
	while(ERR == UART_WaitBusyClear(UART0_ID, 7000))
	{
			(void)UART_ReceiveByte(UART0_ID);
	}
	
	UART_InstallCallBackFunc(UART0_ID,UART_INT_RBFI,uart0_callBack); // ��ʼ�����ж� �����жϻص�����
	/* enable received data available interrupt  */
    UART_IntMask(UART0_ID, UART_INT_RBFI, UNMASK);

    /* enable IRQ */
    NVIC_EnableIRQ(UART0_IRQn);
	
	UART_Init(UART0_ID, &tUartCfgStruct);
}

__weak void uart0_callBack(void)
{
	uint8_t ch;
	
	if(SET == UART_GetLineStatus(UART0_ID,UART_LINESTA_DR))
	{
		ch = UART_ReceiveByte(UART0_ID);
		
		UART_SendByte(UART0_ID, ch);
	}
}

#if UART0_PRINT_EN
__weak int uart0_printf(const char* format,...)
{
	unsigned int ret = 0;
	
	va_list pArgs; // ����va_list����ָ�룬���ڴ洢�����ĵ�ַ
	
	va_start(pArgs,format); // ��ʼ��pArgs
	
	ret = uart_printf(UART0_ID,format,pArgs);
	
	va_end(pArgs);// ����ȡ����
	
	return ret;
}
#endif

#if UART0_LIBC_PRINT_EN
int fputc(int ch,FILE *p)
{
	UART_SendByte(UART0_ID, ch);
	while(UART_GetLineStatus(UART0_ID,UART_LINESTA_TEMT) == RESET);
	return ch;
}
#endif

#endif
#endif

#ifdef UART1_EN
#if UART1_EN

void uart1_init(uint32_t baudRate)
{
	CLK_ModuleSrc(CLK_UART1, CLK_SRC_OSC40M); // ���ô���1ʱ��
	CLK_SetClkDivider(CLK_UART1, CLK_DIV_1);  // ����1ʱ�Ӳ���Ƶ
	SYSCTRL_EnableModule(SYSCTRL_UART1); // ʱ�Ӵ���1����
	
	CLK_ModuleSrc(CLK_PORTC, CLK_SRC_OSC40M); // ���ô���1��GPIOʱ��
	SYSCTRL_EnableModule(SYSCTRL_PORTC); // ʱ�Ӵ���1��GPIO
	
	PORT_PinmuxConfig(PORT_C, GPIO_7, PTC7_UART1_TX); // ��PTC3 ����Ϊuart1_tx
	
	PORT_PinmuxConfig(PORT_C, GPIO_6, PTC6_UART1_RX); // ��PTC2 ����Ϊuart1_rx
	
	UART_Config_t tUartCfgStruct;
	
	tUartCfgStruct.baudRate = baudRate;
	tUartCfgStruct.dataBits = UART_DATABITS_8;
	tUartCfgStruct.parity = UART_PARITY_NONE;
	tUartCfgStruct.stopBits = UART_STOPBITS_1;
	tUartCfgStruct.autoFlowControl = DISABLE;
	tUartCfgStruct.oscFreq = 40000000;
	
	while(ERR == UART_WaitBusyClear(UART1_ID, 7000))
	{
			(void)UART_ReceiveByte(UART1_ID);
	}
	
	UART_InstallCallBackFunc(UART1_ID,UART_INT_RBFI,uart1_callBack); // ��ʼ�����ж� �����жϻص�����
	/* enable received data available interrupt  */
    UART_IntMask(UART1_ID, UART_INT_RBFI, UNMASK);

    /* enable IRQ */
    NVIC_EnableIRQ(UART1_IRQn);
	
	UART_Init(UART1_ID, &tUartCfgStruct);
}

__weak void uart1_callBack(void)
{
	static uint8_t data[3];
	static uint16_t i=0;
	if(SET == UART_GetLineStatus(UART1_ID,UART_LINESTA_DR))
	{
		data[i++] = UART_ReceiveByte(UART1_ID);
		UART_SendByte(UART1_ID, data[i-1]);
		if(i==3) {
			i=0;
			if(data[0] == 0xaa && data[2] == 0xbb) {
				pack.head = data[0];
				pack.end = data[2];
				pack.data[0] = data[1];
				switch(pack.data[0]) {
					case 0x00:
						p_flag = 1;
						break;
					case 0x11:
						p_flag = 2;
						break;
					case 0x12:
						p_flag = 3;
						break;
					case 0x13:
						p_flag = 4;
						break;
					case 0x14:
						p_flag = 5;
						break;
					case 0x15:
						p_flag = 6;
						break;
					case 0x16:
						p_flag = 7;
						break;	

					case 0x01:
						p_flag = 8;
						break;
					case 0x02:
						p_flag = 9;
						break;
					case 0x03:
						p_flag = 10;
						break;
					case 0x04:
						p_flag = 11;
						break;
					case 0x05:
						p_flag = 12;
						break;
					case 0x06:
						p_flag = 13;
						break;				
					default:
						break;
				}
			}
		}
	}
}

#if UART1_PRINT_EN
__weak int uart1_printf(const char* format,...)
{
	unsigned int ret = 0;
	
	va_list pArgs; // ����va_list����ָ�룬���ڴ洢�����ĵ�ַ
	
	va_start(pArgs,format); // ��ʼ��pArgs
	
	ret = uart_printf(UART1_ID,format,pArgs);
	
	va_end(pArgs);// ����ȡ����
	
	return ret;
}
#endif

#if UART1_LIBC_PRINT_EN
int fputc(int ch,FILE *p)
{
	UART_SendByte(UART1_ID, ch);
	while(UART_GetLineStatus(UART1_ID,UART_LINESTA_TEMT) == RESET);
	return ch;
}
#endif

#endif
#endif

static void uart_send_char(UART_ID_t uartId,char ch)
{
	UART_SendByte(uartId, ch);
	while(UART_GetLineStatus(uartId,UART_LINESTA_TEMT) == RESET);
}

static int uart_printf(UART_ID_t uartId,const char* format,va_list pArgs)
{
	if (format == NULL) return -1;
	
	unsigned int ret_num = 0;// ���ش�ӡ�ַ��ĸ���
	char* pStr = (char*)format;// ָ��str
	int ArgIntVal = 0;  // ��������
	unsigned long ArgHexVal = 0;// ��ʮ������
	char* ArgStrVal = NULL;  // �����ַ���
	double ArgFloVal = 0.0; // ���ܸ�����
	unsigned long val_seg = 0;   // �����з�
	unsigned long val_temp = 0;  // ��ʱ��������
	int cnt = 0;       // ���ݳ��ȼ���
	int i = 0;
	
	while (*pStr != '\0')
	{
		switch (*pStr)
		{
			case ' ':
				uart_send_char(uartId,*pStr); ret_num++; break;
			case '\t':
				uart_send_char(uartId,*pStr); ret_num += 4; break;
			case '\r':
				uart_send_char(uartId,*pStr); ret_num++; break;
			case '\n':
				uart_send_char(uartId,*pStr); ret_num++; break;
			case '%':
				pStr++;
				// % ��ʽ����
				switch (*pStr)
				{
					case '%':
						uart_send_char(uartId,'%');// %%�����%
						ret_num++;
						pStr++;
						continue;
					case 'c':
						ArgIntVal = va_arg(pArgs, int);// %c�����char
						uart_send_char(uartId,(char)ArgIntVal);
						ret_num++;
						pStr++;
						continue;
					case 'd':
						// ��������
						ArgIntVal = va_arg(pArgs, int);
						if (ArgIntVal < 0)// ���Ϊ������ӡ������
						{
							ArgIntVal = -ArgIntVal;// ȡ�෴��

							uart_send_char(uartId,'-');
							ret_num++;
						}
						val_seg = ArgIntVal;// ��ֵ�� val_seg��������
						// ����ArgIntVal����
						if (ArgIntVal)
						{
							while (val_seg) {
								cnt++;
								val_seg /= 10;
							}
						}
						else cnt = 1;// ����0�ĳ���Ϊ1

						ret_num += cnt;// �ַ��������������ĳ���

						// ������תΪ�����ַ���ӡ
						while (cnt)
						{
							val_seg = ArgIntVal / pow(10, cnt - 1);
							ArgIntVal %= (int)pow(10, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						pStr++;
						continue;
					case 'o':
						// ��������
						ArgIntVal = va_arg(pArgs, int);
						if (ArgIntVal < 0)// ���Ϊ������ӡ������
						{
							ArgIntVal = -ArgIntVal;// ȡ�෴��

							uart_send_char(uartId,'-');
							ret_num++;
						}
						val_seg = ArgIntVal;// ��ֵ�� val_seg��������
						// ����ArgIntVal����
						if (ArgIntVal)
						{
							while (val_seg) {
								cnt++;
								val_seg /= 8;
							}
						}
						else cnt = 1;// ����0�ĳ���Ϊ1

						ret_num += cnt;// �ַ��������������ĳ���

						// ������תΪ�����ַ���ӡ
						while (cnt)
						{
							val_seg = ArgIntVal / pow(8, cnt - 1);
							ArgIntVal %= (int)pow(8, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						pStr++;
						continue;
					case 'x':
						// ����16����
						ArgHexVal = va_arg(pArgs, unsigned long);
						val_seg = ArgHexVal;
						// ����ArgIntVal����
						if (ArgHexVal)
						{
							while (val_seg) {
								cnt++;
								val_seg /= 16;
							}
						}
						else cnt = 1;// ����0�ĳ���Ϊ1

						ret_num += cnt;// �ַ��������������ĳ���
						// ������תΪ�����ַ���ӡ
						while (cnt)
						{
							val_seg = ArgHexVal / pow(16, cnt - 1);
							ArgHexVal %= (int)pow(16, cnt - 1);
							if (val_seg <= 9)
								uart_send_char(uartId,(char)val_seg + '0');
							else
							{
								//uart_send_char(uartId,(char)val_seg - 10 + 'a'); //Сд��ĸ
								uart_send_char(uartId,(char)val_seg - 10 + 'A');
							}
							cnt--;
						}
						pStr++;
						continue;
					case 'b':
						// ��������
						ArgIntVal = va_arg(pArgs, int);
						val_seg = ArgIntVal;
						// ����ArgIntVal����
						if (ArgIntVal)
						{
								while (val_seg) {
										cnt++;
										val_seg /= 2;
								}
						}
						else cnt = 1;// ����0�ĳ���Ϊ1

						ret_num += cnt;// �ַ��������������ĳ���
						// ������תΪ�����ַ���ӡ
						while (cnt)
						{
								val_seg = ArgIntVal / pow(2, cnt - 1);
								ArgIntVal %= (int)pow(2, cnt - 1);
								uart_send_char(uartId,(char)val_seg + '0');
								cnt--;
						}
						pStr++;
						continue;
					case 's':
						// �����ַ�
						ArgStrVal = va_arg(pArgs, char*);
						ret_num += (unsigned int)strlen(ArgStrVal);
						while (*ArgStrVal)
						{
							uart_send_char(uartId,*ArgStrVal);
							ArgStrVal++;
						}
						pStr++;
						continue;
					case 'f':
						// ���ո����� ����6ΪС��������ȡ��������
						ArgFloVal = va_arg(pArgs, double);
						val_seg = (unsigned long)ArgFloVal;// ȡ��������
						val_temp = val_seg;      // ��ʱ����������������
						ArgFloVal = ArgFloVal - val_seg;// �ó����µ�С������
						// �����������ֳ���
						if (val_seg)
						{
							while (val_seg) {
									cnt++;
									val_seg /= 10;
							}
						}
						else cnt = 1;// ����0�ĳ���Ϊ1
						ret_num += cnt;// �ַ��������������ĳ���
						// ������תΪ�����ַ���ӡ
						while (cnt)
						{
							val_seg = val_temp / pow(10, cnt - 1);
							val_temp %= (int)pow(10, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						// ��ӡС����
						uart_send_char(uartId,'.');
						ret_num++;
						// ��ʼ���С������
						ArgFloVal *= 1000000;
						// printf("\r\n %f\r\n", ArgFloVal);
						cnt = 6;
						val_temp = (int)ArgFloVal;// ȡ��������
						while (cnt)
						{
							val_seg = val_temp / pow(10, cnt - 1);
							val_temp %= (int)pow(10, cnt - 1);
							uart_send_char(uartId,(char)val_seg + '0');
							cnt--;
						}
						ret_num += 6;
						pStr++;
						continue;
					default:// % ƥ�����������ո�
						uart_send_char(uartId,' ');
						ret_num++;
						continue;
					}
			default:
					uart_send_char(uartId,*pStr); ret_num++;
					break;
		}
		pStr++;
	}

	return ret_num;
}
