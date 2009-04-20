// #include <standard library headers>
#include <iostream>
#include <time.h>
#include <stdlib.h>

// #include <other library headers>

// #include "customer headers"
#include "mailsender.h"
#include "base64helper.h"
#include "filehelper.h"


#pragma warning (push)
#pragma warning (disable :4996)


namespace mail
{


#ifdef _WIN32
    WSAInitial MailSender::sWinsockInit;
#endif

// constants defination
/////////////////////////////////////

    enum {MAX_BUFFER_SIZE = 255};

    const std::string Priority::important = "1";
    const std::string Priority::normal    = "3";
    const std::string Priority::trivial   = "5";

    const std::string MailWrapper::_mailer_name = "MailSender";
    const std::string MailWrapper::_boundary    = "#BOUNDARY#";


// Member functions of class MimeContent
/////////////////////////////////////

    MimeContent::MimeContent(const std::string& content) : _content(content)
    {
    }


    std::string MimeContent::GetDisposition() const
    {
        return "";
    }


// Member functions of class PlainTextContent
/////////////////////////////////////

    TextPlainContent::TextPlainContent(const std::string& content,
                                       const std::string& charset)
        :MimeContent(content), _charset(charset)
    {
    }


    std::string TextPlainContent::GetType() const
    {
        return "text/plain; charset=" + _charset;
    }


    std::string TextPlainContent::GetTransEncoding() const
    {
        return "8bit";
    }


    std::string& TextPlainContent::GetContent()
    {
        // you can add more codes here, such as wrapping lines
        // or replacing '\n' with '\r\n', etc.
        return _content;
    }


// Member functions of class TextHtmlContent
/////////////////////////////////////

    TextHtmlContent::TextHtmlContent(const std::string& content,
                                     const std::string& charset)
        :MimeContent(content), _charset(charset)
    {
    }

    std::string TextHtmlContent::GetType() const
    {
        return "text/html; charset=" + _charset;
    }

    std::string TextHtmlContent::GetTransEncoding() const
    {
        return "8bit";
    }

    std::string& TextHtmlContent::GetContent()
    {
        // you can add more codes here, such as wrapping lines
        // or replacing '\n' with '\r\n', etc.
        return _content;
    }

// Member functions of class AppOctStrmContent
/////////////////////////////////////

    AppOctStrmContent::AppOctStrmContent(const std::string& file_name)
        :_file_name(file_name)
    {
        char drive[_MAX_DRIVE];
        char direc[_MAX_DIR];
        char fname[_MAX_FNAME];
        char ext[_MAX_EXT];
        _splitpath(file_name.c_str(), drive, direc, fname, ext);
        _name  = fname;
        _name += ext;
    }

    std::string AppOctStrmContent::GetType() const
    {
        return "application/octet-stream; name=" + _name;
    }

    std::string AppOctStrmContent::GetDisposition() const
    {
        return "attachment; filename=" + _name;
    }

    std::string AppOctStrmContent::GetTransEncoding() const
    {
        return "base64";
    }

    std::string& AppOctStrmContent::GetContent()
    {
        // you can add more codes here, such as wrapping lines
        // or replacing '\n' with '\r\n', etc.
        FileHelper::Open(_file_name, _content);
        _content = Base64Helper::Encode(_content);
        return _content;
    }

// Member functions of class MailInfo
/////////////////////////////////////

    MailInfo::MailInfo() : _priority(Priority::normal)
    {
    }

    void MailInfo::SetSenderName(const std::string& name)
    {
        _sender_name = name;
    }

    void MailInfo::SetSenderAddress(const std::string& address)
    {
        _sender_address = address;
    }

    std::string MailInfo::GetSenderName() const
    {
        return _sender_name;
    }

    std::string MailInfo::GetSenderAddress() const
    {
        return _sender_address;
    }

    void MailInfo::AddReceiver(const std::string& name,
                               const std::string& address)
    {
        _receivers.insert(Receivers::value_type(name, address));
    }

