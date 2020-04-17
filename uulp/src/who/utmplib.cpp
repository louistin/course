#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>

#define NRECS   16
#define NULLUT  ((struct utmp *)NULL)
#define UTSIZE  (sizeof(struct utmp))

static char utmp_buff[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open(char *filename) {
    fd_utmp = open(filename, O_RDONLY);
    cur_rec = 0;
    num_recs = 0;

    return fd_utmp;
}

// 一次性读取多条数据到缓冲区中
// 注意 read() 每次调用后, 文件指针会产生位移
static int utmp_reload() {
    int amt_read;
    amt_read = read(fd_utmp, utmp_buff, sizeof(utmp_buff));
    num_recs = amt_read / UTSIZE;
    cur_rec = 0;

    return num_recs;
}

struct utmp *utmp_next() {
    struct utmp *recp;
    if (fd_utmp == -1) {
        return NULLUT;
    }

    // 当前数目与读取值相等时, 表示当前数组处理完毕, 需要再次读入一组数据
    if (cur_rec == num_recs && utmp_reload() == 0) {
        return NULLUT;
    }

    recp = (struct utmp *)&utmp_buff[cur_rec * UTSIZE];
    cur_rec++;

    return recp;
}

void utmp_close() {
    if (fd_utmp != -1) {
        close(fd_utmp);
    }
}