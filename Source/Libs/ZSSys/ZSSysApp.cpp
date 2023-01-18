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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
Application
*******************************************************************************/

//------------------------------------------------------------------------------
/*! @brief Parses the arguments passed to the program and returns the parameters
           and their (optional) values in string lists which are more easy to evaluate.

    If multiple arguments are passed, they must be separated by a delimiter which may be
    a space (' '), a slash ('/'), a minus sign ('-'), a comma (',') or a semicolon.

    An argument consists at least of a parameter name. A value may optionally be assigned
    to the parameter by adding an equal sign ('=').

    @param i_argc Number of arguments contained in i_argv[].
    @param i_argv The arguments passed to the program.
    @param io_strlstPars Will contain a list with parsed parameter names.
    @param io_strlstVals Will contain a list with the values assigned to the parameters.
                         If the parameter does not have a value an empty string entry is added.

    **Usage**

    @code
    main(int argc, argv)
    {
        QStringList strlstArgsPar;
        QStringList strlstArgsVal;
        parseAppArgs(argc, argv, strlstArgsPar, strlstArgsVal);
        for( int idxArg = 0; idxArg < strlstArgsPar.length() && idxArg < strlstArgsVal.length(); idxArg++ )
        {
            strArg = strlstArgsPar[idxArg];
            strVal = strlstArgsVal[idxArg];
            if( strArg.compare("...",Qt::CaseInsensitive) == 0 )
            {
                strPar1 = strVal;
            }
            else if( strArg.compare("...",Qt::CaseInsensitive) == 0 )
            {
                strPar2 = strVal;
            }
        }
    }
    @endcode
*/
void ZS::System::parseAppArgs(
    int&         i_argc,
    char*        i_argv[],
    QStringList& io_strlstPars,
    QStringList& io_strlstVals )
//------------------------------------------------------------------------------
{
    if( i_argc > 1 )
    {
        int     idxArg;
        QString strArg;
        QString strPar;
        QString strVal;

        strArg = i_argv[1];

        for( idxArg = 1; idxArg < i_argc; idxArg++ )
        {
            strArg = i_argv[idxArg];
            strPar = "";
            strVal = "";

            if( strArg.at(0) == '/' )
            {
                strArg = strArg.remove(0,1);
            }
            else if( strArg.at(0) == '-' )
            {
                strArg = strArg.remove(0,1);
            }
            else if( strArg.endsWith(",") )
            {
                strArg = strArg.remove(strArg.size()-1,1);
            }
            else if( strArg.endsWith(";") )
            {
                strArg = strArg.remove(strArg.size()-1,1);
            }
            if( strArg.contains("=") )
            {
                strPar = strArg.section("=",0,0);
                strVal = strArg.section("=",1,1);

                if( !strVal.isEmpty() && strVal.at(0) == '"' )
                {
                    strVal = strVal.section("\"",1,1);
                }
            }
            else
            {
                strPar = strArg;
            }
            if( !strPar.isEmpty() )
            {
                io_strlstPars.append(strPar);
                io_strlstVals.append(strVal);
            }
        }
    }

} // parseAppArgs

