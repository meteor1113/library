/* -*- mode: C++; -*- */

#ifndef MAIL_BASE64HELPER_H
#define MAIL_BASE64HELPER_H


// #include <standard library headers>
#include <string>

// #include <other library headers>

// #include "customer headers"


namespace mail
{

    class Base64Helper
    {
    public:
        // convert from Base64 to ANSI
        static std::string Encode(const std::string& in_str);
        // convert from ANSI to Base64
        static std::string Decode(const std::string& in_str);

    private:
        // encode table
        const static std::string _base64_encode_chars;
        // decode table
        const static char _base64_decode_chars[128];
    };

} // namespace mail

#endif  // MAIL_BASE64HELPER_H
