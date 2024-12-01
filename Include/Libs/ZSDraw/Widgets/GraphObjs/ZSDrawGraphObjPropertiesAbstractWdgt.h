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

#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawPhysValRect.h"
#include "ZSDraw/Common/ZSDrawSettings.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
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
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strClassName,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjPropertiesAbstract();
protected: // ctor auxiliary methods
    QWidget* createButtonsLineWidget();
signals:
    /*! This signal is emitted if the indicated content has been changed. */
    void contentChanged();
public: // overridables
    virtual bool setKeyInTree(const QString& i_strKeyInTree);
    QString getKeyInTree() const;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    virtual bool hasErrors() const;
    virtual bool hasChanges() const;
    virtual void acceptChanges();
    virtual void rejectChanges();
    virtual void applySettings(bool i_bImmediatelyApplySettings = true);
protected slots: // overridables
    virtual void onBtnApplyClicked(bool i_bChecked = false);
    virtual void onBtnResetClicked(bool i_bChecked = false);
protected: // overridables
    virtual void fillEditControls();
    virtual void updateButtonsEnabled();
protected slots: // overridables
    virtual void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize);
    virtual void onDrawingSceneDrawSettingsChanged(const CDrawSettings& i_drawSettings);
protected slots: // overridables
    virtual void onGraphObjAboutToBeDestroyed(CGraphObj* i_pGraphObj);
    virtual void onGraphObjTypeChanged(CGraphObj* i_pGraphObj, EGraphObjType i_graphObjType);
    virtual void onGraphObjSelectedChanged(CGraphObj* i_pGraphObj, bool i_bIsSelected);
    virtual void onGraphObjGeometryOnSceneChanged(CGraphObj* i_pGraphObj, bool i_bParentOfParentChanged = false);
    virtual void onGraphObjDrawSettingsChanged(CGraphObj* i_pGraphObj);
    //virtual void onGraphObjMoved();
    //virtual void onGraphObjRenamed();
private slots:
    //void onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree);
    //void onDrawingSceneGraphObjMoved(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch);
    //void onDrawingSceneGraphObjRenamed(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName);
    //void onDrawingSceneGraphObjAboutToBeDestroyed(const QString& i_strKeyInTree);
protected: // instance methods (tracing emitting signals)
    void emit_contentChanged();
protected: // class members
    /*!< Class variable to store the current state of the labels widget.
         Used to restore the widget state after changing node types. */
    static bool s_bWdgtLabelsVisible;
    /*!< Class variable to store the current state of the geometry widget.
         Used to restore the widget state after changing node types. */
    static bool s_bWdgtGeometryVisible;
    /*!< Class variable to store the current state of the line style widget.
         Used to restore the widget state after changing node types. */
    static bool s_bWdgtLineStyleVisible;
    /*!< Class variable to store the current state of the fill style widget.
         Used to restore the widget state after changing node types. */
    static bool s_bWdgtFillStyleVisible;
    /*!< Class variable to store the current state of the text style widget.
         Used to restore the widget state after changing node types. */
    static bool s_bWdgtTextStyleVisible;
protected: // instance members
    /*!< Pointer to drawing scene. */
    CDrawingScene* m_pDrawingScene;
    /*!< Unique key of the graphical to be edited. Stored and set as key string to also
         support changing geometry of CDrawingView. */
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
         If set to true the widget connects to the "geometryOnSceneChanged" signal of the graphical object. */
    bool m_bContentUpdateOnGeometryChanged;
    /*!< Flag to filter the indicated properties to the draw settings of the graphical object.
         This flag is set to false as default. If the property widget has to react on
         "drawSettingsChanged" signal the flag has to be set in the constructor of the derived class.
         If set to true the widget connects to the "drawSettingsChanged" signal of the graphical object. */
    bool m_bContentUpdateOnDrawSettingsChanged;
    /*!< Flag to indicate that the content of an edit control has been changed while the "contentChanged"
         signal was blocked by the "contentChanged" counter. */
    bool m_bContentChanged;
    /*!< Counts how ofter the "contentChanged" signal has been blocked. A value greater than 0
         for the counter means that the signal "contentChanged" should not be emitted. Instead
         the flag m_bContentChanged should be set to true.
         If the counter is decremented and reaches 0 the flag "m_bContentChanged" is checked and
         the signal "contentChanged" is emitted if the flag is set. */
    int m_iContentChangedSignalBlockedCounter;
    /*!< Flag to indicate whether the widget is connected to the drawingSizeChanged signal of
         the drawing scene to avoid multiple connections or disconnections if not connected. */
    bool m_bDrawingSceneDrawingSizeChangedSignalConnected;
    /*!< Flag to indicate whether the widget is connected to the drawSettingsChanged signal of
         the drawing scene to avoid multiple connections or disconnections if not connected. */
    bool m_bDrawingSceneDrawSettingsChangedSignalConnected;
    /*!< Main Layout for the Edit Controls. */
    QVBoxLayout* m_pLyt;
    /*!<  As several property widgets may be combined in a parent widget (the GraphObjLine
          property widget contains property widgets to modify Labels, Geometry and LineStyle)
          those widgets should be organized in a grid layout with equal column widths.
          The columns should have the same spacing which is defined here. */
    int m_cxSpacing;
    /*!<  As several property widgets may be combined in a parent widget (the GraphObjLine
          property widget contains property widgets to modify Labels, Geometry and LineStyle)
          those widgets should be organized in a grid layout with equal column widths.
          The columns should have the same size or at least the columns should be aligned.
          For this the following member contains a list with predefined column widths which
          may be used by the derived property widgets.
          To apply the layout to the grid layout use the following code in the constructor
          of the derived classes:

            for (int idxClm = 0; idxClm < m_ariClmWidths.size(); ++idxClm) {
                m_pGridLayout->setColumnMinimumWidth(idxClm, m_ariClmWidths[idxClm]);
            }
            m_pGridLayout->setColumnStretch(m_ariClmWidths.size(), 1);

          Example of the grid Layout used by the TextStyle widget:
             |     0    |1| 2 |3|  4  |5| 6    |7| 8    |9|  10   | 
           --+----------+-+---+-+-----+-+------+-+------+-+-------+----
           0 |Font:     | |   | |Font          | |Size  | |Color  |<-->
           1 |Effects:  | |   | |Bold | |Kursiv| |UnderL| |StrikeT|<--> */
    QVector<int> m_ariClmWidths;
    /*!< Widget containing the buttons. */
    QWidget* m_pWdgtButtons;
    /*!< Layout for the widget containing the buttons. */
    QHBoxLayout* m_pLytWdgtButtons;
    /*!< Apply button. */
    QPushButton* m_pBtnApply;
    /*!< Reset button. */
    QPushButton* m_pBtnReset;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtGraphObjPropertiesAbstract

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjPropertiesAbstractWdgt_h
