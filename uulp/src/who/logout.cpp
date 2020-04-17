#include "logout.h"
#include <utmp.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int logout_tty(char *line) {
    int fd;
    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;

    if ((fd = open(UTMP_FILE, O_RDWR)) == -1) {
        return -1;
    }

    // 负责注销的程序, 循环读取 utmp 记录
    while (read(fd, &rec, len) == len) {
        // 当 Devicename 与当前终端一致时, 将登陆状态置于 DEAD_PROCESS
        // ut_time 字段需要改为注销时间, 即当前时间
        // 操作文件指针, 将数据回写
        if (strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0) {
            rec.ut_type = DEAD_PROCESS;
            if (time((time_t *)&rec.ut_time) != -1) {
                if (lseek(fd, -len, SEEK_CUR) != -1) {
                    if (write(fd, &rec, len) == len) {
                        retval = 0;
                    }
                }
            }

            break;
        }
    }

    if (close(fd) == -1) {
        retval = -1;
    }

    return retval;
}