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

#ifndef ZSDraw_DrawGraphObjLineGeometryEditPropertyDlg_h
#define ZSDraw_DrawGraphObjLineGeometryEditPropertyDlg_h

#include "ZSDraw/Common/ZSDrawCommon.h"

#if QT_VERSION < 0x050000
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace PhysVal {
class CPhysVal;
namespace GUI {
class CWdgtEditPhysVal;
} } // PhysVal::GUI

namespace Draw
{
class CDrawingScene;
class CDrawingSize;
class CGraphObjLine;

//******************************************************************************
/*! @brief Dialog to edit a property of a line.

    On changing the value through the up and down buttons or by using the
    keyboards accelerator's Up and Down arrows or by pressing the enter key
    the value will be immediately set at the graphical object.
*/
class ZSDRAWDLL_API CDlgGraphObjLineGeometryEditProperty : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CDlgGraphObjLineGeometryEditProperty"; }
public: // ctors and dtor
    CDlgGraphObjLineGeometryEditProperty(
        CDrawingScene* i_pDrawingScene,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CDlgGraphObjLineGeometryEditProperty();
public: // instance methods
    void setCoordinate(CGraphObjLine* i_pGraphObj, const QString& i_strCoorSpec);
protected slots:
    void onEdtCoorValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize);
    void onGraphObjAboutToDestroyed();
protected: // class members
    /*!< Trace admin object for method tracing. */
    static ZS::System::CTrcAdminObj* s_pTrcAdminObj;
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    CGraphObjLine* m_pGraphObj;
    int m_iContentChangedSignalBlockedCounter;
    CEnumDrawingDimensionUnit m_eDimensionUnit;
    QString m_strCoorPart1;
    QString m_strCoorPart2;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytHeadLine;
    QLineEdit* m_pEdtPath;
    QHBoxLayout* m_pLytLineEdtVal;
    QLabel* m_pLblCoorPart1;
    QLabel* m_pLblCoorPart2;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtCoor;
    QHBoxLayout* m_pLytButtons;
    QPushButton* m_pBtnOk;

}; // class CDlgGraphObjLineGeometryEditProperty

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLineGeometryEditPropertyDlg_h
