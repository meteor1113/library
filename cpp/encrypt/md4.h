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


#ifndef MD4_H
#define MD4_H


// #include <standard library headers>
#include <string>

// #include <other library headers>

// #include "customer headers"


namespace encrypt
{


    /**
     * description
     */
    class MD4
    {
    public:
        MD4();
        ~MD4();

    private:
        MD4(const MD4& rhs);
        MD4& operator=(const MD4& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned int inputLen);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString() { return BytesToHexString(_digest, 16); }

    private:
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);

    private:
        unsigned long _state[4]; // state (ABCD)
        unsigned long _count[2]; // number of bits, modulo 2^64 (lsb first)
        unsigned char _buffer[64]; // input buffer
        unsigned char _digest[16]; // message digest

    };


    // helper function
    std::string MD4Data(const void* data, unsigned int length);
    std::string MD4String(const std::string& str);
    std::string MD4File(const std::string& file);


}

#endif