    void MailInfo::SetReceiver(const std::string& name,
                               const std::string& address)
    {
        _receivers.clear();
        _receivers.insert(Receivers::value_type(name, address));
    }

    const Receivers& MailInfo::GetReceivers() const
    {
        return _receivers;
    }

    void MailInfo::SetPriority(std::string priority)
    {
        _priority = priority;
    }

    std::string MailInfo::GetPriority() const
    {
        return _priority;
    }

    void MailInfo::SetSubject(const std::string& subject)
    {
        _subject = subject;
    }

    std::string MailInfo::GetSubject() const
    {
        return _subject;
    }

    void MailInfo::AddMimeContent(MimeContent* content)
    {
        _contents.push_back(content);
    }

    void MailInfo::ClearMimeContents()
    {
        _contents.clear();
    }

    const MimeContents& MailInfo::GetMimeContents() const
    {
        return _contents;
    }

// Member functions of class MailWrapper
/////////////////////////////////////

    MailWrapper::MailWrapper(MailInfo* mail_info) : _mail_info(mail_info)
    {
    }

    std::string MailWrapper::GetSenderAddress()
    {
        std::string address;

        address  = "<";
        address += _mail_info->GetSenderAddress();
        address += ">";

        return address;
    }

    std::string MailWrapper::GetHeader()
    {
        std::string header;

        header  = "From: ";
        header += PrepareFrom() + "\r\n";

        header += "To: ";
        header += PrepareTo() + "\r\n";

        header += "Date: ";
        header += PrepareDate() + "\r\n";

        header += "Subject: ";
        header += _mail_info->GetSubject() + "\r\n";

        header += "X-Mailer: ";
        header += _mailer_name + "\r\n";

        header += "X-Priority: ";
        header += _mail_info->GetPriority() + "\r\n";

        header += "MIME-Version: 1.0\r\n";
        header += "Content-type: multipart/mixed; boundary=\"";
        header += _boundary + "\"\r\n";

        header += "\r\n";

        return header;
    }

    void MailWrapper::TraverseReceiver()
    {
        _rcv_itr = _mail_info->GetReceivers().begin();
    }

    bool MailWrapper::HasMoreReceiver()
    {
        return ( _rcv_itr != _mail_info->GetReceivers().end() );
    }

    std::string MailWrapper::NextReceiverAddress()
    {
        std::string address;

        address  = "<";
        address += (_rcv_itr++)->second;
        address += ">";

        return address;
    }

    void MailWrapper::TraverseContent()
    {
        _con_itr = _mail_info->GetMimeContents().begin();
    }

    bool MailWrapper::HasMoreContent()
    {
        return ( _con_itr != _mail_info->GetMimeContents().end() );
    }

    std::string& MailWrapper::NextContent()
    {
        _content  = "--" + _boundary + "\r\n";

        _content += "Content-Type: ";
        _content += (*_con_itr)->GetType() + "\r\n";

        std::string disposition = (*_con_itr)->GetDisposition();
        if ( !disposition.empty() )
        {
            _content += "Content-Disposition: ";;
            _content += disposition + "\r\n";
        }

        _content += "Content-Transfer-Encoding: ";
        _content += (*_con_itr)->GetTransEncoding() + "\r\n\r\n";

        _content += (*_con_itr)->GetContent() + "\r\n\r\n";

        _con_itr++;

        return _content;
    }

    std::string MailWrapper::GetEnd()
    {
        std::string end;

        end += "\r\n--" + _boundary + "--\r\n";

        end += ".\r\n";

        return end;
    }

    std::string MailWrapper::PrepareFrom()
    {
        std::string from_string;

        from_string = PrepareName(_mail_info->GetSenderName());
        from_string += GetSenderAddress();

        return from_string;
    }

    std::string MailWrapper::PrepareTo()
    {
        std::string to_string;

        TraverseReceiver();
        while (HasMoreReceiver())
        {
            to_string += PrepareName(_rcv_itr->first);
            to_string += NextReceiverAddress() + ", ";
        }

        return to_string.substr(0, to_string.length()-2);
    }

