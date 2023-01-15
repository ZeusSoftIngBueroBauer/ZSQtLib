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

#ifndef ZSDiagramLabelStyles_h
#define ZSDiagramLabelStyles_h

#include <QtCore/qstring.h>
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>
#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagramAux.h"

namespace ZS
{
namespace Diagram
{
struct SFrameStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CLabelStyle
//******************************************************************************
{
public: // ctors and dtor
    CLabelStyle();
    CLabelStyle( const QString& i_strLabel );
    CLabelStyle(
        const QString&        i_strLabel,
        const Qt::BrushStyle& i_brushStyle,
        const QColor&         i_colFg,
        const QColor&         i_colBg,
        const QFont&          i_fntLabel,
        int                   i_iMarginTop,
        int                   i_iMarginBottom,
        int                   i_iMarginLeft,
        int                   i_iMarginRight,
        int                   i_iAlignmentFlags = Qt::AlignVCenter|Qt::AlignHCenter,
        SFrameStyle*          i_pFrameStyle = nullptr );
    CLabelStyle( const CLabelStyle& i_labelStyleOther );
    virtual ~CLabelStyle();
public: // instance methods
    void setLabel( const QString& i_strLabel );
    QString getLabel() const;
    void setBrushStyle( const Qt::BrushStyle& i_brushStyle );
    Qt::BrushStyle getBrushStyle() const;
    void setColFg( const QColor& i_col );
    QColor getColFg() const;
    void setColBg( const QColor& i_col );
    QColor getColBg() const;
    void setFont( const QFont& i_fnt );
    QFont getFont() const;
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    void setAlignment( int i_iAlignmentFlags );
    int getAlignment() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    QRect boundingRect() const;
public: // overridables
    virtual void draw( QPainter* i_pPainter, const QRect& i_rect ) const;
protected: // instance members
    QString        m_strLabel;
    Qt::BrushStyle m_brushStyle;
    QColor         m_colFg;
    QColor         m_colBg;
    QFont          m_fntLabel;
    int            m_iMarginTop;
    int            m_iMarginBottom;
    int            m_iMarginLeft;
    int            m_iMarginRight;
    int            m_iAlignmentFlags;
    SFrameStyle*   m_pFrameStyle;

}; // struct CLabelStyle

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramLabelStyles_h
