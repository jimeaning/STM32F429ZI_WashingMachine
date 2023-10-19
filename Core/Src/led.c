#include "led.h"	// <> : library
#include "button.h"

#define LED_ALL_ON 0
#define LED_ALL_OFF 1
#define FLOWER_ON 2
#define FLOWER_OFF 3
#define LED_KEEPON_UP 4
#define LED_KEEPON_DOWN 5
#define LED_ON_UP 6
#define LED_ON_DOWN 7

void led_main(void);
void flower_on(void);
void flower_off(void);
void led_keepon_up(void);
void led_keepon_down(void);
void led_on_up(void);
void led_on_down(void);
void led_all_on(void);
void led_all_off(void);

void button0_led_all_on_off_toggle(void);
void button0_toggle(void);

extern volatile int t1ms_counter;	// volatile : disable optimize 최적화 방지

void button0_led_all_on_off_toggle(void)
{
	static int button0_count = 0;	// static으로 선언을 하면 전역변수처럼 동작한다

	// 버튼을 한 번 눌렀다 뗀 상태라면
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		button0_count++;
		button0_count %= 2;
		if (button0_count)
		{
			led_all_on();
		} else
		{
			led_all_off();
		}
	}
}

void button0_toggle(void)
{
	// 1: led_all_on()
	// 2: led_all_off()
	// 3: flower_on()
	// 4: flower_off()
	// 5: led_keepon_up()
	// 6: led_keepon_down()
	// 7: led_on_up()
	// 8: led_on_down()

	static int button0_count = 0;

	void (*func_arr[9])(void) = { NULL, led_all_on, led_all_off, flower_on, flower_off,
			led_keepon_up, led_keepon_down, led_on_up, led_on_down };

	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		button0_count++;
		button0_count %= 9;
		func_arr[button0_count]();
	}
}

int i = 0;

void led_bar_up(void)
{
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		i++;
	}
	if (get_button(BUTTON1_GPIO_Port,BUTTON1_Pin, 1) == BUTTON_PRESS)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 0);
		i--;
	}
}

int func_index = 0;

void led_main(void)
{
	while(1)
	{

#if 0
		if (t1ms_counter >= 200)
		{
			t1ms_counter = 0;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LED 1
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED 2
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED 3
		}
#endif

#if 0
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	// LED 1
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);	// LED 2
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);	// LED 3
	    HAL_Delay(200);	// 200ms
#endif

#if 1
		button0_toggle();
#endif

//		button0_led_all_on_off_toggle();

		// Phase #5
//		flower_on();
//		flower_off();

		// Phase #4
//		led_keepon_up();
//		led_all_off();
//		led_keepon_down();
//		led_all_off();

#if 0
//		led_bar_up();

		// Phase #3 (순차 점화)
		// 0 - 1 - 2 - 3 - 4 - 5 - 6 - 7
		switch(func_index)
		{
			case LED_ALL_ON:
				led_all_on();
				break;
			case LED_ALL_OFF:
				led_all_off();
				break;
			case FLOWER_ON:
				flower_on();
				break;
			case FLOWER_OFF:
				flower_off();
				break;
			case LED_KEEPON_UP:
				led_keepon_up();
				break;
			case LED_KEEPON_DOWN:
				led_keepon_down();
				break;
			case LED_ON_UP:
				led_on_up();
				break;
			case LED_ON_DOWN:
				led_on_down();
				break;
			default:
				break;
		}

//		led_on_up();
//		// 7 - 6 - 5 - 4 - 3 - 2 - 1 - 0
//		led_on_down();
#endif

		// Phase #2
//		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_All);
//		HAL_Delay(500);
		// Phase #1
//		led_all_on();
//		HAL_Delay(300);
//		led_all_off();
//		HAL_Delay(300);
	}
}

void led_all_on(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0xff, 1);
		func_index = LED_ALL_OFF;
	}
#else
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
//			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOD, 0xff, 1);
#endif
}

void led_all_off(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0xff, 0);
		func_index = FLOWER_ON;
	}
#else
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
//				| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOD, 0xff, 0);
#endif
}

void flower_on(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x10 << i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> i, 1);
		i++;
		if (i > 8)
		{
			i = 0;
			func_index = FLOWER_OFF;
		}
	}
#else
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x10 << i, 1);
		HAL_GPIO_WritePin(GPIOD, 0x08 >> i, 1);

		HAL_Delay(100 * (i + 1));
	}
#endif
}

void flower_off(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 0);
		i++;
		if (i > 8)
		{
			i = 0;
			func_index = LED_KEEPON_UP;
		}
	}
#else
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 0);

		HAL_Delay(500 - 100 * (i + 1));
	}
#endif
	HAL_Delay(50);
}

void led_keepon_up(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		i++;
		if (i >= 8)
		{
			i = 0;
			func_index = LED_KEEPON_DOWN;
		}
	}
#else
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
#endif
}

void led_keepon_down(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
		i++;
		if (i >= 8)
		{
			i = 0;
			func_index = LED_ON_UP;
		}
	}
#else
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 0);
		HAL_Delay(200);
	}
#endif
}

void led_on_up(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		i++;
		if (i >= 8)
		{
			i = 0;
			func_index = LED_ON_DOWN;
		}
	}
#else	// original
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x01 << i, 1);
		HAL_Delay(200);
	}
#endif
}

void led_on_down(void)
{
	static int i = 0;
#if 1
	if (t1ms_counter >= 200)
	{
		t1ms_counter = 0;
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		i++;
		if (i >= 8)
		{
			i = 0;
			func_index = LED_ALL_ON;
		}
	}
#else
	for (int i = 0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOD, 0x80 >> i, 1);
		HAL_Delay(200);
	}
#endif
}


