#ifndef BOX_MONITOR_H
#define BOX_MONITOR_H

#include "box_error.h"

void box_monitor_add_error(const char *func, box_status status);
bool box_monitor_ok();
box_status box_monitor_last_error();
const char *box_monitor_last_error_string();
void box_monitor_clear();

#define BOX_ERROR(__stat) (box_monitor_add_error(__func__, __stat))
#define BOX_OK (box_monitor_ok())
#define BOX_LAST_ERROR (box_monitor_last_error())
#define BOX_LAST_ERROR_STRING (box_monitor_last_error_string())
#define BOX_ERROR_CLEAR (box_monitor_clear())

#endif // BOX_MONITOR_H