//------------------------------------------------------------------------------
/*! @brief Returns the absolute path to the config directory of the application.

    The directory in which applications should write configuration and setting
    files depends on the one hand on the underlying operating system.
    But the path may also depend on whether the files should be saved in a location
    specific to the current user or in a global location.

    To ensure that the directory exists during runtime you may set the flag
    i_bCreateDirIfNotExisting to true.

    @param i_strIniFileScope [in]
        Range ["AppDir", "User", "System"]
        AppDir ... Directory is below the applications startup directory.
        User ..... Directory depends on where the operating system saves user specific files.
        System ... Directory depends on where the operating system saves global setting files
                   ("/ProgramData/CompanyName/ApplicationName" on Windows machines).
    @param i_bCreateDirIfNotExisting [in]
        true if the log directory should be created by this method, false otherwise.

    @return Absolute path to the directory.
*/
QString ZS::System::getAppConfigDir( const QString& i_strIniFileScope, bool i_bCreateDirIfNotExisting )
//------------------------------------------------------------------------------
{
    QString strOrgNameNormalized = QCoreApplication::organizationName();
    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The organization name may contain characters which are invalid in file names:
    strOrgNameNormalized.remove(":");
    strOrgNameNormalized.remove("\\");
    strOrgNameNormalized.remove("/");
    strOrgNameNormalized.remove("<");
    strOrgNameNormalized.remove(">");

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strIniFileAbsDirPath;
    QString strIniFileBaseName = strAppNameNormalized;
    QString strIniFileSuffix = "ini";
    QString strIniFileAbsFilePath;

    if( i_strIniFileScope.compare("AppDir",Qt::CaseInsensitive) == 0 )
    {
        QString strAppDirAbsPath = QCoreApplication::applicationDirPath();

        // Applications installation directory
        QDir dirApp(strAppDirAbsPath);

        strIniFileAbsDirPath = dirApp.absolutePath();

        int idxStrPos = strIniFileAbsDirPath.indexOf("Bin",0,Qt::CaseInsensitive);

        if( idxStrPos >= 0 ) // Development environment
        {
            strIniFileAbsDirPath = strIniFileAbsDirPath.left(idxStrPos);
            strIniFileAbsDirPath += "Config";

            strIniFileAbsFilePath = strIniFileAbsDirPath + "/" + strIniFileBaseName + ".ini";

            QSettings settingsFile( strIniFileAbsFilePath, QSettings::IniFormat );
            strIniFileAbsFilePath = settingsFile.fileName();
            QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
            strIniFileAbsDirPath = fileInfoIniFile.absolutePath();
        }
        else // if( idxStrPos == 0 )  // Program Files
        {
            strIniFileAbsFilePath = strIniFileAbsDirPath + "/" + strIniFileBaseName + ".ini";
            QSettings settingsFile( strIniFileAbsFilePath, QSettings::IniFormat );
            strIniFileAbsFilePath = settingsFile.fileName();
            QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
            strIniFileAbsDirPath = fileInfoIniFile.absolutePath();
        }
    } // if( i_strIniFileScope.compare("AppDir",Qt::CaseInsensitive) == 0 )

    else if( i_strIniFileScope.compare("User",Qt::CaseInsensitive) == 0 )
    {
        QSettings settingsFile(
            /* format          */ QSettings::IniFormat,
            /* scope           */ QSettings::UserScope,
            /* strOrganization */ strOrgNameNormalized,
            /* strAppName      */ strAppNameNormalized );
        strIniFileAbsFilePath = settingsFile.fileName();
        QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
        strIniFileAbsDirPath = fileInfoIniFile.absolutePath();

        if( !strIniFileAbsDirPath.endsWith(strAppNameNormalized,Qt::CaseInsensitive) )
        {
            strIniFileAbsDirPath += "/" + strAppNameNormalized;
        }
    } // if( i_strIniFileScope.compare("User",Qt::CaseInsensitive) == 0 )

    else // if( i_strIniFileScope.compare("System",Qt::CaseInsensitive) == 0 ) Default
    {
        QSettings settingsFile(
            /* format          */ QSettings::IniFormat,
            /* scope           */ QSettings::SystemScope,
            /* strOrganization */ strOrgNameNormalized,
            /* strAppName      */ strAppNameNormalized );
        strIniFileAbsFilePath = settingsFile.fileName();
        QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
        strIniFileAbsDirPath = fileInfoIniFile.absolutePath();

        if( !strIniFileAbsDirPath.endsWith(strAppNameNormalized,Qt::CaseInsensitive) )
        {
            strIniFileAbsDirPath += "/" + strAppNameNormalized;
        }
    } // if( i_strIniFileScope.compare("System",Qt::CaseInsensitive) == 0 )

    if( i_bCreateDirIfNotExisting )
    {
        QDir().mkpath(strIniFileAbsDirPath);
    }

    return strIniFileAbsDirPath;

} // getAppConfigDir

