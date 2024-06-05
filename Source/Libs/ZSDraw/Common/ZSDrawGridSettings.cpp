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

#include "ZSDraw/Common/ZSDrawGridSettings.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"

#include "ZSSys/ZSSysAux.h"

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CDrawGridSettings
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawGridSettings::CDrawGridSettings() :
//------------------------------------------------------------------------------
    m_strName(),
    m_bLinesVisible(false),
    m_iLinesDistMin_px(20),
    m_eLinesStyle(ELineStyle::DotLine),
    m_iLinesWidth_px(1),
    m_colorLines(Qt::lightGray),
    m_bLabelsVisible(false),
    m_fntLabels("Terminal"),
    m_textSizeLabels(ETextSize8),
    m_colorLabelsText(Qt::black),
    m_eTextStyleLabels(ETextStyle::Normal),
    m_eTextEffectLabels(ETextEffect::None)
{
}

//------------------------------------------------------------------------------
CDrawGridSettings::CDrawGridSettings(const CDrawGridSettings& i_other) :
//------------------------------------------------------------------------------
    m_strName(),
    m_bLinesVisible(i_other.m_bLinesVisible),
    m_iLinesDistMin_px(i_other.m_iLinesDistMin_px),
    m_eLinesStyle(i_other.m_eLinesStyle),
    m_iLinesWidth_px(i_other.m_iLinesWidth_px),
    m_colorLines(i_other.m_colorLines),
    m_bLabelsVisible(i_other.m_bLabelsVisible),
    m_fntLabels(i_other.m_fntLabels),
    m_textSizeLabels(i_other.m_textSizeLabels),
    m_colorLabelsText(i_other.m_colorLabelsText),
    m_eTextStyleLabels(i_other.m_eTextStyleLabels),
    m_eTextEffectLabels(i_other.m_eTextEffectLabels)
{
}

