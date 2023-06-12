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

#include "ZSDiagram/ZSDiagramProcPixmap.h"
#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qprinter.h>
#else
#include <QtPrintSupport/qprinter.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;

//lint -e834

/*******************************************************************************
class CPixmapDiagram : public CDataDiagram
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

const char* CPixmapDiagram::g_arszImageFormatFileFilter[EImageFormatCount+1] = {
    "*.bmp",
    "*.jpg",
    "*.xpm",
    "*.png",
    nullptr
};
const char* CPixmapDiagram::g_arszImageFormatFileExt[EImageFormatCount+1] = {
    "bmp",
    "jpg",
    "xpm",
    "png",
    nullptr
};
const int CPixmapDiagram::g_ariImageFormat[EImageFormatCount+1] = {
    EImageFormatBMP,
    EImageFormatJPG,
    EImageFormatXPM,
    EImageFormatPNG,
    -1
};
const char* CPixmapDiagram::g_arszQtImageFormat[EImageFormatCount+1] = {
    "BMP",
    "JPEG",
    "XPM",
    "PNG",
    nullptr
};

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
EImageFormat CPixmapDiagram::GetImageFormat( const QString& i_strImageFormat )
//------------------------------------------------------------------------------
{
    EImageFormat imageFormat = EImageFormatUndefined;
    int          idxFileExt;

    for( idxFileExt = 0; idxFileExt < EImageFormatCount; idxFileExt++ )
    {
        if( g_arszImageFormatFileExt[idxFileExt] != nullptr )
        {
            #if QT_VERSION >= 0x040100
            if( QString(g_arszImageFormatFileExt[idxFileExt]).contains(i_strImageFormat,Qt::CaseInsensitive) )
            #else
            if( QString(g_arszImageFormatFileExt[idxFileExt]).contains(i_strImageFormat,false) )
            #endif
            {
                imageFormat = static_cast<EImageFormat>(g_ariImageFormat[idxFileExt]);
                break;
            }
        }
    }
    if( imageFormat == EImageFormatUndefined )
    {
        for( idxFileExt = 0; idxFileExt < EImageFormatCount; idxFileExt++ )
        {
            if( g_arszImageFormatFileFilter[idxFileExt] != nullptr )
            {
                #if QT_VERSION >= 0x040100
                if( QString(g_arszImageFormatFileFilter[idxFileExt]).contains(i_strImageFormat,Qt::CaseInsensitive) )
                #else
                if( QString(g_arszImageFormatFileFilter[idxFileExt]).contains(i_strImageFormat,false) )
                #endif
                {
                    imageFormat = static_cast<EImageFormat>(g_ariImageFormat[idxFileExt]);
                    break;
                }
            }
        }
    }
    if( imageFormat == EImageFormatUndefined )
    {
        for( idxFileExt = 0; idxFileExt < EImageFormatCount; idxFileExt++ )
        {
            if( g_arszQtImageFormat[idxFileExt] != nullptr )
            {
                #if QT_VERSION >= 0x040100
                if( QString(g_arszQtImageFormat[idxFileExt]).contains(i_strImageFormat,Qt::CaseInsensitive) )
                #else
                if( QString(g_arszQtImageFormat[idxFileExt]).contains(i_strImageFormat,false) )
                #endif
                {
                    imageFormat = static_cast<EImageFormat>(g_ariImageFormat[idxFileExt]);
                    break;
                }
            }
        }
    }
    return imageFormat;

} // GetImageFormat

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPixmapDiagram::CPixmapDiagram(
    const QString&     i_strObjName,
    EDiagramUpdateType i_updateType ) :
//------------------------------------------------------------------------------
    CDataDiagram(i_strObjName,i_updateType),
    m_colErase(Qt::gray),
    m_colBgPartCenter(Qt::black),
    m_pPixmap(nullptr),
    m_pPrinter(nullptr),
    m_size(),
    m_cyMinimumHeightPartTop(0),
    m_cyMinimumHeightPartBottom(0),
    m_cxMinimumWidthPartLeft(0),
    m_cxMinimumWidthPartRight(0),
    m_iMarginTop(4),
    m_iMarginBottom(4),
    m_iMarginLeft(4),
    m_iMarginRight(4),
    m_rectPartTop(),
    m_rectPartBottom(),
    m_rectPartLeft(),
    m_rectPartRight(),
    m_rectPartCenter(),
    m_pFrameStyle(nullptr),
    m_pFrameStylePartCenter(nullptr),
    m_rectFramePartCenterTopLineInv(),
    m_rectFramePartCenterBottomLineInv(),
    m_rectFramePartCenterLeftLineInv(),
    m_rectFramePartCenterRightLineInv(),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjEvents(nullptr),
    m_pTrcAdminObjLayout(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), m_strObjName);
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", m_strObjName);
    m_pTrcAdminObjEvents = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Events", m_strObjName);
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Layout", m_strObjName);
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Validate", m_strObjName);
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", UpdateType: " + diagramUpdateType2Str(i_updateType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CPixmapDiagram::~CPixmapDiagram()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pPixmap;
    }
    catch(...)
    {
    }
    m_pPixmap = nullptr;

    try
    {
        delete m_pPrinter;
    }
    catch(...)
    {
    }
    m_pPrinter = nullptr;

    try
    {
        delete m_pFrameStyle;
    }
    catch(...)
    {
    }
    m_pFrameStyle = nullptr;

    try
    {
        delete m_pFrameStylePartCenter;
    }
    catch(...)
    {
    }
    m_pFrameStylePartCenter = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjEvents);
    m_pTrcAdminObjEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjLayout);
    m_pTrcAdminObjLayout = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjValidate);
    m_pTrcAdminObjValidate = nullptr;

} // dtor

/*==============================================================================
public: // copy ctor not allowed but diagrams may be cloned
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram* CPixmapDiagram::clone( EDiagramUpdateType i_diagramUpdateType ) const
//------------------------------------------------------------------------------
{
    CDataDiagram* pDiagram = nullptr;

    if( i_diagramUpdateType < EDiagramUpdateTypePixmap )
    {
        pDiagram = CDataDiagram::clone(i_diagramUpdateType);
    }
    else
    {
        CPixmapDiagram* pPixmapDiagram = new CPixmapDiagram(m_strObjName);

        // Data Diagram
        //-------------

        pPixmapDiagram->m_uUpdateFlags = m_uUpdateFlags;
        pPixmapDiagram->m_measState = m_measState;
        pPixmapDiagram->m_measMode = m_measMode;
        pPixmapDiagram->m_iMeasType = m_iMeasType;

        for (int idx = 0; idx < CEnumScaleDir::count(); idx++)
        {
            pPixmapDiagram->m_arSpacing[idx] = m_arSpacing[idx];
        }
        for (int idx = 0; idx < m_arpDiagScales.size(); ++idx)
        {
            m_arpDiagScales[idx]->clone(pPixmapDiagram);
        }
        for (int idx = 0; idx < m_arpDiagTraces.size(); ++idx)
        {
            m_arpDiagTraces[idx]->clone(pPixmapDiagram);
        }
        for (int idx = 0; idx < m_arpDiagObjs.size(); ++idx)
        {
            CDiagObj* pDiagObj = m_arpDiagObjs[idx]->clone(pPixmapDiagram);
            QString strDiagObjKey = m_arpDiagObjs[idx]->className() + "::" + m_arpDiagObjs[idx]->getObjName();
            pPixmapDiagram->m_hshpDiagObjs[strDiagObjKey] = pDiagObj;
        }

        // Pixmap Diagram
        //---------------

        pPixmapDiagram->m_colErase = m_colErase;
        pPixmapDiagram->m_colBgPartCenter = m_colBgPartCenter;

        pPixmapDiagram->m_size = m_size;
        pPixmapDiagram->m_cyMinimumHeightPartTop = m_cyMinimumHeightPartTop;
        pPixmapDiagram->m_cyMinimumHeightPartBottom = m_cyMinimumHeightPartBottom;
        pPixmapDiagram->m_cxMinimumWidthPartLeft = m_cxMinimumWidthPartLeft;
        pPixmapDiagram->m_cxMinimumWidthPartRight = m_cxMinimumWidthPartRight;
        pPixmapDiagram->m_iMarginTop = m_iMarginTop;
        pPixmapDiagram->m_iMarginBottom = m_iMarginBottom;
        pPixmapDiagram->m_iMarginLeft = m_iMarginLeft;
        pPixmapDiagram->m_iMarginRight = m_iMarginRight;
        pPixmapDiagram->m_rectPartTop = m_rectPartTop;
        pPixmapDiagram->m_rectPartBottom = m_rectPartBottom;
        pPixmapDiagram->m_rectPartLeft = m_rectPartLeft;
        pPixmapDiagram->m_rectPartRight = m_rectPartRight;
        pPixmapDiagram->m_rectPartCenter = m_rectPartCenter;

        if( m_pFrameStyle != nullptr )
        {
            pPixmapDiagram->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
        }
        if( m_pFrameStylePartCenter != nullptr )
        {
            pPixmapDiagram->m_pFrameStylePartCenter = new SFrameStyle(*m_pFrameStylePartCenter);
        }
        pDiagram = pPixmapDiagram;
    }
    return pDiagram;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( const QString& i_strFileName, EImageFormat i_imageFormat )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "save",
        /* strAddInfo   */ "" );

    bool bSuccess = false;

    if( m_uUpdateFlags != EUpdateNone )
    {
        // Update pixmap (but not the widget)
        updateData();
        updatePixmap();
    }

    if( m_pPixmap != nullptr && i_strFileName.length() > 0
     && i_imageFormat >= EImageFormatMin && i_imageFormat <= EImageFormatMax )
    {
        QString strFileName = i_strFileName;
        bool    bFileNameHasValidExtension = false;
        int     idxImageFormat;

        for( idxImageFormat = 0; idxImageFormat < EImageFormatCount; idxImageFormat++ )
        {
            #if QT_VERSION >= 0x040100
            if( strFileName.endsWith( "." + QString(g_arszImageFormatFileExt[idxImageFormat]), Qt::CaseInsensitive ) )
            #else
            if( strFileName.endsWith( "." + QString(g_arszImageFormatFileExt[idxImageFormat]), false ) )
            #endif
            {
                bFileNameHasValidExtension = true;
                break;
            }
        }
        if( !bFileNameHasValidExtension )
        {
            strFileName.append( "." + QString(g_arszImageFormatFileExt[i_imageFormat]) );
        }
        bSuccess = m_pPixmap->save( strFileName, g_arszQtImageFormat[i_imageFormat] );
    }
    return bSuccess;

} // save

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( const QString& i_strFileName, const QString& i_strQtImageFormat )
//------------------------------------------------------------------------------
{
    return save( i_strFileName, GetImageFormat(i_strQtImageFormat) );
}

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( QIODevice* i_pDevice, EImageFormat i_imageFormat )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "save",
        /* strAddInfo   */ "" );

    bool bSuccess = false;

    if( m_uUpdateFlags != EUpdateNone )
    {
        // Update pixmap (but not the widget)
        updateData();
        updatePixmap();
    }

    if( m_pPixmap != nullptr && i_pDevice != nullptr
     && i_imageFormat >= EImageFormatMin && i_imageFormat <= EImageFormatMax )
    {
        bSuccess = m_pPixmap->save( i_pDevice, g_arszQtImageFormat[i_imageFormat] );
    }
    return bSuccess;

} // save

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( QIODevice* i_pDevice, const QString& i_strQtImageFormat )
//------------------------------------------------------------------------------
{
    return save( i_pDevice, GetImageFormat(i_strQtImageFormat) );
}

