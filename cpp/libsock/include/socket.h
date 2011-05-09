/**
 * Copyright (C) 2008 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


#ifndef SOCK_SOCKET_H_
#define SOCK_SOCKET_H_


// #include <standard library headers>
#include <string>

// #include <other library headers>
#ifdef _WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <memory.h>
#include <arpa/inet.h>
#endif

// #include "customer headers"


#ifdef _WIN32
typedef int socklen_t;
#else
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#endif


namespace sock
{

    const int MAXSEND = 1000;
    const int MAXRECV = 1000;

    class Socket
    {
    public:
        Socket(int type = SOCK_STREAM,
               unsigned int connto = 10,
               unsigned int recvto = 10,
               unsigned int sendto = 10);
        virtual ~Socket();

    public:
        // Server initialization
        bool Bind(int port);
        bool Listen();
        bool Accept(Socket& ns);

        // Client initialization
        bool Connect(const std::string& host, int port);
        bool Reconnect();

        // TCP
        bool Send(const std::string& s);
        bool Send(const char* buf, int len);
        int Recv(std::string& s);
        int Recv(char* buf, int len);

        // UDP
        bool SendTo(const std::string& s, const std::string& host, int port);
        bool SendTo(const char* buf, int len, const std::string& host,
                    int port);
        int RecvFrom(std::string& s, std::string& host, int& port);
        int RecvFrom(char* buf, int len, std::string& host, int& port);

        void SetTimeout(unsigned int connto,
                        unsigned int recvto,
                        unsigned int sendto);
        bool IsValid() const;

        const sockaddr_in& GetSocketAddr() const { return mAddr; }
        std::string GetIp() const { return inet_ntoa(mAddr.sin_addr); }
        u_short GetPort() const { return ntohs(mAddr.sin_port); }

    private:
        bool SetNonBlocking(const bool b);
        bool Create(int type);
        void Close();
        int Select(int type, long sec, long usec);

        static void PrintError();
        static bool MakeSockaddr(sockaddr_in* addr, const std::string& host,
                                 int port);
        static bool ParseSockAddr(const sockaddr_in& addr, std::string& host,
                                  int& port);

    private:
        Socket(const Socket& sock);
        Socket& operator = (const Socket& sock);

    private:
        SOCKET mSock;
        unsigned int mConnTimeout;
        unsigned int mRecvTimeout;
        unsigned int mSendTimeout;
        sockaddr_in mAddr;
        std::string mHost;
        int mPort;

    }; // end of class Socket

}


#endif // SOCK_SOCKET_H_
