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


#ifndef LOKI_SCOPEGUARDEXTEND_H
#define LOKI_SCOPEGUARDEXTEND_H


#include <loki/scopeguard.h>


namespace Loki
{
    template <typename Ret>
    inline ScopeGuardImpl0<Ret(*)()> MakeGuard(Ret(*pf)())
    {
        return ScopeGuardImpl0<Ret(*)()>::MakeGuard(pf);
    }

    template <typename Ret, typename P1a, typename P1>
    inline ScopeGuardImpl1<Ret(*)(P1a), P1> MakeGuard(Ret(*pf)(P1a), P1 p1)
    {
        return ScopeGuardImpl1<Ret(*)(P1a), P1>::MakeGuard(pf, p1);
    }


    template <typename Ret, class Obj1>
    inline ObjScopeGuardImpl0<Obj1,Ret(Obj1::*)()>
    MakeGuard(Ret(Obj1::*memFun)(), Obj1 &obj)
    {
        return ObjScopeGuardImpl0<Obj1,Ret(Obj1::*)()>::MakeObjGuard(
            obj,memFun);
    }

} // namespace Loki


#endif
