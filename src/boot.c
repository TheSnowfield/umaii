#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <sys/fault.h>
#include <sys/dfu.h>
#include <sys/tick.h>
#include <driver/config.h>

#include <app/app.h>

#include <frost/api.h>

static void __rcc_reconfigure() {

  uint32_t VOLTAGE_SCALE;
  uint32_t FLASH_LATENCY;
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

#ifdef STM32F411xE

  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;

  FLASH_LATENCY = FLASH_LATENCY_3;
  VOLTAGE_SCALE = PWR_REGULATOR_VOLTAGE_SCALE1;

#elif STM32F401xC

  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  
  // 25Mhz OSC up to 80Mhz
  // RCC_OscInitStruct.PLL.PLLN = 64;

  // 25Mhz OSC Overclock to 120Mhz
  RCC_OscInitStruct.PLL.PLLN = 96;

  // 25Mhz OSC Overclock to 125Mhz
  // RCC_OscInitStruct.PLL.PLLN = 100;

  // 25Mhz OSC Overclock to 130Mhz (unstable)
  // RCC_OscInitStruct.PLL.PLLN = 104;

  FLASH_LATENCY = FLASH_LATENCY_2;
  VOLTAGE_SCALE = PWR_REGULATOR_VOLTAGE_SCALE1;

#elif STM32F412Rx

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;

  FLASH_LATENCY = FLASH_LATENCY_2;
  VOLTAGE_SCALE = PWR_REGULATOR_VOLTAGE_SCALE1;
  
#else
#error "RCC: Unsupported chip."
#endif

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(VOLTAGE_SCALE);

  HAL_PWR_EnableBkUpAccess(); {
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_HSE_DIV2);
    __HAL_RCC_RTC_ENABLE();
  }

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) panic(NULL);
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY) != HAL_OK) panic(NULL);
  
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void __gpio_reconfigure() {

  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All ^ GPIO_SWDEBUG.Pin);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  // configure serial interface
  __HAL_RCC_USART1_CLK_ENABLE(); {
    HAL_GPIO_Init(GPIOA, &port_usart1);
    HAL_UART_Init(&usart1);
  }

  // wait pheripherals to be ready
  HAL_Delay(100);
}

/**
 * @brief blink fault reason
 *
 * @param reason the reason id
 */
void __blink_fault_reason(status_t reason) {

  // HAL_Init();
  __rcc_reconfigure();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  // initialize led pin only
  HAL_GPIO_DeInit(GPIO_PORT_LED, GPIO_LED.Pin);
  HAL_GPIO_Init(GPIO_PORT_LED, &GPIO_LED);
  HAL_GPIO_WritePin(GPIO_PORT_LED, GPIO_LED.Pin, GPIO_PIN_SET);

  while(true) {

    // blink error id
    uint32_t _bit = 0;
    status_t _rea = reason << 24;
    for(uint8_t i = 0; i < 8; ++i) {
      
      _bit = _rea & 0x80000000;

      // show current bit
      tick_delay(500);
      HAL_GPIO_WritePin(GPIO_PORT_LED, GPIO_LED.Pin, GPIO_PIN_RESET);
      tick_delay(_bit ? 200 : 50);
      HAL_GPIO_WritePin(GPIO_PORT_LED, GPIO_LED.Pin, GPIO_PIN_SET);

      // next bit
      _rea <<= 1;

    }

    tick_delay(2000);
  }
}

int main() {

  // after reset, check reset reason then do action
  switch(get_fault_reason()) {
    case err_ok: break;
    case err_dfu_mode: dfu_jump_to_dlmode(); break;
    default: __blink_fault_reason(get_fault_reason()); break;
  }

  HAL_Init(); {

    // reconfigure rcc
    __rcc_reconfigure();

    // reconfigure gpio
    __gpio_reconfigure();
  }

  // async call appmain
  frost_init();
  frost_task_run(appmain);

  // run scheduler
  while(1) frost_schedule_tasks();
}
