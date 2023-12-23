#ifndef _SYS_EVENT_H
#define _SYS_EVENT_H

typedef enum {
  event_none = 0,

  event_button_down,
  event_button_up,
  event_button_hold,
  event_button_hold_click,
  event_button_release,

} event_t;

typedef uint32_t param_t;

#endif /* _SYS_EVENT_H */