//------------------------------------------------------------------------------
/*! @brief Returns the absolute path to the log directory of the application.

    The directory in which applications should write log files depends on the
    one hand on the underlying operating system. But the path may also depend on
    whether the log files should be saved in a location specific to the current
    user or in a global location.

    To ensure that the log directory exists during runtime you may set the
    flag i_bCreateDirIfNotExisting to true.

    @param i_strIniFileScope [in]
        Range ["AppDir", "User", "System"]
        AppDir ... Log file directory is below the applications startup directory.
        User ..... Log directory depends on where the operating system saves
                   user specific log files.
        System ... Log directory depends on where the operating system saves global settings
                   files ("/ProgramData/CompanyName/ApplicationName" on Windows machines).
    @param i_bCreateDirIfNotExisting [in]
        true if the log directory should be created by this method, false otherwise.

    @return Absolute path to the directory.
*/
QString ZS::System::getAppLogDir( const QString& i_strIniFileScope, bool i_bCreateDirIfNotExisting )
//------------------------------------------------------------------------------
{
    QString strLogFileAbsDirPath;

    QString strOrgNameNormalized = QCoreApplication::organizationName();
    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The organization name may contain characters which are invalid in file names:
    strOrgNameNormalized.remove(":");
    strOrgNameNormalized.remove("\\");
    strOrgNameNormalized.remove("/");
    strOrgNameNormalized.remove("<");
    strOrgNameNormalized.remove(">");

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strIniFileAbsDirPath;
    QString strIniFileBaseName = strAppNameNormalized;
    QString strIniFileSuffix = "ini";
    QString strIniFileAbsFilePath;

    if( i_strIniFileScope.compare("AppDir",Qt::CaseInsensitive) == 0 )
    {
        QString strAppDirAbsPath = QCoreApplication::applicationDirPath();

        // Applications installation directory
        QDir dirApp(strAppDirAbsPath);

        strIniFileAbsDirPath = dirApp.absolutePath();

        int idxStrPos = strIniFileAbsDirPath.indexOf("Bin",0,Qt::CaseInsensitive);

        if( idxStrPos >= 0 ) // Development environment
        {
            strIniFileAbsDirPath = strIniFileAbsDirPath.left(idxStrPos);
            strLogFileAbsDirPath = strIniFileAbsDirPath;
            strLogFileAbsDirPath += "Log";
        }
        else // if( idxStrPos == 0 )  // Program Files
        {
            strIniFileAbsFilePath = strIniFileAbsDirPath + "/" + strIniFileBaseName + ".ini";
            QSettings settingsFile( strIniFileAbsFilePath, QSettings::IniFormat );
            strIniFileAbsFilePath = settingsFile.fileName();
            QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
            strLogFileAbsDirPath = fileInfoIniFile.absolutePath();
            strLogFileAbsDirPath += "/Log";
        }
    } // if( i_strIniFileScope.compare("AppDir",Qt::CaseInsensitive) == 0 )

    else if( i_strIniFileScope.compare("User",Qt::CaseInsensitive) == 0 )
    {
        QSettings settingsFile(
            /* format          */ QSettings::IniFormat,
            /* scope           */ QSettings::UserScope,
            /* strOrganization */ strOrgNameNormalized,
            /* strAppName      */ strAppNameNormalized );
        strIniFileAbsFilePath = settingsFile.fileName();
        QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
        strLogFileAbsDirPath = fileInfoIniFile.absolutePath();

        if( !strLogFileAbsDirPath.endsWith(strAppNameNormalized,Qt::CaseInsensitive) )
        {
            strLogFileAbsDirPath += "/" + strAppNameNormalized;
        }
        strLogFileAbsDirPath += "/Log";

    } // if( i_strIniFileScope.compare("User",Qt::CaseInsensitive) == 0 )

    else // if( i_strIniFileScope.compare("System",Qt::CaseInsensitive) == 0 ) Default
    {
        QSettings settingsFile(
            /* format          */ QSettings::IniFormat,
            /* scope           */ QSettings::SystemScope,
            /* strOrganization */ strOrgNameNormalized,
            /* strAppName      */ strAppNameNormalized );
        strIniFileAbsFilePath = settingsFile.fileName();
        QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
        strLogFileAbsDirPath = fileInfoIniFile.absolutePath();

        if( !strLogFileAbsDirPath.endsWith(strAppNameNormalized,Qt::CaseInsensitive) )
        {
            strLogFileAbsDirPath += "/" + strAppNameNormalized;
        }
        strLogFileAbsDirPath += "/Log";

    } // if( i_strIniFileScope.compare("System",Qt::CaseInsensitive) == 0 )

    if( i_bCreateDirIfNotExisting )
    {
        QDir().mkpath(strLogFileAbsDirPath);
    }

    return strLogFileAbsDirPath;

} // getAppLogDir

