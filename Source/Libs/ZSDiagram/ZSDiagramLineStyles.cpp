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

#include <QtGui/qpainter.h>
#include "ZSDiagram/ZSDiagramLineStyles.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Diagram;


/*******************************************************************************
struct SLineStyle
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SLineStyle::SLineStyle() :
//------------------------------------------------------------------------------
    m_col(Qt::black),
    m_penStyle(Qt::SolidLine),
    m_iLineWidth(1)
{
} // ctor

//------------------------------------------------------------------------------
SLineStyle::SLineStyle(
    const QColor&       i_col,
    const Qt::PenStyle& i_penStyle,
    int                 i_iLineWidth ) :
//------------------------------------------------------------------------------
    m_col(i_col),
    m_penStyle(i_penStyle),
    m_iLineWidth(i_iLineWidth)
{
} // ctor

//------------------------------------------------------------------------------
SLineStyle::SLineStyle( const SLineStyle& i_lineStyleOther ) :
//------------------------------------------------------------------------------
    m_col(i_lineStyleOther.m_col),
    m_penStyle(i_lineStyleOther.m_penStyle),
    m_iLineWidth(i_lineStyleOther.m_iLineWidth)
{
} // copy ctor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void SLineStyle::setCol( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_col = i_col;
}

//------------------------------------------------------------------------------
QColor SLineStyle::getCol() const
//------------------------------------------------------------------------------
{
    return m_col;
}

//------------------------------------------------------------------------------
void SLineStyle::setPenStyle( const Qt::PenStyle& i_penStyle )
//------------------------------------------------------------------------------
{
    m_penStyle = i_penStyle;
}

//------------------------------------------------------------------------------
Qt::PenStyle SLineStyle::getPenStyle() const
//------------------------------------------------------------------------------
{
    return m_penStyle;
}

//------------------------------------------------------------------------------
void SLineStyle::setLineWidth( int i_iLineWidth )
//------------------------------------------------------------------------------
{
    m_iLineWidth = i_iLineWidth;
}

//------------------------------------------------------------------------------
int SLineStyle::getLineWidth() const
//------------------------------------------------------------------------------
{
    return m_iLineWidth;
}

//------------------------------------------------------------------------------
void SLineStyle::draw( QPainter* i_pPainter, const QPoint& i_pt1, const QPoint& i_pt2 )
//------------------------------------------------------------------------------
{
    QPen pen(m_col,m_iLineWidth,m_penStyle);

    i_pPainter->save();
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(i_pt1,i_pt2);
    i_pPainter->restore();

} // draw
