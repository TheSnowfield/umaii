#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <sys/fault.h>
#include <sys/dfu.h>
#include <sys/tick.h>
#include <hardware/common.h>

#include <app/app.h>
#include <app/led.h>
#include <frost/api.h>

/**
 * @brief reconfigure rcc
 */
static void __rcc_reconfigure() {

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  if(HAL_RCC_OscConfig(&(RCC_OscInitTypeDef) {
    .OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE,
    .HSEState = RCC_HSE_ON,
    .LSEState = RCC_LSE_OFF,
    .LSIState = RCC_LSI_ON,
    .PLL.PLLState = RCC_PLL_ON,
    .PLL.PLLSource = RCC_PLLSOURCE_HSE,
    .PLL.PLLM = 25,
    .PLL.PLLN = 200,
    .PLL.PLLP = RCC_PLLP_DIV2,
    .PLL.PLLQ = 2,
    .PLL.PLLR = 2
  }) != HAL_OK) fault_ex(err_hwinit);

  if(HAL_RCC_ClockConfig(&(RCC_ClkInitTypeDef) {
    .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV2,
    .APB2CLKDivider = RCC_HCLK_DIV2
  }, FLASH_LATENCY_2) != HAL_OK) fault_ex(err_hwinit);

  if(HAL_RCCEx_PeriphCLKConfig(&(RCC_PeriphCLKInitTypeDef) {
    .PeriphClockSelection = RCC_PERIPHCLK_RTC,
    .RTCClockSelection = RCC_RTCCLKSOURCE_LSI
  }) != HAL_OK) fault_ex(err_hwinit);

  if(HAL_RCCEx_PeriphCLKConfig(&(RCC_PeriphCLKInitTypeDef) {
    .PeriphClockSelection = RCC_PERIPHCLK_FMPI2C1,
    .RTCClockSelection = RCC_FMPI2C1CLKSOURCE_SYSCLK
  }) != HAL_OK) fault_ex(err_hwinit);

  if(HAL_RCCEx_PeriphCLKConfig(&(RCC_PeriphCLKInitTypeDef) {
    .PLLI2S.PLLI2SN = 150,
    .PLLI2S.PLLI2SM = 26,
    .PLLI2S.PLLI2SR = 2,
    .PLLI2S.PLLI2SQ = 3,
    .Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLI2SQ,
    .PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC
  }) != HAL_OK) fault_ex(err_hwinit);

  HAL_PWR_EnableBkUpAccess(); {
    // __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);
    __HAL_RCC_RTC_ENABLE();
  }

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void __gpio_reconfigure() {

  // deinit all pin without SWDIO(13) and SWCLK(14)
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All ^ (GPIO_PIN_13 | GPIO_PIN_14));
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_All);
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_All);
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  // wait clks to be ready
  HAL_Delay(100);
}

/**
 * @brief blink fault reason
 *
 * @param reason the reason id
 */
void __blink_fault_reason(status_t reason) {

  // HAL_Init();
  // __rcc_reconfigure();
  // __HAL_RCC_GPIOC_CLK_ENABLE();
  // __HAL_RCC_GPIOH_CLK_ENABLE();
  // __HAL_RCC_GPIOA_CLK_ENABLE();
  // __HAL_RCC_GPIOB_CLK_ENABLE();
  // __HAL_RCC_GPIOD_CLK_ENABLE();

  // initialize led pin only
  led_init();

  while(true) {

    // blink error id
    uint32_t _bit = 0;
    status_t _rea = reason << 24;
    for(uint8_t i = 0; i < 8; ++i) {
      
      _bit = _rea & 0x80000000;

      // show current bit
      tick_delay(500);
      led_enable(true);
      tick_delay(_bit ? 200 : 50);
      led_enable(false);

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