//------------------------------------------------------------------------------
void CPixmapDiagram::print( const QPrinter* i_pPrinter ) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "print",
        /* strAddInfo   */ "" );

    if( i_pPrinter != nullptr )
    {
        //QPainter painter;

        //i_pPrinter->setFullPage(true);

        //if( !painter.begin(i_pPrinter) )
        //{
        //    return;
        //}

        //painter.drawText(20,20,"hallo");
        //paintDiagObjects(&painter);

        //QPaintDeviceMetrics metrics( painter.device() );

        //int dpiy = metrics.logicalDpiY();
        //int iMargin = static_cast<int>( (2.0/2.54)*dpiy ); // 2 cm margins

        //QRect rctView( iMargin, iMargin, metrics.width() - 2*iMargin, metrics.height() - 2*iMargin );

        //QSimpleRichText richText( QStyleSheet::convertFromPlainText(medit->text()),
        //                          QFont(),
        //                          medit->context(),
        //                          medit->styleSheet(),
        //                          medit->mimeSourceFactory(),
        //                          rctView.height() );
        //richText.setWidth( &painter, rctView.width() );

        //int iPage = 1;

        //do {

            //richText.draw( &painter, iMargin, iMargin, rctView, colorGroup() );

            //rctView.moveBy( 0, rctView.height() );

            //painter.translate( 0 , -rctView.height() );

            //painter.drawText(
            //    rctView.right() - painter.fontMetrics().width( QString::number(iPage) ),
            //    rctView.bottom() + painter.fontMetrics().ascent() + 5,
            //    QString::number( iPage ) );

            //if ( rctView.top() - iMargin >= richText.height() )
            //{
            //    break;
            //}

            //QString msg( "Printing (page " );
            //msg += QString::number( ++pageNo );
            //msg += ")...";

            //i_pPrinter->newPage();

            //iPage++;

        //} while( true );

    } // if( i_pPrinter != nullptr )

} // print

