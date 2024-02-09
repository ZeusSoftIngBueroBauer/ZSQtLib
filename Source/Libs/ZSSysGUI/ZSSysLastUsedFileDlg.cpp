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

#include "ZSSysGUI/ZSSysLastUsedFileDlg.h"
#include "ZSSysGUI/ZSSysLastUsedFileTreeWdgtItem.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include <QtCore/qdir.h>
#include <QtCore/qsettings.h>
#include <QtCore/qstandardpaths.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qfiledialog.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtreewidget.h>
#else
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtreewidget.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgOpenLastUsedFile : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgOpenLastUsedFile* CDlgOpenLastUsedFile::CreateInstance(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    int             i_iLastUsedFilesCountMax,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }
    return new CDlgOpenLastUsedFile(
        /* strDlgTitle            */ i_strDlgTitle,
        /* strObjName             */ i_strObjName,
        /* iLastUsedFilesCountMax */ i_iLastUsedFilesCountMax,
        /* pWdgtParent            */ i_pWdgtParent,
        /* wFlags                 */ i_wFlags );
}

//------------------------------------------------------------------------------
CDlgOpenLastUsedFile* CDlgOpenLastUsedFile::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgOpenLastUsedFile*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
SLastUsedFile CDlgOpenLastUsedFile::getSelectedFile()
//------------------------------------------------------------------------------
{
    return s_selectedFile;
}

/*==============================================================================
protected: // class members
==============================================================================*/

