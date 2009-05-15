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

    sock::Socket s(SOCK_DGRAM);
    if (!s.Bind(port))
    {
        std::cout << "bind failed!" << std::endl;
    }

    while (true)
    {
        std::string h;
        int p = -1;
        char buf[1000];
        std::cout << "host:" << h << "    port:" << p << std::endl;
        int len = s.RecvFrom(buf, 1000, h, p);
        if (len <= 0)
        {
            std::cout << "recvfrom return " << len << std::endl;
            continue;
        }
        std::cout << "received(from " << h << ":" << p
                  << "):" << buf << std::endl;
        if (!s.SendTo(buf, len, h, p))
        {
            std::cout << "sendto error" << std::endl;
        }

        std::cout << "ok...try next" << std::endl;
    }
    return 0;
}
