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


// #include <standard library headers>
#include <fstream>

// #include <other library headers>

// #include "customer headers"
#include "md4.h"


#pragma warning(push)
#pragma warning(disable: 4996)


// Constants for MD4_Transform routine.
#define MD4_S11 3
#define MD4_S12 7
#define MD4_S13 11
#define MD4_S14 19
#define MD4_S21 3
#define MD4_S22 5
#define MD4_S23 9
#define MD4_S24 13
#define MD4_S31 3
#define MD4_S32 9
#define MD4_S33 11
#define MD4_S34 15

// MD4F, MD4G and MD4H are basic MD4 functions.
#define MD4F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD4G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4H(x, y, z) ((x) ^ (y) ^ (z))

// MD4_ROTL rotates x left n bits.
#define MD4_ROTL(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// MD4FF, MD4GG and MD4HH are transformations for rounds 1, 2 and 3
// Rotation is separate from addition to prevent recomputation
#define MD4FF(a, b, c, d, x, s) {               \
        (a) += MD4F ((b), (c), (d)) + (x);      \
        (a) = MD4_ROTL ((a), (s));              \
    }
#define MD4GG(a, b, c, d, x, s) {                                       \
        (a) += MD4G ((b), (c), (d)) + (x) + (unsigned long)0x5a827999;  \
        (a) = MD4_ROTL ((a), (s));                                      \
    }
#define MD4HH(a, b, c, d, x, s) {                                       \
        (a) += MD4H ((b), (c), (d)) + (x) + (unsigned long)0x6ed9eba1;  \
        (a) = MD4_ROTL ((a), (s));                                      \
    }


namespace encrypt
{

    static void MD4_Transform(unsigned long *state, const unsigned char *block);
    static void MD4_Encode(unsigned char *output, const unsigned long *input,
                           unsigned int len);
    static void MD4_Decode(unsigned long *output, const unsigned char *input,
                           unsigned int len);
    static unsigned char MD4_PADDING[64] =
    {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };


    /**
     * description
     */
    MD4::MD4()
    {
        Init();
    }


    /**
     * description
     */
    MD4::~MD4()
    {
    }


    // MD4 initialization. Begins an MD4 operation, writing a new context
    void MD4::Init()
    {
        _count[0] = _count[1] = 0;

        // Load magic initialization constants
        _state[0] = 0x67452301;
        _state[1] = 0xefcdab89;
        _state[2] = 0x98badcfe;
        _state[3] = 0x10325476;
    }


    // MD4 block update operation. Continues an MD4 message-digest
    //     operation, processing another message block, and updating the
    //     context
    void MD4::Update(const void* in, unsigned int inputLen)
    {
        const unsigned char* input = (const unsigned char*)in;
        unsigned int i = 0, index = 0, partLen = 0;

        // Compute number of bytes mod 64
        index = (unsigned int)((_count[0] >> 3) & 0x3F);

        // Update number of bits
        if ((_count[0] += ((unsigned long)inputLen << 3))
            < ((unsigned long)inputLen << 3))
            _count[1]++;
        _count[1] += ((unsigned long)inputLen >> 29);

        partLen = 64 - index;

        // Transform as many times as possible
        if (inputLen >= partLen)
        {
            memcpy((unsigned char*)&_buffer[index], (unsigned char*)input,
                   partLen);
            MD4_Transform(_state, _buffer);

            for (i = partLen; i + 63 < inputLen; i += 64)
                MD4_Transform (_state, &input[i]);

            index = 0;
        }
        else i = 0;

        // Buffer remaining input
        std::memcpy((unsigned char*)&_buffer[index],
                    (unsigned char*)&input[i], inputLen - i);
    }

    // MD4 finalization. Ends an MD4 message-digest operation, writing the
    //     the message digest and zeroizing the context.
    void MD4::Final()
    {
        unsigned char bits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned int index = 0, padLen = 0;

        // Save number of bits
        MD4_Encode (bits, _count, 8);

        // Pad out to 56 mod 64.
        index = (unsigned int)((_count[0] >> 3) & 0x3f);
        padLen = (index < 56) ? (56 - index) : (120 - index);
        Update(MD4_PADDING, padLen);

        // Append length (before padding)
        Update(bits, 8);

        // Store state in digest
        MD4_Encode(_digest, _state, 16);
    }


    /**
     * Convert unsigned char array to hex string.
     */
    std::string MD4::BytesToHexString(const unsigned char* input,
                                      unsigned int length)
    {
        std::string str;
        str.reserve(length << 1);
        char b[3];
        for (unsigned int i = 0; i < length; i++)
        {
            sprintf(b, "%02X", input[i]);
            str.append(1, b[0]);
            str.append(1, b[1]);
        }
        return str;
    }


