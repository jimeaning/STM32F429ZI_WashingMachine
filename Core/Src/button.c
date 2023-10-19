#include "button.h"

extern RTC_HandleTypeDef hrtc;
uint8_t lcd_display_mode_flag = 0;
RTC_TimeTypeDef mTime;	// time information

// 초기 버튼 상태 table
char button_status[BUTTON_NUMBER] =
{
		BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE
};


void lcd_display_mode_select(void);

// get_button(gpio, pin, button num)
// 완전히 눌렀다 떼면 BUTTON_RELEASE(1)을 리턴
int get_button(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN, uint8_t button_number)
{
	unsigned char curr_state;

	curr_state = HAL_GPIO_ReadPin(GPIO, GPIO_PIN);	// 0, 1

	//버튼이 눌러졌으나 처음 상태
	if (curr_state == BUTTON_PRESS && button_status[button_number] == BUTTON_RELEASE)
	{
		HAL_Delay(100);	// noise가 지나가기를 기다림
		button_status[button_number] = curr_state;	// 현재의 버튼 상태를 저장
		return BUTTON_RELEASE;	// 버튼이 눌려진 상태이나 아직은 noise 상태로 인정
	}
	// 이전에 버튼이 눌려진 상태였고 지금은 버튼을 뗀 상태이면
	else if (curr_state == BUTTON_RELEASE && button_status[button_number] == BUTTON_PRESS)
	{
		button_status[button_number] = BUTTON_RELEASE;	// button_status table 초기화
		HAL_Delay(30);
		return BUTTON_PRESS;	// 버튼을 한 번 눌렀다 뗀 것으로 인정한다
	}
	return BUTTON_RELEASE;
}

void lcd_display_mode_select(void)
{
	char lcd_buff[40];

	if (get_button(GPIOC, GPIO_PIN_13, 4) == BUTTON_PRESS)
	{
		i2c_lcd_init();
		if (lcd_display_mode_flag == 0)
		{
			move_cursor(0,0);
			lcd_string("*");
		}
		if (lcd_display_mode_flag == 1)
		{
			move_cursor(0,8);
			lcd_string("*");
		}
		if (lcd_display_mode_flag == 2)
		{
			move_cursor(1,0);
			lcd_string("*");
		}
		washing_init();
		lcd_display_mode_flag++;
		lcd_display_mode_flag %= 3;
	}
}
