#include "basic.h"
#include "basic_string.h"
#include "basic_reference.h"
#include "basic_time.h"

int basic_init() {
    easylogger_init();
    elog_set_text_color_enabled(false);

    basic_string_init();
    basic_time_init();
    basic_reference_init();

    LOG_INFO("basic init ok.");

    return 0;
}