//------------------------------------------------------------------------------
/*! @brief Returns the absolute path to the data directory of the application.

    The directory in which applications should write data files depends on the
    one hand on the underlying operating system. But the path may also depend on
    whether the data files should be saved in a location specific to the current
    user or in a global location.

    To ensure that the data directory exists during runtime you may set the
    flag i_bCreateDirIfNotExisting to true.

    @param i_strIniFileScope [in]
        Range ["AppDir", "User", "System"]
        AppDir ... Data file directory is below the applications startup directory.
        User ..... Data directory depends on where the operating system saves
                   user specific log files.
        System ... Data directory depends on where the operating system saves global settings
                   files ("/ProgramData/CompanyName/ApplicationName" on Windows machines).
    @param i_bCreateDirIfNotExisting [in]
        true if the log directory should be created by this method, false otherwise.

    @return Absolute path to the directory.
*/
QString ZS::System::getAppDataDir( const QString& i_strIniFileScope, bool i_bCreateDirIfNotExisting )
//------------------------------------------------------------------------------
{
    QString strDataFileAbsDirPath;

    QString strOrgNameNormalized = QCoreApplication::organizationName();
    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The organization name may contain characters which are invalid in file names:
    strOrgNameNormalized.remove(":");
    strOrgNameNormalized.remove("\\");
    strOrgNameNormalized.remove("/");
    strOrgNameNormalized.remove("<");
    strOrgNameNormalized.remove(">");

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strIniFileAbsDirPath;
    QString strIniFileBaseName = strAppNameNormalized;
    QString strIniFileSuffix = "ini";
    QString strIniFileAbsFilePath;

    if( i_strIniFileScope.compare("AppDir",Qt::CaseInsensitive) == 0 )
    {
        QString strAppDirAbsPath = QCoreApplication::applicationDirPath();

        // Applications installation directory
        QDir dirApp(strAppDirAbsPath);

        strIniFileAbsDirPath = dirApp.absolutePath();

        int idxStrPos = strIniFileAbsDirPath.indexOf("Bin",0,Qt::CaseInsensitive);

        if( idxStrPos >= 0 ) // Development environment
        {
            strIniFileAbsDirPath = strIniFileAbsDirPath.left(idxStrPos);
            strDataFileAbsDirPath = strIniFileAbsDirPath;
            strDataFileAbsDirPath += "Data";
        }
        else // if( idxStrPos == 0 )  // Program Files
        {
            strIniFileAbsFilePath = strIniFileAbsDirPath + "/" + strIniFileBaseName + ".ini";
            QSettings settingsFile( strIniFileAbsFilePath, QSettings::IniFormat );
            strIniFileAbsFilePath = settingsFile.fileName();
            QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
            strDataFileAbsDirPath = fileInfoIniFile.absolutePath();
            strDataFileAbsDirPath += "/Data";
        }
    } // if( i_strIniFileScope.compare("AppDir",Qt::CaseInsensitive) == 0 )

    else if( i_strIniFileScope.compare("User",Qt::CaseInsensitive) == 0 )
    {
        QSettings settingsFile(
            /* format          */ QSettings::IniFormat,
            /* scope           */ QSettings::UserScope,
            /* strOrganization */ strOrgNameNormalized,
            /* strAppName      */ strAppNameNormalized );
        strIniFileAbsFilePath = settingsFile.fileName();
        QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
        strDataFileAbsDirPath = fileInfoIniFile.absolutePath();

        if( !strDataFileAbsDirPath.endsWith(strAppNameNormalized,Qt::CaseInsensitive) )
        {
            strDataFileAbsDirPath += "/" + strAppNameNormalized;
        }
        strDataFileAbsDirPath += "/Data";

    } // if( i_strIniFileScope.compare("User",Qt::CaseInsensitive) == 0 )

    else // if( i_strIniFileScope.compare("System",Qt::CaseInsensitive) == 0 ) Default
    {
        QSettings settingsFile(
            /* format          */ QSettings::IniFormat,
            /* scope           */ QSettings::SystemScope,
            /* strOrganization */ strOrgNameNormalized,
            /* strAppName      */ strAppNameNormalized );
        strIniFileAbsFilePath = settingsFile.fileName();
        QFileInfo fileInfoIniFile(strIniFileAbsFilePath);
        strDataFileAbsDirPath = fileInfoIniFile.absolutePath();

        if( !strDataFileAbsDirPath.endsWith(strAppNameNormalized,Qt::CaseInsensitive) )
        {
            strDataFileAbsDirPath += "/" + strAppNameNormalized;
        }
        strDataFileAbsDirPath += "/Data";

    } // if( i_strIniFileScope.compare("System",Qt::CaseInsensitive) == 0 )

    if( i_bCreateDirIfNotExisting )
    {
        QDir().mkpath(strDataFileAbsDirPath);
    }

    return strDataFileAbsDirPath;

} // getAppDataDir


