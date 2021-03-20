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

#ifndef ZSDiagObjTable_h
#define ZSDiagObjTable_h

#include <QtGui/qcolor.h>
#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagObj.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QPixmap;

namespace ZS
{
namespace Diagram
{
class CDiagObjValueProvider;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjTable : public ZS::Diagram::CDiagObj
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    // Please note:
    // A cell may be of data type "ValueWithPhysUnit". But if the DisplayDataType
    // has been explicitely set to "Value" the cell indicates the value without
    // the unit string. This is useful if the units of the cell values should
    // be indicated in a separated column.
    typedef enum {
        EDataTypeMin      = 0,
        EDataTypeString   = 0,
        EDataTypePixmap   = 1,
        EDataTypePhysUnit = 2,
        EDataTypePhysVal  = 3,
        EDataTypeMax      = 3,
        EDataTypeCount,
        EDataTypeUndefined
    }   EDataType;
    struct SCellStyle
    {
    public: // ctors
        SCellStyle() :
            m_brushStyle(Qt::NoBrush),
            m_colBg(Qt::gray),
            m_penStyle(Qt::NoPen),
            m_colFg(Qt::black)
        {
        }
        SCellStyle(
            Qt::BrushStyle i_brushStyle,
            const QColor&  i_colBg,
            Qt::PenStyle   i_penStyle = Qt::NoPen,
            const QColor&  i_colFg = Qt::black ) :
            m_brushStyle(i_brushStyle),
            m_colBg(i_colBg),
            m_penStyle(i_penStyle),
            m_colFg(i_colFg)
        {
        }
    public: // instance members
        Qt::BrushStyle m_brushStyle;
        QColor         m_colBg;
        Qt::PenStyle   m_penStyle;
        QColor         m_colFg;
    };
public: // ctors and dtor
    CDiagObjTable(
        const QString& i_strObjName,
        int            i_iRowCountMax,
        int            i_iClmCountMax,
        ELayoutPos     i_layoutPos );
    virtual ~CDiagObjTable();
public: // instance methods
    void showGridLines( EOrientation i_orientation = EOrientationCount );
    void hideGridLines( EOrientation i_orientation = EOrientationCount );
    void setGridLinesColor( const QColor& i_col );
    QColor getGridLinesColor() const;
    void setTextColor( const QColor& i_col );
    QColor getTextColor() const;
    void setFont( const QFont& i_fnt );
    QFont getFont() const;
    void setIndentTop( int i_iIndent );
    int getIndentTop() const;
    void setIndentBottom( int i_iIndent );
    int getIndentBottom() const;
    void setIndentLeft( int i_iIndent );
    int getIndentLeft() const;
    void setIndentRight( int i_iIndent );
    int getIndentRight() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    void setRowCount( int i_iRowCount );
    int getRowCount() const;
    void setClmCount( int i_iClmCount );
    int getClmCount() const;
    void setClmIndent( int i_cxClmIndent );
    int getClmIndent() const;
    // Please note that for the following "set" methods an index value
    // of -1 means either all columns, all rows, or all cells.
    void setClmAlignmentFlags( int i_idxClm, int i_iAlignmentFlags );
    int getClmAlignmentFlags( int i_idxClm ) const;
    void setClmFixedWidth( int i_idxClm, int i_cxWidth );
    int getClmFixedWidth( int i_idxClm ) const;
    void setClmMinimumWidth( int i_idxClm, int i_cxWidth );
    int getClmMinimumWidth( int i_idxClm ) const;
    void setClmMaximumWidth( int i_idxClm, int i_cxWidth );
    int getClmMaximumWidth( int i_idxClm ) const;
    void setClmInvalidValueStr( int i_idxClm, const QString& i_str );
    QString getClmInvalidValueStr( int i_idxClm ) const;
    void setClmValueFormatProvider( int i_idxClm, const PhysVal::SValueFormatProvider& i_valueFormat );
    PhysVal::SValueFormatProvider getClmValueFormatProvider( int i_idxClm ) const;
    void setClmCellStyle( int i_idxClm, const SCellStyle& i_cellStyle );
    SCellStyle getClmCellStyle( int i_idxClm ) const;
    void setClmValueDataType( int i_idxClm, EDataType i_dataType );
    EDataType getClmValueDataType( int i_idxClm ) const;
    void setClmDisplayDataType( int i_idxClm, EDataType i_dataType );
    EDataType getClmDisplayDataType( int i_idxClm ) const;
    void setCellAlignmentFlags( int i_idxRow, int i_idxClm, int i_iAlignmentFlags );
    int getCellAlignmentFlags( int i_idxRow, int i_idxClm ) const;
    void setCellValueProvider( int i_idxRow, int i_idxClm, CDiagObjValueProvider* i_pDiagObjValueProvider );
    CDiagObjValueProvider* getCellValueProvider( int i_idxRow, int i_idxClm );
    void setCellValueFormatProvider( int i_idxRow, int i_idxClm, const PhysVal::SValueFormatProvider& i_valueFormat );
    PhysVal::SValueFormatProvider getCellValueFormatProvider( int i_idxRow, int i_idxClm ) const;
    void setCellStyle( int i_idxRow, int i_idxClm, const SCellStyle& i_cellStyle );
    SCellStyle getCellStyle( int i_idxRow, int i_idxClm ) const;
    void setCellValueDataType( int i_idxRow, int i_idxClm, EDataType i_dataType );
    EDataType getCellValueDataType( int i_idxRow, int i_idxClm ) const;
    void setCellDisplayDataType( int i_idxRow, int i_idxClm, EDataType i_dataType );
    EDataType getCellDisplayDataType( int i_idxRow, int i_idxClm ) const;
    // Please note that the following "setData" methods implicitly are setting
    // the value and display data types correspondingly (if not already set).
    void setCellDataInvalid( int i_idxRow, int i_idxClm );
    void setCellDataValid( int i_idxRow, int i_idxClm );
    void setCellData( int i_idxRow, int i_idxClm, const QString& i_str );
    void setCellData( int i_idxRow, int i_idxClm, const QPixmap& i_pxm );
    void setCellData( int i_idxRow, int i_idxClm, double i_fVal );
    void setCellData( int i_idxRow, int i_idxClm, PhysVal::CUnit* i_pUnit );
    void setCellData( int i_idxRow, int i_idxClm, const PhysVal::CPhysVal& i_physVal );
    ZS::System::EValueValidity getCellDataValidity( int i_idxRow, int i_idxClm ) const;
    QString getCellDataString( int i_idxRow, int i_idxClm ) const;
    QPixmap getCellDataPixmap( int i_idxRow, int i_idxClm ) const;
    double getCellDataValue( int i_idxRow, int i_idxClm, PhysVal::CUnit* i_pUnit = nullptr ) const;
    PhysVal::CUnit* getCellDataUnit( int i_idxRow, int i_idxClm ) const;
    PhysVal::CPhysVal getCellDataPhysVal( int i_idxRow, int i_idxClm ) const;
public: // overridables of base class CDiagObj
    virtual QSize sizeHint();
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected: // instance methods
    void updateLayout();
    void updateData();
    void updatePixmap( QPaintDevice* i_pPaintDevice );
    void updateWidget();
    int getCellIdx( int i_idxRow, int i_idxClm ) const;
protected slots:
    void clmValueChanged( ZS::Diagram::CDiagObj* i_pDiagObjValSrc );
    void cellValueChanged( ZS::Diagram::CDiagObj* i_pDiagObjValSrc );
private: // copy ctor not allowed
    CDiagObjTable( const CDiagObjTable& );
private: // assignment operator not allowed
    void operator=( const CDiagObjTable& );
protected:  // instance members
    // The following table and cell properties will be set:
    bool                             m_arbShowGridLines[EOrientationCount];
    QColor                           m_colGridLines;
    QColor                           m_colText;
    QFont                            m_fnt;
    int                              m_iIndentTop;    // Indents are the space between
    int                              m_iIndentBottom; // the content rect and the table frame.
    int                              m_iIndentLeft;
    int                              m_iIndentRight;
    SFrameStyle*                     m_pFrameStyle;   // Frame around the table.
    int                              m_iMarginTop;    // Margins are the space between
    int                              m_iMarginBottom; // the table frame and the table cells
    int                              m_iMarginLeft;
    int                              m_iMarginRight;
    int                              m_iRowCountMax;
    int                              m_iClmCountMax;
    int                              m_iRowCount;
    int                              m_iClmCount;
    int                              m_cxClmIndent;
    int*                             m_ariClmAlignmentFlags;
    int*                             m_arcxClmFixedWidth;
    int*                             m_arcxClmMinimumWidth;
    int*                             m_arcxClmMaximumWidth;
    QString*                         m_arstrClmInvalidValue;
    PhysVal::SValueFormatProvider**  m_arpValueFormatClm;
    SCellStyle*                      m_arcellStyleClm;
    EDataType*                       m_ardataTypeClmVal;  // Values may be internally stored with their physical units ..
    EDataType*                       m_ardataTypeClmDpl;  // but may be indicated without the unit strings.
    // The following table and cell properties (dimensions and coordinates)
    // will be calculated within the update method:
    int                              m_cyRowHeight;
    int*                             m_arcxClmWidth;
    int                              m_cyTableHeight;
    QRect                            m_rectTable; // including indents, frame, and margin (but may differ from "m_rectContent")
    // Cell states and data
    int*                             m_arariCellAlignmentFlags;
    CDiagObjValueProvider**          m_ararpDiagObjValueProvider;
    PhysVal::SValueFormatProvider**  m_ararpValueFormatCell;
    SCellStyle*                      m_ararcellStyleCell;
    EDataType*                       m_arardataTypeCellVal; // Values may be internally stored with their physical units ...
    EDataType*                       m_arardataTypeCellDpl; // but may be indicated without the unit strings.
    ZS::System::EValueValidity*                  m_ararvalidityCellData;
    QString*                         m_ararstrCellData;
    QPixmap**                        m_ararpPxmCellData;
    PhysVal::CPhysVal**              m_ararpPhysValCellData;
    // Flag to indicate that the table content has been changed.
    bool                             m_bUpdWidget;

}; // class CDiagObjTable

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjTable_h
