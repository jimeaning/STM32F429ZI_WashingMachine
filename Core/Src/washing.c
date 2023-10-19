#include "main.h"
#include "i2c_lcd.h"
#include "button.h"

uint8_t washing_mode_flag = 0;
uint8_t us_flag = 0;

extern TIM_HandleTypeDef htim4;
extern uint16_t CCR_UP_Value;		// PWM UP control
extern uint16_t CCR_DOWN_Value;		// PWM DOWN control

extern void washing_option(void);
extern void dcmotor_normal(void);
extern void dcmotor_speedy(void);

void washing_select(void)
{
	// Button1을 누르고 Washing에 진입하면 Normal, Speedy 옵션을 보여준다
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0) == BUTTON_PRESS)
	{
		i2c_lcd_init();
		washing_option();
	}

	// Button2를 눌러 Normal / Speedy를 움직일 수 있다
	if (get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1) == BUTTON_PRESS)
	{
		if (washing_mode_flag == 0)
		{
			move_cursor(0,0);
			lcd_string("*");
			move_cursor(1,0);
			lcd_string(" ");
		}
		if (washing_mode_flag == 1)
		{
			move_cursor(0,0);
			lcd_string(" ");
			move_cursor(1,0);
			lcd_string("*");
		}
		washing_mode_flag++;
		washing_mode_flag %= 2;
	}

	// Button3을 누르면 해당 옵션을 선택한 것으로 간주하고 모터를 돌린다
	if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2) == BUTTON_PRESS)
	{
		us_flag = ~us_flag;
	}

	if (us_flag)
	{
		// Normal Mode 선택 시
		if (washing_mode_flag % 2 == 1)
		{
			dcmotor_normal();
		}
		if (washing_mode_flag % 2 == 0)
		{
			dcmotor_speedy();
		}
	}
}
