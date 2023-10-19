#include "button.h"
#include "dcmotor.h"
#include "led.h"
#include <string.h>
#include "i2c_lcd.h"
#include "ultrasonic.h"

extern TIM_HandleTypeDef htim4;
extern volatile int t1ms_counter;
extern volatile int dcmotor_t1ms_counter;

extern void ultrasonic_processing(void);
extern void fnd4digit_motor_clock(void);
extern void fnd4digit_sec_clock(void);

void dcmotor_pwm_control(void);
void forward_backward_dcmotor(void);
void led_on_off(void);
void DCMOTOR_Init(void);
void lcd_print(void);

// BUTTON0 : start/stop : LED1 (toggle)
// BUTTON1 : SPEED-UP : LED2 (toggle)
// BUTTON2 : SPEED-DOWN : LED3 (toggle)
uint8_t pwm_start_flag = 0; // dcmotor start/stop indicator flag
uint8_t fnd4digit_flag = 0;
uint8_t auto_flag = 0;
uint16_t CCR_UP_Value = 0;  //PWM UP control
uint16_t CCR_DOWN_Value = 0;  //PWM DOWN control

volatile int led_counter = 4;
volatile int remain_time = 10;
volatile int remain_2x_time = 5;

uint8_t for_back_flag = 0;
char lcd_buff[20];

void DCMOTOR_Init(void)
{
   auto_flag = 0;
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,1);
   sprintf(lcd_buff,"manual mode");
   move_cursor(0,0);
   lcd_string(lcd_buff);
}

void dcmotor_normal(void)
{
	 char lcd_buff[40];

	 if(!pwm_start_flag)
	 {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1); // int1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0); // int2

		pwm_start_flag = 1;
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,80); // 껐다 켰을 때 다시 처음 속도로
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
		HAL_GPIO_WritePin(GPIOD, 0x0f, 1);
	 }
	 else
	 {
			fnd4digit_motor_clock();
			if (dcmotor_t1ms_counter >= 1000)
			{
				dcmotor_t1ms_counter = 0;
				remain_time--;

				ultrasonic_processing();
				move_cursor(1, 0);
				sprintf(lcd_buff, "Remain time: %2d", remain_time);
				lcd_string(lcd_buff);

				if (remain_time <= 0)
				{
					remain_time = 0;
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);

					i2c_lcd_init();
					move_cursor(0, 0);
					lcd_string("Completed!!!");
					led_all_off();
					return;
				}
			}
	 }
}

void dcmotor_speedy(void)
{
	char lcd_buff[40];

	// DC motor가 돌아가면
	 if(!pwm_start_flag)
	 {
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1); // int1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0); // int2

		pwm_start_flag = 1;
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,160); // 껐다 켰을 때 다시 처음 속도로
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
		HAL_GPIO_WritePin(GPIOD, 0x0f, 1);
	 }
	 else
	 {
		 	fnd4digit_motor_clock();
			if (dcmotor_t1ms_counter >= 1000)
			{
				dcmotor_t1ms_counter = 0;
				remain_2x_time--;

				ultrasonic_processing();
				move_cursor(1, 0);
				sprintf(lcd_buff, "Remain time: %2d", remain_2x_time);
				lcd_string(lcd_buff);
				fnd4digit_sec_clock();

				if (remain_2x_time <= 0)
				{
					remain_2x_time = 0;
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);

					i2c_lcd_init();
					move_cursor(0, 0);
					lcd_string("Completed!!!");
					led_all_off();
					return;
				}
			}
	 }
}

void forward_backward_dcmotor(void)
{
   if(for_back_flag == 0)
   {
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0); // int1
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1); // int2
      for_back_flag = 1;


   }
   else if(for_back_flag == 1)
   {
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1); // int1
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0); // int2
      for_back_flag = 0;

   }
}
void led_on_off(void)
{
   if(auto_flag == 0)
   {
      if(pwm_start_flag == 1)
         {
            if(for_back_flag == 1)
            {
               if(t1ms_counter>=50)
               {
                  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
                  t1ms_counter = 0;
               }
            }
            else if(for_back_flag == 0)
               {
                  if(t1ms_counter>=100)
                  {
                     HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
                     t1ms_counter = 0;
                  }
               }
         }
      else if(pwm_start_flag == 0)
      {
         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
      }
   }
}
void lcd_print(void)
{
   if(auto_flag == 1)
   {

      move_cursor(0,0);
      sprintf(lcd_buff,"auto mode");
      lcd_string(lcd_buff);
      move_cursor(1,0);
      sprintf(lcd_buff,"time:%2d(+10 -10)", remain_time);
      lcd_string(lcd_buff);


      if(pwm_start_flag == 1)
      {
         if(remain_time > 0)
         {
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
            if(t1ms_counter >= 1000)

            {
               remain_time--;

               t1ms_counter = 0;
            }
         }
         else if(remain_time<=0)
         {
            remain_time = 0;
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1); // int1
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1); // int2
         }
      }


   }
   else if(auto_flag == 0)
   {

      move_cursor(0,0);
      sprintf(lcd_buff,"manual mode");
      lcd_string(lcd_buff);
      sprintf(lcd_buff,"                ");
      move_cursor(1,0);
      lcd_string(lcd_buff);
   }
}
