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

#ifndef ZSDiagObjLabel_h
#define ZSDiagObjLabel_h

#include "ZSDiagram/ZSDiagObj.h"

#include <QtGui/qfont.h>
//#include <QtGui/qpixmap.h>

namespace ZS
{
namespace Diagram
{
struct SFrameStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjLabel : public CDiagObj
//******************************************************************************
{
public: // class methods
    static QString ClassName() { return "CDiagObjLabel"; }
public: // ctors and dtor
    CDiagObjLabel(
        const QString& i_strObjName,
        const QString& i_strText,
        ELayoutPos     i_layoutPos,
        int            i_iStateCount = 1 );
    CDiagObjLabel(
        const QString& i_strObjName,
        const QPixmap& i_pxm,
        ELayoutPos     i_layoutPos,
        int            i_iStateCount = 1 );
    virtual ~CDiagObjLabel();
public: // instance methods
    void setState( int i_iState );
    int getState() const;
    void setMinimumWidth( int i_cxWidth );
    int getMinimumWidth() const;
    void setMaximumWidth( int i_cxWidth );
    int getMaximumWidth() const;
    void setMinimumHeight( int i_cyHeight );
    int getMinimumHeight() const;
    void setMaximumHeight( int i_cyHeight );
    int getMaximumHeight() const;
    void setLayoutPosAlignmentFlags( int i_iAlignmentFlags );
    int getLayoutPosAlignmentFlags() const;
    void setLayoutPosOffsX( int i_cxOffs );
    int getLayoutPosOffsX() const;
    void setLayoutPosOffsY( int i_cyOffs );
    int getLayoutPosOffsY() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    void setColBg( const QColor& i_col, int i_iState = 0 );
    QColor getColBg( int i_iState = 0 ) const;
    void setBrushStyle( Qt::BrushStyle i_brushStyle, int i_iState = 0 );
    Qt::BrushStyle getBrushStyle( int i_iState = 0 ) const;
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    void setText( const QString& i_strText, int i_uState = 0 );
    QString getText( int i_iState = 0 ) const;
    void setColText( const QColor& i_col, int i_iState = 0 );
    QColor getColText( int i_iState = 0 ) const;
    void setFont( const QFont& i_fnt );
    QFont getFont() const;
    void setTextAlignmentFlags( int i_iAlignmentFlags );
    int getTextAlignmentFlags() const;
    void setTextOrientation( ETextOrientation i_textOrientation );
    ETextOrientation getTextOrientation() const;
    void setPixmap( const QPixmap& i_pxm, int i_iState = 0 );
    QPixmap getPixmap( int i_iState = 0 ) const;
    void setPixmapAlignmentFlags( int i_iAlignmentFlags );
    int getPixmapAlignmentFlags() const;
public: // overridables of base class CDiagObj
    virtual QSize sizeHint();
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected: // overridables of base class CDiagObj
    //virtual ZS::System::SErrResultInfo onReceivedReqSetValue( CMsgReqSetValue* i_pMsgReq ) override;
private: // copy ctor not allowed
    CDiagObjLabel( const CDiagObjLabel& );
private: // assignment operator not allowed
    void operator=( const CDiagObjLabel& );
protected:  // instance members
    int             m_iState;
    int             m_iStateCount;
    int             m_cxMinimumWidth;
    int             m_cxMaximumWidth;
    int             m_cyMinimumHeight;
    int             m_cyMaximumHeight;
    int             m_iLayoutPosAlignmentFlags;
    int             m_cxLayoutPosOffs;
    int             m_cyLayoutPosOffs;
    SFrameStyle*    m_pFrameStyle;
    QColor*         m_arcolBg;
    Qt::BrushStyle* m_arbrushStyle;
    int             m_iMarginTop;
    int             m_iMarginBottom;
    int             m_iMarginLeft;
    int             m_iMarginRight;
    QString*        m_arstrText;
    QColor*         m_arcolText;
    QFont           m_fntText;
    int             m_iTextAlignmentFlags;
    ETextOrientation  m_textOrientation;
    QPixmap*        m_arpxm;
    int             m_iPixmapAlignmentFlags;
    QRect           m_rectOuterFrame;
    QRect           m_rectInnerFrame;
    QRect           m_rectText;
    QRect           m_rectPixmap;
    bool            m_bUpdWidget;

}; // class CDiagObjLabel

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjLabel_h
