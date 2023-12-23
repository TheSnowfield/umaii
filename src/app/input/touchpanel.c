#include "driver/mpr121.h"
#include "touchpanel.h"

void touchpanel_init() {
  mpr121_init();
  // mpr121_set_irqcb(cb_mpr121_touched);
  // mpr121_enable_irq(true);
}

void touchpanel_load_usercfg() {
  // todo: load user config

}

void touchpanel_enable_irq() {

}
