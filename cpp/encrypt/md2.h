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


#ifndef MD2_H
#define MD2_H


// #include <standard library headers>
#include <string>

// #include <other library headers>

// #include "customer headers"


namespace encrypt
{


    /**
     * description
     */
    class MD2
    {
    public:
        MD2();
        ~MD2();

    private:
        MD2(const MD2& rhs);
        MD2& operator=(const MD2& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned int len);
        void Final();
        const unsigned char* GetDigest() { return _digest; }
        std::string ToString() { return BytesToHexString(_digest, 16); }

    private:
        std::string BytesToHexString(const unsigned char *input,
                                     unsigned int length);

    private:
        unsigned char m_X[48], m_C[16], m_buf[16];
        unsigned int m_count;
        unsigned char _digest[16];      // message digest

    };


    // helper function
    std::string MD2Data(const void* data, unsigned int length);
    std::string MD2String(const std::string& str);
    std::string MD2File(const std::string& file);

}

#endif
