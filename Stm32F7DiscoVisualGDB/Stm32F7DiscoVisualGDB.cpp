#include <stm32f7xx_hal.h>
#include <stm32746g_discovery.h>
#include <stm32746g_discovery_lcd.h>
#include <cstdlib>
#include "logo.h"
#include "CStringBuilder.hpp"


const uint_fast16_t benchmarkTimeLength = 1000;    // Time of one benchmark in systicks (ms)
volatile uint_fast16_t tickDownCount = benchmarkTimeLength;


#ifdef __cplusplus
extern "C"
#endif
 void SysTick_Handler(void)
{
    if (tickDownCount) {
        tickDownCount--;
    }
    
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}   
    


    


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	  /* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;  
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
  
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if (ret != HAL_OK)
	{
		while (1) {
			;
		}
	}
  
	/* Activate the OverDrive to reach the 216 MHz Frequency */  
	ret = HAL_PWREx_EnableOverDrive();
	if (ret != HAL_OK)
	{
		while (1) {
			;
		}
	}
  
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 
  
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if (ret != HAL_OK)
	{
		while (1) {
			;
		}
	}  
}

int main(void)
{
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	SCB_EnableICache();
	SCB_EnableDCache();
  
	HAL_Init();
  
	SystemClock_Config();

    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize()*BSP_LCD_GetYSize() * 4));
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SelectLayer(1);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"Ha Ha Ha", LEFT_MODE); 
    
  //  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)"Ha!"); 
    //BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    
    // ---------- EMPTY BENCHMARK -------------
    tickDownCount = benchmarkTimeLength;
    uint64_t count = 0;
    int x = std::rand();
    char buffer[100] = "simple insertion of x=10";
    
    while (tickDownCount) {
        x = std::rand();
        count++;
        if (!x)
        {
            BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)buffer, LEFT_MODE); 
        }
    }
    
    
    sprintf(buffer, "Simple rnd %ul", count);
    BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)buffer, LEFT_MODE); 
    
    
    // -------- snprintf1 BENCHMARK -----------
    tickDownCount = benchmarkTimeLength;
    count = 0;
    x = std::rand();    
    while (tickDownCount) {
        x = std::rand() % 10000;
        snprintf(buffer, 100, "simple insertion of x=%i", std::rand());
        count++;
        if (!x)
        {
            BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)buffer, LEFT_MODE); 
        }
    }
    sprintf(buffer, "snprintf1 %ul", count);
    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)buffer, LEFT_MODE);
    
    
    // -------- csb test BENCHMARK -----------
    tickDownCount = benchmarkTimeLength;
    count = 0;
    
    while (tickDownCount) {
        x = std::rand()%10000;        
        CStringBuilder sb(buffer, 100);
        sb.append("simple insertion of x=");
        sb.append((uint32_t)x);
        count++;
        if (!x)
        {
            BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)buffer, LEFT_MODE); 
        }
    }
    sprintf(buffer, "csb %ul", count);
    BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)buffer, LEFT_MODE); 
    
    
    // -------- REVERSE BENCHMARK 1-----------
    uint32_t start = HAL_GetTick();
    uint64_t countDown = 10000000;
    
    while (countDown--) {
        x = std::rand();
        count++;
        if (!x)
        {
            BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)buffer, LEFT_MODE); 
        }
    }
    uint32_t stop = HAL_GetTick();
    sprintf(buffer, "rand %u", stop - start);
    BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)buffer, LEFT_MODE);
    
    
    // -------- REVERSE BENCHMARK 2 -----------
    start = HAL_GetTick();
    countDown = 10000000;
    
    while (countDown--) {
        x = std::rand();
        snprintf(buffer, 100, "simple insertion of x=%i", x);
        count++;
        if (!x)
        {
            BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)buffer, LEFT_MODE); 
        }
    }
    stop = HAL_GetTick();
    sprintf(buffer, "snprintf %u", stop - start);
    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)buffer, LEFT_MODE);
    
    
    // -------- REVERSE BENCHMARK 3 -----------
    start = HAL_GetTick();
    countDown = 10000000;
    
    while (countDown--) {
        x = std::rand();        
        CStringBuilder sb(buffer, 100);
        sb.append("simple insertion of x=");
        sb.append(x);
        count++;
        if (!x)
        {
            BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)buffer, LEFT_MODE); 
        }
    }
    stop = HAL_GetTick();
    sprintf(buffer, "CStringBuilder %u", stop - start);
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)buffer, LEFT_MODE);
    
    
	for (;;)
	{
    	    	
	}
}

