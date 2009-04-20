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


#ifndef SHA1_H
#define SHA1_H


// #include <standard library headers>
#include <limits.h>
#include <string>

// #include <other library headers>

// #include "customer headers"


/* define an unsigned 32-bit type */
#if UINT_MAX == 0xffffffff
typedef   unsigned int     sha1_32t;
#elif ULONG_MAX == 0xffffffff
typedef   unsigned long    sha1_32t;
#else
#error Please define sha1_32t as an unsigned 32 bit type in sha2.h
#endif

#define SHA1_BLOCK_SIZE  64
#define SHA1_DIGEST_SIZE 20


namespace encrypt
{

    /**
     * description
     */
    class SHA1
    {
        struct sha1_ctx
        {
            sha1_32t count[2];
            sha1_32t hash[5];
            sha1_32t wbuf[16];
        };

    public:
        SHA1();
        virtual ~SHA1();

    private:
        SHA1(const SHA1& rhs);
        SHA1& operator=(const SHA1& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned int len);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString() { return BytesToHexString(_digest, 20); }

    private:
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);
        void Compile();

    private:
        sha1_ctx _ctx;
        unsigned char _digest[SHA1_DIGEST_SIZE];

    };


    // helper function
    std::string SHA1Data(const void* data, unsigned int length);
    std::string SHA1String(const std::string& str);
    std::string SHA1File(const std::string& file);

}

#endif
