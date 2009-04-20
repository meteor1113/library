/* -*- mode: C++; -*- */
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


#ifndef MD5_H
#define MD5_H


// #include <standard library headers>
#include <string>

// #include <other library headers>

// #include "customer headers"


namespace encrypt
{

    /**
     * MD5 declaration.
     */
    class MD5
    {
    public:
        MD5();
        ~MD5();

    private:
        MD5(const MD5& rhs);
        MD5& operator=(const MD5& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned int length);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString() { return BytesToHexString(_digest, 16); }
        std::string ToString16() { return BytesToHexString(&_digest[4], 8); }

    private:
        void Transform(const unsigned char block[64]);
        void Encode(const unsigned int *input, unsigned char *output,
                    unsigned int length);
        void Decode(const unsigned char *input, unsigned int *output,
                    unsigned int length);
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);

    private:
        unsigned int _state[4];  // state (ABCD)
        unsigned int _count[2];  // number of bits,
                                 // modulo 2^64 (low-order word first)
        unsigned char _buffer[64]; // input buffer
        unsigned char _digest[16]; // message digest

    };


    // helper function
    std::string MD5Data(const void* data, unsigned int length);
    std::string MD5String(const std::string& str);
    std::string MD5File(const std::string& file);

    std::string MD5Data16(const void* data, unsigned int length);
    std::string MD5String16(const std::string& str);
    std::string MD5File16(const std::string& file);

}

#endif // MD5_H
