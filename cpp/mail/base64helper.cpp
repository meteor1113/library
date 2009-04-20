// #include <standard library headers>

// #include <other library headers>

// #include "customer headers"
#include "base64helper.h"


namespace mail
{

    const std::string Base64Helper::_base64_encode_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const char Base64Helper::_base64_decode_chars[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
    };

    std::string Base64Helper::Encode(const std::string& in_str)
    {
        std::string out_str;
        unsigned char c1, c2, c3;
        int i = 0;
        int len = static_cast<int>(in_str.length());

        while (i < len)
        {
            // read the first byte
            c1 = in_str[i++];
            if (i == len)       // pad with "="
            {
                out_str += _base64_encode_chars[c1 >> 2];
                out_str += _base64_encode_chars[(c1 & 0x3) << 4];
                out_str += "==";
                break;
            }

            // read the second byte
            c2 = in_str[i++];
            if (i == len)       // pad with "="
            {
                out_str += _base64_encode_chars[c1 >> 2];
                out_str += _base64_encode_chars[((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4)];
                out_str += _base64_encode_chars[(c2 & 0xF) << 2];
                out_str += "=";
                break;
            }

            // read the third byte
            c3 = in_str[i++];
            // convert into four bytes string
            out_str += _base64_encode_chars[c1 >> 2];
            out_str += _base64_encode_chars[((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4)];
            out_str += _base64_encode_chars[((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6)];
            out_str += _base64_encode_chars[c3 & 0x3F];
        }

        return out_str;
    }

    std::string Base64Helper::Decode(const std::string& in_str)
    {
        std::string out_str;
        char c1, c2, c3, c4;
        int i = 0;
        int len = static_cast<int>(in_str.length());

        while (i < len)
        {
            // read the first byte
            do
            {
                c1 = _base64_decode_chars[in_str[i++]];
            }
            while (i < len && c1 == -1);

            if (c1 == -1)
            {
                break;
            }

            // read the second byte
            do
            {
                c2 = _base64_decode_chars[in_str[i++]];
            }
            while (i < len && c2 == -1);

            if (c2 == -1)
            {
                break;
            }

            // assamble the first byte
            out_str += char((c1 << 2) | ((c2 & 0x30) >> 4));

            // read the third byte
            do
            {
                c3 = in_str[i++];
                if (c3 == 61)       // meet with "=", break
                {
                    return out_str;
                }
                c3 = _base64_decode_chars[c3];
            }
            while (i < len && c3 == -1);

            if (c3 == -1)
            {
                break;
            }

            // assamble the second byte
            out_str += char(((c2 & 0XF) << 4) | ((c3 & 0x3C) >> 2));

            // read the fourth byte
            do
            {
                c4 = in_str[i++];
                if (c4 == 61)       // meet with "=", break
                {
                    return out_str;
                }
                c4 = _base64_decode_chars[c4];
            }
            while (i < len && c4 == -1);

            if (c4 == -1)
            {
                break;
            }

            // assamble the third byte
            out_str += char(((c3 & 0x03) << 6) | c4);
        }

        return out_str;
    }

} // namespace mail
