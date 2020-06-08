from socket import *
from datetime import *

def main():
    tcp_client_socket = socket(AF_INET,SOCK_STREAM)
    tcp_client_socket.connect(("111.229.202.186", 9999))

    while True:
        now_time = datetime.now()
        meg = now_time
        tcp_client_socket.send(str(meg))
        print("send:", str(meg))
        recv_data = tcp_client_socket.recv(1024)
        if recv_data:
            print("recv:", recv_data)
        else:
            print("error")
            break

    tcp_client_socket.close()


if __name__ == '__main__':
    main()