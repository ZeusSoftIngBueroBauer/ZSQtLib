/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDiagObjRect_h
#define ZSDiagObjRect_h

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagObj.h"

class QPolygon;

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjRect : public CDiagObj
//******************************************************************************
{
public: // ctors and dtor
    CDiagObjRect(
        const QString& i_strObjName,
        CDiagTrace*    i_pDiagTrace );
    virtual ~CDiagObjRect();
public: // instance methods
    void setPen( const QPen& i_pen );
    QPen getPen() const { return m_pen; }
    void setBrush( const QBrush& i_brush );
    QBrush getBrush() const { return m_brush; }
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
private: // copy ctor not allowed
    CDiagObjRect( const CDiagObjRect& );
private: // assignment operator not allowed
    void operator=( const CDiagObjRect& );
protected:  // instance members
    QPen   m_pen;
    QBrush m_brush;
    QRect  m_rct;
    bool   m_bUpdWidget;

}; // class CDiagObjRect

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjRect_h
