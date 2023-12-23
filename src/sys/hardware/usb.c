#include <driver/config.h>
#include "usb.h"

static PCD_HandleTypeDef pcd = {
  .Instance = USB_OTG_FS,
  .Init.dev_endpoints = 6,
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

void usb_init() {

  // initialize gpio
  HAL_GPIO_Init(GPIO_PORT_USB, &GPIO_USB);

  // enable bus clk
  __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

  // HAL_PCD_Init();
  // HAL_PCD_Start()


}
