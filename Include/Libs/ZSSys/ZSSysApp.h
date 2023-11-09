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

#ifndef ZSSys_App_h
#define ZSSys_App_h

#include "ZSSys/ZSSysDllMain.h"

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>

class QSettings;

namespace ZS
{
namespace System
{
class CErrLog;

/*******************************************************************************
Application
*******************************************************************************/

ZSSYSDLL_API void parseAppArgs(
    int&         i_argc,
    char*        i_argv[],
    QStringList& io_strlstPars,
    QStringList& io_strlstVals );

ZSSYSDLL_API QString getAppConfigDir( const QString& i_strIniFileScope = "User", bool i_bCreateDirIfNotExisting = true );
ZSSYSDLL_API QString getAppLogDir( const QString& i_strIniFileScope = "User", bool i_bCreateDirIfNotExisting = true );
ZSSYSDLL_API QString getAppDataDir( const QString& i_strIniFileScope = "User", bool i_bCreateDirIfNotExisting = true );

struct ZSSYSDLL_API SLastUsedFile {
    SLastUsedFile() :
        m_strAbsFilePath(""),
        m_dtLastUsed()
    {
    }
    SLastUsedFile(const SLastUsedFile& i_other) :
        m_strAbsFilePath(i_other.m_strAbsFilePath),
        m_dtLastUsed(i_other.m_dtLastUsed)
    {
    }
    SLastUsedFile(const QString& i_strAbsFilePath, const QDateTime& i_dt) :
        m_strAbsFilePath(i_strAbsFilePath),
        m_dtLastUsed(i_dt)
    {
    }
    QString m_strAbsFilePath;
    QDateTime m_dtLastUsed;
};

ZSSYSDLL_API QList<SLastUsedFile> readLastUsedFiles(const QSettings& i_settings, const QString& i_strKey = "LastUsedFiles");
ZSSYSDLL_API void writeLastUsedFiles(const QList<SLastUsedFile>& i_arLastUsedFiles, QSettings& i_settings, const QString& i_strKey = "LastUsedFiles");
ZSSYSDLL_API void sortLastUsedFiles(QList<SLastUsedFile>& i_arLastUsedFiles);

//******************************************************************************
class ZSSYSDLL_API CCoreApp : public QCoreApplication
//******************************************************************************
{
public: // ctors and dtor
    CCoreApp( int& i_argc, char* i_argv[] );
    ~CCoreApp();
protected: // overridables of base class QCoreApplication
    virtual bool notify( QObject* i_pObjRcv, QEvent* i_pEv );

}; // class CCoreApp

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_App_h
