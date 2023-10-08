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

#include <QtCore/qglobal.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qfontcombobox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qfontcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsTextStyle.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysPushButton.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtFormatGraphObjsTextStyle : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsTextStyle::CWdgtFormatGraphObjsTextStyle(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene, "CWdgtFormatGraphObjsLineStyle", "", i_pGraphObj, i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytTextColor(nullptr),
    m_pLblTextColor(nullptr),
    m_pPxmBtnTextColor(nullptr),
    m_rctBtnTextColor(0,11,16,5),
    m_pBtnTextColor(nullptr),
    m_pLytFont(nullptr),
    m_pLblFont(nullptr),
    m_pCmbFont(nullptr),
    m_pLytSize(nullptr),
    m_pLblSize(nullptr),
    m_pCmbSize(nullptr),
    m_pLytSepLineFontStyles(nullptr),
    m_pLblSepLineFontStyles(nullptr),
    m_pSepLineFontStyles(nullptr),
    m_pLytFontStyleItalic(nullptr),
    m_pChkFontStyleItalic(nullptr),
    m_pLytFontStyleBold(nullptr),
    m_pChkFontStyleBold(nullptr),
    m_pLytSepLineTextEffects(nullptr),
    m_pLblSepLineTextEffects(nullptr),
    m_pSepLineTextEffects(nullptr),
    m_pLytTextEffectStrikeout(nullptr),
    m_pChkTextEffectStrikeout(nullptr),
    m_pLytTextEffectUnderline(nullptr),
    m_pChkTextEffectUnderline(nullptr),
    m_pLytSepLineSample(nullptr),
    m_pLblSepLineSample(nullptr),
    m_pSepLineSample(nullptr),
    m_pLytSample(nullptr),
    m_pLblSample(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsTextStyle");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::GraphObjFormat", ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 60;
    //int cxEdtWidth = 80;
    int idx;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Text Style");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Button> Text Color
    //---------------------

    m_pLytTextColor = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytTextColor);
    m_pLblTextColor = new QLabel("Color:");
    m_pLblTextColor->setFixedWidth(cxLblWidth);
    m_pLytTextColor->addWidget(m_pLblTextColor);
    m_pBtnTextColor = new QPushButton();
    m_pLytTextColor->addWidget(m_pBtnTextColor);
    m_pLytTextColor->addStretch();

    m_pPxmBtnTextColor = new QPixmap(":/ZS/Draw/DrawSettingsTextStyle16x16.png");

    if( !connect(
        /* pObjSender   */ m_pBtnTextColor,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTextColorClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <ComboBox> Font
    //-----------------

    m_pLytFont = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFont);
    m_pLblFont = new QLabel("Font:");
    m_pLblFont->setFixedWidth(cxLblWidth);
    m_pLytFont->addWidget(m_pLblFont);
    m_pCmbFont = new QFontComboBox();
    m_pLytFont->addWidget(m_pCmbFont);
    m_pLytFont->addStretch();

    if( !connect(
        /* pObjSender   */ m_pCmbFont,
        /* szSignal     */ SIGNAL(currentFontChanged(const QFont&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbFontCurrentFontChanged(const QFont&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <ComboBox> Font Size
    //----------------------

    m_pLytSize = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSize);
    m_pLblSize = new QLabel("Size:");
    m_pLblSize->setFixedWidth(cxLblWidth);
    m_pLytSize->addWidget(m_pLblSize);
    m_pCmbSize = new QComboBox();
    m_pLytSize->addWidget(m_pCmbSize);
    m_pLytSize->addStretch();

    for( idx = 0; idx < ETextSizeCount; idx++ )
    {
        m_pCmbSize->addItem( textSize2Str(static_cast<ETextSize>(idx)), textSize2SizeInPixels(static_cast<ETextSize>(idx)) );
    }

    if( !connect(
        /* pObjSender   */ m_pCmbSize,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbSizeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Separator> Font Style
    //----------------------------

    m_pLyt->addSpacing(4);

    m_pLytSepLineFontStyles = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineFontStyles);

    m_pLblSepLineFontStyles = new QLabel("Font Styles:");
    m_pLytSepLineFontStyles->addWidget(m_pLblSepLineFontStyles);
    m_pSepLineFontStyles = new QFrame();
    m_pSepLineFontStyles->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineFontStyles->addWidget(m_pSepLineFontStyles,1);

    m_pLyt->addSpacing(4);

    // <CheckBox> Font Style Italic
    //-----------------------------

    m_pLytFontStyleItalic = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFontStyleItalic);
    m_pChkFontStyleItalic = new QCheckBox("Italic:");
    m_pLytFontStyleItalic->addWidget(m_pChkFontStyleItalic);
    m_pLytFontStyleItalic->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkFontStyleItalic,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkFontStyleItalicStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Font Style Bold
    //-----------------------------

    m_pLytFontStyleBold = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFontStyleBold);
    m_pChkFontStyleBold = new QCheckBox("Bold:");
    m_pLytFontStyleBold->addWidget(m_pChkFontStyleBold);
    m_pLytFontStyleBold->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkFontStyleBold,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkFontStyleBoldStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Separator> Text Effect
    //----------------------------

    m_pLyt->addSpacing(4);

    m_pLytSepLineTextEffects = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineTextEffects);

    m_pLblSepLineTextEffects = new QLabel("Text Effects:");
    m_pLytSepLineTextEffects->addWidget(m_pLblSepLineTextEffects);
    m_pSepLineTextEffects = new QFrame();
    m_pSepLineTextEffects->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineTextEffects->addWidget(m_pSepLineTextEffects,1);

    m_pLyt->addSpacing(4);

    // <CheckBox> Text Effect Italic
    //-----------------------------

    m_pLytTextEffectStrikeout = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytTextEffectStrikeout);
    m_pChkTextEffectStrikeout = new QCheckBox("Strikeout:");
    m_pLytTextEffectStrikeout->addWidget(m_pChkTextEffectStrikeout);
    m_pLytTextEffectStrikeout->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkTextEffectStrikeout,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkTextEffectStrikeoutStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Text Effect Underline
    //---------------------------------

    m_pLytTextEffectUnderline = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytTextEffectUnderline);
    m_pChkTextEffectUnderline = new QCheckBox("Underline:");
    m_pLytTextEffectUnderline->addWidget(m_pChkTextEffectUnderline);
    m_pLytTextEffectUnderline->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkTextEffectUnderline,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkTextEffectUnderlineStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Separator> Sample
    //----------------------------

    m_pLyt->addSpacing(4);

    m_pLytSepLineSample = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSepLineSample);

    m_pLblSepLineSample = new QLabel("Sample:");
    m_pLytSepLineSample->addWidget(m_pLblSepLineSample);
    m_pSepLineTextEffects = new QFrame();
    m_pSepLineTextEffects->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytSepLineSample->addWidget(m_pSepLineTextEffects,1);

    m_pLyt->addSpacing(4);

    // <Label> Sample
    //---------------

    m_pLytSample = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytSample);
    m_pLblSample = new QLabel("AaBbGgÁáZz");
    m_pLblSample->setMinimumHeight( textSize2SizeInPixels(ETextSizeMax) );
    m_pLblSample->setStyleSheet( "QLabel { background: white; }" );
    m_pLytSample->addWidget(m_pLblSample,1);

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

    // Set settings at GUI controls
    //-----------------------------

    setTextColor(m_drawSettings);

    QFont fnt = m_drawSettings.getTextFont();

    m_pCmbFont->setCurrentFont(fnt);

    ETextSize textSize = m_drawSettings.getTextSize();

    m_pCmbSize->setCurrentIndex(textSize);

    fnt.setPixelSize( textSize2SizeInPixels(textSize) );

    CEnumTextStyle textStyle = m_drawSettings.getTextStyle();

    bool bItalic = (textStyle == ETextStyle::Italic || textStyle == ETextStyle::BoldItalic);
    bool bBold   = (textStyle == ETextStyle::Bold || textStyle == ETextStyle::BoldItalic);

    m_pChkFontStyleItalic->setChecked(bItalic);
    m_pChkFontStyleBold->setChecked(bBold);

    fnt.setItalic(bItalic);
    fnt.setBold(bBold);

    CEnumTextEffect textEffect = m_drawSettings.getTextEffect();

    bool bStrikeout = (textEffect == ETextEffect::Strikeout || textEffect == ETextEffect::StrikeoutUnderline);
    bool bUnderline = (textEffect == ETextEffect::Underline || textEffect == ETextEffect::StrikeoutUnderline);

    m_pChkTextEffectStrikeout->setChecked(bStrikeout);
    m_pChkTextEffectUnderline->setChecked(bUnderline);

    fnt.setStrikeOut(bStrikeout);
    fnt.setUnderline(bUnderline);

    m_pLblSample->setFont(fnt);

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsTextStyle::~CWdgtFormatGraphObjsTextStyle()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pPxmBtnTextColor;
    }
    catch(...)
    {
    }
    m_pPxmBtnTextColor = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytTextColor = nullptr;
    m_pLblTextColor = nullptr;
    m_pPxmBtnTextColor = nullptr;
    m_pBtnTextColor = nullptr;
    m_pLytFont = nullptr;
    m_pLblFont = nullptr;
    m_pCmbFont = nullptr;
    m_pLytSize = nullptr;
    m_pLblSize = nullptr;
    m_pCmbSize = nullptr;
    m_pLytSepLineFontStyles = nullptr;
    m_pLblSepLineFontStyles = nullptr;
    m_pSepLineFontStyles = nullptr;
    m_pLytFontStyleItalic = nullptr;
    m_pChkFontStyleItalic = nullptr;
    m_pLytFontStyleBold = nullptr;
    m_pChkFontStyleBold = nullptr;
    m_pLytSepLineTextEffects = nullptr;
    m_pLblSepLineTextEffects = nullptr;
    m_pSepLineTextEffects = nullptr;
    m_pLytTextEffectStrikeout = nullptr;
    m_pChkTextEffectStrikeout = nullptr;
    m_pLytTextEffectUnderline = nullptr;
    m_pChkTextEffectUnderline = nullptr;
    m_pLytSepLineSample = nullptr;
    m_pLblSepLineSample = nullptr;
    m_pSepLineSample = nullptr;
    m_pLytSample = nullptr;
    m_pLblSample = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::applyChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strAddTrcInfo );

    if (m_pGraphObj != nullptr) {
        m_pGraphObj->setDrawSettings(m_drawSettings);
    }
    else if (m_pDrawingScene != nullptr) {
        // If graphical objects are selected ...
        QList<QGraphicsItem*> arpGraphicsItemsSelected = m_pDrawingScene->selectedItems();
        if (arpGraphicsItemsSelected.size() > 0) {
            // .. all selected graphical objects will be changed according to the new settings.
            for (int idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++) {
                QGraphicsItem* pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if (pGraphObj != nullptr) {
                    pGraphObj->setDrawSettings(m_drawSettings);
                }
            }
        }
        // If no graphical object is selected ...
        else
        {
            // .. set default attribute for following draw commands.
            m_pDrawingScene->setDrawSettings(m_drawSettings);
        }
    }
} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::resetChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pGraphObj != nullptr )
    {
        m_drawSettings = m_pGraphObj->getDrawSettings();
    }
    else if( m_pDrawingScene != nullptr )
    {
        m_drawSettings = m_pDrawingScene->getDrawSettings();
    }

    // Set settings at GUI controls
    //-----------------------------

    setTextColor(m_drawSettings);

    QFont fnt = m_drawSettings.getTextFont();

    m_pCmbFont->setCurrentFont(fnt);

    ETextSize textSize = m_drawSettings.getTextSize();

    m_pCmbSize->setCurrentIndex(textSize);

    fnt.setPixelSize( textSize2SizeInPixels(textSize) );

    CEnumTextStyle textStyle = m_drawSettings.getTextStyle();

    bool bItalic = (textStyle == ETextStyle::Italic || textStyle == ETextStyle::BoldItalic);
    bool bBold   = (textStyle == ETextStyle::Bold || textStyle == ETextStyle::BoldItalic);

    m_pChkFontStyleItalic->setChecked(bItalic);
    m_pChkFontStyleBold->setChecked(bBold);

    fnt.setItalic(bItalic);
    fnt.setBold(bBold);

    CEnumTextEffect textEffect = m_drawSettings.getTextEffect();

    bool bStrikeout = (textEffect == ETextEffect::Strikeout || textEffect == ETextEffect::StrikeoutUnderline);
    bool bUnderline = (textEffect == ETextEffect::Underline || textEffect == ETextEffect::StrikeoutUnderline);

    m_pChkTextEffectStrikeout->setChecked(bStrikeout);
    m_pChkTextEffectUnderline->setChecked(bUnderline);

    fnt.setStrikeOut(bStrikeout);
    fnt.setUnderline(bUnderline);

    m_pLblSample->setFont(fnt);

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsTextStyle::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;

    if( m_pGraphObj != nullptr )
    {
        if( m_pGraphObj->getTextColor() != m_drawSettings.getTextColor() )
        {
            bHasChanges = true;
        }
        if( m_pGraphObj->getTextFont() != m_drawSettings.getTextFont() )
        {
            bHasChanges = true;
        }
        if( m_pGraphObj->getTextSize() != m_drawSettings.getTextSize() )
        {
            bHasChanges = true;
        }
        if( m_pGraphObj->getTextStyle() != m_drawSettings.getTextStyle() )
        {
            bHasChanges = true;
        }
        if( m_pGraphObj->getTextEffect() != m_drawSettings.getTextEffect() )
        {
            bHasChanges = true;
        }
    }
    else if( m_pDrawingScene != nullptr )
    {
        if( m_pDrawingScene->getTextColor() != m_drawSettings.getTextColor() )
        {
            bHasChanges = true;
        }
        if( m_pDrawingScene->getTextFont() != m_drawSettings.getTextFont() )
        {
            bHasChanges = true;
        }
        if( m_pDrawingScene->getTextSize() != m_drawSettings.getTextSize() )
        {
            bHasChanges = true;
        }
        if( m_pDrawingScene->getTextStyle() != m_drawSettings.getTextStyle() )
        {
            bHasChanges = true;
        }
        if( m_pDrawingScene->getTextEffect() != m_drawSettings.getTextEffect() )
        {
            bHasChanges = true;
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bHasChanges);
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ "" );
}

