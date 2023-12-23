#include <stdint.h>
#include <stm32f4xx_it.h>

#include <sys/fault.h>
#include <driver/config.h>
#include <app/input/gamebtn.h>

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief this function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
  panic();
}

/**
  * @brief this function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
  // for(;;);
  panic();
}

/**
  * @brief this function handles Memory management fault.
  */
void MemManage_Handler(void) {
  panic();
}

/**
  * @brief this function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
  panic();
}

/**
  * @brief this function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
  panic();
}

/**
  * @brief this function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {

}

/**
  * @brief this function handles Debug monitor.
  */
void DebugMon_Handler(void) {

}

/**
  * @brief this function handles Pendable request for system service.
  */
void PendSV_Handler(void) {

}

/**
  * @brief this function handles System tick timer.
  */
void SysTick_Handler(void) {
  HAL_IncTick();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief this function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_0);
}

/**
  * @brief this function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_1);
}

/**
  * @brief this function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_2);
}

/**
  * @brief this function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_3);
}

/**
  * @brief this function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_4);
}

/**
  * @brief this function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_5);
  __exti_gamebtn_handler(GPIO_PIN_6);
  __exti_gamebtn_handler(GPIO_PIN_7);
}

/**
  * @brief this function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void) {
  __exti_gamebtn_handler(GPIO_PIN_15);
}
