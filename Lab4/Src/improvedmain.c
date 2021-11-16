/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************

  * @brief  Main program
  * @param  None
  * @retval None
  */

#include "main.h"
#include <stdio.h>
#include <string.h>

struct led {
  char ledname[5];
  GPIO_TypeDef* port;
  char portname;
  uint32_t pin;
};


//declaration for the GPIO pins for the LED

#define LED2_PIN                         GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED1_PIN                         GPIO_PIN_5
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()   
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void LED2_Init(struct led);
void LED2_On(struct led);
void LED2_Off(struct led);
void LED2_DeInit(struct led);
void LED2_Toggle(struct led);


int main(void)
{
/* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  struct led myleds[2];

  HAL_Init();

  /* Configure the System clock to have a frequency of 80 MHz */
  SystemClock_Config();

  strcpy(myleds[0].ledname, "LED2");
  myleds[0].port=LED2_GPIO_PORT;
  myleds[0].pin=LED2_PIN;
  myleds[0] .portname='B';
    
  strcpy(myleds[1].ledname, "LED1");
  myleds[1].port=LED1_GPIO_PORT;
  myleds[1].pin=LED1_PIN;
  myleds[1] .portname='A';

  /* Configure the User LED */

  LED2_Init(myleds[1]);
  /* turn the LED on */
  LED2_On(myleds[1]);

    /* loop for ever */
    while (1)
      {
	LED2_On(myleds[1]);
	HAL_Delay(1000);  //delay for 1000 milliseconds - namely 1 second
	LED2_Off(myleds[1]);
	HAL_Delay(1000);  //delay for 1000 milliseconds - namely 1 second
      }

}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/*
Inititalise the LED2 GPIO port
*/

void LED2_Init(struct led this)
{

   GPIO_InitTypeDef  gpio_init_structure;
  
   switch (this.portname){
   case 'A':
     LED1_GPIO_CLK_ENABLE();
   break;
   case 'B':
     LED2_GPIO_CLK_ENABLE();
   break;
   }
  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin   = this.pin;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(this.port, &gpio_init_structure);
}

/*

deinit the GPIO for LED2

*/
void LED2_DeInit(struct led this)
{
  GPIO_InitTypeDef  gpio_init_structure;

  switch (this.portname){
   case 'A':
     LED1_GPIO_CLK_DISABLE();
   break;
   case 'B':
     LED2_GPIO_CLK_DISABLE();
   break;
   }
  gpio_init_structure.Pin = this.pin;
  
  /* Turn off LED */
  HAL_GPIO_WritePin(this.port, this.pin, GPIO_PIN_RESET);
  HAL_GPIO_DeInit(this.port, gpio_init_structure.Pin);
}


/*

Turn LED2 on

*/
void LED2_On(struct led this)
{
  HAL_GPIO_WritePin(this.port, this.pin, GPIO_PIN_SET);
}

/* 
turn LED2 off
*/

void LED2_Off(struct led this)
{
  HAL_GPIO_WritePin(this.port, this.pin, GPIO_PIN_RESET);
}

void LED2_Toggle(struct led this)
{
  HAL_GPIO_TogglePin(this.port, this.pin);
}

