/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges();
    virtual void resetChanges();
    virtual bool hasChanges() const;
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
private: // instance members
    QPointF                  m_ptPos;   // Values (pos, size, plg, angle, zValue) are used to  detect whether coordinates have been changed.
    QSizeF                   m_size;    // Changing the size of a polygon also changes the shape points of the polygon.
    QPolygonF                m_plg;     // Changing the shape points of a polygon may also change the size of the object.
    double                   m_fRotAngle_deg;
    double                   m_fZValue;
    bool                     m_bIsChangingValue;
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QWidget*                 m_pWdgtPosition;
    QVBoxLayout*             m_pLytWdgtPosition;
    QHBoxLayout*             m_pLytLinePosition;
    QLabel*                  m_pLblLinePosition;
    QFrame*                  m_pSepLinePosition;
    QHBoxLayout*             m_pLytPositionX;
    QLabel*                  m_pLblPositionX;
    QDoubleSpinBox*          m_pEdtPositionX;
    QHBoxLayout*             m_pLytPositionY;
    QLabel*                  m_pLblPositionY;
    QDoubleSpinBox*          m_pEdtPositionY;
    QWidget*                 m_pWdgtSize;
    QVBoxLayout*             m_pLytWdgtSize;
    QHBoxLayout*             m_pLytLineSize;
    QLabel*                  m_pLblLineSize;
    QFrame*                  m_pSepLineSize;
    QHBoxLayout*             m_pLytSizeWidth;
    QLabel*                  m_pLblSizeWidth;
    QDoubleSpinBox*          m_pEdtSizeWidth;
    QHBoxLayout*             m_pLytSizeHeight;
    QLabel*                  m_pLblSizeHeight;
    QDoubleSpinBox*          m_pEdtSizeHeight;
    QWidget*                 m_pWdgtShapePoints;
    QVBoxLayout*             m_pLytWdgtShapePoints;
    QHBoxLayout*             m_pLytLineShapePoints;
    QLabel*                  m_pLblLineShapePoints;
    QFrame*                  m_pSepLineShapePoints;
    QHBoxLayout*             m_pLytShapePoints;
    QStandardItemModel*      m_pModelShapePoints;
    QTableView*              m_pViewShapePoints;
    QWidget*                 m_pWdgtShapePointButtons;
    QVBoxLayout*             m_pLytWdgtShapePointButtons;
    ZS::System::GUI::CPushButton*    m_pBtnShapePointAdd;
    ZS::System::GUI::CPushButton*    m_pBtnShapePointDelete;
    ZS::System::GUI::CPushButton*    m_pBtnShapePointUp;
    ZS::System::GUI::CPushButton*    m_pBtnShapePointDown;
    bool                     m_bOnModelShapePointsItemChangedInProcess;
    QWidget*                 m_pWdgtRotation;
    QVBoxLayout*             m_pLytWdgtRotation;
    QHBoxLayout*             m_pLytLineRotation;
    QLabel*                  m_pLblLineRotation;
    QFrame*                  m_pSepLineRotation;
    QHBoxLayout*             m_pLytRotationAngle;
    QLabel*                  m_pLblRotationAngle;
    QDoubleSpinBox*          m_pEdtRotationAngle;
    QWidget*                 m_pWdgtZValue;
    QVBoxLayout*             m_pLytWdgtZValue;
    QHBoxLayout*             m_pLytLineZValue;
    QLabel*                  m_pLblLineZValue;
    QFrame*                  m_pSepLineZValue;
    QHBoxLayout*             m_pLytZValue;
    QLabel*                  m_pLblZValue;
    QDoubleSpinBox*          m_pEdtZValue;
    // Trace
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsGeometry

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsGeometry_h
