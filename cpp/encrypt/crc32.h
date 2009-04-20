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


#ifndef CRC32_H
#define CRC32_H


// #include <standard library headers>
#include <string>

// #include <other library headers>

// #include "customer headers"


namespace encrypt
{


    /**
     * description
     */
    class CRC32
    {
    public:
        CRC32();
        virtual ~CRC32();

    private:
        CRC32(const CRC32& rhs);
        CRC32& operator=(const CRC32& rhs);

    public:
        void Init();
        void Update(const void* in, unsigned int uBufSize);
        void Final();
        unsigned long GetValue() const { return _value; }
        std::string ToString() const;

    protected:

    private:
        unsigned long _value;

    };


    // helper function
    unsigned long CRC32Data(const void* data, unsigned int length);
    unsigned long CRC32String(const std::string& str);
    unsigned long CRC32File(const std::string& file);

}

#endif
