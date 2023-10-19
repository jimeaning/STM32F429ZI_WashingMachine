#include "uart.h"
#include <string.h>	// strncmp, strcpy, ...

void pc_command_processing(void);
void bt_command_processing(void);

extern void led_main(void);
extern void flower_on(void);
extern void flower_off(void);
extern void led_keepon_up(void);
extern void led_keepon_down(void);
extern void led_on_up(void);
extern void led_on_down(void);
extern void led_all_on(void);
extern void led_all_off(void);

extern void DHT11_processing(void);
extern void set_rtc(char *date_time);

extern int dht11time;
extern volatile int DHT11_flag;
extern volatile int ultrasonic_flag;

extern uint8_t rx_data;
extern uint8_t bt_rx_data;	// uart6 rx data

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6; //BT

// HW와 SW의 만나는 약속장소: call back function
// move from HAL_UART_RxCpltCallback of stm32f4xx_hal_uart to here
// UART로 1 바이트가 수신되면 H/W가 call을 해준다.
// UART RX INT가 발생되면 이곳에 자동적으로 들어 온다
// 1. 숙제#1 : 1차원 배열 가지고는 overflow가 발생되므로
// 2차원 배열 circular queue를 구성하여 overflow가 발생되지 않도록 구현해야 한다.
// comport master
// ledallon\n
// ledalloff\n
// led_toggle\n
#define COMMAND_LENGTH 40
volatile unsigned char rx_buff[COMMAND_LENGTH];	// UART3으로부터 수신된 char를 저장하는 공간 (\n을 만날 때까지)
volatile int rx_index = 0; // rx_buff의 save 위치
volatile int newline_detect_flag = 0; // new line을 만났을 때의 indicator 예) ledallon\n

volatile unsigned char bt_rx_buff[COMMAND_LENGTH]; //UART6 으로부터 수신된 char를 저장하는 공간. (\n을 만날 때까지)
volatile int bt_rx_index = 0; // bt_rx_buff의 save 위치
volatile int bt_newline_detect_flag = 0; // new line을 만났을 때의 indicator 예) ledallon\n

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart3)	// comport master와 연결된 uart
	{
		if (rx_index < COMMAND_LENGTH)	// 현재까지 들어온 바이트가 40바이트를 넘지 않으면
		{
			if (rx_data == '\n' || rx_data == '\r')
			{
				rx_buff[rx_index] = 0;	// '\0'
				newline_detect_flag = 1;	// new line을 만났다는 flag를 set한다
				rx_index = 0;	// 다음 message 저장을 위해서 rx_index 값을 0으로 한다
			}
			else
			{
				rx_buff[rx_index++] = rx_data;
			}
		}
		else
		{
			rx_index = 0;
			printf("Message Overflow!!!\n");
		}
		// 주의: 반드시 HAL_UART_Recieve_IT를 call 해줘야 다음 INT가 발생된다.
		HAL_UART_Receive_IT(&huart3, &rx_data, 1);
	}
}

void pc_command_processing(void)
{
	if (newline_detect_flag)	// \n을 만나면
	{
		newline_detect_flag = 0;
		printf("%s\n", rx_buff);
		if (!strncmp(rx_buff, "led_all_on", strlen("led_all_on")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			led_all_on();
			return;
		}
		if (!strncmp(rx_buff, "led_all_off", strlen("led_all_off")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			led_all_off();
			return;
		}
		if (!strncmp(rx_buff, "led_on_down", strlen("led_on_down")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			led_on_down();
			return;
		}
		if (!strncmp(rx_buff, "led_on_up", strlen("led_on_up")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			led_on_up();
			return;
		}
		if (!strncmp(rx_buff, "flower_on", strlen("flower_on")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			flower_on();
			return;
		}
		if (!strncmp(rx_buff, "flower_off", strlen("flower_off")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			flower_off();
			return;
		}
		if (!strncmp(rx_buff, "led_keepon_up", strlen("led_keepon_up")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			led_keepon_up();
			return;
		}
		if (!strncmp(rx_buff, "led_keepon_down", strlen("led_keepon_down")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			led_keepon_down();
			return;
		}


		if (!strncmp(rx_buff, "dht11_on", strlen("dht11_on")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			DHT11_flag = 1;
			return;
		}
		if (!strncmp(rx_buff, "dht11_off", strlen("dht11_off")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			DHT11_flag = 0;
			return;
		}
		if (!strncmp(rx_buff, "ultra_on", strlen("ultra_on")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			ultrasonic_flag = 1;
			return;
		}
		if (!strncmp(rx_buff, "ultra_off", strlen("ultra_off")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			ultrasonic_flag = 0;
			return;
		}

		if (!strncmp(rx_buff, "dht11time200", strlen("dht11time200")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			dht11time = 200;
			return;
		}
		if (!strncmp(rx_buff, "dht11time300", strlen("dht11time300")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			dht11time = 300;
			return;
		}


		if (!strncmp(rx_buff, "setrtc", strlen("setrtc")))	// if (strncmp(rx_buff, "ledallon", strlen("ledallon")) == 0)
		{
			set_rtc(rx_buff);
			return;
		}
	}
}

void bt_command_processing(void)
{
	if(bt_newline_detect_flag)		//BT로부터 완전한 문장이 들어 오면 (\n을 만나면)
	{
		bt_newline_detect_flag = 0;
		printf("%s\n", bt_rx_buff);
		if(!strncmp(bt_rx_buff, "led_all_on",strlen("led_all_on")))
		{
			led_all_on();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_all_off",strlen("led_all_off")))
		{
			led_all_off();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_on_down",strlen("led_on_down")))
		{
			led_on_down();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_on_up",strlen("led_on_up")))
		{
			led_on_up();
			return;
		}
		if(!strncmp(bt_rx_buff, "flower_on",strlen("flower_on")))
		{
			flower_on();
			return;
		}
		if(!strncmp(bt_rx_buff, "flower_off",strlen("flower_off")))
		{
			flower_off();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_keepon_down",strlen("led_keepon_down")))
		{
			led_keepon_down();
			return;
		}
		if(!strncmp(bt_rx_buff, "led_keepon_up",strlen("led_keepon_up")))
		{
			led_keepon_up();
			return;
		}
	}
}
