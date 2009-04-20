/* -*- mode: C++; -*- */

#ifndef MAIL_MAILSENDER_H
#define MAIL_MAILSENDER_H


// #include <standard library headers>
#include <string>
#include <vector>
#include <map>
#include <exception>

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
#include <string>
#include <arpa/inet.h>
#endif

// #include "customer headers"


namespace mail
{


#ifdef _WIN32

    class WSAInitial
    {
    public:
        WSAInitial()
            {
                WSADATA wsaData;
                if (WSAStartup(0x101, &wsaData))
                {
                    exit(-1);
                }
            }
        virtual ~WSAInitial() {WSACleanup();}
    };

#endif


// class MimeContent(Abstract class)
    class MimeContent
    {
    public:
        MimeContent(const std::string& content = "");

    public:
        virtual std::string  GetType() const = 0;
        virtual std::string  GetDisposition() const;
        virtual std::string  GetTransEncoding() const = 0;
        virtual std::string& GetContent() = 0;

    protected:
        std::string _content;
    };


    typedef std::vector<MimeContent*> MimeContents;


// class PlainTextContent
    class TextPlainContent : public MimeContent
    {
    public:
        TextPlainContent(const std::string& content,
                         const std::string& charset = "utf-8");

    public:
        virtual std::string  GetType() const;
        virtual std::string  GetTransEncoding() const;
        virtual std::string& GetContent();

    private:
        std::string _charset;
    };


// class TextHtmlContent
    class TextHtmlContent : public MimeContent
    {
    public:
        TextHtmlContent(const std::string& content,
                        const std::string& charset = "utf-8");

    public:
        virtual std::string  GetType() const;
        virtual std::string  GetTransEncoding() const;
        virtual std::string& GetContent();

    private:
        std::string _charset;
    };


// class AppOctStrmContent
    class AppOctStrmContent : public MimeContent
    {
    public:
        AppOctStrmContent(const std::string& file_name);

    public:
        virtual std::string  GetType() const;
        virtual std::string  GetDisposition() const;
        virtual std::string  GetTransEncoding() const;
        virtual std::string& GetContent();

    private:
        std::string _file_name;
        std::string _name;
    };


// class Priority(Helper class)
    class Priority
    {
    public:
        static const std::string important;
        static const std::string normal;
        static const std::string trivial;
    };


// class MailInfo
    typedef std::multimap<std::string, std::string> Receivers;


    class MailInfo
    {
    public:
        MailInfo();

    public:
        void SetSenderName(const std::string& name);
        void SetSenderAddress(const std::string& address);
        std::string GetSenderName() const;
        std::string GetSenderAddress() const;

        void AddReceiver(const std::string& name, const std::string& address);
        void SetReceiver(const std::string& name, const std::string& address);
        const Receivers& GetReceivers() const;

        void SetPriority(std::string priority);
        std::string GetPriority() const;

        void SetSubject(const std::string& subject);
        std::string GetSubject() const;

        void AddMimeContent(MimeContent* content);
        void ClearMimeContents();
        const MimeContents& GetMimeContents() const;

    private:
        std::string  _sender_name;
        std::string  _sender_address;
        Receivers    _receivers;
        std::string  _priority;
        std::string  _subject;
        MimeContents _contents;
    };


// class MailWrapper
    class MailWrapper
    {
    public:
        MailWrapper(MailInfo* mail_info);

    public:
        std::string GetSenderAddress();
        std::string GetHeader();
        std::string GetEnd();

        void TraverseReceiver();
        bool HasMoreReceiver();
        std::string NextReceiverAddress();

        void TraverseContent();
        bool HasMoreContent();
        std::string& NextContent();

    private:
        std::string PrepareFrom();
        std::string PrepareTo();
        std::string PrepareDate();
        std::string PrepareName(const std::string& raw_name);

    private:
        static const std::string _mailer_name;
        static const std::string _boundary;

        MailInfo* _mail_info;
        Receivers::const_iterator _rcv_itr;

        std::string _content;
        MimeContents::const_iterator _con_itr;
    };


// class MailSender
    class MailSender
    {
        friend class ErrorMessage;

        enum {SERVICE_PORT = 25};
        enum Operaion
        {
            send_helo_cmd,
            send_auth_cmd,
            send_username,
            send_password,
            send_mail_cmd,
            send_rcpt_cmd,
            send_data_cmd,
            send_header,
            send_content,
            send_end,
            send_quit_cmd,
        };

    public:
        MailSender(const std::string& server_name,
                   int server_port = SERVICE_PORT,
                   const std::string& user_name = "",
                   const std::string& user_pwd = "");
        ~MailSender();

    public:
        void SetMail(MailWrapper* mail);
        void SendMail();
        static void SendMail(const std::string& ip, int port,
                             const std::string& receiver,
                             const std::string& sender = "",
                             const std::string& title = "",
                             const std::string& content = "",
                             const std::string& username = "",
                             const std::string& password = "");
        static void SendMail(const std::string& ip,
                             const std::string& receiver,
                             const std::string& sender = "",
                             const std::string& title = "",
                             const std::string& content = "",
                             const std::string& username = "",
                             const std::string& password = "");

    private:
        void Conn();
        void Hello();
        void Login();
        void SendHeader();
        void SendContent();
        void SendEnd();
        void Quit();
        void SendRequest(Operaion operation, const std::string& content);
        void RcvResponse(const std::string& expected_response);

    private:
        std::string _server_name;
        int _server_port;
        std::string _user_name;
        std::string _user_pwd;
        SOCKET _socket;

        MailWrapper* _mail;

#ifdef _WIN32
        static WSAInitial sWinsockInit;
#endif
    };


// class ErrorMessage(Helper class)
    class ErrorMessage
    {
    private:
        ErrorMessage();

    public:
        static ErrorMessage& GetInstance();

    public:
        std::string& Request(MailSender::Operaion request_operation);
        std::string& Response(const std::string& expected_response);

    private:
        std::map<MailSender::Operaion, std::string> _request_errmsg_map;
        std::map<std::string, std::string> _respons_errmsg_map;
    };


// class MailException(Helper class)
    class MailException : public std::exception
    {
    public:
        MailException(const std::string& s = "") : msg(s) {}
        virtual ~MailException() {};

    public:
        const char* what() const throw () { return msg.c_str(); }
        virtual const std::string What() const throw() { return msg; }

    private:
        std::string msg;
    };

} // namespace mail

#endif // MAIL_MAILSENDER_H