//------------------------------------------------------------------------------
CDrawGridSettings::~CDrawGridSettings()
//------------------------------------------------------------------------------
{
    //m_strName;
    m_bLinesVisible = false;
    m_iLinesDistMin_px = 0;
    m_eLinesStyle = static_cast<ELineStyle>(0);
    m_iLinesWidth_px = 0;
    //m_colorLines;
    m_bLabelsVisible = false;
    //m_fntLabels;
    m_textSizeLabels = static_cast<ETextSize>(0);
    //m_colorLabelsText;
    m_eTextStyleLabels = static_cast<ETextStyle>(0);
    m_eTextEffectLabels = static_cast<ETextEffect>(0);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawGridSettings::operator == (const CDrawGridSettings& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_bLinesVisible != i_other.m_bLinesVisible) {
        bEqual = false;
    }
    else if (m_iLinesDistMin_px != i_other.m_iLinesDistMin_px) {
        bEqual = false;
    }
    else if (m_eLinesStyle != i_other.m_eLinesStyle) {
        bEqual = false;
    }
    else if (m_iLinesWidth_px != i_other.m_iLinesWidth_px) {
        bEqual = false;
    }
    else if (m_colorLines != i_other.m_colorLines) {
        bEqual = false;
    }
    else if (m_bLabelsVisible != i_other.m_bLabelsVisible) {
        bEqual = false;
    }
    else if (m_fntLabels != i_other.m_fntLabels) {
        bEqual = false;
    }
    else if (m_textSizeLabels != i_other.m_textSizeLabels) {
        bEqual = false;
    }
    else if (m_colorLabelsText != i_other.m_colorLabelsText) {
        bEqual = false;
    }
    else if (m_eTextStyleLabels != i_other.m_eTextStyleLabels) {
        bEqual = false;
    }
    else if (m_eTextEffectLabels != i_other.m_eTextEffectLabels) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CDrawGridSettings::operator != ( const CDrawGridSettings& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawGridSettings::setLinesVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    m_bLinesVisible = i_bVisible;
}

//------------------------------------------------------------------------------
bool CDrawGridSettings::areLinesVisible() const
//------------------------------------------------------------------------------
{
    return m_bLinesVisible;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLinesDistMin(int i_iDistMin_px)
//------------------------------------------------------------------------------
{
    m_iLinesDistMin_px = i_iDistMin_px;
}

//------------------------------------------------------------------------------
int CDrawGridSettings::linesDistMin() const
//------------------------------------------------------------------------------
{
    return m_iLinesDistMin_px;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLinesStyle(const CEnumLineStyle& i_eLineStyle)
//------------------------------------------------------------------------------
{
    m_eLinesStyle = i_eLineStyle;
}

//------------------------------------------------------------------------------
CEnumLineStyle CDrawGridSettings::linesStyle() const
//------------------------------------------------------------------------------
{
    return m_eLinesStyle;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLinesWidth(int i_iWidth_px)
//------------------------------------------------------------------------------
{
    m_iLinesWidth_px = i_iWidth_px;
}

//------------------------------------------------------------------------------
int CDrawGridSettings::linesWidth() const
//------------------------------------------------------------------------------
{
    return m_iLinesWidth_px;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLinesColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    m_colorLines = i_color;
}

//------------------------------------------------------------------------------
QColor CDrawGridSettings::linesColor() const
//------------------------------------------------------------------------------
{
    return m_colorLines;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawGridSettings::setLabelsVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    m_bLabelsVisible = i_bVisible;
}

//------------------------------------------------------------------------------
bool CDrawGridSettings::areLabelsVisible() const
//------------------------------------------------------------------------------
{
    return m_bLabelsVisible;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLabelsFont(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    m_fntLabels = i_fnt;
}

//------------------------------------------------------------------------------
QFont CDrawGridSettings::labelsFont() const
//------------------------------------------------------------------------------
{
    return m_fntLabels;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLabelsTextSize(ETextSize i_textSize)
//------------------------------------------------------------------------------
{
    m_textSizeLabels = i_textSize;
}

//------------------------------------------------------------------------------
ETextSize CDrawGridSettings::labelsTextSize() const
//------------------------------------------------------------------------------
{
    return m_textSizeLabels;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLabelsTextColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    m_colorLabelsText = i_color;
}

//------------------------------------------------------------------------------
QColor CDrawGridSettings::labelsTextColor() const
//------------------------------------------------------------------------------
{
    return m_colorLabelsText;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLabelsTextStyle(const CEnumTextStyle& i_eTextStyle)
//------------------------------------------------------------------------------
{
    m_eTextStyleLabels = i_eTextStyle;
}

//------------------------------------------------------------------------------
CEnumTextStyle CDrawGridSettings::labelsTextStyle() const
//------------------------------------------------------------------------------
{
    return m_eTextStyleLabels;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLabelsTextEffect(const CEnumTextEffect& i_eTextEffect)
//------------------------------------------------------------------------------
{
    m_eTextEffectLabels = i_eTextEffect;
}

//------------------------------------------------------------------------------
CEnumTextEffect CDrawGridSettings::labelsTextEffect() const
//------------------------------------------------------------------------------
{
    return m_eTextEffectLabels;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawGridSettings::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrLinesVisible, bool2Str(areLinesVisible()));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrLinesDistMin, QString::number(linesDistMin()));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrLineStyle, CEnumLineStyle(linesStyle()).toString());
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrLineWidth, QString::number(linesWidth()));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrLineColor, linesColor().name());
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrLabelsVisible, bool2Str(areLabelsVisible()));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrFont, labelsFont().family());
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrTextSize, QString::number(labelsTextSize()));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrTextColor, labelsTextColor().name());
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrTextStyle, CEnumTextStyle(labelsTextStyle()).toString());
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrTextEffect, CEnumTextEffect(labelsTextEffect()).toString());
}