/*******************************************************************************
class CCoreApp : public QCoreApplication
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CCoreApp::CCoreApp( int& i_argc, char* i_argv[] ) :
//------------------------------------------------------------------------------
    QCoreApplication(i_argc,i_argv)
{
} // ctor

//------------------------------------------------------------------------------
CCoreApp::~CCoreApp()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
protected: // overridables of base class QCoreApplication
==============================================================================*/

//------------------------------------------------------------------------------
bool CCoreApp::notify( QObject* i_pObjRcv, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool     bResult = false;
    CMsgErr* pMsgErr = nullptr;
    CMsgErr* pMsgExc = nullptr;

    if( i_pEv->type() != QEvent::Timer )
    {
        pMsgErr = dynamic_cast<CMsgErr*>(i_pEv);
    }

    if( pMsgErr != nullptr )
    {
        if( pMsgErr->wasException() )
        {
            // The error (exception) message will be deleted by Qt.
            pMsgExc = pMsgErr;
        }
        else
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                SErrResultInfo errResultInfo = pMsgErr->getErrResultInfo();
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        pMsgErr = nullptr;

    } // if( pMsgErr != nullptr )

    if( pMsgExc == nullptr )
    {
        QString strExcType;

        try
        {
            bResult = QCoreApplication::notify(i_pObjRcv,i_pEv);
        }
        catch( ZS::System::CException& exc )
        {
            if( QThread::currentThread() != thread() )
            {
                // This type of exception has already been logged on throwing (creating) the exception.
                pMsgExc = new CMsgErr(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ QCoreApplication::instance(),
                    /* exc          */ exc,
                    /* strExcType   */ "ZS::System::CException" );
                QCoreApplication::postEvent(this,pMsgExc);
                pMsgExc = nullptr;
                throw;
            }
        }
        catch(...)
        {
            if( QThread::currentThread() != thread() )
            {
                // An ZS::System::CException instance will be temporarily created.
                // This exception instance is used to create an exception error message.
                // The ctor of this exception adds an entry of this event to the error log model.
                ZS::System::CException exc(
                    /* strFile    */ "",
                    /* iLineNr    */ -1,
                    /* result     */ EResultUnknownException,
                    /* strAddInfo */ "" );
                pMsgExc = new CMsgErr(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ QCoreApplication::instance(),
                    /* exc          */ exc,
                    /* strExcType   */ "Unknown exception" );
                QCoreApplication::postEvent(this,pMsgExc);
                pMsgExc = nullptr;
                throw;
            }
        } // catch(...)
    } // if( pMsgExc == nullptr )

    return bResult;

} // notify
