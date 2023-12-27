#include <math.h>
#include <driver/ws2812.h>
#include <sys/tick.h>
#include <sys/debug.h>
#include <frost/api.h>

#include "light.h"

static frost_task_ctx_t* _task_anim = NULL;

typedef void(* effect_runner_t)(uint64_t time);

/**
 * @brief [anim renderer] rainbow spinning effect
 *
 */
static void __anim_renderer_rainbow_spin(uint64_t time) {


}

/**
 * @brief [anim renderer] white spinning effect
 *
 */
static void __anim_renderer_white_spin(uint64_t time) {

  size_t _freq;
  tls_get_value(1, &_freq);

  size_t _mid = (time / _freq) % 23;
  int8_t _off_s1 = _mid - 1;
  int8_t _off_s2 = _mid - 2;
  int8_t _off_a1 = _mid + 1;
  int8_t _off_a2 = _mid + 2;

  // pattern -=#=-
  ws2812_clear( ws2812_rgb(0, 0, 0)); {
    ws2812_set_pixel(_off_s2 < 0 ? 23 + _off_s2 : _off_s2, ws2812_rgb(5, 5, 5));
    ws2812_set_pixel(_off_s1 < 0 ? 23 + _off_s1 : _off_s1, ws2812_rgb(50, 50, 50));
    ws2812_set_pixel(_mid, ws2812_rgb(255, 255, 255));
    ws2812_set_pixel(_off_a1 >= 23 ? _off_a1 - 23 : _off_a1, ws2812_rgb(50, 50, 50));
    ws2812_set_pixel(_off_a2 >= 23 ? _off_a2 - 23 : _off_a2, ws2812_rgb(5, 5, 5));
  }
}

/**
 * @brief [anim renderer] white breath effect
 *
 */
static void __anim_renderer_white_breath(uint64_t time) {
  
  size_t _freq;
  tls_get_value(1, &_freq);

  uint8_t _bright = 0; {
    _bright = (sinf((time - (_freq / 2.0) * 3.1415926) / _freq) + 1) * (127.5);
  }

  ws2812_clear(ws2812_rgb(_bright, _bright, _bright));
}

static void __callback_task_anim() {

  // run checks
  frost_task_ctx_t* _task;
  if(!frost_ok(frost_task_get_context(&_task)) || !_task->tls) {
    log_warning(TAG, "get task context failed!");
    return;
  }

  // run effect runner
  effect_runner_t _renderer; {
    if(!frost_ok(tls_get_value(0, (size_t *)&_renderer)) || _renderer == NULL) {
      log_warning(TAG, "failed to perform effect runner, invalid anim proc type.");
      return;
    }
  }

  // render the led strip
  _renderer(tick_get_time());
}

void light_init(size_t count) {

  // init and clear ws2812 led strip
  ws2812_init(count); {

    // clear with black(all off)
    ws2812_clear(ws2812_rgb(0, 0, 0));

    // enable dma output
    ws2812_dma_enable(true);
    log_debug(TAG, "ws2812 inited");
  }

  // create effect runner
  // renderring graphics as 24fps per second
  frost_task_interval(1000 / 24, &__callback_task_anim, &_task_anim);

  // suspend the task and activate it as needed
  frost_task_set_flag(_task_anim, frost_flag_suspend);
}

void light_show_mode(light_mode_t mode, uint32_t speed) {

  // create tls context
  if(!_task_anim->tls && !frost_ok(tls_alloc_ex(_task_anim))) {
    log_warning(TAG, "cannot create task tls.");
    return;
  }

  // clear led strip
  ws2812_clear(ws2812_rgb(0, 0, 0)); {
    switch(mode) {
      case light_mode_off:
      case light_mode_passive:
        log_info(TAG, "passive mode");
      break;

      case light_mode_white_spin:
        tls_set_value_ex(_task_anim, 0, (size_t)&__anim_renderer_white_spin);
        tls_set_value_ex(_task_anim, 1, speed);
        log_info(TAG, "set renderer: white spin");

      break;

      case light_mode_white_breath:
        tls_set_value_ex(_task_anim, 0, (size_t)&__anim_renderer_white_breath);
        tls_set_value_ex(_task_anim, 1, speed);
        log_info(TAG, "set renderer: white breath");
      break;

      case light_mode_rainbow_spin:
        tls_set_value_ex(_task_anim, 0, (size_t)&__anim_renderer_rainbow_spin);
        tls_set_value_ex(_task_anim, 1, speed);
        log_info(TAG, "set renderer: rainbow spin");
      break;

      case light_mode_full_white:
        ws2812_clear(ws2812_rgb(255, 255, 255));
        log_info(TAG, "set mode: full white");
      break;
    }
  }

  // enable/disable anim renderer task
  frost_flag_t _task_flag;
  frost_task_get_flag(_task_anim, &_task_flag);
  frost_task_set_flag(_task_anim, ((mode & 1) != 0)
    ? (_task_flag & (~frost_flag_suspend))
    : (_task_flag | frost_flag_suspend)
  );

}
