#include <stdlib.h>

#include <iostream>

#include "socket.h"


int main(int argc, char *argv[])
{
    int port = 2222;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    sock::Socket s;
    s.SetTimeout(10, 100, 100);
    if (!s.Bind(port))
    {
        std::cout << "bind failed!" << std::endl;
    }
    if (!s.Listen())
    {
        std::cout << "listen failed!" << std::endl;
    }
    std::cout << "listen on:" << port << std::endl;

    while (true)
    {
        sock::Socket ns;
        std::cout << "accepting" << std::endl;
        if (s.Accept(ns))
        {
            std::cout << "accepted!" << std::endl;
        }
        else
        {
            std::cout << "accept error!" << std::endl;
            continue;
        }
        char buf[1000];
        int len = ns.Recv(buf, 1000);
        if (len <= 0)
        {
            std::cout << "recv return " << len << std::endl;
        }
        else
        {
            std::cout << "received:" << buf << std::endl;
        }
        ns.Send(buf, len);
    }
    return 0;
}