//------------------------------------------------------------------------------
void CDrawGridSettings::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    QString strElemName = i_xmlStreamReader.name().toString();
    QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();
    if (!i_xmlStreamReader.hasError()) {
        bool bGridLinesVisible = XmlStreamParser::getBoolVal(
            i_xmlStreamReader, xmlStreamAttrs, strElemName,
            XmlStreamParser::c_strXmlAttrLinesVisible, false, false);
        if (!i_xmlStreamReader.hasError()) {
            setLinesVisible(bGridLinesVisible);
        }
        if (!i_xmlStreamReader.hasError()) {
            int iDistMin_px = XmlStreamParser::getIntVal(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrLinesDistMin, false, 1);
            if (!i_xmlStreamReader.hasError()) {
                setLinesDistMin(iDistMin_px);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            CEnumLineStyle eLineStyle = XmlStreamParser::getLineStyle(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrLineStyle, false, ELineStyle::SolidLine);
            if (!i_xmlStreamReader.hasError() && eLineStyle.isValid()) {
                setLinesStyle(eLineStyle);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            int iPenWidth = XmlStreamParser::getIntVal(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrLineWidth, false, 1);
            if (!i_xmlStreamReader.hasError()) {
                setLinesWidth(iPenWidth);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            QColor clrPen = XmlStreamParser::getColor(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrLineColor, false, Qt::black);
            if (!i_xmlStreamReader.hasError()) {
                setLinesColor(clrPen);
            }
        }
        bool bGridLabelsVisible = false;
        if (!i_xmlStreamReader.hasError()) {
            bGridLabelsVisible = XmlStreamParser::getBoolVal(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrLabelsVisible, false, bGridLinesVisible);
            if (!i_xmlStreamReader.hasError()) {
                setLabelsVisible(bGridLabelsVisible);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            QFont fnt = XmlStreamParser::getFont(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrFont, false, QFont());
            if (!i_xmlStreamReader.hasError()) {
                setLabelsFont(fnt);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            ETextSize eTextSize = XmlStreamParser::getTextSize(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrTextSize, false, ETextSize8);
            if (!i_xmlStreamReader.hasError()) {
                setLabelsTextSize(eTextSize);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            QColor clrText = XmlStreamParser::getColor(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrTextColor, false, Qt::black);
            if (!i_xmlStreamReader.hasError()) {
                setLabelsTextColor(clrText);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            CEnumTextStyle eTextStyle = XmlStreamParser::getTextStyle(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrTextStyle, false, ETextStyle::Normal);
            if (!i_xmlStreamReader.hasError()) {
                setLabelsTextStyle(eTextStyle);
            }
        }
        if (!i_xmlStreamReader.hasError()) {
            CEnumTextEffect eTextEffect = XmlStreamParser::getTextEffect(
                i_xmlStreamReader, xmlStreamAttrs, strElemName,
                XmlStreamParser::c_strXmlAttrTextEffect, false, ETextEffect::None);
            if (!i_xmlStreamReader.hasError()) {
                setLabelsTextEffect(eTextEffect);
            }
        }
    }
} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawGridSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    if (!m_strName.isEmpty()) str += m_strName + ", ";
    str += "Lines {Visible: " + bool2Str(m_bLinesVisible)
        + ", MinDist: " + QString::number(m_iLinesDistMin_px) + " px"
        + ", Style: " + m_eLinesStyle.toString()
        + ", Color: " + m_colorLines.name()
        + ", PenWidth: " + QString::number(m_iLinesWidth_px) + "}"
        + ", Labels {Visible: " + bool2Str(m_bLabelsVisible)
        + ", Font: " + m_fntLabels.family()
        + ", TextStyle: " + m_eTextStyleLabels.toString()
        + ", TextSize: " + textSize2Str(m_textSizeLabels)
        + ", TextEffect: " + m_eTextEffectLabels.toString()
        + ", TextColor: " + m_colorLabelsText.name() + "}";
    return str;
}