/*==============================================================================
protected slots: // instance methods (Text Style)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onBtnTextColorClicked()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnTextColorClicked",
        /* strAddInfo   */ "" );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_drawSettings.getTextColor(),
        /* pWdgtParent */ m_pBtnTextColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if( clr.isValid() )
    {
        m_drawSettings.setTextColor(clr);

        setTextColor(m_drawSettings);
    }

} // onBtnTextColorClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onCmbFontCurrentFontChanged( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbFontCurrentFontChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_drawSettings.setTextFont(i_fnt);

    setFont(m_drawSettings);

} // onCmbFontCurrentFontChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onCmbSizeCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbSizeCurrentIndexChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_drawSettings.setTextSize( static_cast<ETextSize>(i_idx) );

    setFont(m_drawSettings);

} // onCmbSizeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onChkFontStyleItalicStateChanged( int )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkFontStyleItalicStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumTextStyle textStyle = ETextStyle::Normal;

    if( m_pChkFontStyleItalic->isChecked() && m_pChkFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::BoldItalic;
    }
    else if( m_pChkFontStyleItalic->isChecked() )
    {
        textStyle = ETextStyle::Italic;
    }
    else if( m_pChkFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::Bold;
    }

    m_drawSettings.setTextStyle(textStyle.enumerator());

    setFont(m_drawSettings);

} // onChkFontStyleItalicStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onChkFontStyleBoldStateChanged( int )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkFontStyleBoldStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumTextStyle textStyle = ETextStyle::Normal;

    if( m_pChkFontStyleItalic->isChecked() && m_pChkFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::BoldItalic;
    }
    else if( m_pChkFontStyleItalic->isChecked() )
    {
        textStyle = ETextStyle::Italic;
    }
    else if( m_pChkFontStyleBold->isChecked() )
    {
        textStyle = ETextStyle::Bold;
    }

    m_drawSettings.setTextStyle(textStyle.enumerator());

    setFont(m_drawSettings);

} // onChkFontStyleBoldStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onChkTextEffectStrikeoutStateChanged( int )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkTextEffectStrikeoutStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumTextEffect textEffect = ETextEffect::None;

    if( m_pChkTextEffectStrikeout->isChecked() && m_pChkTextEffectUnderline->isChecked() )
    {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if( m_pChkTextEffectStrikeout->isChecked() )
    {
        textEffect = ETextEffect::Strikeout;
    }
    else if( m_pChkTextEffectUnderline->isChecked() )
    {
        textEffect = ETextEffect::Underline;
    }

    m_drawSettings.setTextEffect(textEffect.enumerator());

    setFont(m_drawSettings);

} // onChkTextEffectStrikeoutStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::onChkTextEffectUnderlineStateChanged( int )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkTextEffectUnderlineStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumTextEffect textEffect = ETextEffect::None;

    if( m_pChkTextEffectStrikeout->isChecked() && m_pChkTextEffectUnderline->isChecked() )
    {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if( m_pChkTextEffectStrikeout->isChecked() )
    {
        textEffect = ETextEffect::Strikeout;
    }
    else if( m_pChkTextEffectUnderline->isChecked() )
    {
        textEffect = ETextEffect::Underline;
    }

    m_drawSettings.setTextEffect(textEffect.enumerator());

    setFont(m_drawSettings);

} // onChkTextEffectUnderlineStateChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::setTextColor( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setTextColor",
        /* strAddInfo   */ strAddTrcInfo );

    QPainter painter(m_pPxmBtnTextColor);

    painter.setPen(i_drawSettings.getTextColor());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);
    painter.setBrush(i_drawSettings.getTextColor());
    painter.drawRect(m_rctBtnTextColor);

    m_pBtnTextColor->setIcon(*m_pPxmBtnTextColor);

} // setTextColor

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsTextStyle::setFont( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFont",
        /* strAddInfo   */ strAddTrcInfo );

    // <ComboBox> Font
    //---------------------

    QFont fnt = i_drawSettings.getTextFont();

    // <ComboBox> Size
    //---------------------

    ETextSize textSize = i_drawSettings.getTextSize();

    fnt.setPixelSize( textSize2SizeInPixels(textSize) );

    // <CheckBoxes> Font Styles
    //-------------------------

    CEnumTextStyle textStyle = i_drawSettings.getTextStyle();

    bool bItalic = (textStyle == ETextStyle::Italic || textStyle == ETextStyle::BoldItalic);
    bool bBold   = (textStyle == ETextStyle::Bold || textStyle == ETextStyle::BoldItalic);

    fnt.setItalic(bItalic);
    fnt.setBold(bBold);

    // <CheckBoxes> Text Effects
    //--------------------------

    CEnumTextEffect textEffect = i_drawSettings.getTextEffect();

    bool bStrikeout = (textEffect == ETextEffect::Strikeout || textEffect == ETextEffect::StrikeoutUnderline);
    bool bUnderline = (textEffect == ETextEffect::Underline || textEffect == ETextEffect::StrikeoutUnderline);

    fnt.setStrikeOut(bStrikeout);
    fnt.setUnderline(bUnderline);

    // <Label> Sample
    //--------------------------

    m_pLblSample->setFont(fnt);

} // setFont
