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

#ifndef ZSDiagObjCurve_h
#define ZSDiagObjCurve_h

#include "ZSDiagram/ZSDiagObj.h"

#include <QtGui/qcolor.h>

class QPolygon;

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjCurve : public CDiagObj
//******************************************************************************
{
public: // ctors and dtor
    CDiagObjCurve(
        const QString& i_strObjName,
        CDiagTrace*    i_pDiagTrace );
    virtual ~CDiagObjCurve();
public: // instance methods
    void setCol( const QColor&  i_col );
    QColor getCol() const;
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
private: // copy ctor not allowed
    CDiagObjCurve( const CDiagObjCurve& );
private: // assignment operator not allowed
    void operator=( const CDiagObjCurve& );
protected:  // instance members
    QColor m_col;
    #if QT_VERSION >= 0x040100
    QPolygon* m_pPtArr;
    #else
    QPointArray* m_pPtArr;
    #endif
    bool m_bUpdWidget;

}; // class CDiagObjCurve

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjCurve_h
