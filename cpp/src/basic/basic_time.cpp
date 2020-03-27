#include "basic_time.h"

static int basic_time_test()
{
    time_t now = time(NULL);

    char *dt = ctime(&now);

    cout << "local time: " << dt << endl;

    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);

    cout << "UTC: " << dt << endl;

    return 0;
}

int basic_time_init() {
    basic_time_test();
    return 0;
}