#ifndef _SYS_CORE_H
#define _SYS_CORE_H

typedef enum {
  err_ok,
  err_dfu_mode,
  err_fatal_error,
  err_task_timeout,
  err_invalid_parameter,
  err_task_canceled,
  err_out_of_memory,
  err_need_initialize,
  err_bad_button1,
  err_bad_button2,
  err_bad_button3,
  err_bad_button4,
  err_bad_button5,
  err_bad_button6,
  err_bad_button7,
  err_bad_button8,
} status_t;

#define is_ok(setresult, x) (((setresult) = (x)) == err_ok)

#endif /* _SYS_CORE_H */