    std::string MailWrapper::PrepareDate()
    {
        char date_string[MAX_BUFFER_SIZE];

        time_t seconds;
        time(&seconds);
        strftime(date_string, MAX_BUFFER_SIZE,
                 "%a, %d %b %y %H:%M:%S +0800",
                 localtime(&seconds));          // +0800 maybe hard code

        return date_string;
    }

    std::string MailWrapper::PrepareName(const std::string& raw_name)
    {
        std::string decorated_name;

        if (!raw_name.empty())
        {
            decorated_name  = "\"";
            decorated_name += raw_name;
            decorated_name += "\" ";
        }

        return decorated_name;
    }

// Member functions of class MailSender
/////////////////////////////////////

    MailSender::MailSender(const std::string& server_name,
                           int server_port,
                           const std::string& user_name,
                           const std::string& user_pwd)
        :_server_name(server_name), _server_port(server_port),
         _user_name(user_name), _user_pwd(user_pwd), _mail(0)
    {
        Conn();

        if (!user_name.empty())
        {
            Login();
        }

        Hello();
    }

    MailSender::~MailSender()
    {
        Quit();
    }

    void MailSender::SetMail(MailWrapper* mail)
    {
        _mail = mail;
    }

    void MailSender::SendMail()
    {
        if (!_mail)
        {
            return;
        }

        SendHeader();
        SendContent();
        SendEnd();
    }

    void MailSender::SendMail(const std::string& ip, int port,
                              const std::string& receiver,
                              const std::string& sender,
                              const std::string& title,
                              const std::string& content,
                              const std::string& username,
                              const std::string& password)
    {
        TextPlainContent textContent(content);
        MailInfo info;
        info.SetSenderName(sender);
        info.SetSenderAddress(sender);
        info.AddReceiver(receiver, receiver);
        info.SetSubject(title);
        info.AddMimeContent(&textContent);

        MailSender mailSender(ip, port, username, password);
        MailWrapper mailWrapper(&info);
        mailSender.SetMail(&mailWrapper);
        mailSender.SendMail();
    }


    void MailSender::SendMail(const std::string& ip,
                              const std::string& receiver,
                              const std::string& sender,
                              const std::string& title,
                              const std::string& content,
                              const std::string& username,
                              const std::string& password)
    {
        SendMail(ip, SERVICE_PORT, receiver, sender, title,
                 content, username, password);
    }


