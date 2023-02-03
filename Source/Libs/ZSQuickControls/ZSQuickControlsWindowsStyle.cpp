/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSQuickControls/ZSQuickControlsWindowsStyle.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qdiriterator.h>
#include <QtCore/qxmlstream.h>
#include <QtGui/qguiapplication.h>
#include <QtQml/qqmlapplicationengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQuickControls2/qquickstyle.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::QuickControls;


/*******************************************************************************
class CWindowsStyle : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CWindowsStyle::c_strDayTheme = "Day";
const QString CWindowsStyle::c_strNightTheme = "Night";
const QString CWindowsStyle::c_strDefaultTheme = c_strDayTheme;

//------------------------------------------------------------------------------
QStringList CWindowsStyle::themes()
//------------------------------------------------------------------------------
{
    return s_strlstThemes;
}

/*==============================================================================
protected: // class members
==============================================================================*/

CWindowsStyle* CWindowsStyle::s_pTheInst = nullptr;
QStringList CWindowsStyle::s_strlstThemes = {c_strDayTheme, c_strNightTheme};

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the address of the theme.

    This method does neither create an instance of the class nor increments
    the reference counter.
    If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure
    that the instance is already existing this method should be preferred to the
    createInstance call as this method does not affect the reference counter and
    there is no need to call releaseInstance later on.

    @note After a getInstance call a releaseInstance MUST NOT be called.

    @return Pointer to trace server or nullptr, if the instance has not been created yet.
*/
CWindowsStyle* CWindowsStyle::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Creates the theme if not already existing.

    If the instance is already existing the reference to the existing
    instance is returned and a reference counter is incremented.

    @return Pointer to theme instance.
*/
CWindowsStyle* CWindowsStyle::CreateInstance(
    QQmlApplicationEngine* i_pQmlAppEngine,
    CIdxTree*              i_pIdxTreeStyles )
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr )
    {
        // The ctor sets s_pTheInst to this.
        // If the ctor itself calls methods of other classes which again recursively
        // call "GetInstance" this way "GetInstance" does not return null but the
        // pointer to the server instance currently beeing created.
        // But of course this requires special caution as within the ctor it must
        // be assured that recursively accessed instance members are already valid.
        new CWindowsStyle(i_pQmlAppEngine, i_pIdxTreeStyles);
    }
    s_pTheInst->incrementRefCount();
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Releases the theme instance.

    Before invoking this method a reference to the instance must have been retrieved
    with a createInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @note A reference returned by getInstance MUST NOT be freed.
*/
void CWindowsStyle::ReleaseInstance()
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, NameSpace() + "::" + ClassName());
    }

    int iRefCount = s_pTheInst->decrementRefCount();

    if( iRefCount == 0 )
    {
        try
        {
            delete s_pTheInst;
        }
        catch(...)
        {
        }
        s_pTheInst = nullptr;
    }
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWindowsStyle::isValidThemeName(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    bool bValid = ((i_strTheme == c_strDayTheme) || (i_strTheme == c_strNightTheme));
    if( !bValid && CErrLog::GetInstance() != nullptr ) {
        SErrResultInfo errResultInfo(
            NameSpace(), ClassName(), "", "isValidThemeName",
            EResultArgOutOfRange, EResultSeverityError, "Invalid Theme Name: " + i_strTheme);
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }
    return bValid;
}

//------------------------------------------------------------------------------
QColor CWindowsStyle::mergedColors(
    const QColor& i_colorA, const QColor& i_colorB, int i_iFactor)
