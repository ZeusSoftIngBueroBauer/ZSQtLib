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

#ifndef ZSApps_TestRemCmdServer_WdgtTest_h
#define ZSApps_TestRemCmdServer_WdgtTest_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSys/ZSSysErrResult.h"

class QComboBox;
class QDoubleSpinBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QTimer;
class QFormLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Apps
{
namespace Test
{
namespace RemCmdServer
{
class CDatabase;
class CTestModule1;
class CTestModule2Thread;
class CTestModule2;

//******************************************************************************
class CWdgtTest : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CWdgtTest* GetInstance(); // singleton class
public: // ctors and dtor
    CWdgtTest(
        QWidget*        i_pWdgtParent = 0,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CWdgtTest();
public: // instance methods
    void setDb( CDatabase* i_pDb );
protected slots:
    void onDbDestroyed( QObject* i_pDb );
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected: // overridables of base class QWidget
    virtual void paintEvent( QPaintEvent* i_pEv );
protected slots:
    void onTimerWdgtUpdateTimeout();
protected slots:
    void onBtnTestModule1CreateDeleteClicked();
    void onBtnTestModule1CreateDeleteModule2Clicked();
    void onBtnTestModule1SendMsgToModule2Clicked();
protected slots:
    void onCmbSigGenWaveformCurrentIndexChanged( int i_idx );
    void onEdtSigGenYMinValueChanged( double i_fVal );
    void onEdtSigGenYMaxValueChanged( double i_fVal );
    void onEdtSigGenFreqHzValueChanged( double i_fVal );
    void onEdtSigAnaSamplingRateHzValueChanged( double i_fVal );
private: // class members
    static CWdgtTest*   s_pThis;
protected: // instance members
    // All test modules
    QTimer*              m_pTimerWdgtUpdate;
    // Test modules
    QPushButton*         m_pBtnTestModule1CreateDelete;
    QPushButton*         m_pBtnTestModule1CreateDeleteModule2;
    QPushButton*         m_pBtnTestModule1SendMsgToModule2;
    // Test module 1
    QRect                m_rectTestModule1;
    QRect                m_rectTitleTestModule1;
    QRect                m_rectBtnTestModule1CreateDelete;
    QRect                m_rectBtnTestModule1CreateDeleteModule2;
    QRect                m_rectBtnTestModule1SendMsgToModule2;
    // Test module 2 (started in context of a separated thread)
    QRect                m_rectTestModule2ThreadCtor;
    QRect                m_rectTitleTestModule2ThreadCtorLine1;
    QRect                m_rectTitleTestModule2ThreadCtorLine2;
    QRect                m_rectTestModule2ThreadCtx;
    QRect                m_rectTitleTestModule2ThreadCtxLine1;
    QRect                m_rectTitleTestModule2ThreadCtxLine2;
    QRect                m_rectTestModule2;
    QRect                m_rectTitleTestModule2;
    // Database
    CDatabase*           m_pDb;
    // Signal generator
    QGroupBox*           m_pWdgtSigGen;
    QFormLayout*         m_pLytWdgtSigGen;
    QComboBox*           m_pCmbSigGenWaveform;
    QDoubleSpinBox*      m_pEdtSigGenYMin;
    QDoubleSpinBox*      m_pEdtSigGenYMax;
    QDoubleSpinBox*      m_pEdtSigGenFreq_Hz;
    // Signal analyzer
    QGroupBox*           m_pWdgtSigAna;
    QFormLayout*         m_pLytWdgtSigAna;
    QDoubleSpinBox*      m_pEdtSigAnaSamplingRate_Hz;
    QLineEdit*           m_pEdtSigAnaYVal;
    // Tracing
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWgdtTest

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestRemCmdServer_WdgtTest_h
