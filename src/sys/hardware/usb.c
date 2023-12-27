#include <hardware/common.h>
#include "usb.h"


static PCD_HandleTypeDef pcd = {
  .Instance = USB_OTG_FS,
  .Init.dev_endpoints = 8,
  .Init.speed = PCD_SPEED_FULL,
  .Init.dma_enable = DISABLE,
  .Init.phy_itface = PCD_PHY_EMBEDDED,
  .Init.Sof_enable = DISABLE,
  .Init.low_power_enable = DISABLE,
  .Init.lpm_enable = DISABLE,
  .Init.battery_charging_enable = DISABLE,
  .Init.vbus_sensing_enable = DISABLE,
  .Init.use_dedicated_ep1 = DISABLE,
};

#define USB_PORT GPIOA
#define USB_PIN GPIO_PIN_11 | GPIO_PIN_12

void usb_init() {

  // initialize gpio
  HAL_GPIO_Init(USB_PORT, &(GPIO_InitTypeDef) {
    .Pin = USB_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    .Pull = GPIO_NOPULL,
    .Alternate = GPIO_AF10_OTG_FS
  });

  // enable bus clk
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE(); {
    HAL_PCD_Init(&pcd);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  }

  HAL_PCD_Start(&pcd);
}