//------------------------------------------------------------------------------
{
    const int iMaxFactor = 100;
    QColor colorTmp = i_colorA;
    colorTmp.setRed(
        (colorTmp.red() * i_iFactor) / iMaxFactor
      + (i_colorB.red() * (iMaxFactor - i_iFactor)) / iMaxFactor);
    colorTmp.setGreen(
        (colorTmp.green() * i_iFactor) / iMaxFactor
      + (i_colorB.green() * (iMaxFactor - i_iFactor)) / iMaxFactor);
    colorTmp.setBlue(
        (colorTmp.blue() * i_iFactor) / iMaxFactor
      + (i_colorB.blue() * (iMaxFactor - i_iFactor)) / iMaxFactor);
    return colorTmp;
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the theme.

    The constructor is protected. The singleton class must be created via the
    static method createInstance.

*/
CWindowsStyle::CWindowsStyle(
    QQmlApplicationEngine* i_pQmlAppEngine,
    CIdxTree* i_pIdxTreeStyles ) :
//------------------------------------------------------------------------------
    QObject(),
    m_iRefCount(0),
    m_pQmlAppEngine(i_pQmlAppEngine),
    m_strCurrentTheme(c_strDefaultTheme),
    m_arhshBaseColors(CEnumRowVersion::count()),
    m_arhshPopupWindowColors(CEnumRowVersion::count()),
    m_arhshBackgroundStartColors(CEnumRowVersion::count()),
    m_arhshBackgroundStopColors(CEnumRowVersion::count()),
    m_arhshButtonColors(CEnumRowVersion::count()),
    //m_arhshLightShadeColors(CEnumRowVersion::count()),
    //m_arhshDarkShadeColors(CEnumRowVersion::count()),
    m_arhshShadowColors(CEnumRowVersion::count()),
    m_arhshTopShadowColors(CEnumRowVersion::count()),
    m_arhshInnerContrastLineColors(CEnumRowVersion::count()),
    m_arhshHighlightColors(CEnumRowVersion::count()),
    m_arhshHighlightedTextColors(CEnumRowVersion::count()),
    m_arhshOutlineColors(CEnumRowVersion::count()),
    m_arhshHighlightedOutlineColors(CEnumRowVersion::count()),
    //m_arhshTabFrameColors(CEnumRowVersion::count()),
    m_arhshTextColors(CEnumRowVersion::count()),
    m_arhshWindowTextColors(CEnumRowVersion::count()),
    m_arhshLinkColors(CEnumRowVersion::count()),
    m_arhshButtonTextColors(CEnumRowVersion::count()),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
{
    setObjectName("theInst");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName() + "::NoisyMethods", objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTreeStyles == nullptr ? "nullptr" : i_pIdxTreeStyles->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    // See comment in "CreateInstance" above.
    s_pTheInst = this;

    QPalette palette = QGuiApplication::palette();

    int iRowVersionOrig = static_cast<int>(ERowVersion::Original);

    QColor clr;

    clr = palette.color(QPalette::Normal, QPalette::Base);
    m_arhshBaseColors[iRowVersionOrig][c_strDayTheme] = clr; //.lighter(124);
    m_arhshBaseColors[iRowVersionOrig][c_strNightTheme] = clr.darker();

    clr = palette.color(QPalette::Normal, QPalette::Window);
    m_arhshPopupWindowColors[iRowVersionOrig][c_strDayTheme] = clr; //.lighter(124);
    m_arhshPopupWindowColors[iRowVersionOrig][c_strNightTheme] = clr.darker();

    clr = palette.color(QPalette::Normal, QPalette::Window);
    m_arhshBackgroundStartColors[iRowVersionOrig][c_strDayTheme] = clr; //.lighter(124);
    m_arhshBackgroundStartColors[iRowVersionOrig][c_strNightTheme] = clr.darker();

    clr = palette.color(QPalette::Normal, QPalette::Window);
    m_arhshBackgroundStopColors[iRowVersionOrig][c_strDayTheme] = clr; //.lighter(102);
    m_arhshBackgroundStopColors[iRowVersionOrig][c_strNightTheme] = clr.darker();

    clr = palette.color(QPalette::Normal, QPalette::Button);
    m_arhshButtonColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshButtonColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    //m_arhshLightShadeColors[iRowVersionOrig][c_strDayTheme] = clr;
    //    QColor(255, 255, 255, 90);
    //m_arhshLightShadeColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();
    //    QColor(255, 255, 255, 90).lighter();

    //m_arhshLightShadeColors[iRowVersionOrig][c_strDayTheme] = clr;
    //    QColor(0, 0, 0, 60);
    //m_arhshLightShadeColors[iRowVersionOrig][c_strNightTheme] = clr.darker();
    //    QColor(0, 0, 0, 60).darker();

    clr = palette.color(QPalette::Normal, QPalette::Shadow);
    m_arhshShadowColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshShadowColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    clr = QColor(0, 0, 0, 18);
    m_arhshTopShadowColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshTopShadowColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    clr = QColor(255, 255, 255, 30);
    m_arhshInnerContrastLineColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshInnerContrastLineColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    m_arhshHighlightColors[iRowVersionOrig][c_strDayTheme] =
        palette.color(QPalette::Normal, QPalette::Highlight);
    m_arhshHighlightColors[iRowVersionOrig][c_strNightTheme] =
        palette.color(QPalette::Normal, QPalette::Highlight).lighter();

    m_arhshHighlightedTextColors[iRowVersionOrig][c_strDayTheme] =
        palette.color(QPalette::Normal, QPalette::HighlightedText);
    m_arhshHighlightedTextColors[iRowVersionOrig][c_strNightTheme] =
        palette.color(QPalette::Normal, QPalette::HighlightedText).lighter();

    clr = palette.color(QPalette::Normal, QPalette::Window).darker(140);
    m_arhshOutlineColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshOutlineColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    clr = palette.color(QPalette::Normal, QPalette::Highlight).darker(125);
    if (clr.value() > 160) {
        clr.setHsl(clr.hue(), clr.saturation(), 160);
    }
    m_arhshHighlightedOutlineColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshHighlightedOutlineColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    //m_arhshTabFrameColors[iRowVersionOrig][c_strDayTheme] =
    //    palette.color(QPalette::Normal, QPalette::Button).lighter(104);
    //m_arhshTabFrameColors[iRowVersionOrig][c_strNightTheme] =
    //    palette.color(QPalette::Normal, QPalette::Button).lighter();

    clr = palette.color(QPalette::Normal, QPalette::Text);
    m_arhshTextColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshTextColors[iRowVersionOrig][c_strNightTheme] = "white";

    clr = palette.color(QPalette::Normal, QPalette::WindowText);
    m_arhshWindowTextColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshWindowTextColors[iRowVersionOrig][c_strNightTheme] = "white";

    clr = palette.color(QPalette::Normal, QPalette::Link);
    m_arhshLinkColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshLinkColors[iRowVersionOrig][c_strNightTheme] = clr.lighter();

    clr = palette.color(QPalette::Normal, QPalette::ButtonText);
    m_arhshButtonTextColors[iRowVersionOrig][c_strDayTheme] = clr;
    m_arhshButtonTextColors[iRowVersionOrig][c_strNightTheme] = "white";

    setToDefault();

    CIdxTreeEntry* pBranch = new CIdxTreeEntry(EIdxTreeEntryType::Branch, "Windows");
    i_pIdxTreeStyles->add(pBranch);

    QMap<QString, QString> entriesSorted;

    QDirIterator qrc(":/" + StyleName(), QDirIterator::Subdirectories);
    while(qrc.hasNext())
    {
        QString strDir = qrc.next();
        QFileInfo fi = qrc.fileInfo();
        QString strAbsoluteFilePath = fi.absoluteFilePath();
        QString strAbsolutePath = fi.absolutePath();
        QString strBaseName = fi.baseName();
        QString strBundleName = fi.bundleName();
        QString strCanonicalFilePath = fi.canonicalFilePath();
        QString strCanonicalPath = fi.canonicalPath();
        QString strCompleteBaseName = fi.completeBaseName();
        QString strCompleteSuffix = fi.completeSuffix();
        QString strFileName = fi.fileName();
        QString strFilePath = fi.filePath();
        QString strGroup = fi.group();
        QString strIsRoot = QString(fi.isRoot() ? "Root" : "");
        QString strIsDir = QString(fi.isDir() ? "Dir" : "");
        QString strIsFile = QString(fi.isFile() ? "File" : "");
        QString strIsExecutable = QString(fi.isExecutable() ? "Executable" : "");
        QString strIsHidden = QString(fi.isHidden() ? "Hidden" : "");
        QString strIsReadable = QString(fi.isReadable() ? "Readable" : "");
        QString strIsWritable = QString(fi.isWritable() ? "Writable" : "");
        QString strLastModified = fi.lastModified().toString("dd.MM.yyyy hh:mm:ss.zzz");
        QString strLastRead = fi.lastRead().toString("dd.MM.yyyy hh:mm:ss.zzz");
        QString strPath = fi.path();
        QString strSize_bytes = QString::number(fi.size()) + " Bytes";
        QString strSuffix = fi.suffix();

        if( fi.isFile() )
        {
            if( strCompleteBaseName.compare("qmldir", Qt::CaseInsensitive) != 0
             && strCompleteBaseName.compare("Theme", Qt::CaseInsensitive) != 0 )
            {
                entriesSorted.insert(strCompleteBaseName, strAbsoluteFilePath);
            }
        }
    }

    for( const QString& strCompleteBaseName : entriesSorted.keys() )
    {
        CIdxTreeEntry* pLeave = new CIdxTreeEntry(EIdxTreeEntryType::Leave, strCompleteBaseName);
        i_pIdxTreeStyles->add(pLeave, pBranch);
    }

    QQmlContext* pQmlCtx = m_pQmlAppEngine->rootContext();

    pQmlCtx->setContextProperty(ContextPropertyName(), this);

    QObject::connect(
        this, &CWindowsStyle::currentThemeChanged,
        this, &CWindowsStyle::onCurrentThemeChanged);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the theme.

    The destructor is protected. The singleton class must be destroyed via the
    static method releaseInstance.
*/
CWindowsStyle::~CWindowsStyle()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcAdminObjNoisyMethods != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    }

    m_iRefCount = 0;
    m_pQmlAppEngine = nullptr;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (save/recall)
==============================================================================*/

//------------------------------------------------------------------------------
QString CWindowsStyle::getSaveRecallAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "getSaveRecallAbsoluteFilePath",
        /* strMethodInArgs    */ "" );
    QString strAppConfigDir = ZS::System::getAppConfigDir("System");
    QString strFileSuffix = "xml";
    QString strBaseName = "ZSWindowsStyle";
    QString strAbsFilePath = strAppConfigDir + QDir::separator() + strBaseName + "." + strFileSuffix;
    strAbsFilePath = QDir::toNativeSeparators(strAbsFilePath);
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(strAbsFilePath);
    }
    return strAbsFilePath;
}

