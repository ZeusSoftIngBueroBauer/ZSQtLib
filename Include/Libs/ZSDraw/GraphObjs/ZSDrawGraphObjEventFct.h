/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_GraphObjEventFct_h
#define ZSDraw_GraphObjEventFct_h

#include "ZSDraw/Common/ZSDrawDllMain.h"

class QGraphicsSceneMouseEvent;
class QKeyEvent;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Draw
{
class CGraphObj;

/*==============================================================================
Signatures for simulation event methods
==============================================================================*/

typedef void (*TFctMouseEvent)( void* i_pvThis, void* i_pvData, CGraphObj* i_pGraphObj, QGraphicsSceneMouseEvent* i_pEv );
typedef void (*TFctKeyEvent)( void* i_pvThis, void* i_pvData, CGraphObj* i_pGraphObj, QKeyEvent* i_pEv );


//==============================================================================
struct ZSDRAWDLL_API SGraphObjEventFct
//==============================================================================
{
protected: // ctors and dtor
    SGraphObjEventFct() :
        m_pvThis(nullptr),
        m_pvData(nullptr)
    {
    }
    SGraphObjEventFct( void* i_pvThis, void* i_pvData ) :
        m_pvThis(i_pvThis),
        m_pvData(i_pvData)
    {
    }
    ~SGraphObjEventFct()
    {
        m_pvThis = nullptr;
        m_pvData = nullptr;
    }
public: // struct members
    void* m_pvThis;
    void* m_pvData;

}; // struct SGraphObjEventFct


//==============================================================================
struct ZSDRAWDLL_API SGraphObjMouseEventFct : public SGraphObjEventFct
//==============================================================================
{
public: // ctors and dtor
    SGraphObjMouseEventFct() :
        SGraphObjEventFct(),
        m_pFct(nullptr)
    {
    }
    SGraphObjMouseEventFct( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData ) :
        SGraphObjEventFct(i_pvThis,i_pvData),
        m_pFct(i_pFct)
    {
    }
    ~SGraphObjMouseEventFct()
    {
        m_pFct = nullptr;
    }
public: // struct members
    TFctMouseEvent m_pFct;

}; // struct SGraphObjMouseEventFct


//==============================================================================
struct ZSDRAWDLL_API SGraphObjKeyEventFct : public SGraphObjEventFct
//==============================================================================
{
public: // ctors and dtor
    SGraphObjKeyEventFct() :
        SGraphObjEventFct(),
        m_pFct(nullptr)
    {
    }
    SGraphObjKeyEventFct( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData ) :
        SGraphObjEventFct(i_pvThis,i_pvData),
        m_pFct(i_pFct)
    {
    }
    ~SGraphObjKeyEventFct()
    {
        m_pFct = nullptr;
    }
public: // struct members
    TFctKeyEvent m_pFct;

}; // struct SGraphObjKeyEventFct


} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjEventFct_h
