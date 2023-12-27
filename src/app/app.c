#include <stdio.h>
#include <string.h>

#include <sys/tick.h>
#include <sys/dfu.h>
#include <sys/event.h>
#include <sys/fault.h>
#include <sys/debug.h>

#include <manager/resource.h>
#include <manager/version.h>

#include <input/gamebtn.h>
#include <input/touchpanel.h>
#include <hardware/usb.h>
#include <hardware/usbhub.h>

#include <app/led.h>
#include <app/light.h>
#include <app/misc/logprint.h>

#include <frost/api.h>

#include <tusb_config.h>
#include <tusb.h>

void appmain() {

  // init logprint uart
  logprint_init(); {
    logprint_print("UmaiiDX\r\n");
    logprint_print(BUILD_VERSION);
    logprint_print("\r\n===================================\r\n");
  }

  // initialize resources
  // if(!resource_init(0x08010000)) {
  //   panic(NULL);
  //   return;
  // }

  // turn on led
  led_init(); {
    led_enable(true);
  }

  light_init(3 * 8); {
    light_show_mode(light_mode_white_breath);
  }

  // initialize usb
  // usb_init();
  // tusb_init(); {
  //   // frost_task_interval(0, &tud_task, NULL);
  // }

  // setup usbhub oem information
  // usbhub_init(); {
  //   usbhub_eepemu_setup();
  //   usbhub_enable(true);
  //   usbhub_eepemu_release();
  // }
  
  // initialize touch panel
  touchpanel_init(); {
    // touchpanel_enable_irq(true);
  }

  // initialize game button
  gamebtn_init(); {
    gamebtn_enable_irq(true);
  }
}
