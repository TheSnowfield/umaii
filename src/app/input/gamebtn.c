#include <sys/fault.h>
#include <sys/debug.h>
#include <hardware/common.h>
#include <frost/api.h>
#include <config.h>
#include <board/led15070.h>

#include "gamebtn.h"

static gamebtn_stat_t _stat_mask;
static gamebtn_stat_t _old_status;

#define GAMEBTN_PORT GPIOA
#define GAMEBTN_PIN (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_15)

/**
 * @brief initialize the game button
 * @param mode gpio mode
 * @param pull gpio pull mode
 */
static void __init_button(uint32_t mode, uint32_t pull) {
  HAL_GPIO_DeInit(GAMEBTN_PORT, GAMEBTN_PIN);
  HAL_GPIO_Init(GAMEBTN_PORT, &(GPIO_InitTypeDef) {
    .Pin = GAMEBTN_PIN,
    .Mode = mode,
    .Pull = pull,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH
  });

  HAL_GPIO_WritePin(GAMEBTN_PORT, GAMEBTN_IOMASK, GPIO_PIN_SET);
}

/**
 * @brief find bad button
 * @param state current button state
 * @param expected expected value
 */
static status_t __get_bad_button_reason(gamebtn_stat_t state, gamebtn_stat_t expected) {
  return err_bad_button1;
}

/**
 * @brief get button status
 * @return uint16_t status
 */
static gamebtn_stat_t __button_status() {
  return ((gamebtn_stat_t)GAMEBTN_PORT->IDR & GAMEBTN_IOMASK);
}

/**
 * @brief exti nvic handler. see stm32f4xx_it.c
 * @param pin interrupt pin number
 */
void __exti_gamebtn_handler(uint16_t pin) {

  // check exti line interrupt
  if(__HAL_GPIO_EXTI_GET_IT(pin) == RESET)
    return;
  
  // clear the flag
  __HAL_GPIO_EXTI_CLEAR_IT(pin);

  // new button state
  gamebtn_stat_t _newstat = __button_status();
  uint16_t _btn_sel = _newstat & 0b1000000000000000;
  _newstat = _newstat & (~0b1000000000001000);

  if(_btn_sel) _newstat |= 0b1000;

#ifdef UMAIIDX_LIGHT_MUX_GAMEBTN
    led15070_mux_uart_tx((uint8_t*)&_newstat, 2);
#endif
  // }
}

void gamebtn_init() {

  _stat_mask = GAMEBTN_IOMASK;

  // check buttons
  gamebtn_stat_t _state = 0;
  uint32_t _pull = GPIO_PULLDOWN; {

  #if UMAIIDX_PHOTO_INTERRUPTER == NPN_NORMAL_CLOSED

    // NPN type normal closed photo interrupters
    // normal: high resistance (gpio internal pulled up) HIGH
    // triged: pull down, LOW
    _stat_mask = 0; // do not invert value
    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLUP);

    log_info(TAG, "photo interrupter type: npn-normal-closed");

  #elif UMAIIDX_PHOTO_INTERRUPTER == PNP_NORMAL_CLOSED

    // PNP type normal closed photo interrupters
    // normal: high resistance (gpio internal pulled down), DOWN
    // triged: pull up, HIGH
    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLDOWN);

    log_info(TAG, "photo interrupter type: pnp-normal-closed");

  #elif UMAIIDX_PHOTO_INTERRUPTER == NPN_NORMAL_OPEN

    // NPN type normal open photo interrupters
    // normal: pull down, LOW
    // triged: high resistance (gpio internal pulled up), HIGH

    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLUP);

    log_info(TAG, "photo interrupter type: npn-normal-open");

  #elif UMAIIDX_PHOTO_INTERRUPTER == PNP_NORMAL_OPEN

    // PNP type normal open photo interrupters
    // normal: pull up, HIGH
    // triged: high resistance (gpio internal pulled down), DOWN
    _stat_mask = 0; // do not invert value
    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLDOWN);

    log_info(TAG, "photo interrupter type: pnp-normal-open");

  #elif
    #error "Please define your photo interrupter type in the 'config.h'"
  #endif

    // initial state check
    // if((_pull == GPIO_PULLUP   && (_state = __button_status()) != 0) ||
    //    (_pull == GPIO_PULLDOWN && (_state = __button_status()) != GAMEBTN_IOMASK)) {

    //   // state not match the expected value
    //   fault_ex(__get_bad_button_reason(_state, _pull == GPIO_PULLUP ? GPIO_PULLUP : GPIO_PULLDOWN));
    //   return;
    // }
  }

  // checks have passed
  // set the game buttons to interrupt mode
  __init_button(GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP); {

    // save button state as initial state
    _old_status = __button_status();
  }

  log_info(TAG, "game button initialized");

}

void gamebtn_enable_irq(bool en) {

  if(en) {
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  }

  else {
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_NVIC_DisableIRQ(EXTI1_IRQn);
    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    HAL_NVIC_DisableIRQ(EXTI4_IRQn);
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  }
}

gamebtn_stat_t gamebtn_get_stat() {
  return _old_status;
}

