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

#include "ZSDraw/Drawing/ZSDrawingSize.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

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

namespace ZS {
namespace System {
class CTrcAdminObj;

namespace GUI {
class CSepLine; // as often used forward declaration here in base widget
} } // System::GUI

namespace PhysVal {
class CPhysVal;
namespace GUI {
class CWdgtEditPhysVal;
} } // PhysVal::GUI

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
signals:
    /*! This signal is emitted if any property of the graphical object is changed
        by an edit control of the widget. */
    void contentChanged();
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
    virtual void fillEditControls();
    virtual void updateButtonsEnabled();
    virtual void applySettings();
protected slots: // overridables
    virtual void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize);
protected slots: // overridables
    virtual void onGraphObjSelectedChanged();
    virtual void onGraphObjGeometryChanged();
    //virtual void onGraphObjMoved();
    //virtual void onGraphObjRenamed();
    virtual void onGraphObjAboutToDestroyed();
private slots:
    //void onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree);
    //void onDrawingSceneGraphObjMoved(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch);
    //void onDrawingSceneGraphObjRenamed(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName);
    //void onDrawingSceneGraphObjAboutToBeDestroyed(const QString& i_strKeyInTree);
protected: // instance methods (tracing emitting signals)
    void emit_contentChanged();
protected: // instance members
    /*!< Pointer to drawing scene. */
    CDrawingScene* m_pDrawingScene;
    // Caching values
    CDrawingSize m_drawingSize;
    /*!< Unique key of the graphical to be edited. */
    QString m_strKeyInTree;
    /*!< If the unique key is set the drawing scene is queried to get the pointer to
         graphical object which should be edited. */
    CGraphObj* m_pGraphObj;
    /*!< Current graphical object type of the edited object. This member together with
         m_graphObjTypePrev may be used by derived classed to decide whether the current
         object type has also been changed when the object to be edited is changed.
         Not all of the edit controls need to be filled with new settings if the type does
         not change. E.g. the combo box with the different possible anchor types for labels
         don't need to refilled with new items if the type does not changed. */
    EGraphObjType m_graphObjTypeCurr;
    /*!< This member together with m_graphObjTypeCurr may be used by derived classed to
         decide whether the current object type has also been changed when the object to be
         edited is changed. After fillEditControls has been called the member
         m_graphObjTypePrev is set to m_graphObjTypeCurr. */
    EGraphObjType m_graphObjTypePrev;
    /*!< Flag to filter the indicated properties to the selected state of the graphical object.
         This flag is set to false as default. If the property widget has to react on
         "selectedChanged" signal the flag has to be set in the constructor of the derived class.
         If set to true the widget connects to the "selectedChanged" signal of the graphical object. */
    bool m_bContentUpdateOnSelectedChanged;
    /*!< Flag to filter the indicated properties to the geometry of the graphical object.
         This flag is set to false as default. If the property widget has to react on
         "selectedChanged" signal the flag has to be set in the constructor of the derived class.
         If set to true the widget connects to the "geometryChanged" signal of the graphical object. */
    bool m_bContentUpdateOnGeometryChanged;
    /*!< Flag to indicate that the content of an edit control has been changed while the "contentChanged"
         signal was blocked by the "contentChanged" counter. */
    bool m_bContentChanged;
    /*!< Counts how ofter the "contentChanged" signal has been blocked. A value greater than 0
         for the counter means that the signal "contentChanged" should not be emitted. Instead
         the flag m_bContentChanged should be set to true.
         If the counter is decremented and reaches 0 the flag "m_bContentChanged" is checked and
         the signal "contentChanged" is emitted if the flag is set. */
    int m_iContentChangedSignalBlockedCounter;
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
