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

#ifndef ZSDiagObjGrid_h
#define ZSDiagObjGrid_h

#include "ZSDiagram/ZSDiagObj.h"

#include <QtGui/qcolor.h>

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjGrid : public CDiagObj
//******************************************************************************
{
public: // class methods
    static QString ClassName() { return "CDiagObjGrid"; }
public: // ctors and dtor
    CDiagObjGrid(
        const QString& i_strObjName,
        CDiagScale*    i_pDiagScaleX,
        CDiagScale*    i_pDiagScaleY );
    virtual ~CDiagObjGrid();
public: // instance methods
    void show( const ZS::System::CEnumDivLineLayer& i_eLayer );
    void hide( const ZS::System::CEnumDivLineLayer& i_eLayer );
    void setCol( const ZS::System::CEnumDivLineLayer& i_eLayer, const QColor& i_col );
    QColor getCol( const ZS::System::CEnumDivLineLayer& i_eLayer ) const;
    void setPenStyle( const ZS::System::CEnumDivLineLayer& i_eLayer, const Qt::PenStyle& i_penStyle );
    Qt::PenStyle getPenStyle( const ZS::System::CEnumDivLineLayer& i_eLayer ) const;
public: // overridables of base class CDiagObj
    virtual void show( bool i_bInformDiagram = true ) override;
    virtual void hide( bool i_bInformDiagram = true ) override;
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const override;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr ) override;
private: // copy ctor not allowed
    CDiagObjGrid( const CDiagObjGrid& );
private: // assignment operator not allowed
    void operator=( const CDiagObjGrid& );
protected:  // instance members
    QVector<QColor> m_arcol; //[EDivLineLayerCount];
    QVector<Qt::PenStyle> m_arpenStyle; //[EDivLineLayerCount];
    QVector<bool> m_arbShow; //[EDivLineLayerCount];
    bool m_bUpdWidget;

}; // class CDiagObjGrid

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjGrid_h