/*==============================================================================
public: // instance methods to set and get the attributes of the diagram
==============================================================================*/

//------------------------------------------------------------------------------
void CPixmapDiagram::setWidth( int i_cxWidth )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_cxWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strTrcMsg );

    QSize sizeNew = m_size;
    sizeNew.setWidth(i_cxWidth);
    setSize(sizeNew);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getWidth() const
//------------------------------------------------------------------------------
{
    return m_size.width();
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setHeight( int i_cyHeight )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_cyHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strTrcMsg );

    QSize sizeNew = m_size;
    sizeNew.setHeight(i_cyHeight);
    setSize(sizeNew);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getHeight() const
//------------------------------------------------------------------------------
{
    return m_size.height();
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setSize( const QSize& i_size )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(m_size.width());
        strTrcMsg +=  "," + QString::number(m_size.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strTrcMsg );

    QSize sizePrev     = m_size;
    bool  bSizeChanged = false;

    if( m_size.width()  != i_size.width()
     || m_size.height() != i_size.height() )
    {
        bSizeChanged = true;
        m_size = i_size;
    }

    if( bSizeChanged )
    {
        int cxWidthDist  = m_size.width() - sizePrev.width();
        int cyHeightDist = m_size.height() - sizePrev.height();

        // Before updating the layout the scale objects will be provided with their
        // (probable) new extents. Those extent may change later on if the real
        // size of the diagram areas will be calculated as some objects may change
        // their size according to the number and position of the scale division lines.
        // But setting the (probable) scale geometry in advance may save time ...
        m_rectPartCenter.setWidth( m_rectPartCenter.width() + cxWidthDist );
        m_rectPartCenter.setHeight( m_rectPartCenter.height() + cyHeightDist );

        // It is NOT sufficient to mark layout processing as necessary.
        // Even if on executing the layout process the geometry of the objects
        // might be changed (if necessary) and the necessary update flags would
        // be set anyway. To recalculate the geometry of the objects some of those
        // objects need to recalculate their internal data in advance to provide
        // the correct sizeHint for the diagram. E.g. on resizing the diagram the
        // number of division lines might change and therefore an axis label object
        // need to change the number of digits to scale the diagram.
        invalidate(nullptr,EUpdateLayoutDataPixmapWidget);

        // Layout processing is sufficient as a paint event will follow anyway and there
        // are more resive events than paint events. So it looks like that its more
        // efficient to let the objects recalculate their data and graphical output
        // later within the paint event method.
        //CDataDiagram::update(EUpdateLayout);
    }

} // setSize

