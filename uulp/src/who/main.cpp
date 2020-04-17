#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//#define SHOWHOST

static void show_time(long timeval) {
    char *cp = ctime(&timeval);
    printf("%24s", cp + 4); // 不读取开始的星期数, cp 默认带有 \n
}

static void show_info(struct utmp *buff) {
    if (buff->ut_type != USER_PROCESS) {
        return;
    }

    printf("%-8s", buff->ut_user);  // 左对齐, 8位长度, 不够补空格
    printf(" ");
    printf("%-8s", buff->ut_line);
    printf(" ");
    show_time(buff->ut_tv.tv_sec);
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", buff->ut_host);
#endif
    printf("\n");
}

int main(int argc, char const *argv[]) {
    struct utmp current_record;
    int record_len = sizeof(struct utmp);
    int utmp_fd;

    if ((utmp_fd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE);
        exit(1);
    }

    while (read(utmp_fd, &current_record, record_len) == record_len) {
        show_info(&current_record);
    }

    close(utmp_fd);

    return 0;
}