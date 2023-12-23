#include <sys/fault.h>
#include <driver/config.h>
#include <usrcfg.h>

#include <frost/api.h>

#include "gamebtn.h"

static gamebtn_stat_t _stat_mask;
static gamebtn_stat_t _old_status;

/**
 * @brief initialize the game button
 * @param mode gpio mode
 * @param pull gpio pull mode
 */
static void __init_button(uint32_t mode, uint32_t pull) {
  HAL_GPIO_DeInit(GPIO_PORT_GAMEBTN, GPIO_GAMEBTN.Pin);
  HAL_GPIO_Init(GPIO_PORT_GAMEBTN, &(GPIO_InitTypeDef) {
    .Pin = GPIO_GAMEBTN.Pin,
    .Mode = mode,
    .Pull = pull,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH
  });

  HAL_GPIO_WritePin(GPIO_PORT_GAMEBTN, GAMEBTN_IOMASK, GPIO_PIN_SET);
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
  return (gamebtn_stat_t)GPIO_PORT_GAMEBTN->IDR & GAMEBTN_IOMASK;
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

  // update button state
  gamebtn_stat_t _newstat = __button_status();
  if(_newstat != _old_status) {
    _old_status = _newstat;

    // _newstat

  }
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

  #elif UMAIIDX_PHOTO_INTERRUPTER == PNP_NORMAL_CLOSED

    // PNP type normal closed photo interrupters
    // normal: high resistance (gpio internal pulled down), DOWN
    // triged: pull up, HIGH
    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLDOWN);

  #elif UMAIIDX_PHOTO_INTERRUPTER == NPN_NORMAL_OPEN

    // NPN type normal open photo interrupters
    // normal: pull down, LOW
    // triged: high resistance (gpio internal pulled up), HIGH
    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLUP);

  #elif UMAIIDX_PHOTO_INTERRUPTER == PNP_NORMAL_OPEN

    // PNP type normal open photo interrupters
    // normal: pull up, HIGH
    // triged: high resistance (gpio internal pulled down), DOWN
    _stat_mask = 0; // do not invert value
    __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLDOWN);

  #elif
    #error "Please define your photo interrupter type in the 'config.h'"
  #endif

    // switch(userconf.photointpr_type) {
    
    //   // NPN type normal closed photo interrupters
    //   // normal: high resistance (gpio internal pulled up) HIGH
    //   // triged: pull down, LOW
    //   case photointpr_type_npn_nc:
    //     _stat_mask = 0; // do not invert value
    //     __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLUP);
    //   break;

    //   // PNP type normal closed photo interrupters
    //   // normal: high resistance (gpio internal pulled down), DOWN
    //   // triged: pull up, HIGH
    //   case photointpr_type_pnp_nc:
    //     __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLDOWN);
    //   break;

    //   // NPN type normal open photo interrupters
    //   // normal: pull down, LOW
    //   // triged: high resistance (gpio internal pulled up), HIGH
    //   case photointpr_type_npn_no:
    //     __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLUP);
    //   break;

    //   // PNP type normal open photo interrupters
    //   // normal: pull up, HIGH
    //   // triged: high resistance (gpio internal pulled down), DOWN
    //   case photointpr_type_pnp_no:
    //     _stat_mask = 0; // do not invert value
    //     __init_button(GPIO_MODE_OUTPUT_PP, _pull = GPIO_PULLDOWN);
    //   break;

    // }

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

