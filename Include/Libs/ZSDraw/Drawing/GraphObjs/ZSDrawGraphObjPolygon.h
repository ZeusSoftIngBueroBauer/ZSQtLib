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

#ifndef ZSDraw_GraphObjPolygon_h
#define ZSDraw_GraphObjPolygon_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolyline.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CGraphObjPolygon : public CGraphObjPolyline
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjPolygon"; }
public: // ctors and dtor
    CGraphObjPolygon(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    virtual ~CGraphObjPolygon();
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables of base class CGraphObj
    //virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr ) const override;
public: // reimplementing methods of base class QGraphicItem
    //void setCursor( const QCursor& cursor );
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;

}; // class CGraphObjPolygon

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjPolygon_h