//------------------------------------------------------------------------------
QSize CPixmapDiagram::getSize() const
//------------------------------------------------------------------------------
{
    return m_size;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMarginTop",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginTop = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMarginBottom",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginBottom = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMarginLeft",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginLeft = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMarginRight",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginRight = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartTop() const
//------------------------------------------------------------------------------
{
    return m_rectPartTop;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartBottom() const
//------------------------------------------------------------------------------
{
    return m_rectPartBottom;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartLeft() const
//------------------------------------------------------------------------------
{
    return m_rectPartLeft;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartRight() const
//------------------------------------------------------------------------------
{
    return m_rectPartRight;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartCenter() const
//------------------------------------------------------------------------------
{
    return m_rectPartCenter;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setColBgPartCenter( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setColBgPartCenter",
        /* strAddInfo   */ "" );

    m_colBgPartCenter = i_col;
    invalidate(nullptr,EUpdatePixmapWidget);
}

//------------------------------------------------------------------------------
QColor CPixmapDiagram::getColBgPartCenter() const
//------------------------------------------------------------------------------
{
    return m_colBgPartCenter;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumHeightPartTop( int i_cyHeight )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_cyHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMinimumHeightPartTop",
        /* strAddInfo   */ strTrcMsg );

    m_cyMinimumHeightPartTop = i_cyHeight;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumHeightPartTop() const
//------------------------------------------------------------------------------
{
    return m_cyMinimumHeightPartTop;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumHeightPartBottom( int i_cyHeight )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_cyHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMinimumHeightPartBottom",
        /* strAddInfo   */ strTrcMsg );

    m_cyMinimumHeightPartBottom = i_cyHeight;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumHeightPartBottom() const
//------------------------------------------------------------------------------
{
    return m_cyMinimumHeightPartBottom;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumWidthPartLeft( int i_cxWidth )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_cxWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMinimumWidthPartLeft",
        /* strAddInfo   */ strTrcMsg );

    m_cxMinimumWidthPartLeft = i_cxWidth;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumWidthPartLeft() const
//------------------------------------------------------------------------------
{
    return m_cxMinimumWidthPartLeft;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumWidthPartRight( int i_cxWidth )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strTrcMsg = QString::number(i_cxWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMinimumWidthPartRight",
        /* strAddInfo   */ strTrcMsg );

    m_cxMinimumWidthPartRight = i_cxWidth;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumWidthPartRight() const
//------------------------------------------------------------------------------
{
    return m_cxMinimumWidthPartRight;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setColErase( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setColErase",
        /* strAddInfo   */ "" );

    m_colErase = i_col;
    invalidate(nullptr,EUpdatePixmapWidget);
}

//------------------------------------------------------------------------------
QColor CPixmapDiagram::getColErase() const
//------------------------------------------------------------------------------
{
    return m_colErase;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFrameStyle",
        /* strAddInfo   */ "" );

    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;

} // setFrameStyle

//------------------------------------------------------------------------------
SFrameStyle* CPixmapDiagram::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setFrameStylePartCenter( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFrameStylePartCenter",
        /* strAddInfo   */ "" );

    delete m_pFrameStylePartCenter;
    m_pFrameStylePartCenter = i_pFrameStyle;

} // setFrameStylePartCenter

//------------------------------------------------------------------------------
SFrameStyle* CPixmapDiagram::getFrameStylePartCenter()
//------------------------------------------------------------------------------
{
    return m_pFrameStylePartCenter;
}

//------------------------------------------------------------------------------
QPixmap* CPixmapDiagram::getPixmap()
//------------------------------------------------------------------------------
{
    return m_pPixmap;
}

/*==============================================================================
public: // overridables ob base class CDataDiagram
==============================================================================*/

//lint -esym(818,i_pDiagObj)
//------------------------------------------------------------------------------
void CPixmapDiagram::invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName();
        }
        strTrcMsg += updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        unsigned int uUpdateFlagsForObjects = EUpdateNone;

        // If a bit is set the corresponding update process need to be executed
        // (keeping the current update processes that still need to be executed):
        m_uUpdateFlags |= i_uUpdateFlags;

        // If method was called by the diagram itself ..
        if( i_pDiagObj == nullptr )
        {
            // .. we pass on the update flags to the objects.
            uUpdateFlagsForObjects = i_uUpdateFlags;
        }
        // If the method was called by one of the diagram objects ..
        else
        {
            // If pixmap processing is necessary for one object
            // all other objects also need to be redrawn.
            if( i_uUpdateFlags & EUpdatePixmap )
            {
                uUpdateFlagsForObjects = EUpdatePixmap;
            }
        }

        // If layout processing was announced to the diagram because of e.g. changing
        // the visibility of a diagram object the other objects also need to be invalidated.
        // It is not sufficient to have the layout update flag set at the diagram even if
        // this leads to "setGeometry" calls - if necessary. On changing the geometry of an
        // object the object will be invalidated anyway. But not if the object does not change
        // its geometry ...

        // If pixmap processing is necessary for one object
        // all other objects also need to be redrawn.
        if( uUpdateFlagsForObjects != EUpdateNone )
        {
            for (CDiagObj* pDiagObj : m_arpDiagObjs)
            {
                // Don't invalidate the sender object. Its not necessary and
                // might lead to endless recursive calls.
                if( pDiagObj != i_pDiagObj )
                {
                    pDiagObj->invalidate(uUpdateFlagsForObjects,false);
                }
            }
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // invalidate
//lint +esym(818,i_pDiagObj)

/*==============================================================================
protected: // overridables of base class CDataDiagram
==============================================================================*/

//------------------------------------------------------------------------------
void CPixmapDiagram::update( CDiagObj* i_pDiagObj, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = QString(i_pDiagObj == nullptr ? "null" : i_pDiagObj->getObjName());
        strMthInArgs += ", " + qRect2Str(i_rect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

    CDataDiagram::update(i_pDiagObj, i_rect);

    updatePixmap();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

} // update

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CDataDiagram
==============================================================================*/

//------------------------------------------------------------------------------
void CPixmapDiagram::updateLayout()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    QSize sizePixmap;

    #if QT_VERSION >= 0x040100

    delete m_pPixmap;
    m_pPixmap = nullptr;
    m_pPixmap = new QPixmap(m_size);

    #else

    if( m_size.isEmpty() )
    {
        delete m_pPixmap;
        m_pPixmap = nullptr;
    }
    else
    {
        if( m_pPixmap == nullptr )
        {
            m_pPixmap = new QPixmap(m_size);
        }
        else if( m_pPixmap->size() != m_size )
        {
            m_pPixmap->resize(m_size);
        }
    }
    #endif

    // Please note that the center area of the diagram is used by e.g. grid or
    // curve objects and is considered to be expandable.
    // All other (used) areas (left, top, right, bottom) are usually used
    // by axis labels and are considered to have a minimum size which will be
    // queried by the "sizeHint" methods.

    // As the size of the center area is expandable its width and height may
    // need to be changed. The scale objects extent correspond to the extent
    // of the center area and therefore the number and position of the division
    // lines depend on the size of the center area. The sizeHint of some objects
    // (e.g. axis labels) again depend on the number and position of the division
    // lines. And again the center area may need to be resized.
    // So the whole layout stuff is an iterative process and will be repeated until
    // the size of center area is no longer changing ...

    QRect rectPartCenterPrev;
    int iIterations;
    int cyPartTopHeight;
    int cyPartBottomHeight;
    int cxPartLeftWidth;
    int cxPartRightWidth;
    int cxPartCenterWidth;
    int cyPartCenterHeight;
    int iFrameWidth = 0;
    int iFrameWidthPartCenter = 0;
    int iDiagObjExtent;
    int* ariDiagObjOffset = new int[getDiagObjsCount()];
    int* ariDiagObjExtent = new int[getDiagObjsCount()];
    ELayoutPos* arLytPosDiagObj = new ELayoutPos[getDiagObjsCount()];
    int* piDiagObjOffset;
    int* piDiagObjExtent;
    ELayoutPos* pLytPosDiagObj;
    QRect rectDiagObjContent;

    if( m_pFrameStyle != nullptr )
    {
        iFrameWidth = m_pFrameStyle->getFrameWidth();
    }

    if( m_pFrameStylePartCenter != nullptr )
    {
        QRect rectFramePartCenterTopLineInv;
        QRect rectFramePartCenterBottomLineInv;
        QRect rectFramePartCenterLeftLineInv;
        QRect rectFramePartCenterRightLineInv;

        iFrameWidthPartCenter = m_pFrameStylePartCenter->getFrameWidth();

        rectFramePartCenterTopLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterBottomLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setTop( m_rectPartCenter.bottom() );
        rectFramePartCenterBottomLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterLeftLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        rectFramePartCenterRightLineInv.setLeft( m_rectPartCenter.right() );
        rectFramePartCenterRightLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        m_rectFramePartCenterTopLineInv    |= rectFramePartCenterTopLineInv;
        m_rectFramePartCenterBottomLineInv |= rectFramePartCenterBottomLineInv;
        m_rectFramePartCenterLeftLineInv   |= rectFramePartCenterLeftLineInv;
        m_rectFramePartCenterRightLineInv  |= rectFramePartCenterRightLineInv;
    }

    // Before updating the layout the scale objects will be provided with their
    // (probable) new extents. Those extent may change later on if the real
    // size of the diagram areas will be calculated as some objects may change
    // their size according to the number and position of the scale division lines.
    // But setting the (probable) scale geometry in advance may save time ...
    for (CDiagScale* pDiagScale : m_arpDiagScales)
    {
        // If the scale will be changed the method "scaleChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed scale.
        switch( pDiagScale->getScaleDir() )
        {
            case EScaleDir::X:
            {
                if( pDiagScale->getMinValPix() != m_rectPartCenter.left()
                 || pDiagScale->getMaxValPix() != m_rectPartCenter.right() )
                {
                    pDiagScale->setGeometry(m_rectPartCenter.left(),m_rectPartCenter.right());
                }
                break;
            }
            case EScaleDir::Y:
            {
                if( pDiagScale->getMinValPix() != m_rectPartCenter.bottom()
                 || pDiagScale->getMaxValPix() != m_rectPartCenter.top() )
                {
                    pDiagScale->setGeometry(m_rectPartCenter.bottom(),m_rectPartCenter.top());
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    // As long as the maximum number of iterations is not exceeded ...
    for( iIterations = 0; iIterations < 10; iIterations++ )
    {
        cyPartTopHeight    = 0;
        cyPartBottomHeight = 0;
        cxPartLeftWidth    = 0;
        cxPartRightWidth   = 0;
        cxPartCenterWidth  = 0;
        cyPartCenterHeight = 0;
        rectPartCenterPrev = m_rectPartCenter;

        // Trigger the recalculation of the scale division lines for all scale objects.
        for (CDiagScale* pDiagScale : m_arpDiagScales)
        {
            // If the scale will be changed the method "scaleChanged" of the diagram will be
            // called invalidating the data of all objects linked to the changed scale.
            pDiagScale->update();
        }

        // Trigger the recalculation of internal constants within the traces. Those are used
        // to analyze the trace e.g. converting physical values into pixel coordinates and
        // vice versa, value resolutions and other things.
        for (CDiagTrace* pDiagTrace : m_arpDiagTraces)
        {
            // If the trace will be changed the method "traceChanged" of the diagram will be
            // called invalidating the data of all objects linked to the changed trace.
            pDiagTrace->update(EUpdateLayout);
        }

        // Trigger the recalculation of the output data for all diagram objects
        // (e.g. the diagram axis labels have to calculate the rectangles to output
        // the scale division lines, the curves have to transform the X/Y values
        // into point arrays, etc.).
        for (CDiagObj* pDiagObj : m_arpDiagObjs)
        {
            // Please note that also invisible objects need to mark their dirty rectangles
            // during widget processing. For invisible objects layout processing is not
            // really necessary here. But maybe some objects need layout processing to
            // calculate their dirty rectangles. For this layout processing will also be
            // executed for invisible objects. But that should not slow down the
            // the system as only visible objects should really recalculate their data.
            pDiagObj->update(EUpdateLayout);
        }

        piDiagObjOffset = &ariDiagObjOffset[0];
        piDiagObjExtent = &ariDiagObjExtent[0];
        pLytPosDiagObj  = &arLytPosDiagObj[0];

        for (CDiagObj* pDiagObj : m_arpDiagObjs)
        {
            if( pDiagObj->isVisible() )
            {
                switch( pDiagObj->getLayoutPos() )
                {
                    case ELayoutPosTop:
                    {
                        iDiagObjExtent   = pDiagObj->sizeHint().height();
                        *piDiagObjOffset = cyPartTopHeight;
                        *piDiagObjExtent = iDiagObjExtent;
                        cyPartTopHeight += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosBottom:
                    {
                        iDiagObjExtent      = pDiagObj->sizeHint().height();
                        *piDiagObjOffset    = cyPartBottomHeight;
                        *piDiagObjExtent    = iDiagObjExtent;
                        cyPartBottomHeight += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosLeft:
                    {
                        iDiagObjExtent   = pDiagObj->sizeHint().width();
                        *piDiagObjOffset = cxPartLeftWidth;
                        *piDiagObjExtent = iDiagObjExtent;
                        cxPartLeftWidth += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosRight:
                    {
                        iDiagObjExtent    = pDiagObj->sizeHint().width();
                        *piDiagObjOffset  = cxPartRightWidth;
                        *piDiagObjExtent  = iDiagObjExtent;
                        cxPartRightWidth += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosCenter:
                    {
                        *piDiagObjOffset = 0;
                        *piDiagObjExtent = 0;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else // if( !pDiagObj->isVisible() )
            {
                *piDiagObjOffset = 0;
                *piDiagObjExtent = 0;
            }
            *pLytPosDiagObj = pDiagObj->getLayoutPos();

            piDiagObjOffset++;
            piDiagObjExtent++;
            pLytPosDiagObj++;
        }

        if( m_cyMinimumHeightPartTop > 0 && cyPartTopHeight < m_cyMinimumHeightPartTop )
        {
            cyPartTopHeight = m_cyMinimumHeightPartTop;
        }
        if( m_cyMinimumHeightPartBottom > 0 && cyPartBottomHeight < m_cyMinimumHeightPartBottom )
        {
            cyPartBottomHeight = m_cyMinimumHeightPartBottom;
        }
        if( m_cxMinimumWidthPartLeft > 0 && cxPartLeftWidth < m_cxMinimumWidthPartLeft )
        {
            cxPartLeftWidth = m_cxMinimumWidthPartLeft;
        }
        if( m_cxMinimumWidthPartRight > 0 && cxPartRightWidth < m_cxMinimumWidthPartRight )
        {
            cxPartRightWidth = m_cxMinimumWidthPartRight;
        }

        // If we got the minimum sizes for each border area we are able to calculate the
        // size of the (expandable) center area and to set the new rectangle extents
        // and positions of the diagram objects.
        cxPartCenterWidth  = m_size.width() - m_iMarginLeft - m_iMarginRight - 2*iFrameWidth - cxPartLeftWidth - cxPartRightWidth - 2*iFrameWidthPartCenter;     //lint !e834
        cyPartCenterHeight = m_size.height() - m_iMarginTop - m_iMarginBottom - 2*iFrameWidth - cyPartTopHeight - cyPartBottomHeight - 2*iFrameWidthPartCenter;  //lint !e834

        // Calculate the center area
        m_rectPartCenter.setLeft(m_iMarginLeft+iFrameWidth+cxPartLeftWidth+iFrameWidthPartCenter);
        m_rectPartCenter.setTop(m_iMarginTop+iFrameWidth+cyPartTopHeight+iFrameWidthPartCenter);
        m_rectPartCenter.setWidth(cxPartCenterWidth);
        m_rectPartCenter.setHeight(cyPartCenterHeight);

        // If the size of the center area did not change ...
        if( m_rectPartCenter == rectPartCenterPrev )
        {
            break;
        }

        // Set the scale extent of the scale objects. Please note that the scales just
        // store their new extent but do not recalculate the division lines.
        // This must be triggered later ...
        for (CDiagScale* pDiagScale : m_arpDiagScales)
        {
            // If the scale will be changed the method "scaleChanged" of the diagram will be
            // called invalidating the data of all objects linked to the changed scale.
            switch( pDiagScale->getScaleDir() )
            {
                case EScaleDir::X:
                {
                    if( pDiagScale->getMinValPix() != m_rectPartCenter.left()
                     || pDiagScale->getMaxValPix() != m_rectPartCenter.right() )
                    {
                        pDiagScale->setGeometry(m_rectPartCenter.left(),m_rectPartCenter.right());
                    }
                    break;
                }
                case EScaleDir::Y:
                {
                    if( pDiagScale->getMinValPix() != m_rectPartCenter.bottom()
                     || pDiagScale->getMaxValPix() != m_rectPartCenter.top() )
                    {
                        pDiagScale->setGeometry(m_rectPartCenter.bottom(),m_rectPartCenter.top());
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    } // for( iIterations < 10 )

    if( m_pFrameStylePartCenter != nullptr )
    {
        QRect rectFramePartCenterTopLineInv;
        QRect rectFramePartCenterBottomLineInv;
        QRect rectFramePartCenterLeftLineInv;
        QRect rectFramePartCenterRightLineInv;

        iFrameWidthPartCenter = m_pFrameStylePartCenter->getFrameWidth();

        rectFramePartCenterTopLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterBottomLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setTop( m_rectPartCenter.bottom() );
        rectFramePartCenterBottomLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterLeftLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        rectFramePartCenterRightLineInv.setLeft( m_rectPartCenter.right() );
        rectFramePartCenterRightLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        m_rectFramePartCenterTopLineInv    |= rectFramePartCenterTopLineInv;
        m_rectFramePartCenterBottomLineInv |= rectFramePartCenterBottomLineInv;
        m_rectFramePartCenterLeftLineInv   |= rectFramePartCenterLeftLineInv;
        m_rectFramePartCenterRightLineInv  |= rectFramePartCenterRightLineInv;
    }

    // Calculate the border areas which overlap at the corners:
    // (could be optimised by just calculating the used areas)
    m_rectPartTop.setLeft(m_iMarginLeft+iFrameWidth);
    m_rectPartTop.setTop(m_iMarginTop+iFrameWidth);
    m_rectPartTop.setWidth(m_size.width()-m_iMarginLeft-m_iMarginRight-2*iFrameWidth);
    m_rectPartTop.setHeight(cyPartTopHeight);
    m_rectPartBottom.setLeft(m_iMarginLeft+iFrameWidth);
    m_rectPartBottom.setTop(m_iMarginTop+iFrameWidth+cyPartTopHeight+cyPartCenterHeight+2*iFrameWidthPartCenter);
    m_rectPartBottom.setWidth(m_size.width()-m_iMarginLeft-m_iMarginRight-2*iFrameWidth);
    m_rectPartBottom.setHeight(cyPartBottomHeight);
    m_rectPartLeft.setLeft(m_iMarginLeft+iFrameWidth);
    m_rectPartLeft.setTop(m_iMarginTop+iFrameWidth);
    m_rectPartLeft.setWidth(cxPartLeftWidth);
    m_rectPartLeft.setHeight(m_size.height()-m_iMarginTop-m_iMarginBottom-2*iFrameWidth);
    m_rectPartRight.setLeft(m_iMarginLeft+iFrameWidth+cxPartLeftWidth+cxPartCenterWidth+2*iFrameWidthPartCenter);
    m_rectPartRight.setTop(m_iMarginTop+iFrameWidth);
    m_rectPartRight.setWidth(cxPartRightWidth);
    m_rectPartRight.setHeight(m_size.height()-m_iMarginTop-m_iMarginBottom-2*iFrameWidth);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "Areas calculated after ";
        strTrcMsg += QString::number(iIterations) + " Iterations:";
        mthTracer.trace(strTrcMsg);

        strTrcMsg  = "PartTop[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartTop.x()) + ",";
        strTrcMsg += QString::number(m_rectPartTop.y()) + ",";
        strTrcMsg += QString::number(m_rectPartTop.width()) + ",";
        strTrcMsg += QString::number(m_rectPartTop.height());
        mthTracer.trace(strTrcMsg);

        strTrcMsg  = "PartBottom[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartBottom.x()) + ",";
        strTrcMsg += QString::number(m_rectPartBottom.y()) + ",";
        strTrcMsg += QString::number(m_rectPartBottom.width()) + ",";
        strTrcMsg += QString::number(m_rectPartBottom.height());
        mthTracer.trace(strTrcMsg);

        strTrcMsg  = "PartLeft[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartLeft.x()) + ",";
        strTrcMsg += QString::number(m_rectPartLeft.y()) + ",";
        strTrcMsg += QString::number(m_rectPartLeft.width()) + ",";
        strTrcMsg += QString::number(m_rectPartLeft.height());
        mthTracer.trace(strTrcMsg);

        strTrcMsg  = "PartRight[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartRight.x()) + ",";
        strTrcMsg += QString::number(m_rectPartRight.y()) + ",";
        strTrcMsg += QString::number(m_rectPartRight.width()) + ",";
        strTrcMsg += QString::number(m_rectPartRight.height());
        mthTracer.trace(strTrcMsg);

        strTrcMsg  = "PartCenter[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartCenter.x()) + ",";
        strTrcMsg += QString::number(m_rectPartCenter.y()) + ",";
        strTrcMsg += QString::number(m_rectPartCenter.width()) + ",";
        strTrcMsg += QString::number(m_rectPartCenter.height());
        mthTracer.trace(strTrcMsg);
    }

    // Move and resize the diagram objects according to the newly calculated
    // rectangle extents of the border areas:
    piDiagObjOffset = &ariDiagObjOffset[0];
    piDiagObjExtent = &ariDiagObjExtent[0];
    pLytPosDiagObj  = &arLytPosDiagObj[0];

    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        switch( pDiagObj->getLayoutPos() )
        {
            case ELayoutPosTop:
            {
                rectDiagObjContent.setWidth( m_rectPartTop.width() );
                rectDiagObjContent.setHeight( *piDiagObjExtent );
                rectDiagObjContent.moveLeft( m_rectPartTop.left() );
                rectDiagObjContent.moveBottom( m_rectPartTop.bottom() - *piDiagObjOffset );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosBottom:
            {
                rectDiagObjContent.setWidth( m_rectPartBottom.width() );
                rectDiagObjContent.setHeight( *piDiagObjExtent );
                rectDiagObjContent.moveLeft( m_rectPartBottom.left() );
                rectDiagObjContent.moveTop( m_rectPartBottom.top() + *piDiagObjOffset );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosLeft:
            {
                rectDiagObjContent.setWidth( *piDiagObjExtent );
                rectDiagObjContent.setHeight( m_rectPartLeft.height() );
                rectDiagObjContent.moveRight( m_rectPartLeft.right() - *piDiagObjOffset );
                rectDiagObjContent.moveTop( m_rectPartLeft.top() );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosRight:
            {
                rectDiagObjContent.setWidth( *piDiagObjExtent );
                rectDiagObjContent.setHeight( m_rectPartRight.height() );
                rectDiagObjContent.moveLeft( m_rectPartRight.left() + *piDiagObjOffset );
                rectDiagObjContent.moveTop( m_rectPartRight.top() );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosCenter:
            {
                if( pDiagObj->geometry() != m_rectPartCenter )
                {
                    pDiagObj->setGeometry(m_rectPartCenter,false);
                }
                break;
            }
            default:
            {
                break;
            }
        }

        piDiagObjOffset++;
        piDiagObjExtent++;
        pLytPosDiagObj++;
    }

    delete [] ariDiagObjOffset;
    ariDiagObjOffset = nullptr;
    delete [] ariDiagObjExtent;
    ariDiagObjExtent = nullptr;
    delete [] arLytPosDiagObj;
    arLytPosDiagObj = nullptr;

    // Mark current process depth as executed (reset bit):
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        pDiagObj->validate(EUpdateLayout);
    }
    validate(EUpdateLayout);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }
} // updateLayout

/*==============================================================================
protected: // overridable auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPixmapDiagram::updatePixmap()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // The scale objects are "virtual" (not visible) objects and don't draw into a bitmap.

    if( m_pPixmap == nullptr )
    {
        return;
    }

    QBrush brush;
    QPen pen;
    QPainter painterPixmap;
    QRect rectFrame;

    painterPixmap.begin(m_pPixmap);

    // Clear bitmap with the widgets current background color.
    m_pPixmap->fill(m_colErase);

    //#define DEBUG_AREAS

    #ifdef DEBUG_AREAS
    painterPixmap.setPen(Qt::black);
    painterPixmap.drawRect(0,0,getWidth()-1,getHeight()-1);
    rectFrame.setLeft(m_iMarginLeft);
    rectFrame.setTop(m_iMarginTop);
    rectFrame.setWidth( getWidth() - m_iMarginLeft - m_iMarginRight );
    rectFrame.setHeight( getHeight() - m_iMarginTop - m_iMarginBottom );
    brush.setColor(Qt::blue);
    painterPixmap.fillRect(rectFrame,brush);
    #endif

    // Fill center area with background color.
    brush.setColor(m_colBgPartCenter);
    brush.setStyle(Qt::SolidPattern);
    if( m_rectPartCenter.width() > 0 && m_rectPartCenter.height() > 0 )
    {
        painterPixmap.fillRect(m_rectPartCenter,brush);
    }

    // Frame around the whole diagram.
    #ifndef DEBUG_AREAS
    if( m_pFrameStyle != nullptr )
    {
        // The frame will be drawn WITHIN the widget area.
        rectFrame.setLeft(m_iMarginLeft);
        rectFrame.setTop(m_iMarginTop);
        rectFrame.setWidth( getWidth() - m_iMarginLeft - m_iMarginRight );
        rectFrame.setHeight( getHeight() - m_iMarginTop - m_iMarginBottom );
        m_pFrameStyle->draw( &painterPixmap, rectFrame );
    }
    #endif

    // Frame around the center part (output region for X/Y values)
    #ifndef DEBUG_AREAS
    if( m_pFrameStylePartCenter != nullptr )
    {
        // The frame will be drawn AROUND the center area.
        rectFrame.setLeft( m_rectPartCenter.left() - m_pFrameStylePartCenter->getFrameWidth() );
        rectFrame.setTop( m_rectPartCenter.top() - m_pFrameStylePartCenter->getFrameWidth() );
        rectFrame.setWidth( m_rectPartCenter.width() + 2*m_pFrameStylePartCenter->getFrameWidth() );
        rectFrame.setHeight( m_rectPartCenter.height() + 2*m_pFrameStylePartCenter->getFrameWidth() );
        m_pFrameStylePartCenter->draw( &painterPixmap, rectFrame );
    }
    #endif

    #ifdef DEBUG_AREAS
    brush.setColor(Qt::green);
    painterPixmap.fillRect(getRectPartTop(),brush);
    painterPixmap.fillRect(getRectPartBottom(),brush);
    painterPixmap.fillRect(getRectPartLeft(),brush);
    painterPixmap.fillRect(getRectPartRight(),brush);
    #endif

    painterPixmap.end();

    // Let the diagram objects draw their data into the bitmap.
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        // Please note that the pixmap has been erased before and therefore each object
        // need to be redrawn. Thats why before calling "update" of the object the pixmap
        // and widget process depth will be invalidated.
        if( pDiagObj->isVisible() )
        {
            pDiagObj->invalidate(EUpdatePixmapWidget,false);
            pDiagObj->update(EUpdatePixmap,m_pPixmap);
        }
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdatePixmap);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // updatePixmap
