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

#ifndef ZSDiagramLineStyles_h
#define ZSDiagramLineStyles_h

#include <QtGui/qcolor.h>
#include "ZSDiagram/ZSDiagramDllMain.h"

namespace ZS
{
namespace Diagram
{
//******************************************************************************
struct ZSDIAGRAMDLL_API SLineStyle
//******************************************************************************
{
public: // ctors
    SLineStyle();
    SLineStyle(
        const QColor&       i_col,
        const Qt::PenStyle& i_penStyle,
        int                 i_iLineWidth );
    SLineStyle( const SLineStyle& i_lineStyleOther );
public: // instance methods
    void setCol( const QColor& i_col );
    QColor getCol() const;
    void setPenStyle( const Qt::PenStyle& i_penStyle );
    Qt::PenStyle getPenStyle() const;
    void setLineWidth( int i_iLineWidth );
    int getLineWidth() const;
    void draw( QPainter* i_pPainter, const QPoint& i_pt1, const QPoint& i_pt2 );
protected: // instance members
    QColor       m_col;
    Qt::PenStyle m_penStyle;
    int          m_iLineWidth;

}; // struct SLineStyle

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramLineStyles_h
