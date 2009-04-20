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


#ifndef CRC16_H
#define CRC16_H


// #include <standard library headers>
#include <string>

// #include <other library headers>

// #include "customer headers"


namespace encrypt
{


    /**
     * description
     */
    class CRC16
    {
    public:
        CRC16();
        virtual ~CRC16();

    private:
        CRC16(const CRC16& rhs);
        CRC16& operator=(const CRC16& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned int uBufSize);
        void Final();
        unsigned short GetValue() const { return _value; }
        std::string ToString() const;

    protected:

    private:
        unsigned short _value;

    };


    // helper function
    unsigned short CRC16Data(const void* data, unsigned int length);
    unsigned short CRC16String(const std::string& str);
    unsigned short CRC16File(const std::string& file);

}

#endif
