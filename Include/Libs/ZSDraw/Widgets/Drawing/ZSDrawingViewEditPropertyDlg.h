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

#ifndef ZSDraw_DrawinvViewEditPropertyDlg_h
#define ZSDraw_DrawinvViewEditPropertyDlg_h

#include "ZSDraw/Common/ZSDrawCommon.h"

#if QT_VERSION < 0x050000
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System {
class CTrcAdminObj;
}
namespace PhysVal {
class CPhysVal;
}

namespace Draw
{
class CDrawingScene;
class CDrawingSize;
class CDrawingView;

//******************************************************************************
/*! @brief Dialog to edit a property of a line.

    On changing the value through the up and down buttons or by using the
    keyboards accelerator's Up and Down arrows or by pressing the enter key
    the value will be immediately set at the graphical object.
*/
class ZSDRAWDLL_API CDlgDrawingViewEditProperty : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CDlgDrawingViewEditProperty"; }
public: // ctors and dtor
    CDlgDrawingViewEditProperty(CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent = nullptr);
    virtual ~CDlgDrawingViewEditProperty();
public: // instance methods
    void setCoordinate(const QString& i_strCoorSpec);
protected slots: // instance methods
    void onBtnOkClicked(bool i_bChecked = false);
    void onBtnApplyClicked(bool i_bChecked = false);
    void onBtnResetClicked(bool i_bChecked = false);
    void onBtnCancelClicked(bool i_bChecked = false);
protected slots:
    void onDrawUnitsLengthResolutionChanged();
    void onDrawingViewDrawingSizeChanged(const CDrawingSize& i_drawingSize);
protected slots:
    void onEdtCoorValueChanged(double i_fValue);
protected: // auxiliary instance methods
    bool hasChanges() const;
    void acceptChanges();
    void updateButtonStates();
protected: // auxiliary instance methods
    void fillEditControls();
protected: // class members
    /*!< Trace admin object for method tracing. */
    static ZS::System::CTrcAdminObj* s_pTrcAdminObj;
protected: // instance members
    CDrawingView* m_pDrawingView;
    int m_iContentChangedSignalBlockedCounter;
    QString m_strCoorPart1;
    QString m_strCoorPart2;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytHeadLine;
    QLineEdit* m_pEdtPath;
    QHBoxLayout* m_pLytLineEdtVal;
    QLabel* m_pLblCoorPart1;
    QLabel* m_pLblCoorPart2;
    QDoubleSpinBox* m_pEdtCoor;
    QHBoxLayout* m_pLytLineBtns;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // class CDlgDrawingViewEditProperty

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawinvViewEditPropertyDlg_h
