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

#ifndef ZSApps_Draw_MainWindow_h
#define ZSApps_Draw_MainWindow_h

#include "ZSSys/ZSSysApp.h"

#if QT_VERSION < 0x050000
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

class QAction;
class QDockWidget;
class QDoubleSpinBox;
class QGraphicsItem;
class QLabel;
class QModelIndex;
class QPushButton;
class QSpinBox;
class QTreeView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
struct SErrResultInfo;
class CTrcAdminObj;

namespace GUI
{
class CDlgErrLog;
}
}

namespace Trace
{
namespace GUI
{
class CDlgTrcServer;
}
}

namespace Test
{
class CTest;

namespace GUI
{
class CDlgTest;
}
}

namespace Draw
{
class CDrawGridSettings;
class CDrawingScene;
class CDrawingSize;
class CDrawingView;
class CDrawSettings;
class CGraphObj;
class CObjFactoriesModel;
class CObjFactory;
class CObjFactoryEllipse;
class CObjFactoryImage;
class CObjFactoryLine;
class CObjFactoryPoint;
class CObjFactoryPolygon;
class CObjFactoryPolyline;
class CObjFactoryRect;
class CObjFactoryText;
class CObjFactoryConnectionPoint;
class CObjFactoryConnectionLine;
class CObjFactoryGroup;
class CWdgtGraphObjs;

namespace QtWidgets
{
class CObjFactoryWdgtCheckBox;
class CObjFactoryWdgtComboBox;
class CObjFactoryWdgtGroupBox;
class CObjFactoryWdgtLabel;
class CObjFactoryWdgtLineEdit;
class CObjFactoryWdgtPushButton;

} // namespace QtWidgets

namespace Electricity
{
class CObjFactoryCapacitor;
class CObjFactoryDiode;
class CObjFactoryInductor;
class CObjFactoryResistor;
class CObjFactorySwitch;
class CObjFactoryTransistor;
class CObjFactoryVoltageSource;

} // namespace Electricity

} // namespace Draw

namespace Apps
{
namespace Products
{
namespace Draw
{
class CWidgetCentral;

//******************************************************************************
class CMainWindow : public QMainWindow
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "Apps::Products::Draw"; }
    static QString ClassName() { return "CMainWindow"; }
public: // class methods
    static CMainWindow* GetInstance(); // singleton class
public: // class members
    static const QString c_strObjFactoryQtWidgets;
    static const QString c_strObjFactoryElectricity;
public: // class members
    static const QString c_strMenuNameFile;
    static const QString c_strMenuNameFileOpenLastUsed;
    static const QString c_strMenuNameMode;
    static const QString c_strMenuNameEdit;
    static const QString c_strMenuNameEditRotate;
    static const QString c_strMenuNameEditMirror;
    static const QString c_strMenuNameDraw;
    static const QString c_strMenuNameDrawSettings;
    static const QString c_strMenuNameDrawStandardShapes;
    static const QString c_strMenuNameDrawGraphics;
    static const QString c_strMenuNameDrawConnections;
    static const QString c_strMenuNameDrawWidgets;
    static const QString c_strMenuNameDrawElectricity;
    static const QString c_strMenuNameView;
    static const QString c_strMenuNameDebug;
    static const QString c_strMenuNameInfo;
    static const QString c_strActionNameFileNew;
    static const QString c_strActionNameFileOpen;
    static const QString c_strActionNameFileSave;
    static const QString c_strActionNameFileSaveAs;
    //static const QString c_strActionNameFileOpenLastUsed;
    static const QString c_strActionNameFilePageSetup;
    static const QString c_strActionNameFileQuit;
    static const QString c_strActionNameModeEdit;
    static const QString c_strActionNameModeView;
    static const QString c_strActionNameEditSelect;
    static const QString c_strActionNameEditRotateLeft;
    static const QString c_strActionNameEditRotateRight;
    static const QString c_strActionNameEditMirrorVertical;
    static const QString c_strActionNameEditMirrorHorizontal;
    static const QString c_strActionNameEditGroup;
    static const QString c_strActionNameEditUngroup;
    static const QString c_strActionNameDrawSettingsLine;
    static const QString c_strActionNameDrawSettingsFill;
    static const QString c_strActionNameDrawSettingsText;
    static const QString c_strActionNameDrawStandardShapePoint;
    static const QString c_strActionNameDrawStandardShapeLine;
    static const QString c_strActionNameDrawStandardShapeRect;
    static const QString c_strActionNameDrawStandardShapeEllipse;
    static const QString c_strActionNameDrawStandardShapePolyline;
    static const QString c_strActionNameDrawStandardShapePolygon;
    static const QString c_strActionNameDrawStandardShapeText;
    static const QString c_strActionNameDrawGraphicsImage;
    static const QString c_strActionNameDrawConnectionPoint;
    static const QString c_strActionNameDrawConnectionLine;
    static const QString c_strActionNameDrawWdgtCheckBox;
    static const QString c_strActionNameDrawWdgtComboBox;
    static const QString c_strActionNameDrawWdgtGroupBox;
    static const QString c_strActionNameDrawWdgtLabel;
    static const QString c_strActionNameDrawWdgtLineEdit;
    static const QString c_strActionNameDrawWdgtPushButton;
    static const QString c_strActionNameDrawElectricityResistor;
    static const QString c_strActionNameDrawElectricityCapacitor;
    static const QString c_strActionNameDrawElectricityInductor;
    static const QString c_strActionNameDrawElectricitySwitch;
    static const QString c_strActionNameDrawElectricityTransistor;
    static const QString c_strActionNameViewZoomIn;
    static const QString c_strActionNameViewZoomOut;
    static const QString c_strActionNameViewZoom;
    static const QString c_strActionNameDebugErrLog;
    static const QString c_strActionNameDebugUnits;
    static const QString c_strActionNameDebugTraceServer;
    static const QString c_strActionNameDebugTraceAdminObjs;
    static const QString c_strActionNameInfoVersion;
public: // ctors and dtor
    CMainWindow(
        const QString&     i_strWindowTitleAppName,
        ZS::Test::CTest*   i_pTest,
        const QStringList& i_strlstObjFactories,
        QWidget*           i_pWdgtParent = nullptr,
        Qt::WindowFlags    i_wflags = Qt::WindowFlags());
    virtual ~CMainWindow();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
public: // instance methods
    QStringList getAddedObjFactories() const { return m_strlstObjFactories; }
protected: // instance methods
    void updateWindowTitle();
protected: // instance methods (for ctor)
    void createObjFactories();
    void createActions();
    void createMenus();
    void createStatusBar();
    void createToolBars();
    void createDockWidgets();
protected: // overridables of base class QWidget
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
public: // instance methods
    void setCurrentUsedFile( const QString& i_strAbsFilePath );
public: // instance methods
    void setCheckedActionModeEdit(bool i_bChecked);
    void setCheckedActionModeView(bool i_bChecked);
public: // instance methods
    void setCheckedActionEditSelect(bool i_bChecked);
    void triggerActionEditRotateLeft();
    void triggerActionEditRotateRight();
    void triggerActionEditMirrorVertical();
    void triggerActionEditMirrorHorizontal();
    void triggerActionEditGroup();
    void triggerActionEditUngroup();
public: // instance methods
    void setCheckedActionDrawStandardShapePoint(bool i_bChecked);
    void setCheckedActionDrawStandardShapeLine(bool i_bChecked);
    void setCheckedActionDrawStandardShapeRect(bool i_bChecked);
    void setCheckedActionDrawStandardShapeEllipse(bool i_bChecked);
    void setCheckedActionDrawStandardShapePolyline(bool i_bChecked);
    void setCheckedActionDrawStandardShapePolygon(bool i_bChecked);
    void setCheckedActionDrawStandardShapeText(bool i_bChecked);
    void triggerActionDrawGraphicsImage();
    void setCheckedActionDrawConnectionPoint(bool i_bChecked);
    void setCheckedActionDrawConnectionLine(bool i_bChecked);
public slots: // Menu - File
    void onActionFileNewTriggered(bool i_bChecked = false);
    void onActionFileOpenTriggered(bool i_bChecked = false);
    void onActionFileSaveTriggered(bool i_bChecked = false);
    void onActionFileSaveAsTriggered(bool i_bChecked = false);
    void onActionFilePageSetupTriggered(bool i_bChecked = false);
    void onActionFileLastUsedFilesTriggered(bool i_bChecked = false);
    void onActionFileQuitTriggered(bool i_bChecked = false);
public slots: // Menu - Mode
    void onActionModeEditToggled(bool i_bChecked = false);
    void onActionModeViewToggled(bool i_bChecked = false);
public slots: // Menu - Edit - Select/RotateFree
    void onActionEditSelectToggled(bool i_bChecked = false);
public slots: // Menu - Edit - Rotate
    void onActionEditRotateLeftTriggered(bool i_bChecked = false);
    void onActionEditRotateRightTriggered(bool i_bChecked = false);
public slots: // Menu - Edit - Mirror
    void onActionEditMirrorVerticalTriggered(bool i_bChecked = false);
    void onActionEditMirrorHorizontalTriggered(bool i_bChecked = false);
public slots: // Menu - Edit - Group
    void onActionEditGroupTriggered(bool i_bChecked = false);
    void onActionEditUngroupTriggered(bool i_bChecked = false);
public slots: // Menu - Draw - Settings
    void onActionDrawSettingsLineTriggered(bool i_bChecked = false);
    void onActionDrawSettingsFillTriggered(bool i_bChecked = false);
    void onActionDrawSettingsTextTriggered(bool i_bChecked = false);
public slots: // Menu - Draw - Standard Shapes
    void onActionDrawStandardShapePointToggled(bool i_bChecked = false);
    void onActionDrawStandardShapeLineToggled(bool i_bChecked = false);
    void onActionDrawStandardShapeRectToggled(bool i_bChecked = false);
    void onActionDrawStandardShapeEllipseToggled(bool i_bChecked = false);
    void onActionDrawStandardShapePolylineToggled(bool i_bChecked = false);
    void onActionDrawStandardShapePolygonToggled(bool i_bChecked = false);
    void onActionDrawStandardShapeTextToggled(bool i_bChecked = false);
public slots: // Menu - Draw - Graphics
    void onActionDrawGraphicsImageTriggered(bool i_bChecked = false);
public slots: // Menu - Draw - Connections
    void onActionDrawConnectionPointToggled(bool i_bChecked = false);
    void onActionDrawConnectionLineToggled(bool i_bChecked = false);
public slots: // Menu - View - Zoom
    void onActionViewZoomInTriggered(bool i_bChecked = false);
    void onActionViewZoomOutTriggered(bool i_bChecked = false);
    void onEdtViewZoomFactorEditingFinished();
public slots: // Menu - Trace
    void onActionDebugErrLogTriggered(bool i_bChecked = false);
    void onActionDebugUnitsTriggered(bool i_bChecked = false);
    void onActionDebugTraceServerTriggered(bool i_bChecked = false);
    void onActionDebugTraceAdminObjsTriggered(bool i_bChecked = false);
    void onActionDebugTestTriggered(bool i_bChecked = false);
public slots: // Drawing Scene
    void onDrawingSceneSizeChanged( const ZS::Draw::CDrawingSize& i_drawingSize);
    void onDrawingSceneGridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_gridSettings );
    void onDrawingSceneChanged( const QList<QRectF>& i_region );
    void onDrawingSceneFocusItemChanged( QGraphicsItem* i_pNewFocusItem, QGraphicsItem* i_pOldFocusItem, Qt::FocusReason reason );
    void onDrawingSceneSelectionChanged();
    void onDrawingSceneMousePosChanged( const QPointF& i_ptMousePos );
    void onDrawingSceneModeChanged();
    void onDrawingSceneDrawSettingsChanged( const ZS::Draw::CDrawSettings& i_drawSettings );
protected slots: // Drawing View
    void onDrawingViewMousePosChanged( const QPointF& i_ptMousePos );
protected slots: // Tree View Object Factories (ToolBox)
    void onTreeViewObjFactoriesExpanded( const QModelIndex& i_modelIdx );
    void onTreeViewObjFactoriesCurrentChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
public: // instance methods
    void selectTreeViewObjFactoryNode( ZS::Draw::CObjFactory* i_pObjFactory );
protected: // overridables of base class QWidget
    virtual void resizeEvent( QResizeEvent* i_pEv ) override;
    virtual void showEvent( QShowEvent* i_pEv ) override;
protected: // instance methods
    void updateStatusBar();
    void updateActions();
    void updateActionsLastUsedFiles();
protected: // instance methods
    void updateCurrentUsedFile( const QString& i_strAbsFilePath );
protected slots:
    void onErrLogEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    void updateErrorsStatus();
protected: // type definitions and constants
    const int c_iLastUsedFilesCountMax = 10;
protected: // class members
    static CMainWindow* s_pThis; // singleton class
protected: // instance members
    QString m_strWindowTitleAppName;
    QStringList m_strlstObjFactories;
    ZS::Test::CTest* m_pTest;
    // Object Factories (Standard Shapes)
    ZS::Draw::CObjFactoryPoint*           m_pObjFactoryPoint;
    ZS::Draw::CObjFactoryLine*            m_pObjFactoryLine;
    ZS::Draw::CObjFactoryRect*            m_pObjFactoryRect;
    ZS::Draw::CObjFactoryEllipse*         m_pObjFactoryEllipse;
    ZS::Draw::CObjFactoryPolygon*         m_pObjFactoryPolygon;
    ZS::Draw::CObjFactoryPolyline*        m_pObjFactoryPolyline;
    ZS::Draw::CObjFactoryText*            m_pObjFactoryText;
    ZS::Draw::CObjFactoryImage*           m_pObjFactoryImage;
    ZS::Draw::CObjFactoryConnectionPoint* m_pObjFactoryConnectionPoint;
    ZS::Draw::CObjFactoryConnectionLine*  m_pObjFactoryConnectionLine;
    ZS::Draw::CObjFactoryGroup*           m_pObjFactoryGroup;
    // Object Factories (User Defined)
    ZS::Draw::QtWidgets::CObjFactoryWdgtCheckBox*    m_pObjFactoryWdgtCheckBox;
    ZS::Draw::QtWidgets::CObjFactoryWdgtComboBox*    m_pObjFactoryWdgtComboBox;
    ZS::Draw::QtWidgets::CObjFactoryWdgtGroupBox*    m_pObjFactoryWdgtGroupBox;
    ZS::Draw::QtWidgets::CObjFactoryWdgtLabel*       m_pObjFactoryWdgtLabel;
    ZS::Draw::QtWidgets::CObjFactoryWdgtLineEdit*    m_pObjFactoryWdgtLineEdit;
    ZS::Draw::QtWidgets::CObjFactoryWdgtPushButton*  m_pObjFactoryWdgtPushButton;
    ZS::Draw::Electricity::CObjFactoryCapacitor*     m_pObjFactoryElectricityCapacitor;
    ZS::Draw::Electricity::CObjFactoryDiode*         m_pObjFactoryElectricityDiode;
    ZS::Draw::Electricity::CObjFactoryInductor*      m_pObjFactoryElectricityInductor;
    ZS::Draw::Electricity::CObjFactoryResistor*      m_pObjFactoryElectricityResistor;
    ZS::Draw::Electricity::CObjFactorySwitch*        m_pObjFactoryElectricitySwitch;
    ZS::Draw::Electricity::CObjFactoryTransistor*    m_pObjFactoryElectricityTransistor;
    ZS::Draw::Electricity::CObjFactoryVoltageSource* m_pObjFactoryElectricityVoltageSource;
    // Menus and Actions
    QMenuBar* m_pMenuBar;
    // Menu - File
    QMenu*    m_pMenuFile;
    QMenu*    m_pMenuLastUsedFiles;
    QToolBar* m_pToolBarFile;
    QAction*  m_pActFileNew;
    QAction*  m_pActFileOpen;
    QAction*  m_pActFileSave;
    QAction*  m_pActFileSaveAs;
    QAction*  m_pActFilePageSetup;
    //QAction*  m_pActLastUsedFiles;
    QList<ZS::System::SLastUsedFile> m_arLastUsedFiles;
    QList<QAction*> m_arpActsLastUsedFiles;
    bool      m_bDrawingChangedSinceLastSave;
    QString   m_strCurrentFileAbsFilePath;
    QDateTime m_dtCurrentFileLastUsed;
    QAction*  m_pActFileQuit;
    // Menu - Mode
    QMenu*    m_pMenuMode;
    QToolBar* m_pToolBarMode;
    QAction*  m_pActModeEdit;
    QAction*  m_pActModeView;
    // Menu - Edit
    QMenu*    m_pMenuEdit;
    // Menu - Edit - Select
    QToolBar* m_pToolBarEditSelect;
    QAction*  m_pActEditSelect;
    // Menu - Edit - Rotate and Mirror
    QToolBar* m_pToolBarEditRotateAndMirror;
    // Menu - Edit - Rotate
    QMenu*    m_pMenuEditRotate;
    QAction*  m_pActEditRotateLeft;
    QAction*  m_pActEditRotateRight;
    QDoubleSpinBox* m_pEdtEditRotateAngle;
    // Menu - Edit - Mirror
    QMenu*   m_pMenuEditMirror;
    QAction* m_pActEditMirrorVertical;
    QAction* m_pActEditMirrorHorizontal;
    // Menu - Edit - Group
    QToolBar* m_pToolBarEditGroup;
    QAction*  m_pActEditGroup;
    QAction*  m_pActEditUngroup;
    // Menu - Draw
    QMenu*    m_pMenuDraw;
    QAction*  m_pActDrawChecked; // Currently active (checked) draw action
    // Menu - Draw - Settings
    QMenu*    m_pMenuDrawSettings;
    QToolBar* m_pToolBarDrawSettings;
    QAction*  m_pActDrawSettingsLine;
    QAction*  m_pActDrawSettingsFill;
    QAction*  m_pActDrawSettingsText;
    // Menu - Draw - Standard Shapes
    QMenu*    m_pMenuDrawStandardShapes;
    QToolBar* m_pToolBarDrawStandardShapes;
    QAction*  m_pActDrawStandardShapePoint;
    QAction*  m_pActDrawStandardShapeLine;
    QAction*  m_pActDrawStandardShapeRect;
    QAction*  m_pActDrawStandardShapeEllipse;
    QAction*  m_pActDrawStandardShapePolyline;
    QAction*  m_pActDrawStandardShapePolygon;
    QAction*  m_pActDrawStandardShapeText;
    // Menu - Draw - Graphics
    QMenu*    m_pMenuDrawGraphics;
    QToolBar* m_pToolBarDrawGraphics;
    QAction*  m_pActDrawGraphicsImage;
    // Menu - Draw - Connections
    QMenu*    m_pMenuDrawConnections;
    QToolBar* m_pToolBarDrawConnections;
    QAction*  m_pActDrawConnectionPoint;
    QAction*  m_pActDrawConnectionLine;
    // Menu - View
    QMenu*    m_pMenuView;
    QToolBar* m_pToolBarView;
    QAction*  m_pActViewZoomIn;
    QAction*  m_pActViewZoomOut;
    QSpinBox* m_pEdtViewZoomFactor_perCent;
    int       m_iViewZoomFactor_perCent;
    // Menu - Debug
    QMenu*    m_pMenuDebug;
    QAction*  m_pActDebugErrLog;
    QAction*  m_pActDebugUnits;
    QAction*  m_pActDebugTraceServer;
    QAction*  m_pActDebugTraceAdminObjs;
    QAction*  m_pActDebugTest;
    // Menu - Info
    QMenu*    m_pMenuInfo;
    QAction*  m_pActInfoVersion;
    QAction*  m_pActInfoSettingsFile;
    // Dock Widgets
    // Dock Widget - Object Factories
    QDockWidget* m_pDockWdgtObjFactories;
    QTreeView* m_pTreeViewObjFactories;
    ZS::Draw::CObjFactoriesModel* m_pModelObjFactories;
    // Dock Widget - GraphObjs
    QDockWidget* m_pDockWdgtGraphObjs;
    ZS::Draw::CWdgtGraphObjs* m_pWdgtGraphObjs;
    // Dialogs
    ZS::Test::GUI::CDlgTest* m_pDlgTest;
    // Status Bar
    QLabel* m_pLblStatusBarMainWindowSize;
    QLabel* m_pLblStatusBarDrawingSceneEditTool;
    QLabel* m_pLblStatusBarDrawingSceneEditMode;
    QLabel* m_pLblStatusBarDrawingSceneGraphObjEditInfo;
    QLabel* m_pLblStatusBarDrawingSceneRect;
    QPointF m_ptDrawingSceneMouseCursorPos;
    QLabel* m_pLblStatusBarDrawingSceneMouseCursorPos;
    QLabel* m_pLblStatusBarDrawingViewMouseCursorPos;
    QLabel* m_pLblErrors;
    // Central Widget with Drawing
    CWidgetCentral* m_pWdgtCentral;
    // Trace
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseEvents;

}; // class CMainWindow

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_MainWindow_h
