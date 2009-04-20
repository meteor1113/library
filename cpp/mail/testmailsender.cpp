#include <list>
#include <string>
#include <iostream>
#include "mailsender.h"


using namespace mail;

void main()
{
    //WinSockHelper wshelper;

    // make sure the file 'boot.ini' is in the root directory on driver C
    TextPlainContent content1("plain text contentÇãÇ¿Ç†ÇÿÇÍ", "Shift-JIS");
    TextHtmlContent content2("<a href='http://morningspace.51.net'>click meÇæÇ¢ÇêÇÜÇÑÇã</a>", "Shift-JIS");
    AppOctStrmContent content3("c:\\boot.ini");

    MailInfo info;
    info.SetSenderName("morning");
    info.SetSenderAddress("moyingzz@etang.com");
    info.AddReceiver("friend1", "lxin@informedia.net.cn");
    info.AddReceiver("friend2", "yanhw@informedia.net.cn");
//    info.SetPriority(mail::Priority::trivial);
    info.SetSubject("a test mail");
    info.AddMimeContent(&content1);
    info.AddMimeContent(&content2);
    info.AddMimeContent(&content3);

    try
    {
        MailSender sender("192.168.1.19", "", "");
        sender.SetMail(&mail::MailWrapper(&info));
        sender.SendMail();

        MailSender::SendMail("192.168.1.19", "lxin@informedia.net.cn",
                             "sender", "title", "content", "", "");
    }
    catch (MailException& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unkown error" << std::endl;
    }
}
