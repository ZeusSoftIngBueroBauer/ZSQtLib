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

#ifndef ZSSysGUI_ErrDlg_h
#define ZSSysGUI_ErrDlg_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysErrResult.h"

class QDialogButtonBox;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CDlgErr : public QDialog
//******************************************************************************
{
public: // ctors and dtor
    CDlgErr( const QString& i_strDlgTitle, QWidget* i_pWdgtParent = nullptr );
    virtual ~CDlgErr();
public: // instance methods
    void setErrResultInfo( const SErrResultInfo& i_errResultInfo );
    SErrResultInfo getErrResultInfo() const { return m_errResultInfo; }
    SErrResult getErrResult() const { return m_errResultInfo.getErrResult(); }
    EResultSeverity getSeverity() const { return m_errResultInfo.getSeverity(); }
    EResult getResult() const { return m_errResultInfo.getResult(); }
    QString getErrSource() const { return m_errResultInfo.getErrSource().toString(); }
    QString getAddErrInfo() const { return m_errResultInfo.getAddErrInfoDscr(); }
protected: // overridable slots of base class QDialog
    virtual void accept();
    virtual void reject();
protected: // instance members
    SErrResultInfo    m_errResultInfo;
    QVBoxLayout*      m_pLyt;
    // Upper part devided into two columns to increase the size of the severity icon.
    QHBoxLayout*      m_pLytUpperPart;
    QVBoxLayout*      m_pLytUpperLeftPart;
    QHBoxLayout*      m_pLytLineSeverity;
    QLabel*           m_pLblSeverity;
    QLabel*           m_pEdtSeverity;
    QHBoxLayout*      m_pLytLineDate;
    QLabel*           m_pLblDate;
    QLabel*           m_pEdtDate;
    QHBoxLayout*      m_pLytLineTime;
    QLabel*           m_pLblTime;
    QLabel*           m_pEdtTime;
    QVBoxLayout*      m_pLytUpperRightPart;
    QLabel*           m_pLblSeverityIcon;
    // Lower part with one column.
    QHBoxLayout*      m_pLytLineResult;
    QLabel*           m_pLblResult;
    QLabel*           m_pEdtResult;
    QHBoxLayout*      m_pLytLineSource;
    QLabel*           m_pLblSource;
    QLabel*           m_pEdtSource;
    QHBoxLayout*      m_pLytLineAddInfo;
    QLabel*           m_pLblAddInfo;
    QLabel*           m_pEdtAddInfo;
    // Ok button
    QPushButton*      m_pBtnOk;
    QDialogButtonBox* m_pBtnBox;

}; // class CDlgErr

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ErrDlg_h
