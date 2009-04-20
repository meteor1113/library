#include <stdlib.h>

#include <iostream>
#include <string>

#include "socket.hpp"


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
    sock::Socket s(SOCK_DGRAM);
    if (!s.SendTo(content, host, port))
    {
        std::cout << "sendto failed!" << std::endl;
        return 1;
    }
    std::cout << "sendto ok" << std::endl;

    std::string recv;
//    char buf[1000];
    std::string h;
    int p = -1;

    s.RecvFrom(recv, h, p);
    std::cout << "received(from " << h << ":" << p
              << "):" << recv << std::endl;

    return 0;
}

