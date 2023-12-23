#include <sys/dfu.h>
#include <sys/fault.h>
#include <driver/config.h>

void dfu_reset(dfu_flag_t flag) {
  fault_ex(err_dfu_mode);
}

void dfu_jump_to_dlmode() {
  __set_MSP(*(__IO uint32_t *)0x1FFF0000);
  ((void (*)(void))(*(uint32_t *)(0x1FFF0000 + 4)))();
}
