/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qglobal.h>
#include <QtCore/qlibraryinfo.h>
#include <QtGui/qimagewriter.h>

#include "App.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::Apps::Products::MethodTraceClient;


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iAppResult = 0;

    //static const ZS::System::SEnumEntry s_arEnumStrQtLibraryLocations[] = {
    //    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    //    /*  0 */ ZS::System::SEnumEntry( QLibraryInfo::PrefixPath,              "PrefixPath"             ),
    //    /*  1 */ ZS::System::SEnumEntry( QLibraryInfo::DocumentationPath,       "DocumentationPath"      ),
    //    /*  2 */ ZS::System::SEnumEntry( QLibraryInfo::HeadersPath,             "HeadersPath"            ),
    //    /*  3 */ ZS::System::SEnumEntry( QLibraryInfo::LibrariesPath,           "LibrariesPath"          ),
    //    /*  4 */ ZS::System::SEnumEntry( QLibraryInfo::LibraryExecutablesPath,  "LibraryExecutablesPath" ),
    //    /*  5 */ ZS::System::SEnumEntry( QLibraryInfo::BinariesPath,            "BinariesPath"           ),
    //    /*  6 */ ZS::System::SEnumEntry( QLibraryInfo::PluginsPath,             "PluginsPath"            ),
    //    /*  7 */ ZS::System::SEnumEntry( QLibraryInfo::ImportsPath,             "ImportsPath"            ),
    //    /*  8 */ ZS::System::SEnumEntry( QLibraryInfo::Qml2ImportsPath,         "Qml2ImportsPath"        ),
    //    /*  9 */ ZS::System::SEnumEntry( QLibraryInfo::ArchDataPath,            "ArchDataPath"           ),
    //    /* 10 */ ZS::System::SEnumEntry( QLibraryInfo::DataPath,                "DataPath"               ),
    //    /* 11 */ ZS::System::SEnumEntry( QLibraryInfo::TranslationsPath,        "TranslationsPath"       ),
    //    /* 12 */ ZS::System::SEnumEntry( QLibraryInfo::ExamplesPath,            "ExamplesPath"           ),
    //    /* 13 */ ZS::System::SEnumEntry( QLibraryInfo::TestsPath,               "TestsPath"              )
    //    #elif QT_VERSION >= 0x040000
    //    /*  0 */ ZS::System::SEnumEntry( QLibraryInfo::PrefixPath,              "PrefixPath"             ),
    //    /*  1 */ ZS::System::SEnumEntry( QLibraryInfo::DocumentationPath,       "DocumentationPath"      ),
    //    /*  2 */ ZS::System::SEnumEntry( QLibraryInfo::HeadersPath,             "HeadersPath"            ),
    //    /*  3 */ ZS::System::SEnumEntry( QLibraryInfo::LibrariesPath,           "LibrariesPath"          ),
    //    /*  4 */ ZS::System::SEnumEntry( QLibraryInfo::BinariesPath,            "BinariesPath"           ),
    //    /*  5 */ ZS::System::SEnumEntry( QLibraryInfo::PluginsPath,             "PluginsPath"            ),
    //    /*  6 */ ZS::System::SEnumEntry( QLibraryInfo::DataPath,                "DataPath"               ),
    //    /*  7 */ ZS::System::SEnumEntry( QLibraryInfo::TranslationsPath,        "TranslationsPath"       ),
    //    /*  8 */ ZS::System::SEnumEntry( QLibraryInfo::SettingsPath,            "SettingsPath"           ),
    //    /*  9 */ ZS::System::SEnumEntry( QLibraryInfo::DemosPath,               "DemosPath"              ),
    //    /* 10 */ ZS::System::SEnumEntry( QLibraryInfo::ExamplesPath,            "ExamplesPath"           ),
    //    /* 11 */ ZS::System::SEnumEntry( QLibraryInfo::ImportsPath,             "ImportsPath"            )
    //    #endif
    //};

    //QHash<QString, QString> libLocs;

    //QString strLibLoc;
    //QString strLibPath;
    //int     idxLibLoc;

    //QLibraryInfo::LibraryLocation libLoc;

    //for( idxLibLoc = 0; idxLibLoc < _ZSArrLen(s_arEnumStrQtLibraryLocations); idxLibLoc++ )
    //{
    //    libLoc = static_cast<QLibraryInfo::LibraryLocation>(s_arEnumStrQtLibraryLocations[idxLibLoc].m_iEnumerator);
    //    strLibLoc = ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrQtLibraryLocations, _ZSArrLen(s_arEnumStrQtLibraryLocations), idxLibLoc);
    //    strLibPath = QLibraryInfo::location(libLoc);
    //    libLocs.insert(strLibLoc, strLibPath);
    //}

    CApplication* pApp = new CApplication(
        /* argc                  */ argc,
        /* argv                  */ argv,
        /* strOrganizationName   */ "ZeusSoft",
        /* strOrganizationDomain */ "ZeusSoft.de",
        /* strAppName            */ "ZSAppTrcMthClient",
        /* strAppNameWindowTitle */ "Method Trace Client (" + ZS::System::c_strSysVersionNr + ")" );

    iAppResult = pApp->exec();

    try
    {
        delete pApp;
    }
    catch(...)
    {
    }
    pApp = nullptr;

    #ifdef _WINDOWS
    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    #endif
    #endif

    return iAppResult;

} // main
