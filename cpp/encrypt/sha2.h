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


#ifndef SHA2_H
#define SHA2_H


// #include <standard library headers>
#include <limits.h>
#include <string>

// #include <other library headers>

// #include "customer headers"


/*  Defines for suffixes to 32 and 64 bit unsigned numeric values   */
#define sfx_lo(x,y) x##y
#define sfx_hi(x,y) sfx_lo(x,y)
#define n_u32(p)    sfx_hi(0x##p,s_u32)
#define n_u64(p)    sfx_hi(0x##p,s_u64)

/* define an unsigned 32-bit type */
#if UINT_MAX == 0xffffffff
typedef   unsigned int     sha2_32t;
#define s_u32    u
#elif ULONG_MAX == 0xffffffff
typedef   unsigned long    sha2_32t;
#define s_u32   ul
#else
#error Please define sha2_32t as an unsigned 32 bit type in sha2.h
#endif

/* define an unsigned 64-bit type */
#if defined( _MSC_VER )
typedef unsigned __int64   sha2_64t;
#define s_u64 ui64
#elif ULONG_MAX == 0xffffffffffffffff
typedef unsigned long      sha2_64t;
#define s_u64   ul
#elif ULONG_MAX == 0xffffffff
typedef unsigned long long sha2_64t;   /* a somewhat dangerous guess */
#define s_u64  ull
#else
#error Please define sha2_64t as an unsigned 64 bit type in sha2.h
#endif

#define SHA256_DIGEST_SIZE  32
#define SHA384_DIGEST_SIZE  48
#define SHA512_DIGEST_SIZE  64

#define SHA256_BLOCK_SIZE   64
#define SHA384_BLOCK_SIZE  128
#define SHA512_BLOCK_SIZE  128

//#define SHA2_DIGEST_SIZE        SHA256_DIGEST_SIZE
//#define SHA2_MAX_DIGEST_SIZE    SHA512_DIGEST_SIZE


namespace encrypt
{

    /* type to hold the SHA256 context                          */
    struct sha256_ctx
    {
        sha2_32t count[2];
        sha2_32t hash[8];
        sha2_32t wbuf[16];
    };

    /* type to hold the SHA384/512 context                      */
    struct sha512_ctx
    {
        sha2_64t count[2];
        sha2_64t hash[8];
        sha2_64t wbuf[16];
    };
    typedef sha512_ctx  sha384_ctx;


    class SHA256
    {
    public:
        SHA256();
        virtual ~SHA256();

    private:
        SHA256(const SHA256& rhs);
        SHA256& operator=(const SHA256& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned long len);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString()
            { return BytesToHexString(_digest, SHA256_DIGEST_SIZE); }

    private:
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);
        void Compile();

    private:
        sha256_ctx _ctx;
        unsigned char _digest[SHA256_DIGEST_SIZE];

    };


    class SHA384
    {
    public:
        SHA384();
        virtual ~SHA384();

    private:
        SHA384(const SHA384& rhs);
        SHA384& operator=(const SHA384& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned long len);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString()
            { return BytesToHexString(_digest, SHA384_DIGEST_SIZE); }

    private:
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);
        void Compile();

    private:
        sha384_ctx _ctx;
        unsigned char _digest[SHA384_DIGEST_SIZE];

    };


    class SHA512
    {
    public:
        SHA512();
        virtual ~SHA512();

    private:
        SHA512(const SHA512& rhs);
        SHA512& operator=(const SHA512& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned long len);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString()
            { return BytesToHexString(_digest, SHA512_DIGEST_SIZE); }

    private:
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);
        void Compile();

    private:
        sha512_ctx _ctx;
        unsigned char _digest[SHA512_DIGEST_SIZE];

    };


    // helper function
    std::string SHA256Data(const void* data, unsigned int length);
    std::string SHA256String(const std::string& str);
    std::string SHA256File(const std::string& file);

    std::string SHA384Data(const void* data, unsigned int length);
    std::string SHA384String(const std::string& str);
    std::string SHA384File(const std::string& file);

    std::string SHA512Data(const void* data, unsigned int length);
    std::string SHA512String(const std::string& str);
    std::string SHA512File(const std::string& file);

}

#endif
