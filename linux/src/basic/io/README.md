# I/O 多路复用

## EPOLL
> 显著减少程序在大量并发连接中只有少量活跃情况下的CPU占用率, 不会复用文件描述符集合来传递结果, 开发者每次等待时间之前必须重新准备要被监听的文件描述符集合
> 获取事件时, 无需遍历整个被侦听的描述符集, 只要遍历被内核IO事件异步唤醒加入 Ready 队列的描述符集合

### 工作模式

* LT 水平触发
    - 缺省模式, 同时支持 BLOCK 和 NO-BLOCK Socket
    - 内核通知用户一个文件描述符是否就绪, 然后可以对该就绪的 fd 执行 IO 操作; 如果未操作, 内核会一直通知

* ET 边缘触发
    - 高速工作方式, 只支持 NO-BLOCK Socket
    - 内核会在描述符从未就绪到就绪时通知用户, 然后假设用户已经知道描述符就绪, 并不再发送就绪通知. 直到用户执行某种操作, 导致描述符再次未就绪

### 接口
    ```c++
    #include <sys/epoll.h>

    // 创建一个 epoll 句柄
    // size 告诉内核监听数目
    // 创建好 epoll 句柄后, 会占用一个 fd 值, 使用完后必须调用 close() 关闭
    int epoll_create(int size);

    // epoll 事件注册函数
    // epfd 上一步创建的句柄
    // op 执行动作
    //      EPOLL_CTL_ADD   注册新的 fd 到 epfd 中
    //      EPOLL_CTL_MOD   修改已注册的 fd 监听事件
    //      EPOLL_CTL_DEL   从 epfd 中删除一个 fd
    // fd 需要监听的 fd
    // event 监听事件
    //      EPOLLIN     文件描述符可读
    //      EPOLLOUT    文件描述符可写
    //      EPOLLPRI    文件描述符有紧急数据可读(带外数据)
    //      EPOLLERR    文件描述符发生错误
    //      EPOLLHUP    文件描述符被挂断
    //      EPOLLET     将 EPOLL 设置为边缘触发模式
    //      EPOLLONESHOT    只监听一次事件, 监听完毕后, 如需继续监听, 需要重新将此 Socket 加入到 EPOLL 队列中
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

    // 等待事件的产生
    // events 从内核得到的事件集合
    // maxevents 告诉内核 events 的大小(不能大于创建时的 size 值)
    // timeout 超时时间 ms (0, 立即返回; -1 不确定/阻塞, >0 等待时间)
    //          epoll 处于单独线程时, 可以设置为 -1 阻塞等待; 和主逻辑一个线程时, 设为 0 保证效率
    // 返回需要处理的事件数目, 返回 0 表示已超时
    int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
    ```

## 使用框架
```c++
int nfds;

for (;;) {
    nfds = epoll_wait(epfd, )
}
```