SLastUsedFile CDlgOpenLastUsedFile::s_selectedFile;

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgOpenLastUsedFile::CDlgOpenLastUsedFile(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    int             i_iLastUsedFilesCountMax,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strDlgTitle  */ i_strDlgTitle,
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_strObjName,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pLyt(nullptr),
    m_pLytHeadline(nullptr),
    m_pLblHeadline(nullptr),
    m_pTreeWdgtLastUsedFiles(nullptr),
    m_pLytButtons(nullptr),
    m_pBtnFileOpenUserDocDir(nullptr),
    m_pBtnFileOpenLastUsedDir(nullptr),
    m_pBtnFileNew(nullptr),
    m_arLastUsedFiles()
{
    // Invalidate selected file.
    s_selectedFile = {"", QDateTime()};

    QSettings settings;
    m_arLastUsedFiles = ZS::System::readLastUsedFiles(settings);
    while (m_arLastUsedFiles.size() > i_iLastUsedFilesCountMax)
    {
        m_arLastUsedFiles.removeLast();
    }

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLytHeadline = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytHeadline);
    m_pLblHeadline = new QLabel("Open Last Used File");
    m_pLytHeadline->addWidget(m_pLblHeadline);

    m_pTreeWdgtLastUsedFiles = new QTreeWidget();
    m_pLyt->addWidget(m_pTreeWdgtLastUsedFiles);

    m_pTreeWdgtLastUsedFiles->setColumnCount(1);
    m_pTreeWdgtLastUsedFiles->setHeaderHidden(true);
    updateTreeWdgtLastUsedFiles();
    m_pTreeWdgtLastUsedFiles->expandAll();

    QObject::connect(
        m_pTreeWdgtLastUsedFiles, &QTreeWidget::itemClicked,
        this, &CDlgOpenLastUsedFile::onTreeWidgetItemClicked);
    QObject::connect(
        m_pTreeWdgtLastUsedFiles, &QTreeWidget::itemDoubleClicked,
        this, &CDlgOpenLastUsedFile::onTreeWidgetItemDoubleClicked);

    m_pLytButtons = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytButtons);

    QIcon iconFileOpen;
    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.png");
    iconFileOpen.addPixmap(pxmFileOpen16x16);
    m_pBtnFileOpenUserDocDir = new QPushButton(" Open User Doc Dir ...");
    m_pBtnFileOpenUserDocDir->setIcon(iconFileOpen);
    m_pLytButtons->addWidget(m_pBtnFileOpenUserDocDir);
    QObject::connect(
        m_pBtnFileOpenUserDocDir, &QPushButton::clicked,
        this, &CDlgOpenLastUsedFile::onBtnFileOpenUserDocDirClicked);

    m_pBtnFileOpenLastUsedDir = new QPushButton(" Open Last Used Dir ...");
    m_pBtnFileOpenLastUsedDir->setIcon(iconFileOpen);
    m_pLytButtons->addWidget(m_pBtnFileOpenLastUsedDir);
    QObject::connect(
        m_pBtnFileOpenLastUsedDir, &QPushButton::clicked,
        this, &CDlgOpenLastUsedFile::onBtnFileOpenLastUsedDirClicked);

    updateButtonFileOpenLastUsedDir();

    QIcon iconFileNew;
    QPixmap pxmFileNew16x16(":/ZS/Menu/MenuFileNew16x16.png");
    iconFileNew.addPixmap(pxmFileNew16x16);
    m_pBtnFileNew = new QPushButton(" New ...");
    m_pBtnFileNew->setIcon(iconFileNew);
    m_pBtnFileNew->setDefault(true);
    m_pLytButtons->addWidget(m_pBtnFileNew);
    QObject::connect(
        m_pBtnFileNew, &QPushButton::clicked,
        this, &CDlgOpenLastUsedFile::onBtnFileNewClicked);

    m_pLytButtons->addStretch();

    setMinimumWidth(300);
    setMinimumHeight(250);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgOpenLastUsedFile::~CDlgOpenLastUsedFile()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pLytHeadline = nullptr;
    m_pLblHeadline = nullptr;
    m_pTreeWdgtLastUsedFiles = nullptr;
    m_pLytButtons = nullptr;
    m_pBtnFileOpenUserDocDir = nullptr;
    m_pBtnFileOpenLastUsedDir = nullptr;
    m_pBtnFileNew = nullptr;
    //m_arLastUsedFiles.clear();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::updateTreeWdgtLastUsedFiles()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateTreeWdgtLastUsedFiles",
        /* strAddInfo   */ "" );

    m_pTreeWdgtLastUsedFiles->clear();

    QIcon iconApp;

    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.png");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.png");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.png");

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    QTreeWidgetItem* pTreeWdgtItemToday = nullptr;
    QTreeWidgetItem* pTreeWdgtItemYesterday = nullptr;
    QTreeWidgetItem* pTreeWdgtItemThisWeek = nullptr;
    QTreeWidgetItem* pTreeWdgtItemThisMonth = nullptr;
    QTreeWidgetItem* pTreeWdgtItemOlder = nullptr;
    QTreeWidgetItem* pTreeWdgtItemPeriod;
    QTreeWidgetItem* pTreeWdgtItem;

    QDateTime dtCurrent = QDateTime::currentDateTime();
    for (int idxFile = 0; idxFile < m_arLastUsedFiles.size(); ++idxFile) {
        pTreeWdgtItemPeriod = nullptr;
        if (m_arLastUsedFiles[idxFile].m_dtLastUsed.daysTo(dtCurrent) <= 0) {
            if (pTreeWdgtItemToday == nullptr) {
                pTreeWdgtItemToday = new QTreeWidgetItem(
                    m_pTreeWdgtLastUsedFiles, QStringList(QString("Today")));
            }
            pTreeWdgtItemPeriod = pTreeWdgtItemToday;
        }
        else if (m_arLastUsedFiles[idxFile].m_dtLastUsed.daysTo(dtCurrent) <= 1) {
            if (pTreeWdgtItemYesterday == nullptr) {
                pTreeWdgtItemYesterday = new QTreeWidgetItem(
                    m_pTreeWdgtLastUsedFiles, QStringList(QString("Yesterday")));
            }
            pTreeWdgtItemPeriod = pTreeWdgtItemYesterday;
        }
        else if (m_arLastUsedFiles[idxFile].m_dtLastUsed.daysTo(dtCurrent) <= 7) {
            if (pTreeWdgtItemThisWeek == nullptr) {
                pTreeWdgtItemThisWeek = new QTreeWidgetItem(
                    m_pTreeWdgtLastUsedFiles, QStringList(QString("This Week")));
            }
            pTreeWdgtItemPeriod = pTreeWdgtItemThisWeek;
        }
        else if (m_arLastUsedFiles[idxFile].m_dtLastUsed.daysTo(dtCurrent) <= 31) {
            if (pTreeWdgtItemThisMonth == nullptr) {
                pTreeWdgtItemThisMonth = new QTreeWidgetItem(
                    m_pTreeWdgtLastUsedFiles, QStringList(QString("This Month")));
            }
            pTreeWdgtItemPeriod = pTreeWdgtItemThisMonth;
        }
        else {
            if (pTreeWdgtItemOlder == nullptr) {
                pTreeWdgtItemOlder = new QTreeWidgetItem(
                    m_pTreeWdgtLastUsedFiles, QStringList(QString("Older")));
            }
            pTreeWdgtItemPeriod = pTreeWdgtItemOlder;
        }
        if (pTreeWdgtItemPeriod != nullptr)
        {
            QFont fnt = pTreeWdgtItemPeriod->font(0);
            fnt.setBold(true);
            pTreeWdgtItemPeriod->setFont(0, fnt);

            pTreeWdgtItem = new CTreeWdgtItemLastUsedFile(
                pTreeWdgtItemPeriod, m_arLastUsedFiles[idxFile], iconApp);
        }
    }
}

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::updateButtonFileOpenLastUsedDir()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateButtonFileOpenLastUsedDir",
        /* strAddInfo   */ "" );

    if (!m_arLastUsedFiles.isEmpty())
    {
        QFileInfo fileInfo(m_arLastUsedFiles[0].m_strAbsFilePath);
        QDir dir(fileInfo.absoluteDir());
        m_pBtnFileOpenLastUsedDir->setEnabled(dir.exists());
    }
    else
    {
        m_pBtnFileOpenLastUsedDir->setEnabled(false);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::onTreeWidgetItemClicked(
    QTreeWidgetItem* i_pItem, int i_iColumn)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Item: " + QString(i_pItem == nullptr ? "null" : i_pItem->text(i_iColumn));
        strMthInArgs += ", Column: " + QString::number(i_iColumn);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTreeWidgetItemClicked",
        /* strAddInfo   */ strMthInArgs );

    CTreeWdgtItemLastUsedFile* pLastUsedFileItem = dynamic_cast<CTreeWdgtItemLastUsedFile*>(i_pItem);

    if (pLastUsedFileItem != nullptr)
    {
        QFileInfo fileInfo(pLastUsedFileItem->lastUsedFile().m_strAbsFilePath);

        if (!fileInfo.isFile() || !fileInfo.exists())
        {
            QString strMsg;
            strMsg  = "Error on reading file \"" + fileInfo.absoluteFilePath() + "\"";
            strMsg += "\n\nErrorCode:\t" + result2Str(EResultFileNotFound);
            strMsg += "\n\nDo you want to remove the file from list of last open files?";
            QMessageBox::StandardButton msgBoxBtn = QMessageBox::warning(
                /* pWdgtParent */ this,
                /* strTitly    */ windowTitle(),
                /* strText     */ strMsg,
                /* buttons     */ QMessageBox::Yes | QMessageBox::No );
            if( msgBoxBtn == QMessageBox::Yes )
            {
                for (int idxFile = 0; idxFile < m_arLastUsedFiles.size(); ++idxFile)
                {
                    if (m_arLastUsedFiles[idxFile].m_strAbsFilePath == fileInfo.absoluteFilePath())
                    {
                        m_arLastUsedFiles.removeAt(idxFile);
                        break;
                    }
                }
                QSettings settings;
                ZS::System::writeLastUsedFiles(m_arLastUsedFiles, settings);
                updateTreeWdgtLastUsedFiles();
                updateButtonFileOpenLastUsedDir();
                m_pTreeWdgtLastUsedFiles->expandAll();
            }
        }
        else
        {
            s_selectedFile = pLastUsedFileItem->lastUsedFile();
            accept();
        }
    }
}

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::onTreeWidgetItemDoubleClicked(
    QTreeWidgetItem* i_pItem, int i_iColumn)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Item: " + QString(i_pItem == nullptr ? "null" : i_pItem->text(i_iColumn));
        strMthInArgs += ", Column: " + QString::number(i_iColumn);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTreeWidgetItemDoubleClicked",
        /* strAddInfo   */ strMthInArgs );

    CTreeWdgtItemLastUsedFile* pLastUsedFileItem = dynamic_cast<CTreeWdgtItemLastUsedFile*>(i_pItem);

    if (pLastUsedFileItem != nullptr)
    {
        QFileInfo fileInfo(pLastUsedFileItem->lastUsedFile().m_strAbsFilePath);

        if (!fileInfo.isFile() || !fileInfo.exists())
        {
            QString strMsg;
            strMsg  = "Error on reading file \"" + fileInfo.absoluteFilePath() + "\"";
            strMsg += "\n\nErrorCode:\t" + result2Str(EResultFileNotFound);
            strMsg += "\n\nDo you want to remove the file from list of last open files?";
            QMessageBox::StandardButton msgBoxBtn = QMessageBox::warning(
                /* pWdgtParent */ this,
                /* strTitly    */ windowTitle(),
                /* strText     */ strMsg,
                /* buttons     */ QMessageBox::Yes | QMessageBox::No );
            if( msgBoxBtn == QMessageBox::Yes )
            {
                for (int idxFile = 0; idxFile < m_arLastUsedFiles.size(); ++idxFile)
                {
                    if (m_arLastUsedFiles[idxFile].m_strAbsFilePath == fileInfo.absoluteFilePath())
                    {
                        m_arLastUsedFiles.removeAt(idxFile);
                        break;
                    }
                }
                QSettings settings;
                ZS::System::writeLastUsedFiles(m_arLastUsedFiles, settings);
                updateTreeWdgtLastUsedFiles();
                updateButtonFileOpenLastUsedDir();
                m_pTreeWdgtLastUsedFiles->expandAll();
            }
        }
        else
        {
            s_selectedFile = pLastUsedFileItem->lastUsedFile();
            accept();
        }
    }
}

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::onBtnFileOpenUserDocDirClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnFileOpenUserDocDirClicked",
        /* strAddInfo   */ strMthInArgs );

    QDir dir;
    QString strDocsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    dir.setPath(strDocsDir);

    QString strAbsFilePath = QFileDialog::getOpenFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a file to open",
        /* strDir      */ dir.absolutePath(),
        /* strFilter   */ "Drawings (*.xml)" );
    if( !strAbsFilePath.isEmpty() )
    {
        s_selectedFile = {strAbsFilePath, QDateTime::currentDateTime()};
        accept();
    }
}

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::onBtnFileOpenLastUsedDirClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnFileOpenLastUsedDirClicked",
        /* strAddInfo   */ strMthInArgs );

    QSettings settings;
    QList<SLastUsedFile> arLastUsedFiles = ZS::System::readLastUsedFiles(settings);
    if (!arLastUsedFiles.isEmpty())
    {
        QFileInfo fileInfo(arLastUsedFiles[0].m_strAbsFilePath);
        QDir dir(fileInfo.absoluteDir());
        if (dir.exists())
        {
            QString strAbsFilePath = QFileDialog::getOpenFileName(
                /* pWdgtParent */ this,
                /* strCaption  */ "Choose a file to open",
                /* strDir      */ dir.absolutePath(),
                /* strFilter   */ "Drawings (*.xml)" );
            if( !strAbsFilePath.isEmpty() )
            {
                s_selectedFile = {strAbsFilePath, QDateTime::currentDateTime()};
                accept();
            }
        }
    }
}

//------------------------------------------------------------------------------
void CDlgOpenLastUsedFile::onBtnFileNewClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnFileNewClicked",
        /* strAddInfo   */ strMthInArgs );

    reject();
}
