#include <stdio.h>
#include <string.h>

#include <config.h>
#include <sys/tick.h>
#include <sys/dfu.h>
#include <sys/event.h>
#include <sys/fault.h>
#include <sys/debug.h>

#include <manager/resource.h>
#include <manager/version.h>

#include <input/gamebtn.h>
#include <input/touchpanel.h>

#include <app/led.h>
#include <app/light.h>
#include <app/misc/logprint.h>

#include <frost/api.h>

#include <tusb_config.h>
#include <tusb.h>

// void cdc_task(void) {
//   // connected() check for DTR bit
//   // Most but not all terminal client set this when making connection
//   // if ( tud_cdc_connected() )
//   {
//     // connected and there are data available
//     if (tud_cdc_available()) {
//       // read data
//       char buf[64];
//       uint32_t count = tud_cdc_read(buf, sizeof(buf));
//       (void) count;

//       // Echo back
//       // Note: Skip echo by commenting out write() and write_flush()
//       // for throughput test e.g
//       //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
//       tud_cdc_write(buf, count);
//       tud_cdc_write_flush();
//     }
//   }
// }

void appmain() {

  // init logprint
  logprint_init(); {
    logprint_print("Welcome to UmaiiDX!\r\n");
    logprint_print(BUILD_VERSION);
    logprint_print("\r\n===================================\r\n");
  }

  // turn on led strip
  light_init(3 * 8); {
#ifdef UMAIIDX_LIGHT_BOOTANIM
    light_show_mode(BOOTANIM_TYPE, BOOTANIM_SPEED);
#endif
  }

  // turn on status led
  led_init(); {
    led_enable(true);
  }

  // initialize resources
  // if(!resource_init(0x08010000)) {
  //   panic(NULL);
  //   return;
  // }

  // initialize usb
  // usb_init();
  // tusb_init(); {
  //   frost_task_interval(0, &tud_task, NULL);
  //   frost_task_interval(0, &cdc_task, NULL);
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
