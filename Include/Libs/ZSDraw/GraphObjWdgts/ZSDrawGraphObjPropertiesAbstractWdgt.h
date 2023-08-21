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

#ifndef ZSDraw_GraphObjPropertiesAbstractWdgt_h
#define ZSDraw_GraphObjPropertiesAbstractWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/Common/ZSDrawCommon.h"

class QCheckBox;
class QComboBox;
class QFontComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;
class QSettings;
class QSpinBox;
class QStandardItemModel;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CSepLine; // as often used forward declaration here in base widget
}
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjPropertiesAbstract : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObjPropertiesAbstract"; }
public: // ctors and dtor
    CWdgtGraphObjPropertiesAbstract(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strClassName,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjPropertiesAbstract();
protected: // ctor auxiliary methods
    QWidget* createButtonsLineWidget();
public: // overridables
    virtual QString nameSpace() const { return CWdgtGraphObjPropertiesAbstract::NameSpace(); }
    virtual QString className() const { return CWdgtGraphObjPropertiesAbstract::ClassName(); }
signals:
    void propertyChanged();
public: // overridables
    virtual void setKeyInTree(const QString& i_strKeyInTree);
    QString getKeyInTree() const;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    virtual bool hasErrors() const;
    virtual bool hasChanges() const;
    virtual void acceptChanges();
    virtual void rejectChanges();
protected slots: // overridables
    virtual void onBtnApplyClicked(bool i_bChecked = false);
    virtual void onBtnResetClicked(bool i_bChecked = false);
protected: // overridables
    virtual void updateButtonsEnabled();
protected: // overridables
    virtual void onGraphObjChanged();
    virtual void onGraphObjMoved();
    virtual void onGraphObjRenamed();
    virtual void onGraphObjAboutToDestroyed();
private slots:
    void onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree);
    void onDrawingSceneGraphObjMoved(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch);
    void onDrawingSceneGraphObjRenamed(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName);
    void onDrawingSceneGraphObjAboutToBeDestroyed(const QString& i_strKeyInTree);
protected: // instance methods (tracing emitting signals)
    void emit_propertyChanged();
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    QString m_strKeyInTree;
    CGraphObj* m_pGraphObj;
    int m_cxLblWidthClm1;
    int m_cxEdtWidthClm1;
    int m_cxLblWidthClm2;
    int m_cxEdtWidthClm2;
    int m_cxClmSpacing;
    /*!< Blocking signals counter. */
    int m_iValueChangedSignalsBlocked;
    // Edit Controls
    QVBoxLayout* m_pLyt;
    // Button Line
    QWidget* m_pWdgtButtons;
    QHBoxLayout* m_pLytWdgtButtons;
    QPushButton* m_pBtnApply;
    QPushButton* m_pBtnReset;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtGraphObjPropertiesAbstract

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjPropertiesAbstractWdgt_h
