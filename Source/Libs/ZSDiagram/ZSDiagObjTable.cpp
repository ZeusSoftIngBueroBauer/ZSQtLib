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

#include "ZSDiagram/ZSDiagObjTable.h"
#include "ZSDiagram/ZSDiagObjValueProvider.h"
#include "ZSDiagram/ZSDiagramProcPixmap.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


//lint -e661
//lint -e662
//lint -esym(550,bConnect)


/*******************************************************************************
class CDiagObjTable : public CDiagObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjTable::CDiagObjTable(
    const QString& i_strObjName,
    int            i_iRowCountMax,
    int            i_iClmCountMax,
    ELayoutPos     i_layoutPos ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strObjName */ i_strObjName,
        /* pDiagTrace */ nullptr,
        /* layoutPos  */ i_layoutPos ),
    // The following table and cell properties will be set:
    //m_arbShowGridLines[EOrientationCount]
    m_colGridLines(Qt::lightGray),
    m_colText(Qt::black),
    m_fnt(),
    m_iIndentTop(4),
    m_iIndentBottom(4),
    m_iIndentLeft(4),
    m_iIndentRight(4),
    m_pFrameStyle(nullptr),
    m_iMarginTop(0),
    m_iMarginBottom(0),
    m_iMarginLeft(0),
    m_iMarginRight(0),
    m_iRowCountMax(i_iRowCountMax),
    m_iClmCountMax(i_iClmCountMax),
    m_iRowCount(i_iRowCountMax),
    m_iClmCount(i_iClmCountMax),
    m_cxClmIndent(2),
    m_ariClmAlignmentFlags(nullptr),
    m_arcxClmFixedWidth(nullptr),
    m_arcxClmMinimumWidth(nullptr),
    m_arcxClmMaximumWidth(nullptr),
    m_arstrClmInvalidValue(nullptr),
    m_arpValueFormatClm(nullptr),
    m_arcellStyleClm(nullptr),
    m_ardataTypeClmVal(nullptr),
    m_ardataTypeClmDpl(nullptr),
    // The following table and cell properties (dimensions and coordinates)
    // will be calculated within the update method:
    m_cyRowHeight(0),
    m_arcxClmWidth(nullptr),
    m_cyTableHeight(0),
    m_rectTable(),
    // Cell states and data
    m_arariCellAlignmentFlags(nullptr),
    m_ararpDiagObjValueProvider(nullptr),
    m_ararpValueFormatCell(nullptr),
    m_ararcellStyleCell(nullptr),
    m_arardataTypeCellVal(nullptr),
    m_arardataTypeCellDpl(nullptr),
    m_ararvalidityCellData(nullptr),
    m_ararstrCellData(nullptr),
    m_ararpPxmCellData(nullptr),
    m_ararpPhysValCellData(nullptr),
    // Flag to indicate that the table content has been changed.
    m_bUpdWidget(true)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    for( int idxOrientation = 0; idxOrientation < EOrientationCount; idxOrientation++ )
    {
        m_arbShowGridLines[idxOrientation] = false;
    }
    if( m_iRowCountMax == 0 )
    {
        m_iRowCountMax = 1;
    }
    if( m_iClmCountMax == 0 )
    {
        m_iClmCountMax = 1;
    }

    m_ariClmAlignmentFlags = new int[m_iClmCountMax];
    m_arcxClmFixedWidth = new int[m_iClmCountMax];
    m_arcxClmMinimumWidth = new int[m_iClmCountMax];
    m_arcxClmMaximumWidth = new int[m_iClmCountMax];
    m_arcxClmWidth = new int[m_iClmCountMax];
    m_arstrClmInvalidValue = new QString[m_iClmCountMax];
    m_arpValueFormatClm = new SValueFormatProvider*[m_iClmCountMax];
    m_arcellStyleClm = new SCellStyle[m_iClmCountMax];
    m_ardataTypeClmVal = new EDataType[m_iClmCountMax];
    m_ardataTypeClmDpl = new EDataType[m_iClmCountMax];

    for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
    {
        m_ariClmAlignmentFlags[idxClm] = Qt::AlignHCenter|Qt::AlignVCenter; //lint !e655 .. das nervt
        m_arcxClmFixedWidth[idxClm] = -1;
        m_arcxClmMinimumWidth[idxClm] = -1;
        m_arcxClmMaximumWidth[idxClm] = -1;
        m_arcxClmWidth[idxClm] = 0;
        m_arstrClmInvalidValue[idxClm] = "---";
        m_arpValueFormatClm[idxClm] = nullptr;
        //m_arcellStyleClm[idxClm]; with default ctor
        m_ardataTypeClmVal[idxClm] = EDataTypeUndefined;
        m_ardataTypeClmDpl[idxClm] = EDataTypeUndefined;
    }

    m_arariCellAlignmentFlags = new int[m_iRowCountMax*m_iClmCountMax];
    m_ararpDiagObjValueProvider = new CDiagObjValueProvider*[m_iRowCountMax*m_iClmCountMax];
    m_ararpValueFormatCell = new SValueFormatProvider*[m_iRowCountMax*m_iClmCountMax];
    m_ararcellStyleCell = new SCellStyle[m_iRowCountMax*m_iClmCountMax];
    m_arardataTypeCellVal = new EDataType[m_iRowCountMax*m_iClmCountMax];
    m_arardataTypeCellDpl = new EDataType[m_iRowCountMax*m_iClmCountMax];
    m_ararvalidityCellData = new EValueValidity[m_iRowCountMax*m_iClmCountMax];
    m_ararstrCellData = new QString[m_iRowCountMax*m_iClmCountMax];
    m_ararpPxmCellData = new QPixmap*[m_iRowCountMax*m_iClmCountMax];
    m_ararpPhysValCellData = new CPhysVal*[m_iRowCountMax*m_iClmCountMax];

    for( int idxRow = 0, idxCell = 0; idxRow < m_iRowCountMax; idxRow++ )
    {
        for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
        {
            m_arariCellAlignmentFlags[idxCell] = 0;
            m_ararpDiagObjValueProvider[idxCell] = nullptr;
            //m_ararcellStyleCell[idxCell]; with default ctor
            m_ararpValueFormatCell[idxCell] = nullptr;
            m_arardataTypeCellVal[idxCell] = EDataTypeUndefined;
            m_arardataTypeCellDpl[idxCell] = EDataTypeUndefined;
            m_ararvalidityCellData[idxCell] = EValueValidity::Invalid;
            m_ararstrCellData[idxCell] = QString();
            m_ararpPxmCellData[idxCell] = nullptr;
            m_ararpPhysValCellData[idxCell] = nullptr;
            idxCell++;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjTable::~CDiagObjTable()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pFrameStyle;
    }
    catch(...)
    {
    }
    m_pFrameStyle = nullptr;

    if( m_arpValueFormatClm != nullptr && m_iClmCountMax > 0 )
    {
        for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
        {
            try
            {
                delete m_arpValueFormatClm[idxClm];
            }
            catch(...)
            {
            }
            m_arpValueFormatClm[idxClm] = nullptr;
        }
    }

    // So umstaendlich programmiert weil lint sonst keine Ruhe gibt ...
    if( m_ararpDiagObjValueProvider != nullptr && m_iRowCountMax > 0 && m_iClmCountMax > 0 )
    {
        for( int idxRow = 0; idxRow < m_iRowCountMax; idxRow++ )
        {
            for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
            {
                int idxCell = 0;
                try
                {
                    idxCell = getCellIdx(idxRow,idxClm);
                }
                catch(...)
                {
                }
                m_ararpDiagObjValueProvider[idxCell] = nullptr;
            }
        }
    }

    // So umstaendlich programmiert weil lint sonst keine Ruhe gibt ...
    if( m_ararpValueFormatCell != nullptr && m_iRowCountMax > 0 && m_iClmCountMax > 0 )
    {
        for( int idxRow = 0; idxRow < m_iRowCountMax; idxRow++ )
        {
            for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
            {
                int idxCell = 0;
                try
                {
                    idxCell = getCellIdx(idxRow,idxClm);
                    delete m_ararpValueFormatCell[idxCell];
                }
                catch(...)
                {
                }
                m_ararpValueFormatCell[idxCell] = nullptr;
            }
        }
    }

    // So umstaendlich programmiert weil lint sonst keine Ruhe gibt ...
    if( m_ararpPxmCellData != nullptr && m_iRowCountMax > 0 && m_iClmCountMax > 0 )
    {
        for( int idxRow = 0; idxRow < m_iRowCountMax; idxRow++ )
        {
            for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
            {
                int idxCell = 0;
                try
                {
                    idxCell = getCellIdx(idxRow,idxClm);
                    delete m_ararpPxmCellData[idxCell];
                }
                catch(...)
                {
                }
                m_ararpPxmCellData[idxCell] = nullptr;
            }
        }
    }

    // So umstaendlich programmiert weil lint sonst keine Ruhe gibt ...
    if( m_ararpPhysValCellData != nullptr && m_iRowCountMax > 0 && m_iClmCountMax > 0 )
    {
        for( int idxRow = 0; idxRow < m_iRowCountMax; idxRow++ )
        {
            for( int idxClm = 0; idxClm < m_iClmCountMax; idxClm++ )
            {
                int idxCell = 0;
                try
                {
                    idxCell = getCellIdx(idxRow,idxClm);
                    delete m_ararpPhysValCellData[idxCell];
                }
                catch(...)
                {
                    m_ararpPhysValCellData[idxCell] = nullptr;
                }
            }
        }
    }

    delete [] m_ariClmAlignmentFlags;
    m_ariClmAlignmentFlags = nullptr;
    delete [] m_arcxClmFixedWidth;
    m_arcxClmFixedWidth = nullptr;
    delete [] m_arcxClmMinimumWidth;
    m_arcxClmMinimumWidth = nullptr;
    delete [] m_arcxClmMaximumWidth;
    m_arcxClmMaximumWidth = nullptr;
    delete [] m_arcxClmWidth;
    m_arcxClmWidth = nullptr;
    delete [] m_arstrClmInvalidValue; //lint !e1551 .. dann is eh scho Wurscht ..
    m_arstrClmInvalidValue = nullptr;
    delete [] m_arpValueFormatClm;
    m_arpValueFormatClm = nullptr;
    delete [] m_arcellStyleClm;
    m_arcellStyleClm = nullptr;
    delete [] m_ardataTypeClmVal;
    m_ardataTypeClmVal = nullptr;
    delete [] m_ardataTypeClmDpl;
    m_ardataTypeClmDpl = nullptr;
    delete [] m_arariCellAlignmentFlags;
    m_arariCellAlignmentFlags = nullptr;
    delete [] m_ararpDiagObjValueProvider;
    m_ararpDiagObjValueProvider = nullptr;
    delete [] m_ararpValueFormatCell;
    m_ararpValueFormatCell = nullptr;
    delete [] m_ararcellStyleCell;
    m_ararcellStyleCell = nullptr;
    delete [] m_arardataTypeCellVal;
    m_arardataTypeCellVal = nullptr;
    delete [] m_arardataTypeCellDpl;
    m_arardataTypeCellDpl = nullptr;
    delete [] m_ararvalidityCellData;
    m_ararvalidityCellData = nullptr;
    delete [] m_ararstrCellData; //lint !e1551 .. dann is eh scho Wurscht ..
    m_ararstrCellData = nullptr;
    delete [] m_ararpPxmCellData;
    m_ararpPxmCellData = nullptr;
    delete [] m_ararpPhysValCellData;
    m_ararpPhysValCellData = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjTable::showGridLines( EOrientation i_orientation )
//------------------------------------------------------------------------------
{
    int idxMin = i_orientation;
    int idxMax = i_orientation;
    int idxOrientation;

    if( i_orientation == EOrientationCount )
    {
        idxMin = 0;
        idxMax = EOrientationCount-1;
    }
    for( idxOrientation = idxMin; idxOrientation <= idxMax; idxOrientation++ )
    {
        m_arbShowGridLines[idxOrientation] = true;
    }
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);

} // showGridLines

//------------------------------------------------------------------------------
void CDiagObjTable::hideGridLines( EOrientation i_orientation )
//------------------------------------------------------------------------------
{
    int idxMin = i_orientation;
    int idxMax = i_orientation;
    int idxOrientation;

    if( i_orientation == EOrientationCount )
    {
        idxMin = 0;
        idxMax = EOrientationCount-1;
    }
    for( idxOrientation = idxMin; idxOrientation <= idxMax; idxOrientation++ )
    {
        m_arbShowGridLines[idxOrientation] = false;
    }
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);

} // hideGridLines

//------------------------------------------------------------------------------
void CDiagObjTable::setGridLinesColor( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colGridLines = i_col;
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);
}

//------------------------------------------------------------------------------
QColor CDiagObjTable::getGridLinesColor() const
//------------------------------------------------------------------------------
{
    return m_colGridLines;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setTextColor( const QColor& i_col )
//------------------------------------------------------------------------------
{
    m_colText = i_col;
    m_bUpdWidget = true;
    invalidate(EUpdatePixmapWidget,true);
}

//------------------------------------------------------------------------------
QColor CDiagObjTable::getTextColor() const
//------------------------------------------------------------------------------
{
    return m_colText;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setFont( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    m_fnt = i_fnt;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setFont

//------------------------------------------------------------------------------
QFont CDiagObjTable::getFont() const
//------------------------------------------------------------------------------
{
    return m_fnt;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setIndentTop( int i_iIndentTop )
//------------------------------------------------------------------------------
{
    m_iIndentTop = i_iIndentTop;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setIndentTop

//------------------------------------------------------------------------------
int CDiagObjTable::getIndentTop() const
//------------------------------------------------------------------------------
{
    return m_iIndentTop;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setIndentBottom( int i_iIndentBottom )
//------------------------------------------------------------------------------
{
    m_iIndentBottom = i_iIndentBottom;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setIndentBottom

//------------------------------------------------------------------------------
int CDiagObjTable::getIndentBottom() const
//------------------------------------------------------------------------------
{
    return m_iIndentBottom;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setIndentLeft( int i_iIndentLeft )
//------------------------------------------------------------------------------
{
    m_iIndentLeft = i_iIndentLeft;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setIndentLeft

//------------------------------------------------------------------------------
int CDiagObjTable::getIndentLeft() const
//------------------------------------------------------------------------------
{
    return m_iIndentLeft;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setIndentRight( int i_iIndentRight )
//------------------------------------------------------------------------------
{
    m_iIndentRight = i_iIndentRight;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setIndentRight

//------------------------------------------------------------------------------
int CDiagObjTable::getIndentRight() const
//------------------------------------------------------------------------------
{
    return m_iIndentRight;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setFrameStyle

//------------------------------------------------------------------------------
SFrameStyle* CDiagObjTable::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setMarginTop( int i_iMarginTop )
//------------------------------------------------------------------------------
{
    m_iMarginTop = i_iMarginTop;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setMarginTop

//------------------------------------------------------------------------------
int CDiagObjTable::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setMarginBottom( int i_iMarginBottom )
//------------------------------------------------------------------------------
{
    m_iMarginBottom = i_iMarginBottom;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setMarginBottom

//------------------------------------------------------------------------------
int CDiagObjTable::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setMarginLeft( int i_iMarginLeft )
//------------------------------------------------------------------------------
{
    m_iMarginLeft = i_iMarginLeft;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setMarginLeft

//------------------------------------------------------------------------------
int CDiagObjTable::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setMarginRight( int i_iMarginRight )
//------------------------------------------------------------------------------
{
    m_iMarginRight = i_iMarginRight;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setMarginRight

//------------------------------------------------------------------------------
int CDiagObjTable::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setRowCount( int i_iRowCount )
//------------------------------------------------------------------------------
{
    if( i_iRowCount > m_iRowCountMax )
    {
        m_iRowCount = m_iRowCountMax;
    }
    else
    {
        m_iRowCount = i_iRowCount;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setRowCount

//------------------------------------------------------------------------------
int CDiagObjTable::getRowCount() const
//------------------------------------------------------------------------------
{
    return m_iRowCount;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmCount( int i_iClmCount )
//------------------------------------------------------------------------------
{
    if( i_iClmCount > m_iClmCountMax )
    {
        m_iClmCount = m_iClmCountMax;
    }
    else
    {
        m_iClmCount = i_iClmCount;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmCount

//------------------------------------------------------------------------------
int CDiagObjTable::getClmCount() const
//------------------------------------------------------------------------------
{
    return m_iClmCount;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmIndent( int i_cxClmIndent )
//------------------------------------------------------------------------------
{
    m_cxClmIndent = i_cxClmIndent;
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmIndent

//------------------------------------------------------------------------------
int CDiagObjTable::getClmIndent() const
//------------------------------------------------------------------------------
{
    return m_cxClmIndent;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmAlignmentFlags( int i_idxClm, int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_ariClmAlignmentFlags[idxClm] = i_iAlignmentFlags;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmAlignmentFlags

//------------------------------------------------------------------------------
int CDiagObjTable::getClmAlignmentFlags( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return 0;
    }
    return m_ariClmAlignmentFlags[i_idxClm];
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmFixedWidth( int i_idxClm, int i_cxWidth )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_arcxClmFixedWidth[idxClm] = i_cxWidth;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmFixedWidth

//------------------------------------------------------------------------------
int CDiagObjTable::getClmFixedWidth( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return -1;
    }
    return m_arcxClmFixedWidth[i_idxClm];
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmMinimumWidth( int i_idxClm, int i_cxWidth )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_arcxClmMinimumWidth[idxClm] = i_cxWidth;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmMinimumWidth

//------------------------------------------------------------------------------
int CDiagObjTable::getClmMinimumWidth( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return -1;
    }
    return m_arcxClmMinimumWidth[i_idxClm];
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmMaximumWidth( int i_idxClm, int i_cxWidth )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_arcxClmMaximumWidth[idxClm] = i_cxWidth;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmMaximumWidth

//------------------------------------------------------------------------------
int CDiagObjTable::getClmMaximumWidth( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return -1;
    }
    return m_arcxClmMaximumWidth[i_idxClm];
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmInvalidValueStr( int i_idxClm, const QString& i_str )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_arstrClmInvalidValue[idxClm] = i_str;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmInvalidValueStr

//------------------------------------------------------------------------------
QString CDiagObjTable::getClmInvalidValueStr( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return "---";
    }
    return m_arstrClmInvalidValue[i_idxClm];

} // getClmInvalidValueStr

//------------------------------------------------------------------------------
void CDiagObjTable::setClmValueFormatProvider( int i_idxClm, const SValueFormatProvider& i_valueFormat )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        if( m_arpValueFormatClm[idxClm] != nullptr )
        {
            delete m_arpValueFormatClm[idxClm];
            m_arpValueFormatClm[idxClm] = nullptr;
        }
        m_arpValueFormatClm[idxClm] = new SValueFormatProvider(i_valueFormat);
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmValueFormatProvider

//------------------------------------------------------------------------------
SValueFormatProvider CDiagObjTable::getClmValueFormatProvider(int i_idxClm ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat; // invalid format

    if( i_idxClm >= 0 || i_idxClm < m_iClmCountMax )
    {
        if( m_arpValueFormatClm[i_idxClm] != nullptr )
        {
            valueFormat = *m_arpValueFormatClm[i_idxClm];
        }
    }
    return valueFormat;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmCellStyle( int i_idxClm, const SCellStyle& i_cellStyle )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_arcellStyleClm[idxClm] = i_cellStyle;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmCellStyle

//------------------------------------------------------------------------------
CDiagObjTable::SCellStyle CDiagObjTable::getClmCellStyle(int i_idxClm ) const
//------------------------------------------------------------------------------
{
    SCellStyle cellStyle;

    if( i_idxClm >= 0 || i_idxClm < m_iClmCountMax )
    {
        cellStyle = m_arcellStyleClm[i_idxClm];
    }
    return cellStyle;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setClmValueDataType( int i_idxClm, EDataType i_dataType )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_ardataTypeClmVal[idxClm] = i_dataType;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmValueDataType

//------------------------------------------------------------------------------
CDiagObjTable::EDataType CDiagObjTable::getClmValueDataType( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return EDataTypeUndefined;
    }
    return m_ardataTypeClmVal[i_idxClm];

} // getClmValueDataType

//------------------------------------------------------------------------------
void CDiagObjTable::setClmDisplayDataType( int i_idxClm, EDataType i_dataType )
//------------------------------------------------------------------------------
{
    if( i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;

    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
    {
        m_ardataTypeClmDpl[idxClm] = i_dataType;
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setClmDisplayDataType

//------------------------------------------------------------------------------
CDiagObjTable::EDataType CDiagObjTable::getClmDisplayDataType( int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return EDataTypeUndefined;
    }
    return m_ardataTypeClmDpl[i_idxClm];

} // getClmDisplayDataType

//------------------------------------------------------------------------------
void CDiagObjTable::setCellAlignmentFlags( int i_idxRow, int i_idxClm, int i_iAlignmentFlags )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arariCellAlignmentFlags[idxCell] = i_iAlignmentFlags;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellAlignmentFlags

//------------------------------------------------------------------------------
int CDiagObjTable::getCellAlignmentFlags( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return 0;
    }
    int idxCell = getCellIdx(i_idxRow,i_idxClm);
    return m_arariCellAlignmentFlags[idxCell];
}

//------------------------------------------------------------------------------
void CDiagObjTable::setCellValueProvider(
    int                    i_idxRow,
    int                    i_idxClm,
    CDiagObjValueProvider* i_pDiagObjValueProvider )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int  idxRowMin = i_idxRow;
    int  idxRowMax = i_idxRow;
    int  idxRow;
    int  idxClmMin = i_idxClm;
    int  idxClmMax = i_idxClm;
    int  idxClm;
    int  idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);

            if( m_ararpDiagObjValueProvider[idxCell] != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_ararpDiagObjValueProvider[idxCell],
                    /* pcSignal     */ SIGNAL(valueChanged(ZS::Diagram::CDiagObj*)),
                    /* pObjReceiver */ this,
                    /* pcMember     */ SLOT(cellValueChanged(ZS::Diagram::CDiagObj*)) );
            }
            m_ararpDiagObjValueProvider[idxCell] = i_pDiagObjValueProvider;

            if( m_ararpDiagObjValueProvider[idxCell] != nullptr )
            {
                QObject::connect(
                    /* pObjSender   */ m_ararpDiagObjValueProvider[idxCell],
                    /* pcSignal     */ SIGNAL(valueChanged(ZS::Diagram::CDiagObj*)),
                    /* pObjReceiver */ this,
                    /* pcMember     */ SLOT(cellValueChanged(ZS::Diagram::CDiagObj*)) );

                m_arardataTypeCellVal[idxCell] = EDataTypePhysVal;
            }
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellValueProvider

//lint -e1762
//------------------------------------------------------------------------------
CDiagObjValueProvider* CDiagObjTable::getCellValueProvider( int i_idxRow, int i_idxClm )
//------------------------------------------------------------------------------
{
    CDiagObjValueProvider* pDiagObj = nullptr;

    if( i_idxRow >= 0 || i_idxRow < m_iRowCountMax
     || i_idxClm >= 0 || i_idxClm < m_iClmCountMax )
    {
        int idxCell = getCellIdx(i_idxRow,i_idxClm);
        pDiagObj = m_ararpDiagObjValueProvider[idxCell];
    }
    return pDiagObj;
}
//lint +e1762

//------------------------------------------------------------------------------
void CDiagObjTable::setCellValueFormatProvider(
    int                         i_idxRow,
    int                         i_idxClm,
    const SValueFormatProvider& i_valueFormat )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);

            if( m_ararpValueFormatCell[idxCell] != nullptr )
            {
                delete m_ararpValueFormatCell[idxCell];
                m_ararpValueFormatCell[idxCell] = nullptr;
            }
            m_ararpValueFormatCell[idxCell] = new SValueFormatProvider(i_valueFormat);
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellValueFormatProvider

//------------------------------------------------------------------------------
SValueFormatProvider CDiagObjTable::getCellValueFormatProvider( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat; // invalid format

    if( i_idxRow >= 0 || i_idxRow < m_iRowCountMax
     || i_idxClm >= 0 || i_idxClm < m_iClmCountMax )
    {
        int idxCell = getCellIdx(i_idxRow,i_idxClm);
        if( m_ararpValueFormatCell[idxCell] != nullptr )
        {
            valueFormat = *m_ararpValueFormatCell[idxCell];
        }
    }
    return valueFormat;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setCellStyle(
    int               i_idxRow,
    int               i_idxClm,
    const SCellStyle& i_cellStyle )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_ararcellStyleCell[idxCell] = i_cellStyle;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellStyle

//------------------------------------------------------------------------------
CDiagObjTable::SCellStyle CDiagObjTable::getCellStyle( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    SCellStyle cellStyle; // invalid format

    if( i_idxRow >= 0 || i_idxRow < m_iRowCountMax
     || i_idxClm >= 0 || i_idxClm < m_iClmCountMax )
    {
        int idxCell = getCellIdx(i_idxRow,i_idxClm);
        cellStyle = m_ararcellStyleCell[idxCell];
    }
    return cellStyle;
}

//------------------------------------------------------------------------------
void CDiagObjTable::setCellValueDataType( int i_idxRow, int i_idxClm, EDataType i_dataType )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellVal[idxCell] = i_dataType;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellValueDataType

//------------------------------------------------------------------------------
CDiagObjTable::EDataType CDiagObjTable::getCellValueDataType( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return EDataTypeUndefined;
    }
    int idxCell = getCellIdx(i_idxRow,i_idxClm);
    return m_arardataTypeCellVal[idxCell];

} // getCellValueDataType

//------------------------------------------------------------------------------
void CDiagObjTable::setCellDisplayDataType( int i_idxRow, int i_idxClm, EDataType i_dataType )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellDpl[idxCell] = i_dataType;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellDisplayDataType

//------------------------------------------------------------------------------
CDiagObjTable::EDataType CDiagObjTable::getCellDisplayDataType( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return EDataTypeUndefined;
    }
    int idxCell = getCellIdx(i_idxRow,i_idxClm);
    return m_arardataTypeCellDpl[idxCell];

} // getCellDisplayDataType

//------------------------------------------------------------------------------
void CDiagObjTable::setCellDataInvalid( int i_idxRow, int i_idxClm )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_ararvalidityCellData[idxCell] = EValueValidity::Invalid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellDataInvalid

//------------------------------------------------------------------------------
void CDiagObjTable::setCellDataValid( int i_idxRow, int i_idxClm )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellDataValid

//------------------------------------------------------------------------------
void CDiagObjTable::setCellData( int i_idxRow, int i_idxClm, const QString& i_str )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellVal[idxCell] = EDataTypeString;
            m_ararstrCellData[idxCell] = i_str;
            m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellData

//------------------------------------------------------------------------------
void CDiagObjTable::setCellData( int i_idxRow, int i_idxClm, const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellVal[idxCell] = EDataTypePixmap;

            delete m_ararpPxmCellData[idxCell];
            m_ararpPxmCellData[idxCell] = new QPixmap(i_pxm);
            m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellData

//------------------------------------------------------------------------------
void CDiagObjTable::setCellData( int i_idxRow, int i_idxClm, double i_fVal )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellVal[idxCell] = EDataTypePhysVal;

            if( m_ararpPhysValCellData[idxCell] == nullptr )
            {
                m_ararpPhysValCellData[idxCell] = new CPhysVal(i_fVal);
            }
            else
            {
                *m_ararpPhysValCellData[idxCell] = i_fVal;
            }
            m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellData

//------------------------------------------------------------------------------
void CDiagObjTable::setCellData( int i_idxRow, int i_idxClm, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellVal[idxCell] = EDataTypePhysUnit;

            if( m_ararpPhysValCellData[idxCell] == nullptr )
            {
                m_ararpPhysValCellData[idxCell] = new CPhysVal(i_unit);
            }
            else
            {
                m_ararpPhysValCellData[idxCell]->setUnit(i_unit);
            }
            m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellData

//------------------------------------------------------------------------------
void CDiagObjTable::setCellData(
    int             i_idxRow,
    int             i_idxClm,
    const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    if( i_idxRow < -1 || i_idxRow >= m_iRowCountMax
     || i_idxClm < -1 || i_idxClm >= m_iClmCountMax )
    {
        return;
    }

    int idxRowMin = i_idxRow;
    int idxRowMax = i_idxRow;
    int idxRow;
    int idxClmMin = i_idxClm;
    int idxClmMax = i_idxClm;
    int idxClm;
    int idxCell;

    if( i_idxRow == -1 )
    {
        idxRowMin = 0;
        idxRowMax = m_iRowCountMax-1;
    }
    if( i_idxClm == -1 )
    {
        idxClmMin = 0;
        idxClmMax = m_iClmCountMax-1;
    }
    for( idxRow = idxRowMin; idxRow <= idxRowMax; idxRow++ )
    {
        for( idxClm = idxClmMin; idxClm <= idxClmMax; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            m_arardataTypeCellVal[idxCell] = EDataTypePhysVal;

            if( m_ararpPhysValCellData[idxCell] == nullptr )
            {
                m_ararpPhysValCellData[idxCell] = new CPhysVal(i_physVal);
            }
            else
            {
                *m_ararpPhysValCellData[idxCell] = i_physVal;
            }
            m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
        }
    }
    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // setCellData

//------------------------------------------------------------------------------
EValueValidity CDiagObjTable::getCellDataValidity( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return EValueValidity::Invalid;
    }
    int idxCell = getCellIdx(i_idxRow,i_idxClm);
    return m_ararvalidityCellData[idxCell];

} // getCellDataValidity

//------------------------------------------------------------------------------
QString CDiagObjTable::getCellDataString( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return QString();
    }
    int idxCell = getCellIdx(i_idxRow,i_idxClm);
    return m_ararstrCellData[idxCell];

} // getCellDataString

//------------------------------------------------------------------------------
QPixmap CDiagObjTable::getCellDataPixmap( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    QPixmap pxm;

    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return pxm;
    }
    int idxCell = getCellIdx(i_idxRow,i_idxClm);
    if( m_ararpPxmCellData[idxCell] != nullptr )
    {
        return *m_ararpPxmCellData[idxCell];
    }
    return pxm;

} // getCellDataPixmap

//------------------------------------------------------------------------------
double CDiagObjTable::getCellDataValue( int i_idxRow, int i_idxClm, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return 0.0;
    }

    double fVal = 0.0;
    int    idxCell = getCellIdx(i_idxRow,i_idxClm);

    if( m_ararpPhysValCellData[idxCell] != nullptr )
    {
        if( i_pUnit != nullptr )
        {
            fVal = m_ararpPhysValCellData[idxCell]->getVal(i_pUnit);
        }
        else
        {
            fVal = m_ararpPhysValCellData[idxCell]->getVal();
        }
    }
    return fVal;

} // getCellDataValue

//------------------------------------------------------------------------------
CUnit CDiagObjTable::getCellDataUnit( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    CUnit unit;

    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return CUnit();
    }

    int idxCell = getCellIdx(i_idxRow,i_idxClm);

    if( m_ararpPhysValCellData[idxCell] != nullptr )
    {
        unit = m_ararpPhysValCellData[idxCell]->unit();
    }
    return unit;
}

//------------------------------------------------------------------------------
CPhysVal CDiagObjTable::getCellDataPhysVal( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal;

    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return physVal;
    }

    int idxCell = getCellIdx(i_idxRow,i_idxClm);

    if( m_ararpPhysValCellData[idxCell] != nullptr )
    {
        physVal = *m_ararpPhysValCellData[idxCell];
    }
    return physVal;

} // getCellDataPhysVal

/*==============================================================================
public: // overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
QSize CDiagObjTable::sizeHint()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "sizeHint",
        /* strAddInfo   */ "" );

    int cxWidth  = 0;
    int cyHeight = 0;

    if( isVisible() )
    {
        switch( m_layoutPos )
        {
            case ELayoutPosLeft:
            case ELayoutPosRight:
            {
                cxWidth = m_rectTable.width();
                break;
            }
            case ELayoutPosTop:
            case ELayoutPosBottom:
            {
                cyHeight = m_rectTable.height();
                break;
            }
            case ELayoutPosCenter:
            default:
            {
                break;
            }
        } // switch( m_layoutPos )

    } // if( isVisible() )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg  = "Size[w,h]=";
        strTrcMsg += QString::number(cxWidth) + ",";
        strTrcMsg += QString::number(cyHeight);
        mthTracer.setMethodReturn(strTrcMsg);
    }
    return QSize(cxWidth,cyHeight);

} // sizeHint

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjTable::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    CDiagObjTable* pDiagObj = new CDiagObjTable(
        /* strObjName   */ m_strObjName,
        /* iRowCountMax */ m_iRowCountMax,
        /* iClmCountMax */ m_iClmCountMax,
        /* layoutPos    */ m_layoutPos );

    CDiagObjValueProvider* pDiagObjValueProvider;

    int idxOrientation;
    int idxRow;
    int idxClm;
    int idxCell;

    // Members from base class CDiagObj:
    pDiagObj->m_rectContent = m_rectContent;
    pDiagObj->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObj->m_bVisible = m_bVisible;
    pDiagObj->m_state = m_state;
    pDiagObj->m_bIsFocusable = m_bIsFocusable;
    pDiagObj->m_bIsEditable = m_bIsEditable;

    // The following table and cell properties will be set:
    for( idxOrientation = 0; idxOrientation < EOrientationCount; idxOrientation++ )
    {
        pDiagObj->m_arbShowGridLines[idxOrientation] = m_arbShowGridLines[idxOrientation];
    }
    pDiagObj->m_colGridLines = m_colGridLines;
    pDiagObj->m_colText = m_colText;
    pDiagObj->m_fnt = m_fnt;
    pDiagObj->m_iIndentTop = m_iIndentTop;
    pDiagObj->m_iIndentBottom = m_iIndentBottom;
    pDiagObj->m_iIndentLeft = m_iIndentLeft;
    pDiagObj->m_iIndentRight = m_iIndentRight;
    if( m_pFrameStyle != nullptr )
    {
        pDiagObj->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
    }
    pDiagObj->m_iMarginTop = m_iMarginTop;
    pDiagObj->m_iMarginBottom = m_iMarginBottom;
    pDiagObj->m_iMarginLeft = m_iMarginLeft;
    pDiagObj->m_iMarginRight = m_iMarginRight;
    pDiagObj->m_iRowCount = m_iRowCount;
    pDiagObj->m_iClmCount = m_iClmCount;
    pDiagObj->m_cxClmIndent = m_cxClmIndent;

    for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
    {
        pDiagObj->m_ariClmAlignmentFlags[idxClm] = m_ariClmAlignmentFlags[idxClm];
        pDiagObj->m_arcxClmFixedWidth[idxClm] = m_arcxClmFixedWidth[idxClm];
        pDiagObj->m_arcxClmMinimumWidth[idxClm] = m_arcxClmMinimumWidth[idxClm];
        pDiagObj->m_arcxClmMaximumWidth[idxClm] = m_arcxClmMaximumWidth[idxClm];
        pDiagObj->m_arstrClmInvalidValue[idxClm] = m_arstrClmInvalidValue[idxClm];
        if( m_arpValueFormatClm[idxClm] != nullptr )
        {
            pDiagObj->m_arpValueFormatClm[idxClm] = new SValueFormatProvider(*m_arpValueFormatClm[idxClm]);
        }
        pDiagObj->m_arcellStyleClm[idxClm] = m_arcellStyleClm[idxClm];
        pDiagObj->m_ardataTypeClmVal[idxClm] = m_ardataTypeClmVal[idxClm];
        pDiagObj->m_ardataTypeClmDpl[idxClm] = m_ardataTypeClmDpl[idxClm];
    }

    // The following table and cell properties (dimensions and coordinates)
    // will be calculated within the update method:
    //pDiagObj->m_cyRowHeight = m_cyRowHeight;
    //pDiagObj->m_arcxClmWidth = m_arcxClmWidth;
    //pDiagObj->m_cyTableHeight = m_cyTableHeight;
    //pDiagObj->m_rectTable = m_rectTable;

    // Cell states and data
    for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
    {
        for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);

            pDiagObj->m_arariCellAlignmentFlags[idxCell] = m_arariCellAlignmentFlags[idxCell];

            if( m_ararpDiagObjValueProvider[idxCell] != nullptr )
            {
                pDiagObjValueProvider = m_ararpDiagObjValueProvider[idxCell];
                QString strClassName = pDiagObjValueProvider->ClassName();
                QString strObjName = pDiagObjValueProvider->getObjName();
                pDiagObjValueProvider = dynamic_cast<CDiagObjValueProvider*>(
                    i_pDiagramTrg->findDiagObj(strClassName, strObjName));
                pDiagObj->m_ararpDiagObjValueProvider[idxCell] = pDiagObjValueProvider;

                if( !QObject::connect(
                    /* pObjSender   */ pDiagObjValueProvider,
                    /* pcSignal     */ SIGNAL(valueChanged(ZS::Diagram::CDiagObj*)),
                    /* pObjReceiver */ pDiagObj,
                    /* pcMember     */ SLOT(cellValueChanged(ZS::Diagram::CDiagObj*)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
            if( m_ararpValueFormatCell[idxCell] != nullptr )
            {
                pDiagObj->m_ararpValueFormatCell[idxCell] = new SValueFormatProvider(*m_ararpValueFormatCell[idxCell]);
            }
            pDiagObj->m_ararcellStyleCell[idxCell] = m_ararcellStyleCell[idxCell];
            pDiagObj->m_arardataTypeCellVal[idxCell] = m_arardataTypeCellVal[idxCell];
            pDiagObj->m_arardataTypeCellDpl[idxCell] = m_arardataTypeCellDpl[idxCell];
            pDiagObj->m_ararvalidityCellData[idxCell] = m_ararvalidityCellData[idxCell];
            pDiagObj->m_ararstrCellData[idxCell] = m_ararstrCellData[idxCell];
            if( m_ararpPxmCellData[idxCell] != nullptr )
            {
                pDiagObj->m_ararpPxmCellData[idxCell] = new QPixmap(*m_ararpPxmCellData[idxCell]);
            }
            if( m_ararpPhysValCellData[idxCell] != nullptr )
            {
                pDiagObj->m_ararpPhysValCellData[idxCell] = new CPhysVal(*m_ararpPhysValCellData[idxCell]);
            }
        }
    }

    i_pDiagramTrg->addDiagObj(pDiagObj);

    return pDiagObj;

} // clone

//------------------------------------------------------------------------------
void CDiagObjTable::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone )
    {
        return;
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // If the width and height for the sizeHint method need to be recalculated ..
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        updateLayout();
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        updateData();
    }

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        updatePixmap(i_pPaintDevice);
    }

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget )
    {
        updateWidget();
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjTable::updateLayout()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( m_pDiagram == nullptr || !isVisible() )
    {
        return;
    }

    // As a matter of fact there is no sense in adding a table object to
    // a diagram just designed to analyze data.
    if( m_pDiagram->getUpdateType() < EDiagramUpdateTypePixmap )
    {
        return;
    }

    EDataType    dataTypeCellDpl;
    QFontMetrics fntmtr(m_fnt);
    QString      strCell = "Öy,²";
    QString      strCellTmp;
    QRect        rectCell = fntmtr.boundingRect(strCell);
    int          cxClmWidth;
    int          idxRow;
    int          idxClm;
    int          idxCell;

    // Before the column widths may be calculated the content of the
    // cells need to be updated. Thats been done by data processing.
    //-----------------------------------------------------------------

    // If any cell data has been changed ..
    if( m_uUpdateFlags & EUpdateData )
    {
        updateData();
    }

    // Calculate the column widths
    //-----------------------------------------------------------------

    m_cyRowHeight = rectCell.height();

    for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
    {
        m_arcxClmWidth[idxClm] = 0;

        for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);
            rectCell = QRect(); // invalid rectangle with width <= 0 and height <= 0

            dataTypeCellDpl = EDataTypeUndefined;
            if( m_arardataTypeCellDpl[idxCell] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_arardataTypeCellDpl[idxCell];
            }
            else if( m_ardataTypeClmDpl[idxClm] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_ardataTypeClmDpl[idxClm];
            }
            else if( m_arardataTypeCellVal[idxCell] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_arardataTypeCellVal[idxCell];
            }
            else if( m_ardataTypeClmVal[idxClm] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_ardataTypeClmVal[idxClm];
            }
            if( dataTypeCellDpl == EDataTypePixmap )
            {
                if( m_ararpPxmCellData[idxCell] != nullptr )
                {
                    rectCell = QRect( QPoint(0,0), m_ararpPxmCellData[idxCell]->size() );
                }
            }
            else
            {
                strCell = m_ararstrCellData[idxCell];
                rectCell = fntmtr.boundingRect(strCell);
            }
            cxClmWidth = 2*m_cxClmIndent + rectCell.width();
            if( cxClmWidth > m_arcxClmWidth[idxClm] )
            {
                m_arcxClmWidth[idxClm] = cxClmWidth;
            }
            if( m_arcxClmFixedWidth[idxClm] > 0 )
            {
                m_arcxClmWidth[idxClm] = m_arcxClmFixedWidth[idxClm];
            }
            else
            {
                if( m_arcxClmMinimumWidth[idxClm] > 0 && m_arcxClmWidth[idxClm] < m_arcxClmMinimumWidth[idxClm] )
                {
                    m_arcxClmWidth[idxClm] = m_arcxClmMinimumWidth[idxClm];
                }
                if( m_arcxClmMaximumWidth[idxClm] > 0 && m_arcxClmWidth[idxClm] > m_arcxClmMaximumWidth[idxClm] )
                {
                    m_arcxClmWidth[idxClm] = m_arcxClmMaximumWidth[idxClm];
                }
            }
        } // for( idxRow < RowCount )
    } // for( idxClm < ClmCount )

    // Calculate table width and height
    //---------------------------------

    m_rectTable.setHeight( m_iIndentTop + m_iIndentBottom + m_iMarginTop + m_iMarginBottom + m_iRowCount*m_cyRowHeight ); //lint !e713 !e737
    if( m_pFrameStyle != nullptr )
    {
        m_rectTable.setHeight( m_rectTable.height() + (2*m_pFrameStyle->getFrameWidth()) );
    }
    m_rectTable.setWidth( m_iIndentLeft + m_iIndentRight + m_iMarginLeft + m_iMarginRight );
    for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
    {
        m_rectTable.setWidth( m_rectTable.width() + m_arcxClmWidth[idxClm] );
    }
    if( m_pFrameStyle != nullptr )
    {
        m_rectTable.setWidth( m_rectTable.width() + (2*m_pFrameStyle->getFrameWidth()) );
    }

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    // On changing the size of the diagram or the content of the table the table
    // need to be updated on the screen.
    m_bUpdWidget = true;

    validate(EUpdateLayout);

} // updateLayout

//------------------------------------------------------------------------------
void CDiagObjTable::updateData()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateData",
        /* strAddInfo   */ "" );

    EDataType                   dataTypeCellDpl;
    const SValueFormatProvider* pValueFormat;
    int                         idxRow;
    int                         idxClm;
    int                         idxCell;

    // Convert values to be displayed into strings and store them within
    // "m_ararstrCellData" (for all data types).
    //-----------------------------------------------------------------

    for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
    {
        for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);

            dataTypeCellDpl = EDataTypeUndefined;
            if( m_arardataTypeCellDpl[idxCell] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_arardataTypeCellDpl[idxCell];
            }
            else if( m_ardataTypeClmDpl[idxClm] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_ardataTypeClmDpl[idxClm];
            }
            else if( m_arardataTypeCellVal[idxCell] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_arardataTypeCellVal[idxCell];
            }
            else if( m_ardataTypeClmVal[idxClm] != EDataTypeUndefined )
            {
                dataTypeCellDpl = m_ardataTypeClmVal[idxClm];
            }
            switch( dataTypeCellDpl )
            {
                case EDataTypePhysVal:
                {
                    if( m_ararpPhysValCellData[idxCell] == nullptr )
                    {
                        m_ararpPhysValCellData[idxCell] = new CPhysVal();
                    }
                    if( m_ararpValueFormatCell[idxCell] != nullptr )
                    {
                        pValueFormat = m_ararpValueFormatCell[idxCell];
                    }
                    else if( m_arpValueFormatClm[idxClm] != nullptr )
                    {
                        pValueFormat = m_arpValueFormatClm[idxClm];
                    }
                    else
                    {
                        pValueFormat = nullptr;
                    }
                    if( m_ararpDiagObjValueProvider[idxCell] != nullptr )
                    {
                        *m_ararpPhysValCellData[idxCell] = m_ararpDiagObjValueProvider[idxCell]->getVal();
                        m_ararvalidityCellData[idxCell] = EValueValidity::Valid;

                        if( pValueFormat == nullptr )
                        {
                            m_ararstrCellData[idxCell] = m_ararpDiagObjValueProvider[idxCell]->getValString();
                        }
                        else
                        {
                            m_ararstrCellData[idxCell] = m_ararpPhysValCellData[idxCell]->toString(*pValueFormat);
                        }
                    }
                    else
                    {
                        if( pValueFormat == nullptr )
                        {
                            m_ararstrCellData[idxCell] = m_ararpPhysValCellData[idxCell]->toString();
                        }
                        else
                        {
                            m_ararstrCellData[idxCell] = m_ararpPhysValCellData[idxCell]->toString(*pValueFormat);
                        }
                    }
                    break;
                }
                case EDataTypePhysUnit:
                {
                    if( m_ararpPhysValCellData[idxCell] == nullptr )
                    {
                        m_ararpPhysValCellData[idxCell] = new CPhysVal();
                    }
                    if( m_ararpDiagObjValueProvider[idxCell] != nullptr )
                    {
                        *m_ararpPhysValCellData[idxCell] = m_ararpDiagObjValueProvider[idxCell]->getVal();
                        m_ararvalidityCellData[idxCell] = EValueValidity::Valid;
                    }
                    if( m_ararvalidityCellData[idxCell] == EValueValidity::Invalid )
                    {
                        m_ararstrCellData[idxCell] = m_arstrClmInvalidValue[idxClm];
                    }
                    else
                    {
                        if( m_ararpPhysValCellData[idxCell]->unit().isValid() )
                        {
                            m_ararstrCellData[idxCell] = m_ararpPhysValCellData[idxCell]->unit().symbol();
                        }
                        else
                        {
                            m_ararstrCellData[idxCell] = "";
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            } // switch( dataTypeCell )
        } // for( idxClm < ClmCount )
    } // for( idxRow < RowCount )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    // On changing the size of the diagram or the content of the table the table
    // need to be updated on the screen.
    m_bUpdWidget = true;

    validate(EUpdateData);

} // updateData

//------------------------------------------------------------------------------
void CDiagObjTable::updatePixmap( QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    const CPixmapDiagram* pPixmapDiagram = nullptr;

    // As a matter of fact there is no sense in adding an axis label object to
    // a diagram just designed to analyze data.
    if( m_pDiagram != nullptr && m_pDiagram->getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        pPixmapDiagram = dynamic_cast<const CPixmapDiagram*>(m_pDiagram);
    }
    if( pPixmapDiagram == nullptr )
    {
        return;
    }

    if( isVisible() )
    {
        QPainter  painter(i_pPaintDevice);
        EDataType dataTypeCellDpl;
        QRect     rectTableFrame;
        QRect     rectRow;
        QRect     rectClm;
        QRect     rectClmPrev;
        QRect     rectClmNext;
        QRect     rectCell;
        QRect     rectCellPrev;
        QRect     rectCellNext;
        QString   strCell;
        QRect     rectText;
        int       iAlignmentFlags;
        int       idxRow;
        int       idxClm;
        int       idxCell;

        // During update data the table rect has been calculated. But this
        // was for the sizeHint method which the diagram called to calculate
        // the content rectangle of the diagram object. The table rectangle
        // may differ from the content rectangle if the extent should be
        // limited to the diagrams center part.
        m_rectTable = m_rectContent;

        if( m_bAdjustContentRect2DiagPartCenter )
        {
            switch( m_layoutPos )
            {
                case ELayoutPosTop:
                case ELayoutPosBottom:
                {
                    m_rectTable.setLeft(pPixmapDiagram->getRectPartCenter().left());
                    m_rectTable.setWidth(pPixmapDiagram->getRectPartCenter().width());
                    break;
                }
                case ELayoutPosLeft:
                case ELayoutPosRight:
                {
                    m_rectTable.setTop(pPixmapDiagram->getRectPartCenter().top());
                    m_rectTable.setHeight(pPixmapDiagram->getRectPartCenter().height());
                    break;
                }
                default:
                {
                    break;
                }
            } // switch( m_layoutPos )
        }

        rectTableFrame = m_rectTable;
        rectTableFrame.moveLeft( m_rectTable.left() + m_iIndentLeft );
        rectTableFrame.moveTop( m_rectTable.top() + m_iIndentTop );
        rectTableFrame.setWidth( m_rectTable.width() - m_iIndentLeft - m_iIndentRight ); //lint !e834
        rectTableFrame.setHeight( m_rectTable.height() - m_iIndentTop - m_iIndentTop ); //lint !e834

        // Horizontal grid lines
        //----------------------

        if( m_arbShowGridLines[EOrientationHorizontal] )
        {
            painter.setPen(m_colGridLines);

            rectRow.setLeft( rectTableFrame.left() + m_iMarginLeft );
            rectRow.setTop( rectTableFrame.top() + m_iMarginTop );
            rectRow.setWidth( rectTableFrame.width() - m_iMarginLeft - m_iMarginRight ); //lint !e834
            rectRow.setHeight( m_cyRowHeight );

            for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
            {
                painter.drawRect(rectRow);
                rectRow.moveTop( rectRow.top() + m_cyRowHeight );
            }
        }

        // Vertical grid lines
        //----------------------

        if( m_arbShowGridLines[EOrientationVertical] )
        {
            painter.setPen(m_colGridLines);

            rectClm.setLeft( rectTableFrame.left() + m_iMarginLeft );
            rectClm.setTop( rectTableFrame.top() + m_iMarginTop );
            rectClm.setHeight( rectTableFrame.height() - m_iMarginTop - m_iMarginBottom ); //lint !e834

            for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
            {
                rectClm.setWidth( m_arcxClmWidth[idxClm] );
                painter.drawRect(rectClm);
                rectClm.setLeft( rectClm.left() + m_arcxClmWidth[idxClm] );
            }
        }

        // Draw frame around the table
        //----------------------------

        if( m_pFrameStyle != nullptr )
        {
            m_pFrameStyle->draw( &painter, rectTableFrame );
        }
        else if( m_arbShowGridLines[EOrientationHorizontal] || m_arbShowGridLines[EOrientationVertical] )
        {
            painter.setPen(m_colGridLines);
            painter.drawRect(rectTableFrame);
        }

        // Content of the cells
        //----------------------

        // To avoid that strings will be cut at the right side of the cells first
        // the background of all cells will be drawn (erasing the background).
        // After the background of all cells has been drawn their content will be output.

        // First step: draw background of all cells
        rectRow.setLeft( rectTableFrame.left() + m_iMarginLeft );
        rectRow.setTop( rectTableFrame.top() + m_iMarginTop );
        rectRow.setWidth( rectTableFrame.width() - m_iMarginLeft - m_iMarginRight ); //lint !e834
        rectRow.setHeight( m_cyRowHeight );
        for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
        {
            rectClm.setLeft( rectTableFrame.left() + m_iMarginLeft );
            rectClm.setTop( rectTableFrame.top() + m_iMarginTop );
            rectClm.setHeight( rectTableFrame.height() - m_iMarginTop - m_iMarginBottom ); //lint !e834

            for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
            {
                idxCell = getCellIdx(idxRow,idxClm);

                rectClm.setWidth( m_arcxClmWidth[idxClm] );

                rectCell.setLeft( rectClm.left() );
                rectCell.setTop( rectRow.top() );
                rectCell.setWidth( rectClm.width() );
                rectCell.setHeight( rectRow.height() );

                if( m_ararcellStyleCell[idxCell].m_brushStyle != Qt::NoBrush )
                {
                    painter.fillRect( rectCell, QBrush(m_ararcellStyleCell[idxCell].m_colBg,m_ararcellStyleCell[idxCell].m_brushStyle) );
                }
                else if( m_arcellStyleClm[idxClm].m_brushStyle != Qt::NoBrush )
                {
                    painter.fillRect( rectCell, QBrush(m_arcellStyleClm[idxClm].m_colBg,m_arcellStyleClm[idxClm].m_brushStyle) );
                }
                rectClm.setLeft( rectClm.left() + m_arcxClmWidth[idxClm] );

            } // for( idxClm < ClmCount )

            rectRow.moveTop( rectRow.top() + m_cyRowHeight );

        } // for( idxRow < RowCount )

        // Second step: draw content (strings) of all cells
        painter.setFont(m_fnt);

        rectRow.setLeft( rectTableFrame.left() + m_iMarginLeft );
        rectRow.setTop( rectTableFrame.top() + m_iMarginTop );
        rectRow.setWidth( rectTableFrame.width() - m_iMarginLeft - m_iMarginRight ); //lint !e834
        rectRow.setHeight( m_cyRowHeight );
        for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
        {
            rectClmPrev.setWidth(0);
            rectClmPrev.setTop( rectTableFrame.top() + m_iMarginTop );
            rectClmPrev.setHeight( rectTableFrame.height() - m_iMarginTop - m_iMarginBottom );

            rectClm.setLeft( rectTableFrame.left() + m_iMarginLeft );
            rectClm.setTop( rectClmPrev.top() );
            rectClm.setHeight( rectClmPrev.height() );

            rectClmNext.setWidth(0);
            rectClmNext.setTop( rectClm.top() );
            rectClmNext.setHeight( rectClm.height() );

            for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
            {
                idxCell = getCellIdx(idxRow,idxClm);

                if( idxClm == 0 )
                {
                    rectClm.setWidth( m_arcxClmWidth[idxClm] );
                }
                else
                {
                    rectClmPrev = rectClm;
                    rectClm = rectClmNext;
                }
                if( idxClm < m_iClmCount-1 )
                {
                    rectClmNext.setLeft( rectClm.left() + m_arcxClmWidth[idxClm] );
                    rectClmNext.setWidth( m_arcxClmWidth[idxClm+1] );
                }

                rectCell.setLeft( rectClm.left() + m_cxClmIndent );
                rectCell.setTop( rectRow.top() );
                rectCell.setWidth( rectClm.width() - 2*m_cxClmIndent );
                rectCell.setHeight( rectRow.height() );

                dataTypeCellDpl = EDataTypeUndefined;
                if( m_arardataTypeCellDpl[idxCell] != EDataTypeUndefined )
                {
                    dataTypeCellDpl = m_arardataTypeCellDpl[idxCell];
                }
                else if( m_ardataTypeClmDpl[idxClm] != EDataTypeUndefined )
                {
                    dataTypeCellDpl = m_ardataTypeClmDpl[idxClm];
                }
                else if( m_arardataTypeCellVal[idxCell] != EDataTypeUndefined )
                {
                    dataTypeCellDpl = m_arardataTypeCellVal[idxCell];
                }
                else if( m_ardataTypeClmVal[idxClm] != EDataTypeUndefined )
                {
                    dataTypeCellDpl = m_ardataTypeClmVal[idxClm];
                }
                if( dataTypeCellDpl == EDataTypePixmap )
                {
                    if( m_ararpPxmCellData[idxCell] != nullptr )
                    {
                        painter.drawPixmap(rectCell,*m_ararpPxmCellData[idxCell]);
                    }
                }
                else
                {
                    if( m_ararcellStyleCell[idxCell].m_penStyle != Qt::NoPen )
                    {
                        painter.setPen(m_ararcellStyleCell[idxCell].m_penStyle);
                        painter.setPen(m_ararcellStyleCell[idxCell].m_colFg);
                    }
                    else if( m_arcellStyleClm[idxClm].m_penStyle != Qt::NoPen )
                    {
                        painter.setPen(m_arcellStyleClm[idxClm].m_penStyle);
                        painter.setPen(m_arcellStyleClm[idxClm].m_colFg);
                    }
                    else
                    {
                        painter.setPen(m_colText);
                    }
                    if( m_arariCellAlignmentFlags[idxCell] != 0 )
                    {
                        iAlignmentFlags = m_arariCellAlignmentFlags[idxCell];
                    }
                    else
                    {
                        iAlignmentFlags = m_ariClmAlignmentFlags[idxClm];
                    }
                    strCell = m_ararstrCellData[idxCell];
                    rectText = rectCell;

                    // If the text should be output horizontally centered ...
                    if( iAlignmentFlags & Qt::AlignHCenter )
                    {
                        // If the cells of the previous and next columns are empty ...
                        if( idxClm > 0 && idxClm < m_iClmCount-1 )
                        {
                            if( m_ararstrCellData[idxCell-1].isEmpty() && m_ararstrCellData[idxCell+1].isEmpty() )
                            {
                                // .. expand the cell rectangle to the left and to the right.
                                rectText.setLeft(rectClmPrev.left());
                                rectText.setRight(rectClmNext.right());
                            }
                        }
                    }
                    // If the text should be output left aligned ...
                    else if( iAlignmentFlags & Qt::AlignLeft )
                    {
                        // If the cell of the next column is empty ...
                        if( idxClm < m_iClmCount-1 )
                        {
                            if( m_ararstrCellData[idxCell+1].isEmpty() )
                            {
                                // .. expand the cell rectangle to the right.
                                rectText.setRight(rectClmNext.right());
                            }
                        }
                    }
                    // If the text should be output right aligned ...
                    else if( iAlignmentFlags & Qt::AlignRight )
                    {
                        // If the cell of the previous column is empty ...
                        if( idxClm > 0 )
                        {
                            if( m_ararstrCellData[idxCell-1].isEmpty() )
                            {
                                // .. expand the cell rectangle to the left.
                                rectText.setLeft(rectClmPrev.left());
                            }
                        }
                    }
                    painter.drawText(rectText,iAlignmentFlags,strCell);
                }

            } // for( idxClm < ClmCount )

            rectRow.moveTop( rectRow.top() + m_cyRowHeight );

        } // for( idxRow < RowCount )

    } // if( isVisible() )

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdatePixmap);

} // updatePixmap

//------------------------------------------------------------------------------
void CDiagObjTable::updateWidget()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateWidget",
        /* strAddInfo   */ "" );

    // Invalidate output region of the diagram object to update (repaint) content of diagram.
    if( m_rectContentPrev.isValid() && m_bUpdWidget )
    {
        m_pDiagram->update(this, m_rectContentPrev);
        m_rectContentPrev = m_rectContent;
    }
    if( m_rectContent.isValid() && m_bUpdWidget )
    {
        m_pDiagram->update(this, m_rectContent);
    }

    //// Invalidate output region of the diagram object to update (repaint) content of diagram.
    //if( m_rectTable.width() > 0 && m_rectTable.height() > 0 )
    //{
    //    pWdgtDiagram->update(this,m_rectTable);
    //}
    //if( !isVisible() )
    //{
    //    m_rectTable.setWidth(0);
    //    m_rectTable.setHeight(0);
    //}

    // On changing the size of the diagram or the content of the table the table need
    // to be updated on the screen. Updating the content rectangle of the label is
    // therefore only necessary if previously layout or data processing has been executed
    // or if the style of any cell has been changed.
    m_bUpdWidget = false;

    // Mark current process depth as executed (reset bit)
    //---------------------------------------------------

    validate(EUpdateWidget);

} // updateWidget

//------------------------------------------------------------------------------
int CDiagObjTable::getCellIdx( int i_idxRow, int i_idxClm ) const
//------------------------------------------------------------------------------
{
    if( i_idxRow < 0 || i_idxRow >= m_iRowCountMax
     || i_idxClm < 0 || i_idxClm >= m_iClmCountMax )
    {
        return 0;
    }

    int idxCell = i_idxRow*m_iClmCountMax + i_idxClm; //lint !e737

    return idxCell;

} // getCellIdx

/*==============================================================================
protected slots:
==============================================================================*/

//lint -esym(818,i_pDiagObjValSrc)
//------------------------------------------------------------------------------
void CDiagObjTable::clmValueChanged( CDiagObj* i_pDiagObjValSrc )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        if( i_pDiagObjValSrc != nullptr )
        {
            strTrcMsg = i_pDiagObjValSrc->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clmValueChanged",
        /* strAddInfo   */ strTrcMsg );

    // THIS FEATURE IS NOT YET SUPPORTED

    //int            idxClm;
    //int            physUnitClm;
    //CDiagObj*      pDiagObjClm;
    //EScaleDir      scaleDirClm;
    //EValueValidity validity;
    //double         fVal;

    //for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
    //{
    //    if( m_arpValueFormatClm[idxClm] != nullptr )
    //    {
    //    }
    //} // for( idxClm < iClmCount )

    invalidate(EUpdateLayoutDataPixmapWidget,true);

} // clmValueChanged
//lint +esym(818,i_pDiagObjValSrc)

//lint -esym(818,i_pDiagObjValSrc)
//------------------------------------------------------------------------------
void CDiagObjTable::cellValueChanged( CDiagObj* i_pDiagObjValSrc )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        if( i_pDiagObjValSrc != nullptr )
        {
            strTrcMsg = i_pDiagObjValSrc->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "cellValueChanged",
        /* strAddInfo   */ strTrcMsg );

    int          idxRow;
    int          idxClm;
    int          idxCell;
    unsigned int uUpdateFlags = EUpdateDataPixmapWidget;

    for( idxRow = 0; idxRow < m_iRowCount; idxRow++ )
    {
        for( idxClm = 0; idxClm < m_iClmCount; idxClm++ )
        {
            idxCell = getCellIdx(idxRow,idxClm);

            if( m_ararpDiagObjValueProvider[idxCell] == i_pDiagObjValSrc )
            {
                m_ararvalidityCellData[idxCell] = EValueValidity::Invalid;

                // If the column of the changed value does not have a fixed width ..
                if( m_arcxClmFixedWidth[idxClm] == 0 )
                {
                    // ... layout processing is necessary as the column width may need to be changed.
                    uUpdateFlags = EUpdateLayoutDataPixmapWidget;
                }
            }
        }
    }

    invalidate(uUpdateFlags,true);

} // cellValueChanged
//lint +esym(818,i_pDiagObjValSrc)
