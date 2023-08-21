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

#ifndef ZSDraw_WdgtFormatGraphObjsLabelVisibilities_h
#define ZSDraw_WdgtFormatGraphObjsLabelVisibilities_h

#include <QtCore/qglobal.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h"

class QCheckBox;
class QComboBox;
class QFrame;
class QLabel;
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
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtFormatGraphObjsLabelVisibilities : public CWdgtFormatGraphObjs
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsLabelVisibilities"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    enum EClmModelShapePoints
    {
        EClmX = 0,
        EClmY = 1,
        EClmCount
    };
public: // ctors and dtor
    CWdgtFormatGraphObjsLabelVisibilities(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj,
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsLabelVisibilities();
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
protected slots:
    void onCmbNameLabelAnchorSelPtCurrentIndexChanged( int );
    void onChkNameLabelVisibleStateChanged( int );
    void onChkNameLabelAnchorLineVisibleStateChanged( int );
    void onCmbDimensionPosLabelAnchorSelPtCurrentIndexChanged( int );
    void onChkDimensionPosLabelVisibleStateChanged( int );
    void onChkDimensionPosLabelAnchorLineVisibleStateChanged( int );
    void onCmbDimensionWidthLabelAnchorSelPtCurrentIndexChanged( int );
    void onChkDimensionWidthLabelVisibleStateChanged( int );
    void onChkDimensionWidthLabelAnchorLineVisibleStateChanged( int );
    void onCmbDimensionHeightLabelAnchorSelPtCurrentIndexChanged( int );
    void onChkDimensionHeightLabelVisibleStateChanged( int );
    void onChkDimensionHeightLabelAnchorLineVisibleStateChanged( int );
    void onCmbDimensionRotationAngleLabelAnchorSelPtCurrentIndexChanged( int );
    void onChkDimensionRotationAngleLabelVisibleStateChanged( int );
    void onChkDimensionRotationAngleLabelAnchorLineVisibleStateChanged( int );
    void onChkDimensionLinesBoundingRectVisibleStateChanged( int );
    void onChkDimensionLinesBoundingRectDiagonalsVisibleStateChanged( int );
    void onChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisibleStateChanged( int );
    void onChkDimensionLinesBoundingRectVerticalSymmetryAxisVisibleStateChanged( int );
protected: // auxiliary methods
    void fillComboAnchorSelPt( QComboBox* i_pCmb );
    void updateVisibilityCheckStates( QComboBox* i_pCmbSelPt, QCheckBox* i_pChkLabelVisible, QCheckBox* i_pChkLabelAnchorLineVisible );
private: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QWidget*                 m_pWdgtNameLabelVisibilities;
    QVBoxLayout*             m_pLytWdgtNameLabelVisibilities;
    QHBoxLayout*             m_pLytHeadLineWdgtNameLabelVisibilities;
    QLabel*                  m_pLblHeadLineNameLabelVisibilities;
    QFrame*                  m_pSepHeadLineNameLabelVisibilities;
    QHBoxLayout*             m_pLytLineNameLabelVisibilities;
    QLabel*                  m_pLblNameLabel;
    QComboBox*               m_pCmbNameLabelAnchorSelPt;
    QCheckBox*               m_pChkNameLabelVisible;
    QLabel*                  m_pLblNameLabelAnchorLineVisible;
    QCheckBox*               m_pChkNameLabelAnchorLineVisible;
    QWidget*                 m_pWdgtDimensionLabelVisibilities;
    QVBoxLayout*             m_pLytWdgtDimensionLabelVisibilities;
    QHBoxLayout*             m_pLytHeadLineWdgtDimensionLabelVisibilities;
    QLabel*                  m_pLblHeadLineDimensionLabelVisibilities;
    QFrame*                  m_pSepHeadLineDimensionLabelVisibilities;
    QHBoxLayout*             m_pLytLineDimensionPosLabelVisibilities;
    QLabel*                  m_pLblDimensionPosLabel;
    QComboBox*               m_pCmbDimensionPosLabelAnchorSelPt;
    QCheckBox*               m_pChkDimensionPosLabelVisible;
    QLabel*                  m_pLblDimensionPosLabelAnchorLineVisible;
    QCheckBox*               m_pChkDimensionPosLabelAnchorLineVisible;
    QHBoxLayout*             m_pLytLineDimensionWidthLabelVisibilities;
    QLabel*                  m_pLblDimensionWidthLabel;
    QComboBox*               m_pCmbDimensionWidthLabelAnchorSelPt;
    QCheckBox*               m_pChkDimensionWidthLabelVisible;
    QLabel*                  m_pLblDimensionWidthLabelAnchorLineVisible;
    QCheckBox*               m_pChkDimensionWidthLabelAnchorLineVisible;
    QHBoxLayout*             m_pLytLineDimensionHeightLabelVisibilities;
    QLabel*                  m_pLblDimensionHeightLabel;
    QComboBox*               m_pCmbDimensionHeightLabelAnchorSelPt;
    QCheckBox*               m_pChkDimensionHeightLabelVisible;
    QLabel*                  m_pLblDimensionHeightLabelAnchorLineVisible;
    QCheckBox*               m_pChkDimensionHeightLabelAnchorLineVisible;
    QHBoxLayout*             m_pLytLineDimensionRotationAngleLabelVisible;
    QLabel*                  m_pLblDimensionRotationAngleLabel;
    QComboBox*               m_pCmbDimensionRotationAngleLabelAnchorSelPt;
    QCheckBox*               m_pChkDimensionRotationAngleLabelVisible;
    QLabel*                  m_pLblDimensionRotationAngleLabelAnchorLineVisible;
    QCheckBox*               m_pChkDimensionRotationAngleLabelAnchorLineVisible;
    QWidget*                 m_pWdgtDimensionLinesVisibilities;
    QVBoxLayout*             m_pLytWdgtDimensionLinesVisibilities;
    QHBoxLayout*             m_pLytHeadLineWdgtDimensionLinesVisibilities;
    QLabel*                  m_pLblHeadLineDimensionLinesVisibilities;
    QFrame*                  m_pSepHeadLineDimensionLinesVisibilities;
    QHBoxLayout*             m_pLytLineDimensionLinesBoundingRectVisibile;
    QLabel*                  m_pLblDimensionLinesBoundingRectVisible;
    QCheckBox*               m_pChkDimensionLinesBoundingRectVisible;
    QHBoxLayout*             m_pLytLineDimensionLinesBoundingRectDiagonalsVisible;
    QLabel*                  m_pLblDimensionLinesBoundingRectDiagonalsVisible;
    QCheckBox*               m_pChkDimensionLinesBoundingRectDiagonalsVisible;
    QHBoxLayout*             m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible;
    QLabel*                  m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible;
    QCheckBox*               m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible;
    QHBoxLayout*             m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible;
    QLabel*                  m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible;
    QCheckBox*               m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible;
    // Trace
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsLabelVisibilities

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsLabelVisibilities_h
