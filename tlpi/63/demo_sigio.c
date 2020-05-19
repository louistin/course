#include <signal.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static volatile sig_atomic_t got_sigio = 0;

// 信号处理例程
static void sigio_handler(int sig) {
  printf("sigio handler.\n");
  got_sigio = 1;
}

int main(int argc, char const *argv[]) {
  int flags, j, cnt;
  char str[32];
  char ch;
  struct sigaction sa;
  int done;

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  // 1. 为内核发送的通知信号安装一个信号处理例程
  sa.sa_handler = sigio_handler;
  if (sigaction(SIGIO, &sa, NULL) == -1) {
    perror("sigaction");
    return -1;
  }

  // 2. 设定文件描述符的属主, 即接收通知信号的进程或进程组
  if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1) {
    perror("fcntl(F_SETOWN)");
    return -1;
  }

  flags = fcntl(STDIN_FILENO, F_GETFL); // 获取当前 flags
  // 3. 设定 O_NONBLOCK 使能非阻塞 I/O
  //        O_ASYNC 使能信号驱动 I/O
  if (fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1) {
    perror("fcntl(F_SETFL)");
    return -1;
  }

  // TODO: 这里需要做什么暂时没搞清楚

  // 6. 信号驱动 I/O 是边缘触发通知, 一旦进程被通知 I/O 就绪, 应尽可能多的执行 I/O, 直到
  // 系统调用失败位置, 此时错误码为 EAGAIN 或 EWOULDBLOCK
  if (got_sigio) {
    while (read(STDIN_FILENO, &ch, 1) > 0) {
      printf("cnt = %d, read %c\n", cnt, ch);
    }
  }

  return 0;
}
