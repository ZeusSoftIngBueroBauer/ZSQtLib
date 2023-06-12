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

#ifndef ZSDiagObjLine_h
#define ZSDiagObjLine_h

#include "ZSDiagram/ZSDiagObj.h"

namespace ZS
{
namespace Diagram
{
struct SLineStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjLine : public CDiagObj
//******************************************************************************
{
public: // class methods
    static QString ClassName() { return "CDiagObjLine"; }
public: // type definitions and constants
    enum ECoordinatesUnit
    {
        ECoordinatesUnitAxisScale,  // Koordinatenangabe in Basiseinheit der Achsenskalierung
        ECoordinatesUnitPixel       // Koordinatenangabe in Pixelkoordinaten
    };
public: // ctors and dtor
    CDiagObjLine(
        const QString& i_strObjName,
        CDiagScale*    i_pDiagScaleX,
        CDiagScale*    i_pDiagScaleY,
        SLineStyle*    i_lineStyle );
    virtual ~CDiagObjLine();
public: // instance methods
    void setCol( const QColor& i_col );
    QColor getCol() const;
    void setX1( double i_X1 , ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale );
    double getX1( ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale ) const;
    void setY1( double i_Y1 , ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale );
    double getY1( ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale ) const;
    void setX2( double i_X2 , ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale );
    double getX2( ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale ) const;
    void setY2( double i_Y2 , ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale );
    double getY2( ECoordinatesUnit i_pointType = ECoordinatesUnitAxisScale ) const;
public: // overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags , QPaintDevice* i_pPaintDevice = nullptr );
private: // copy constructor not allowed
    CDiagObjLine( const CDiagObjLine& );
private: // assignment operator not allowed
    CDiagObjLine& operator= ( const CDiagObjLine& );
private: // instance members
    SLineStyle* m_pLineStyle;
    // Koordinaten in Pixel
    QPoint           m_pt1;
    QPoint           m_pt2;
    // Koordinaten in Grundeinheit.
    double           m_fX1;
    ECoordinatesUnit m_eX1;
    double           m_fY1;
    ECoordinatesUnit m_eY1;
    double           m_fX2;
    ECoordinatesUnit m_eX2;
    double           m_fY2;
    ECoordinatesUnit m_eY2;

}; // class CDiagObjLine

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjLine_h