//------------------------------------------------------------------------------
SErrResultInfo CWindowsStyle::save() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "save",
        /* strMethodInArgs    */ "" );

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), "save");
    QString strAbsFilePath = getSaveRecallAbsoluteFilePath();

    QFileInfo fileInfo(strAbsFilePath);
    QDir dir = fileInfo.absoluteDir();
    if( !dir.exists() ) {
        dir.mkpath(dir.absolutePath());
    }

    QFile file(strAbsFilePath);
    if( !file.open(QIODevice::WriteOnly) ) {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultFileOpenForRead);
        errResultInfo.setAddErrInfoDscr(strAbsFilePath);
    }
    else if( file.isOpen() ) {
        QByteArray byteArr;
        QXmlStreamWriter xmlStreamWriter(&byteArr);

        xmlStreamWriter.setAutoFormatting(true);
        xmlStreamWriter.writeStartDocument();
        xmlStreamWriter.writeStartElement("ZSWindowsStyle");

        for( const QString& strTheme : themes() ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
            xmlStreamWriter.writeStartElement("Theme");
            xmlStreamWriter.writeAttribute("Name", strTheme);
            if( !isBaseColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("BaseColor");
                xmlStreamWriter.writeAttribute("Value", baseColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isPopupWindowColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("PopupWindowColor");
                xmlStreamWriter.writeAttribute("Value", popupWindowColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isBackgroundStartColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("BackgroundStartColor");
                xmlStreamWriter.writeAttribute("Value", backgroundStartColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isBackgroundStopColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("BackgroundStopColor");
                xmlStreamWriter.writeAttribute("Value", backgroundStopColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isButtonColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("ButtonColor");
                xmlStreamWriter.writeAttribute("Value", buttonBaseColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isShadowColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("ShadowColor");
                xmlStreamWriter.writeAttribute("Value", shadowColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isTopShadowColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("TopShadowColor");
                xmlStreamWriter.writeAttribute("Value", topShadowColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isInnerContrastLineColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("InnerContrastLineColor");
                xmlStreamWriter.writeAttribute("Value", innerContrastLineColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isHighlightColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("HighlightColor");
                xmlStreamWriter.writeAttribute("Value", outlineColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isHighlightedTextColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("HighlightedTextColor");
                xmlStreamWriter.writeAttribute("Value", highlightedTextColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isOutlineColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("OutlineColor");
                xmlStreamWriter.writeAttribute("Value", outlineColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isHighlightedOutlineColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("HighlightedOutlineColor");
                xmlStreamWriter.writeAttribute("Value", highlightedOutlineColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( isTextColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("TextColor");
                xmlStreamWriter.writeAttribute("Value", textColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isWindowTextColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("WindowTextColor");
                xmlStreamWriter.writeAttribute("Value", windowTextColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isLinkColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("LinkColor");
                xmlStreamWriter.writeAttribute("Value", linkColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            if( !isButtonTextColorSetToDefault(strTheme) ) {
                xmlStreamWriter.writeStartElement("ButtonTextColor");
                xmlStreamWriter.writeAttribute("Value", buttonTextColor(strTheme).name());
                xmlStreamWriter.writeEndElement();
            }
            xmlStreamWriter.writeEndElement(); // Theme
        }
        xmlStreamWriter.writeEndElement(); // ZSWindowsStyle
        xmlStreamWriter.writeEndDocument();

        if( !errResultInfo.isErrorResult() ) {
            file.write(byteArr);
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
SErrResultInfo CWindowsStyle::recall(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "recall",
        /* strMethodInArgs    */ strMthInArgs );

    QStringList strlstThemes;
    if( i_strTheme.isEmpty() ) {
        strlstThemes = themes();
    }
    else if( isValidThemeName(i_strTheme) ) {
        strlstThemes << i_strTheme;
    }

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), "recall");
    QString strAbsFilePath = getSaveRecallAbsoluteFilePath();
    QFile file(strAbsFilePath);

    if( file.exists() && !file.open(QIODevice::ReadOnly) ) {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultFileOpenForRead);
        errResultInfo.setAddErrInfoDscr(strAbsFilePath);
    }
    else if( file.isOpen() ) {
        QXmlStreamReader xmlStreamReader(&file);
        QXmlStreamReader::TokenType xmlStreamTokenType = xmlStreamReader.readNext();
        if( xmlStreamTokenType != QXmlStreamReader::StartDocument ) {
            xmlStreamReader.raiseError("Invalid XML document");
        }
        xmlStreamTokenType = xmlStreamReader.readNext();
        QString strElemName = xmlStreamReader.name().toString();
        if( xmlStreamTokenType != QXmlStreamReader::StartElement || strElemName != "ZSWindowsStyle" ) {
            xmlStreamReader.raiseError("Invalid XML ZSWindowsTyle file (missing root node \"ZSWindowsStyle\")");
        }
        if( !xmlStreamReader.hasError() ) {
            QString strTheme;
            while( !xmlStreamReader.atEnd() ) {
                xmlStreamTokenType = xmlStreamReader.readNext();
                if( xmlStreamReader.isStartElement() ) {
                    strElemName = xmlStreamReader.name().toString();
                    if( strElemName == "Theme" ) {
                        strTheme = "";
                        if( !xmlStreamReader.attributes().hasAttribute("Name") ) {
                            xmlStreamReader.raiseError("Attribute \"Name\" for Element \"" + strElemName + "\" is missing.");
                        }
                        else {
                            QString strAttr = xmlStreamReader.attributes().value("Name").toString();
                            if( !isValidThemeName(strAttr) ) {
                                xmlStreamReader.raiseError("Attribute \"Name\" (" + strAttr + ") for Element Theme is invalid.");
                            }
                            else {
                                strTheme = strAttr;
                            }
                        }
                    }
                    else {
                        if( !isValidThemeName(strTheme) ) {
                            xmlStreamReader.raiseError("\"" + strElemName + "\" not expected without valid Theme.");
                        }
                        else if( strlstThemes.contains(strTheme) ) {
                            if( strElemName == "BaseColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setBaseColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "PopupWindowColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setPopupWindowColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "BackgroundStartColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setBackgroundStartColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "BackgroundStopColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setBackgroundStopColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "ButtonColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setButtonColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "ShadowColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setShadowColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "TopShadowColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setTopShadowColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "InnerContrastLineColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setInnerContrastLineColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "HighlightColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setHighlightColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "HighlightedTextColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setHighlightedTextColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "OutlineColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setOutlineColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "HighlightedOutlineColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setHighlightedOutlineColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "TextColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setTextColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "WindowTextColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setWindowTextColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "LinkColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setLinkColor(strAttr, strTheme);
                                }
                            }
                            else if( strElemName == "ButtonTextColor" ) {
                                if( !xmlStreamReader.attributes().hasAttribute("Value") ) {
                                    xmlStreamReader.raiseError("Attribute \"Value\" for Element \"" + strElemName + "\" is missing.");
                                }
                                else {
                                    QString strAttr = xmlStreamReader.attributes().value("Value").toString();
                                    setButtonTextColor(strAttr, strTheme);
                                }
                            }
                            else {
                                //xmlStreamReader.raiseError("Invalid element \"" + strElemName + "\".");
                                if( CErrLog::GetInstance() != nullptr ) {
                                    QString strAddErrInfo;
                                    strAddErrInfo = "Invalid element \"" + strElemName + "\" on reading " + strAbsFilePath + " ";
                                    strAddErrInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
                                    strAddErrInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());
                                    SErrResultInfo errResultInfoTmp = errResultInfo;
                                    errResultInfoTmp.setSeverity(EResultSeverityWarning);
                                    errResultInfoTmp.setResult(EResultFileReadContent);
                                    errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                    CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                 }
                            }
                        }
                    }
                } // if( xmlStreamReader.isStartElement() )
            } // while( !xmlStreamReader.atEnd() )
        } // if( !xmlStreamReader.hasError() )

        if( xmlStreamReader.hasError() )
        {
            QString strAddErrInfo;
            strAddErrInfo = xmlStreamReader.errorString() + " on reading " + strAbsFilePath + " ";
            strAddErrInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
            strAddErrInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
    QStringList strlstThemes;
    if( i_strTheme.isEmpty() ) {
        strlstThemes = themes();
    }
    else if( isValidThemeName(i_strTheme) ) {
        strlstThemes << i_strTheme;
    }
    for( const QString& strTheme : strlstThemes ) {
        setBaseColorToDefault(strTheme);
        setPopupWindowColorToDefault(strTheme);
        setBackgroundStartColorToDefault(strTheme);
        setBackgroundStopColorToDefault(strTheme);
        setButtonColorToDefault(strTheme);
        setShadowColorToDefault(strTheme);
        setTopShadowColorToDefault(strTheme);
        setInnerContrastLineColorToDefault(strTheme);
        setHighlightColorToDefault(strTheme);
        setHighlightedTextColorToDefault(strTheme);
        setOutlineColorToDefault(strTheme);
        setHighlightedOutlineColorToDefault(strTheme);
        setTextColorToDefault(strTheme);
        setWindowTextColorToDefault(strTheme);
        setLinkColorToDefault(strTheme);
        setButtonTextColorToDefault(strTheme);
    }
}

/*==============================================================================
public: // instance methods (current theme)
==============================================================================*/

//------------------------------------------------------------------------------
QString CWindowsStyle::currentTheme() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "currentTheme",
        /* strMethodInArgs    */ "" );
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(m_strCurrentTheme);
    }
    return m_strCurrentTheme;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setCurrentTheme( const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setCurrentTheme",
        /* strMethodInArgs    */ strMthInArgs );

    if( isValidThemeName(i_strTheme) ) {
        if( m_strCurrentTheme != i_strTheme ) {
            bool bWasLightTheme = isLightTheme();
            bool bWasDarkTheme = isDarkTheme();
            m_strCurrentTheme = i_strTheme;
            bool bIsLightTheme = isLightTheme();
            bool bIsDarkTheme = isDarkTheme();
            emit_currentThemeChanged(m_strCurrentTheme);
            if( bWasLightTheme != bIsLightTheme ) {
                emit isLightThemeChanged();
            }
            if( bWasDarkTheme != bIsDarkTheme ) {
                emit isDarkThemeChanged();
            }
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isLightTheme() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isLightTheme",
        /* strMethodInArgs    */ "" );
    bool bIs = (m_strCurrentTheme == c_strDayTheme);
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isDarkTheme() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isDarkTheme",
        /* strMethodInArgs    */ "" );
    bool bIs = !isLightTheme();
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::baseColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "baseColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshBaseColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setBaseColor(
    const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setBaseColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshBaseColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshBaseColors[iRowVersionCurr][strTheme] = i_color;
            emit_baseColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isBaseColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isBaseColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshBaseColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshBaseColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setBaseColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setBaseColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isBaseColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshBaseColors[iRowVersionOrig][strTheme];
            setBaseColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::popupWindowColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "popupWindowColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshPopupWindowColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setPopupWindowColor(
    const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setPopupWindowColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshPopupWindowColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshPopupWindowColors[iRowVersionCurr][strTheme] = i_color;
            emit_popupWindowColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isPopupWindowColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isPopupWindowColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshPopupWindowColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshPopupWindowColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setPopupWindowColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setPopupWindowColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isPopupWindowColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshPopupWindowColors[iRowVersionOrig][strTheme];
            setPopupWindowColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::backgroundStartColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "backgroundStartColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshBackgroundStartColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setBackgroundStartColor(
    const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setBackgroundStartColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshBackgroundStartColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshBackgroundStartColors[iRowVersionCurr][strTheme] = i_color;
            emit_backgroundStartColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isBackgroundStartColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isBackgroundStartColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshBackgroundStartColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshBackgroundStartColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setBackgroundStartColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setBackgroundStartColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isBackgroundStartColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshBackgroundStartColors[iRowVersionOrig][strTheme];
            setBackgroundStartColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::backgroundStopColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "backgroundStopColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshBackgroundStopColors[iRowVersion][strTheme];
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setBackgroundStopColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setBackgroundStopColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshBackgroundStopColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshBackgroundStopColors[iRowVersionCurr][strTheme] = i_color;
            emit_backgroundStopColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isBackgroundStopColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isBackgroundStopColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshBackgroundStopColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshBackgroundStopColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setBackgroundStopColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setBackgroundStopColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isBackgroundStopColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshBackgroundStopColors[iRowVersionOrig][strTheme];
            setBackgroundStopColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::panelStartColor(
    bool i_bHighlighted, bool i_bDown, bool i_bHovered,
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Highlighted: " + bool2Str(i_bHighlighted);
        strMthInArgs += ", Down: " + bool2Str(i_bDown);
        strMthInArgs += ", Hovered: " + bool2Str(i_bHovered);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "panelStartColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshBackgroundStartColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
QColor CWindowsStyle::panelStopColor(
    bool i_bHighlighted, bool i_bDown, bool i_bHovered,
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Highlighted: " + bool2Str(i_bHighlighted);
        strMthInArgs += ", Down: " + bool2Str(i_bDown);
        strMthInArgs += ", Hovered: " + bool2Str(i_bHovered);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "panelStopColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshBackgroundStopColors[iRowVersion][strTheme];
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::buttonBaseColor(const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "buttonBaseColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshButtonColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
QColor CWindowsStyle::buttonColor(
    bool i_bHighlighted, bool i_bDown, bool i_bHovered,
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Highlighted: " + bool2Str(i_bHighlighted);
        strMthInArgs += ", Down: " + bool2Str(i_bDown);
        strMthInArgs += ", Hovered: " + bool2Str(i_bHovered);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "buttonColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshButtonColors[iRowVersion][strTheme];
    }

    int iVal = qGray(clr.rgb());
    clr = clr.lighter(100 + qMax(1, (180 - iVal)/6));
    clr.setHsv(clr.hue(), clr.saturation() * 0.75, clr.value());
    if (i_bHighlighted) {
        clr = mergedColors(clr, highlightedOutlineColor(i_strTheme, i_rowVersion).lighter(130), 90);
    }
    if (!i_bHovered) {
        clr = clr.darker(104);
    }
    if (i_bDown) {
        clr = clr.darker(110);
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setButtonColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setButtonColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshButtonColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshButtonColors[iRowVersionCurr][strTheme] = i_color;
            emit_buttonColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isButtonColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isButtonColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshButtonColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshButtonColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setButtonColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setButtonColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isButtonColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshButtonColors[iRowVersionOrig][strTheme];
            setButtonColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::buttonOutlineColor(
    bool i_bHighlighted, bool i_bEnabled,
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Highlighted: " + bool2Str(i_bHighlighted);
        strMthInArgs += ", Enabled: " + bool2Str(i_bEnabled);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "buttonOutlineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        QColor clr = i_bEnabled && i_bHighlighted ?
            highlightedOutlineColor(i_strTheme, i_rowVersion) : outlineColor(i_strTheme, i_rowVersion);
        clr = !i_bEnabled ? clr.lighter(115) : clr;
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//QColor CWindowsStyle::lightShadeColor(const QString& i_strTheme, ERowVersion i_rowVersion)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
//        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
//        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod          */ "lightShadeColor",
//        /* strMethodInArgs    */ strMthInArgs );
//
//    QColor clr;
//    QString strTheme = i_strTheme;
//    if( i_strTheme.isEmpty() ) {
//        strTheme = m_strCurrentTheme;
//    }
//    if( isValidThemeName(strTheme) ) {
//        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
//        clr = m_arhshLightShadeColors[iRowVersion][strTheme];
//    }
//    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
//        mthTracer.setMethodReturn(clr.name());
//    }
//    return clr;
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//QColor CWindowsStyle::darkShadeColor(const QString& i_strTheme, ERowVersion i_rowVersion)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
//        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
//        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod          */ "darkShadeColor",
//        /* strMethodInArgs    */ strMthInArgs );
//
//    QColor clr;
//    QString strTheme = i_strTheme;
//    if( i_strTheme.isEmpty() ) {
//        strTheme = m_strCurrentTheme;
//    }
//    if( isValidThemeName(strTheme) ) {
//        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
//        clr = m_arhshDarkShadeColors[iRowVersion][strTheme];
//    }
//    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
//        mthTracer.setMethodReturn(clr.name());
//    }
//    return clr;
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::shadowColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "shadowColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshShadowColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setShadowColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setShadowColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshShadowColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshShadowColors[iRowVersionCurr][strTheme] = i_color;
            emit_shadowColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isShadowColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isShadowColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshShadowColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshShadowColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setShadowColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setShadowColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isShadowColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshShadowColors[iRowVersionOrig][strTheme];
            setShadowColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::topShadowColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "topShadowColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshTopShadowColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setTopShadowColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setTopShadowColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshTopShadowColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshTopShadowColors[iRowVersionCurr][strTheme] = i_color;
            emit_topShadowColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isTopShadowColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isTopShadowColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshTopShadowColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshTopShadowColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setTopShadowColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setTopShadowColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isTopShadowColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshTopShadowColors[iRowVersionOrig][strTheme];
            setTopShadowColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::innerContrastLineColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "innerContrastLineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshInnerContrastLineColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setInnerContrastLineColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setInnerContrastLineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshInnerContrastLineColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshInnerContrastLineColors[iRowVersionCurr][strTheme] = i_color;
            emit_innerContrastLineColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isInnerContrastLineColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isInnerContrastLineColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshInnerContrastLineColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshInnerContrastLineColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setInnerContrastLineColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setInnerContrastLineColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isInnerContrastLineColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshInnerContrastLineColors[iRowVersionOrig][strTheme];
            setInnerContrastLineColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::highlightColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "highlightColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshHighlightColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setHighlightColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setHighlightColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshHighlightColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshHighlightColors[iRowVersionCurr][strTheme] = i_color;
            emit_highlightColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isHighlightColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isHighlightColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshHighlightColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshHighlightColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setHighlightColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setHighlightColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isHighlightColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshHighlightColors[iRowVersionOrig][strTheme];
            setHighlightColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::highlightedTextColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "highlightedTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshHighlightedTextColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setHighlightedTextColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setHighlightedTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshHighlightedTextColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshHighlightedTextColors[iRowVersionCurr][strTheme] = i_color;
            emit_highlightedTextColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isHighlightedTextColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isHighlightedTextColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshHighlightedTextColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshHighlightedTextColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setHighlightedTextColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setHighlightedTextColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isHighlightedTextColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshHighlightedTextColors[iRowVersionOrig][strTheme];
            setHighlightedTextColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::outlineColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "outlineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshOutlineColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setOutlineColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setOutlineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshOutlineColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshOutlineColors[iRowVersionCurr][strTheme] = i_color;
            emit_outlineColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isOutlineColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isOutlineColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshOutlineColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshOutlineColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setOutlineColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setOutlineColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isOutlineColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshOutlineColors[iRowVersionOrig][strTheme];
            setOutlineColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::highlightedOutlineColor(
    const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "highlightedOutlineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshHighlightedOutlineColors[iRowVersion][strTheme];
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setHighlightedOutlineColor(
    const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setHighlightedOutlineColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshHighlightedOutlineColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshHighlightedOutlineColors[iRowVersionCurr][strTheme] = i_color;
            emit_highlightedOutlineColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isHighlightedOutlineColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isHighlightedOutlineColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshHighlightedOutlineColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshHighlightedOutlineColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setHighlightedOutlineColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setHighlightedOutlineColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isHighlightedOutlineColorSetToDefault(i_strTheme) ) {
            const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
            QColor clrOrig = m_arhshHighlightedOutlineColors[iRowVersionOrig][strTheme];
            setHighlightedOutlineColor(clrOrig, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//QColor CWindowsStyle::tabFrameColor(
//    const QString& i_strTheme, ERowVersion i_rowVersion)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
//        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
//        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod          */ "tabFrameColor",
//        /* strMethodInArgs    */ strMthInArgs );
//
//    QColor clr;
//    QString strTheme = i_strTheme;
//    if( i_strTheme.isEmpty() ) {
//        strTheme = m_strCurrentTheme;
//    }
//    if( isValidThemeName(strTheme) ) {
//        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
//        clr = m_arhshTabFrameColors[iRowVersion][strTheme];
//    }
//    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
//        mthTracer.setMethodReturn(clr.name());
//    }
//    return clr;
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::textColor(const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "textColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshTextColors[iRowVersion][strTheme];
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setTextColor( const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshTextColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshTextColors[iRowVersionCurr][strTheme] = i_color;
            emit_textColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isTextColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isTextColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshTextColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshTextColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setTextColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setTextColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isTextColorSetToDefault(i_strTheme) ) {
            QColor clrDefault = textColor(i_strTheme, ERowVersion::Original);
            setTextColor(clrDefault, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::windowTextColor(const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "windowTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshWindowTextColors[iRowVersion][strTheme];
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setWindowTextColor( const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setWindowTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshWindowTextColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshWindowTextColors[iRowVersionCurr][strTheme] = i_color;
            emit_windowTextColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isWindowTextColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isWindowTextColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshWindowTextColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshWindowTextColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setWindowTextColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setWindowTextColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isWindowTextColorSetToDefault(i_strTheme) ) {
            QColor clrDefault = windowTextColor(i_strTheme, ERowVersion::Original);
            setWindowTextColor(clrDefault, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::linkColor(const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "linkColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshLinkColors[iRowVersion][strTheme];
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setLinkColor( const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setLinkColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshLinkColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshLinkColors[iRowVersionCurr][strTheme] = i_color;
            emit_linkColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isLinkColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isLinkColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshLinkColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshLinkColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setLinkColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setLinkColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isLinkColorSetToDefault(i_strTheme) ) {
            QColor clrDefault = linkColor(i_strTheme, ERowVersion::Original);
            setLinkColor(clrDefault, i_strTheme);
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CWindowsStyle::buttonTextColor(const QString& i_strTheme, ERowVersion i_rowVersion) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
        strMthInArgs += ", Version: " + CEnumRowVersion(i_rowVersion).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "buttonTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QColor clr;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        int iRowVersion = CEnumRowVersion(i_rowVersion).enumeratorAsInt();
        clr = m_arhshButtonTextColors[iRowVersion][strTheme];
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(clr.name());
    }
    return clr;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setButtonTextColor( const QColor& i_color, const QString& i_strTheme )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color);
        strMthInArgs += ", " + QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setButtonTextColor",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        if( m_arhshButtonTextColors[iRowVersionCurr][strTheme] != i_color ) {
            m_arhshButtonTextColors[iRowVersionCurr][strTheme] = i_color;
            emit_buttonTextColorChanged(i_color, strTheme);
        }
    }
}

//------------------------------------------------------------------------------
bool CWindowsStyle::isButtonTextColorSetToDefault(const QString& i_strTheme) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "isButtonTextColorSetToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    bool bIs = false;
    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        const int iRowVersionOrig = static_cast<int>(ERowVersion::Original);
        const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);
        QColor clrOrig = m_arhshButtonTextColors[iRowVersionOrig][strTheme];
        QColor clrCurr = m_arhshButtonTextColors[iRowVersionCurr][strTheme];
        bIs = (clrOrig == clrCurr);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(bIs);
    }
    return bIs;
}

//------------------------------------------------------------------------------
void CWindowsStyle::setButtonTextColorToDefault(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_strTheme.isEmpty() ? "CurrTheme: " + m_strCurrentTheme : i_strTheme);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setButtonTextColorToDefault",
        /* strMethodInArgs    */ strMthInArgs );

    QString strTheme = i_strTheme;
    if( i_strTheme.isEmpty() ) {
        strTheme = m_strCurrentTheme;
    }
    if( isValidThemeName(strTheme) ) {
        if( !isButtonTextColorSetToDefault(i_strTheme) ) {
            QColor clrDefault = buttonTextColor(i_strTheme, ERowVersion::Original);
            setButtonTextColor(clrDefault, i_strTheme);
        }
    }
}

/*==============================================================================
protected: // instance methods (reference counter)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of active references to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references.
*/
//------------------------------------------------------------------------------
int CWindowsStyle::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CWindowsStyle::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references after decrement.
*/
//------------------------------------------------------------------------------
int CWindowsStyle::decrementRefCount()
{
    if( m_iRefCount <= 0)
    {
        throw CException(__FILE__, __LINE__, EResultObjRefCounterIsZero, ClassName() + "::" + objectName());
    }
    return --m_iRefCount;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWindowsStyle::onCurrentThemeChanged(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onCurrentThemeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    const int iRowVersionCurr = static_cast<int>(ERowVersion::Current);

    emit_baseColorChanged(
        m_arhshBaseColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_popupWindowColorChanged(
        m_arhshPopupWindowColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_backgroundStartColorChanged(
        m_arhshBackgroundStartColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_backgroundStopColorChanged(
        m_arhshBackgroundStopColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_buttonColorChanged(
        m_arhshButtonColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_shadowColorChanged(
        m_arhshShadowColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_topShadowColorChanged(
        m_arhshTopShadowColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_innerContrastLineColorChanged(
        m_arhshInnerContrastLineColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_highlightColorChanged(
        m_arhshHighlightColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_highlightedTextColorChanged(
        m_arhshHighlightedTextColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_outlineColorChanged(
        m_arhshOutlineColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_highlightedOutlineColorChanged(
        m_arhshHighlightedOutlineColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_textColorChanged(
        m_arhshTextColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_windowTextColorChanged(
        m_arhshWindowTextColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_linkColorChanged(
        m_arhshLinkColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
    emit_buttonTextColorChanged(
        m_arhshButtonTextColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWindowsStyle::emit_currentThemeChanged(const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_currentThemeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit currentThemeChanged(i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_baseColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_baseColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit baseColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_popupWindowColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_popupWindowColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit popupWindowColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_backgroundStartColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_backgroundStartColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit backgroundStartColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_backgroundStopColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_backgroundStopColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit backgroundStopColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_buttonColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_buttonColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit buttonColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_shadowColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_shadowColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit shadowColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_topShadowColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_topShadowColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit topShadowColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_innerContrastLineColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_innerContrastLineColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit innerContrastLineColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_highlightColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_highlightColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit highlightColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_highlightedTextColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_highlightedTextColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit highlightedTextColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_outlineColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_outlineColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit outlineColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_highlightedOutlineColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_highlightedOutlineColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit highlightedOutlineColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_textColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_textColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit textColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_windowTextColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_windowTextColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit windowTextColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_linkColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_linkColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit linkColorChanged(i_color, i_strTheme);
}

//------------------------------------------------------------------------------
void CWindowsStyle::emit_buttonTextColorChanged(const QColor& i_color, const QString& i_strTheme)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = GUI::qColor2Str(i_color) + ", " + i_strTheme;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "emit_buttonTextColorChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit buttonTextColorChanged(i_color, i_strTheme);
}
