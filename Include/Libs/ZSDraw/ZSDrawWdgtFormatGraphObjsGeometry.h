/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_WdgtFormatGraphObjsGeometry_h
#define ZSDraw_WdgtFormatGraphObjsGeometry_h

#include <QtCore/qglobal.h>

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjs.h"

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QFrame;
class QLabel;
class QStandardItem;
class QStandardItemModel;
class QTableView;
class QLayout;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CPushButton;
}
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtFormatGraphObjsGeometry : public CWdgtFormatGraphObjs
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsGeometry"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    enum EClmModelShapePoints
    {
        EClmX = 0,
        EClmY = 1,
        EClmCount
    };
public: // ctors and dtor
    CWdgtFormatGraphObjsGeometry(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsGeometry();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges() override;
    virtual void resetChanges() override;
    virtual bool hasChanges() const override;
protected: // must overridables of base class CWdgtFormatGraphObjs
    virtual void onGraphObjChanged() override;
protected: // instance methods
    void fillShapePointsModel();
protected slots:
    void onEdtPositionXValueChanged( double );
    void onEdtPositionYValueChanged( double );
    void onEdtSizeWidthValueChanged( double );
    void onEdtSizeWidthEditingFinished();
    void onEdtSizeHeightValueChanged( double );
    void onEdtSizeHeightEditingFinished();
    void onModelShapePointsItemChanged( QStandardItem* );
    void onBtnShapePointAddClicked();
    void onBtnShapePointDeleteClicked();
    void onBtnShapePointUpClicked();
    void onBtnShapePointDownClicked();
    void onEdtRotationAngleValueChanged( double );
    void onEdtZValueValueChanged( double );
    void onCmbPosLabelAnchorSelPtCurrentIndexChanged( int );
    void onChkPosLabelVisibleStateChanged( int );
    void onChkPosLabelAnchorLineVisibleStateChanged( int );
protected: // auxiliary methods
    void fillComboAnchorSelPt( QComboBox* i_pCmb );
private: // instance members
    QPointF                       m_ptPos;   // Values (pos, size, plg, angle, zValue) are used to  detect whether coordinates have been changed.
    QSizeF                        m_size;    // Changing the size of a polygon also changes the shape points of the polygon.
    QPolygonF                     m_plg;     // Changing the shape points of a polygon may also change the size of the object.
    double                        m_fRotAngle_deg;
    double                        m_fZValue;
    bool                          m_bIsChangingValue;
    QVBoxLayout*                  m_pLyt;
    QLabel*                       m_pLblHeadLine;
    QWidget*                      m_pWdgtPosSize;
    QVBoxLayout*                  m_pLytWdgtPosSize;
    QHBoxLayout*                  m_pLytHeadLineWdgtPosSize;
    QLabel*                       m_pLblHeadLinePosSize;
    QFrame*                       m_pSepHeadLinePosSize;
    QHBoxLayout*                  m_pLytLinePos;
    QLabel*                       m_pLblPos;
    QLabel*                       m_pLblPosX;
    QDoubleSpinBox*               m_pEdtPosX;
    QLabel*                       m_pLblPosY;
    QDoubleSpinBox*               m_pEdtPosY;
    QHBoxLayout*                  m_pLytLineSize;
    QLabel*                       m_pLblSize;
    QLabel*                       m_pLblSizeWidth;
    QDoubleSpinBox*               m_pEdtSizeWidth;
    QLabel*                       m_pLblSizeHeight;
    QDoubleSpinBox*               m_pEdtSizeHeight;
    QWidget*                      m_pWdgtShapePoints;
    QVBoxLayout*                  m_pLytWdgtShapePoints;
    QHBoxLayout*                  m_pLytHeadLineShapePoints;
    QLabel*                       m_pLblHeadLineShapePoints;
    QFrame*                       m_pSepHeadLineShapePoints;
    QHBoxLayout*                  m_pLytShapePoints;
    QStandardItemModel*           m_pModelShapePoints;
    QTableView*                   m_pViewShapePoints;
    QWidget*                      m_pWdgtShapePointButtons;
    QVBoxLayout*                  m_pLytWdgtShapePointButtons;
    ZS::System::GUI::CPushButton* m_pBtnShapePointAdd;
    ZS::System::GUI::CPushButton* m_pBtnShapePointDelete;
    ZS::System::GUI::CPushButton* m_pBtnShapePointUp;
    ZS::System::GUI::CPushButton* m_pBtnShapePointDown;
    bool                          m_bOnModelShapePointsItemChangedInProcess;
    QWidget*                      m_pWdgtRotation;
    QVBoxLayout*                  m_pLytWdgtRotation;
    QHBoxLayout*                  m_pLytHeadLineRotation;
    QLabel*                       m_pLblHeadLineRotation;
    QFrame*                       m_pSepHeadLineRotation;
    QHBoxLayout*                  m_pLytLineRotationAngle;
    QLabel*                       m_pLblRotationAngle;
    QDoubleSpinBox*               m_pEdtRotationAngle;
    QWidget*                      m_pWdgtStackingOrder;
    QVBoxLayout*                  m_pLytWdgtStackingOrder;
    QHBoxLayout*                  m_pLytHeadLineStackingOrder;
    QLabel*                       m_pLblHeadLineStackingOrder;
    QFrame*                       m_pSepHeadLineStackingOrder;
    QHBoxLayout*                  m_pLytLineZValue;
    QLabel*                       m_pLblZValue;
    QDoubleSpinBox*               m_pEdtZValue;
    QWidget*                      m_pWdgtLabelVisibilities;
    QVBoxLayout*                  m_pLytWdgtLabelVisibilities;
    QHBoxLayout*                  m_pLytHeadLineWdgtLabelVisibilities;
    QLabel*                       m_pLblHeadLineLabelVisibilities;
    QFrame*                       m_pSepHeadLineLabelVisibilities;
    QHBoxLayout*                  m_pLytLinePosLabelVisibilities;
    QLabel*                       m_pLblPosLabelAnchorSelPt;
    QComboBox*                    m_pCmbPosLabelAnchorSelPt;
    QLabel*                       m_pLblPosLabelVisible;
    QCheckBox*                    m_pChkPosLabelVisible;
    QLabel*                       m_pLblPosLabelAnchorLineVisible;
    QCheckBox*                    m_pChkPosLabelAnchorLineVisible;
    QHBoxLayout*                  m_pLytLineSizeWidthLabelVisible;
    QLabel*                       m_pLblSizeWidthLabelAnchorSelPt;
    QComboBox*                    m_pCmbSizeWidthLabelAnchorSelPt;
    QLabel*                       m_pLblSizeWidthLabelVisible;
    QCheckBox*                    m_pChkSizeWidthLabelVisible;
    QLabel*                       m_pLblSizeWidthLabelAnchorLineVisible;
    QCheckBox*                    m_pChkSizeWidthLabelAnchorLineVisible;
    QHBoxLayout*                  m_pLytLineSizeHeightLabelVisible;
    QLabel*                       m_pLblSizeHeightLabelAnchorSelPt;
    QComboBox*                    m_pCmbSizeHeightLabelAnchorSelPt;
    QLabel*                       m_pLblSizeHeightLabelVisible;
    QCheckBox*                    m_pChkSizeHeightLabelVisible;
    QLabel*                       m_pLblSizeHeightLabelAnchorLineVisible;
    QCheckBox*                    m_pChkSizeHeightLabelAnchorLineVisible;
    QHBoxLayout*                  m_pLytLineRotationAngleLabelVisible;
    QLabel*                       m_pLblRotationAngleLabelVisible;
    QComboBox*                    m_pCmbRotationAngleLabelAnchorSelPt;
    QLabel*                       m_pLblRotationAngleLabelAnchorLineVisible;
    QCheckBox*                    m_pChkRotationAngleLabelVisible;
    QLabel*                       m_pLblRotationAngleLabelAnchorSelPt;
    QCheckBox*                    m_pChkRotationAngleLabelAnchorLineVisible;
    QWidget*                      m_pWdgtDimensionLinesVisibilities;
    QVBoxLayout*                  m_pLytWdgtDimensionLinesVisibilities;
    QHBoxLayout*                  m_pLytHeadLineWdgtDimensionLinesVisibilities;
    QLabel*                       m_pLblHeadLineDimensionLinesVisibilities;
    QFrame*                       m_pSepHeadLineDimensionLinesVisibilities;
    QHBoxLayout*                  m_pLytLineDimensionLinesBoundingRectVisibile;
    QLabel*                       m_pLblDimensionLinesBoundingRectVisible;
    QCheckBox*                    m_pChkDimensionLinesBoundingRectVisible;
    QHBoxLayout*                  m_pLytLineDimensionLinesBoundingRectDiagonalsVisible;
    QLabel*                       m_pLblDimensionLinesBoundingRectDiagonalsVisible;
    QCheckBox*                    m_pChkDimensionLinesBoundingRectDiagonalsVisible;
    QHBoxLayout*                  m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible;
    QLabel*                       m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible;
    QCheckBox*                    m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible;
    QHBoxLayout*                  m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible;
    QLabel*                       m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible;
    QCheckBox*                    m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible;
    // Trace
    ZS::Trace::CTrcAdminObj*      m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsGeometry

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsGeometry_h
