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

#ifndef ZSDraw_GraphObjConnectionLine_h
#define ZSDraw_GraphObjConnectionLine_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsPolygonItem>
#else
#include <QtWidgets/QGraphicsPolygonItem>
#endif

namespace ZS
{
namespace Draw
{
class CGraphObjConnectionPoint;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjConnectionLine : public CGraphObjPolygon
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjConnectionLine"; }
public: // class methods
    static QPainter::RenderHints painterRenderHints();
    static void setPainterRenderHints(QPainter::RenderHints i_renderHints);
    static void resetPainterRenderHints();
public: // ctors and dtor
    CGraphObjConnectionLine(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    virtual ~CGraphObjConnectionLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // must overridables of base class CGraphObj
    void openFormatGraphObjsDialog() override;
public: // overridables
    virtual bool setConnectionPoint(ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pGraphObjCnctPt);
    virtual ELinePoint getConnectionLinePoint(CGraphObjConnectionPoint* i_pGraphObjCnctPt) const;
    virtual CGraphObjConnectionPoint* getConnectionPoint(ELinePoint i_linePoint) const;
protected: // overridables of base class QGraphicsItem
    void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    QVariant itemChange(GraphicsItemChange i_change, const QVariant& i_value) override;
protected slots:
    void onGraphObjConnectionPointGeometryOnSceneChanged(CGraphObj* i_pGraphObjCnctPt, bool );
    void onGraphObjConnectionPointZValueChanged(CGraphObj* i_pGraphObjCnctPt);
protected: // auxiliary instance methods (method tracing)
    void traceConnectionPoints(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined) const;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // class members
    static QPainter::RenderHints s_painterRenderHints;
protected: // instance members
    /*!< Vector with connection points of the connection line.
         The vector has two elements: Start and End point. */
    QMap<ELinePoint, CGraphObjConnectionPoint*> m_arpCnctPts;
    /*!< Counter to block debug trace outputs for connection points. */
    int m_iTraceConnectionPointsBlockedCounter = 0;

}; // class CGraphObjConnectionLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionLine_h
