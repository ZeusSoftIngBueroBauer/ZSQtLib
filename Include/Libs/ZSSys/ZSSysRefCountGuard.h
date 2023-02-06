/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifndef ZSSys_RefCountGuard_h
#define ZSSys_RefCountGuard_h

#include "ZSSys/ZSSysDllMain.h"

namespace ZS
{
namespace System
{
//******************************************************************************
class ZSSYSDLL_API CRefCountGuard
//******************************************************************************
{
public: // ctors and dtor
    CRefCountGuard( int* i_piRefCount );
    ~CRefCountGuard();
public: // instance methods
    void decrementAndReleaseCounter();
private: // instance members
    int* m_piRefCount;

}; // class CRefCountGuard

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_RefCountGuard_h