    void MailSender::Conn()
    {
        struct hostent* host = gethostbyname(_server_name.c_str());
        if (host == 0)
        {
            throw MailException("Get server infomation error");
        }

        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = *(ULONG *)host->h_addr_list[0];
        server_addr.sin_port = htons(_server_port);

        connect(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
        RcvResponse("220");
    }

    void MailSender::Hello()
    {
        char local_host[MAX_BUFFER_SIZE];
        if (gethostname(local_host, MAX_BUFFER_SIZE) != 0)
        {
            throw MailException("Get local host name error");
        }

        std::string msg;

        msg  = "HELO ";
        msg += std::string(local_host) + "\r\n";
        SendRequest(send_helo_cmd, msg);
        RcvResponse("250");
    }

    void MailSender::Login()
    {
        std::string msg;

        msg = "AUTH LOGIN\r\n";
        SendRequest(send_auth_cmd, msg);
        RcvResponse("334");

        msg = Base64Helper::Encode(_user_name) + "\r\n";
        SendRequest(send_username, msg);
        RcvResponse("334");

        msg = Base64Helper::Encode(_user_pwd) + "\r\n";
        SendRequest(send_password, msg);
        RcvResponse("235");
    }

    void MailSender::SendHeader()
    {
        std::string msg;

        msg  = "MAIL FROM: ";
        msg += _mail->GetSenderAddress() + "\r\n";
        SendRequest(send_mail_cmd, msg);
        RcvResponse("250");

        _mail->TraverseReceiver();
        while (_mail->HasMoreReceiver())
        {
            msg  = "RCPT TO: ";
            msg += _mail->NextReceiverAddress() + "\r\n";
            SendRequest(send_rcpt_cmd, msg);
            RcvResponse("250");
        }

        msg  = "DATA\r\n";
        SendRequest(send_data_cmd, msg);
        RcvResponse("354");

        SendRequest(send_header, _mail->GetHeader());
    }

    void MailSender::SendContent()
    {
        _mail->TraverseContent();
        while (_mail->HasMoreContent())
        {
            SendRequest(send_content, _mail->NextContent());
        }
    }

    void MailSender::SendEnd()
    {
        SendRequest(send_end, _mail->GetEnd());
        RcvResponse("250");
    }

    void MailSender::Quit()
    {
        SendRequest(send_quit_cmd, "QUIT\r\n");
        RcvResponse("221");

        ::closesocket(_socket);
    }

    void MailSender::RcvResponse(const std::string& expected_response)
    {
        char response_buffer[MAX_BUFFER_SIZE];
        int recv_bytes = recv(_socket, response_buffer, MAX_BUFFER_SIZE, 0);
        if (recv_bytes < 0)
        {
            throw MailException(
                ErrorMessage::GetInstance().Response(expected_response));
        }

        std::string response(response_buffer, recv_bytes);
        std::cout << "[INFO]RECV(" << expected_response << "):"
                  << response << std::endl;
        if (response.substr(0, 3) != expected_response)
        {
            throw MailException(
                ErrorMessage::GetInstance().Response(expected_response));
        }
    }

    void MailSender::SendRequest(Operaion operation,
                                 const std::string& content)
    {
        std::cout << "[INFO]SEND:" << content << std::endl;

        if (send(_socket, content.c_str(),
                 static_cast<int>(content.length()), 0) < 0)
        {
            throw MailException(ErrorMessage::GetInstance().Request(operation));
        }
    }

// Member functions of class ErrorMessage
/////////////////////////////////////

    ErrorMessage& ErrorMessage::GetInstance()
    {
        static ErrorMessage _instance;
        return _instance;
    }

    ErrorMessage::ErrorMessage()
    {
        _request_errmsg_map[MailSender::send_helo_cmd] = "Send HELO cmd error";
        _request_errmsg_map[MailSender::send_auth_cmd] = "Send AUTH cmd error";
        _request_errmsg_map[MailSender::send_username] = "Send user name error";
        _request_errmsg_map[MailSender::send_password] = "Send user password error";
        _request_errmsg_map[MailSender::send_mail_cmd] = "Send MAIL FROM cmd error";
        _request_errmsg_map[MailSender::send_rcpt_cmd] = "Send RCPT TO cmd error";
        _request_errmsg_map[MailSender::send_data_cmd] = "Send DATA cmd error";
        _request_errmsg_map[MailSender::send_header  ] = "Send mail header error";
        _request_errmsg_map[MailSender::send_content ] = "Send mail content error";
        _request_errmsg_map[MailSender::send_end     ] = "Send mail end error";
        _request_errmsg_map[MailSender::send_quit_cmd] = "Send QUIT cmd error";

        _respons_errmsg_map["220"] = "Server connect error";
        _respons_errmsg_map["250"] = "General server error";
        _respons_errmsg_map["334"] = "Server authentication error";
        _respons_errmsg_map["235"] = "Password error";
        _respons_errmsg_map["354"] = "Server not ready for data";
        _respons_errmsg_map["221"] = "Server didn't terminate session";
    }

    std::string& ErrorMessage::Request(MailSender::Operaion request_operation)
    {
        return _request_errmsg_map[request_operation];
    }

    std::string& ErrorMessage::Response(const std::string& expected_response)
    {
        return _respons_errmsg_map[expected_response];
    }

} // namespace mail

#pragma warning (pop)