    // MD4 basic transformation. Transforms state based on block.
    static void MD4_Transform(unsigned long *state, const unsigned char *block)
    {
        unsigned long a = state[0];
        unsigned long b = state[1];
        unsigned long c = state[2];
        unsigned long d = state[3];
        unsigned long x[16];

        MD4_Decode(x, block, 64);

        // Round 1
        MD4FF (a, b, c, d, x[ 0], MD4_S11); /* 1 */
        MD4FF (d, a, b, c, x[ 1], MD4_S12); /* 2 */
        MD4FF (c, d, a, b, x[ 2], MD4_S13); /* 3 */
        MD4FF (b, c, d, a, x[ 3], MD4_S14); /* 4 */
        MD4FF (a, b, c, d, x[ 4], MD4_S11); /* 5 */
        MD4FF (d, a, b, c, x[ 5], MD4_S12); /* 6 */
        MD4FF (c, d, a, b, x[ 6], MD4_S13); /* 7 */
        MD4FF (b, c, d, a, x[ 7], MD4_S14); /* 8 */
        MD4FF (a, b, c, d, x[ 8], MD4_S11); /* 9 */
        MD4FF (d, a, b, c, x[ 9], MD4_S12); /* 10 */
        MD4FF (c, d, a, b, x[10], MD4_S13); /* 11 */
        MD4FF (b, c, d, a, x[11], MD4_S14); /* 12 */
        MD4FF (a, b, c, d, x[12], MD4_S11); /* 13 */
        MD4FF (d, a, b, c, x[13], MD4_S12); /* 14 */
        MD4FF (c, d, a, b, x[14], MD4_S13); /* 15 */
        MD4FF (b, c, d, a, x[15], MD4_S14); /* 16 */

        // Round 2
        MD4GG (a, b, c, d, x[ 0], MD4_S21); /* 17 */
        MD4GG (d, a, b, c, x[ 4], MD4_S22); /* 18 */
        MD4GG (c, d, a, b, x[ 8], MD4_S23); /* 19 */
        MD4GG (b, c, d, a, x[12], MD4_S24); /* 20 */
        MD4GG (a, b, c, d, x[ 1], MD4_S21); /* 21 */
        MD4GG (d, a, b, c, x[ 5], MD4_S22); /* 22 */
        MD4GG (c, d, a, b, x[ 9], MD4_S23); /* 23 */
        MD4GG (b, c, d, a, x[13], MD4_S24); /* 24 */
        MD4GG (a, b, c, d, x[ 2], MD4_S21); /* 25 */
        MD4GG (d, a, b, c, x[ 6], MD4_S22); /* 26 */
        MD4GG (c, d, a, b, x[10], MD4_S23); /* 27 */
        MD4GG (b, c, d, a, x[14], MD4_S24); /* 28 */
        MD4GG (a, b, c, d, x[ 3], MD4_S21); /* 29 */
        MD4GG (d, a, b, c, x[ 7], MD4_S22); /* 30 */
        MD4GG (c, d, a, b, x[11], MD4_S23); /* 31 */
        MD4GG (b, c, d, a, x[15], MD4_S24); /* 32 */

        // Round 3
        MD4HH (a, b, c, d, x[ 0], MD4_S31); /* 33 */
        MD4HH (d, a, b, c, x[ 8], MD4_S32); /* 34 */
        MD4HH (c, d, a, b, x[ 4], MD4_S33); /* 35 */
        MD4HH (b, c, d, a, x[12], MD4_S34); /* 36 */
        MD4HH (a, b, c, d, x[ 2], MD4_S31); /* 37 */
        MD4HH (d, a, b, c, x[10], MD4_S32); /* 38 */
        MD4HH (c, d, a, b, x[ 6], MD4_S33); /* 39 */
        MD4HH (b, c, d, a, x[14], MD4_S34); /* 40 */
        MD4HH (a, b, c, d, x[ 1], MD4_S31); /* 41 */
        MD4HH (d, a, b, c, x[ 9], MD4_S32); /* 42 */
        MD4HH (c, d, a, b, x[ 5], MD4_S33); /* 43 */
        MD4HH (b, c, d, a, x[13], MD4_S34); /* 44 */
        MD4HH (a, b, c, d, x[ 3], MD4_S31); /* 45 */
        MD4HH (d, a, b, c, x[11], MD4_S32); /* 46 */
        MD4HH (c, d, a, b, x[ 7], MD4_S33); /* 47 */
        MD4HH (b, c, d, a, x[15], MD4_S34); /* 48 */

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;

        // Zeroize sensitive information.
        std::memset((unsigned char*)x, 0, sizeof(x));
    }


    // MD4_Encodes input (unsigned long) into output (unsigned char).
    // Assumes len is a multiple of 4.
    static void MD4_Encode(unsigned char *output, const unsigned long *input,
                           unsigned int len)
    {
        unsigned int i = 0, j = 0;

        for (i = 0, j = 0; j < len; i++, j += 4)
        {
            output[j] = (unsigned char)(input[i] & 0xff);
            output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
            output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
            output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
        }
    }


    // MD4_Decodes input (unsigned char) into output (unsigned long).
    // Assumes len is a multiple of 4.
    static void MD4_Decode(unsigned long *output, const unsigned char *input,
                           unsigned int len)
    {
        unsigned int i = 0, j = 0;

        for (i = 0, j = 0; j < len; i++, j += 4)
        {
            output[i] = ((unsigned long)input[j])
                |(((unsigned long)input[j+1]) << 8)
                |(((unsigned long)input[j+2]) << 16)
                |(((unsigned long)input[j+3]) << 24);
        }
    }


    std::string MD4Data(const void* data, unsigned int length)
    {
        MD4 obj;
        obj.Update(data, length);
        obj.Final();
        return obj.ToString();
    }


    std::string MD4String(const std::string& str)
    {
        MD4 obj;
        obj.Update((const unsigned char*)str.c_str(),
                   static_cast<unsigned int>(str.length()));
        obj.Final();
        return obj.ToString();
    }


    std::string MD4File(const std::string& file)
    {
        MD4 obj;

        std::ifstream fs(file.c_str(), std::ios::binary);
        const unsigned int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        while (!fs.eof())
        {
            fs.read(buffer, BUFFER_SIZE);
            std::streamsize length = fs.gcount();
            if (length > 0)
            {
                obj.Update((const unsigned char*)buffer, length);
            }
        }
        fs.close();

        obj.Final();
        return obj.ToString();
    }


}
