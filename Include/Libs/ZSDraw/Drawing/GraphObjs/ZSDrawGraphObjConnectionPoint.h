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

#ifndef ZSDraw_GraphObjConnectionPoint_h
#define ZSDraw_GraphObjConnectionPoint_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"

namespace ZS
{
namespace Draw
{
class CGraphObjConnectionLine;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjConnectionPoint : public CGraphObjEllipse
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjConnectionPoint"; }
public: // class methods
    static QPainter::RenderHints painterRenderHints();
    static void setPainterRenderHints(QPainter::RenderHints i_renderHints);
    static void resetPainterRenderHints();
public: // class methods
    static double defaultRadiusInPx();
    static double defaultInnerCircleRadiusInPx();
public: // ctors and dtor
    CGraphObjConnectionPoint(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    ~CGraphObjConnectionPoint() override;
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    CGraphObj* clone() override;
public: // instance methods
    QString pathNameOfLinkedObject() const;
    QString myPathName() const;
public: // instance methods
    SGraphObjSelectionPoint getSelectionPoint() const;
public: // instance methods
    bool appendConnectionLine( CGraphObjConnectionLine* i_pGraphObjCnctLine ); // appends the specified connection line to the list of connection lines. Return false if the line is already connected with the connection point.
    void removeConnectionLine( CGraphObjConnectionLine* i_pGraphObjCnctLine );
    int findConnectionLineIdx( CGraphObjConnectionLine* i_pGraphObjCnctLine );
    int getConnectionLinesCount() const;
    CGraphObjConnectionLine* getConnectionLine( int i_iLineIdx );
public: // overridables
    void setInnerCircleRadiusInPerCent(double i_fRadius_perCent);
    void setInnerCircleRadiusInPx(double i_fRadius_px);
    double getInnerCircleRadiusInPerCent();
    double getInnerCircleRadiusInPx();
public: // overridables of base class CGraphObj
    void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // must overridables of base class QGraphicsItem
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // class members
    static QPainter::RenderHints s_painterRenderHints;
    static double s_fRadius_px;
    static double s_fInnerCircleRadius_px;
    /*!< Default pen width to be used for painting the border of the selection points. */
    static int s_iPenWidth_px;
    /*!< Default color to be used for painting the border of the selection points. */
    static QColor s_colPen;
protected: // instance members
    /*!< List with connection lines linked to the connection point. */
    QList<CGraphObjConnectionLine*> m_lstConnectionLines;
    /*!< Radius for drawing the connection point. */
    double m_fRadius_px;
    /*!< Inner circle radius for drawing the connection point. */
    double m_fInnerCircleRadius_perCent;
    /*!< Defines the type of the selecton point, the linked object and the position at the linked
         object the selection point is linked to. */
    SGraphObjSelectionPoint m_selPt;

}; // class CGraphObjConnectionPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionPoint_h
