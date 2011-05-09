#include <stdlib.h>

#include <iostream>
#include <string>

#include "socket.h"


int main(int argc, char *argv[])
{
    std::string host = "127.0.0.1";
    int port = 2222;
    std::string content = "hello, world!";
    if (argc > 1)
    {
        host = argv[1];
    }
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }
    if (argc > 3)
    {
        content = argv[3];
    }

    std::cout << "host:" << host << std::endl;
    std::cout << "port:" << port << std::endl;
    std::cout << "content:" << content << std::endl;
    sock::Socket s;
    if (!s.Connect(host, port))
    {
        std::cout << "connect failed!" << std::endl;
        exit(1);
    }
    std::cout << "sending" << std::endl;
    if (!s.Send(content))
    {
        std::cout << "send failed!" << std::endl;
    }
    std::string recv;
    s.Recv(recv);
    std::cout << "received:" << recv << std::endl;

//    s.Close();
    if (!s.Send(content + content))
    {
        std::cout << "send failed!" << std::endl;
    }

    if (!s.Reconnect())
    {
        std::cout << "reconnect failed!" << std::endl;
        exit(1);
    }
    std::cout << "sending" << std::endl;
    if (!s.Send(content + content))
    {
        std::cout << "send failed!" << std::endl;
    }
    s.Recv(recv);
    std::cout << "received:" << recv << std::endl;

    return 0;
}
