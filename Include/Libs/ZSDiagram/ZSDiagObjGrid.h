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

#ifndef ZSDiagObjGrid_h
#define ZSDiagObjGrid_h

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagObj.h"

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjGrid : public CDiagObj
//******************************************************************************
{
public: // ctors and dtor
    CDiagObjGrid(
        const QString& i_strObjName,
        CDiagScale*    i_pDiagScaleX,
        CDiagScale*    i_pDiagScaleY );
    virtual ~CDiagObjGrid();
public: // instance methods
    void show( EDivLineLayer i_layer );
    void hide( EDivLineLayer i_layer );
    void setCol( EDivLineLayer i_layer, const QColor& i_col );
    QColor getCol( EDivLineLayer i_layer ) const;
    void setPenStyle( EDivLineLayer i_layer, const Qt::PenStyle& i_penStyle );
    Qt::PenStyle getPenStyle( EDivLineLayer i_layer ) const;
public: // overridables of base class CDiagObj
    virtual void show( bool i_bInformDiagram = true );
    virtual void hide( bool i_bInformDiagram = true );
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
private: // copy ctor not allowed
    CDiagObjGrid( const CDiagObjGrid& );
private: // assignment operator not allowed
    void operator=( const CDiagObjGrid& );
protected:  // instance members
    QColor       m_arcol[EDivLineLayerCount];
    Qt::PenStyle m_arpenStyle[EDivLineLayerCount];
    bool         m_arbShow[EDivLineLayerCount];
    bool         m_bUpdWidget;

}; // class CDiagObjGrid

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjGrid_h
