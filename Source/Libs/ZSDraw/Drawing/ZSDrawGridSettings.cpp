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

#include "ZSDraw/Drawing/ZSDrawGridSettings.h"

#include "ZSSys/ZSSysAux.h"

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
CDrawGridSettings::CDrawGridSettings(const QString& i_strName) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
    m_bIsVisible(false),
    m_lineStyle(ELineStyle::SolidLine),
    m_penColor(Qt::black),
    m_iPenWidth_px(1),
    m_bLabelsVisible(false),
    m_fntLabels(),
    m_textStyleLabels(ETextStyle::Normal),
    m_textSizeLabels(ETextSize8),
    m_textEffectLabels(ETextEffect::None),
    m_colorLabelsText(Qt::black)
{
}

//------------------------------------------------------------------------------
CDrawGridSettings::CDrawGridSettings(const CDrawGridSettings& i_other) :
//------------------------------------------------------------------------------
    m_strName(i_other.m_strName),
    m_bIsVisible(i_other.m_bIsVisible),
    m_lineStyle(i_other.m_lineStyle),
    m_penColor(i_other.m_penColor),
    m_iPenWidth_px(i_other.m_iPenWidth_px),
    m_bLabelsVisible(i_other.m_bLabelsVisible),
    m_fntLabels(i_other.m_fntLabels),
    m_textStyleLabels(i_other.m_textStyleLabels),
    m_textSizeLabels(i_other.m_textSizeLabels),
    m_textEffectLabels(i_other.m_textEffectLabels),
    m_colorLabelsText(i_other.m_colorLabelsText)
{
}

//------------------------------------------------------------------------------
CDrawGridSettings::~CDrawGridSettings()
//------------------------------------------------------------------------------
{
    //m_strName;
    m_bIsVisible = false;
    m_lineStyle = static_cast<ELineStyle>(0);
    //m_penColor;
    m_iPenWidth_px = 0;
    m_bLabelsVisible = false;
    //m_fntLabels;
    m_textStyleLabels = static_cast<ETextStyle>(0);
    m_textSizeLabels = static_cast<ETextSize>(0);
    m_textEffectLabels = static_cast<ETextEffect>(0);
    //m_colorLabelsText;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawGridSettings::operator == (const CDrawGridSettings& i_other) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (m_bIsVisible != i_other.m_bIsVisible) {
        bEqual = false;
    }
    else if (m_lineStyle != i_other.m_lineStyle) {
        bEqual = false;
    }
    else if (m_penColor != i_other.m_penColor) {
        bEqual = false;
    }
    else if (m_iPenWidth_px != i_other.m_iPenWidth_px) {
        bEqual = false;
    }
    else if (m_bLabelsVisible != i_other.m_bLabelsVisible) {
        bEqual = false;
    }
    else if (m_fntLabels != i_other.m_fntLabels) {
        bEqual = false;
    }
    else if (m_textStyleLabels != i_other.m_textStyleLabels) {
        bEqual = false;
    }
    else if (m_textSizeLabels != i_other.m_textSizeLabels) {
        bEqual = false;
    }
    else if (m_textEffectLabels != i_other.m_textEffectLabels) {
        bEqual = false;
    }
    else if (m_colorLabelsText != i_other.m_colorLabelsText) {
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
void CDrawGridSettings::setVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    m_bIsVisible = i_bVisible;
}

//------------------------------------------------------------------------------
bool CDrawGridSettings::isVisible() const
//------------------------------------------------------------------------------
{
    return m_bIsVisible;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setLineStyle(ELineStyle i_lineStyle)
//------------------------------------------------------------------------------
{
    m_lineStyle = i_lineStyle;
}

//------------------------------------------------------------------------------
ELineStyle CDrawGridSettings::lineStyle() const
//------------------------------------------------------------------------------
{
    return m_lineStyle;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setPenColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    m_penColor = i_color;
}

//------------------------------------------------------------------------------
QColor CDrawGridSettings::penColor() const
//------------------------------------------------------------------------------
{
    return m_penColor;
}

//------------------------------------------------------------------------------
void CDrawGridSettings::setPenWidth(int i_iWidth_px)
//------------------------------------------------------------------------------
{
    m_iPenWidth_px = i_iWidth_px;
}

//------------------------------------------------------------------------------
int CDrawGridSettings::penWidth() const
//------------------------------------------------------------------------------
{
    return m_iPenWidth_px;
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
void CDrawGridSettings::setLabelsTextStyle(ETextStyle i_textStyle)
//------------------------------------------------------------------------------
{
    m_textStyleLabels = i_textStyle;
}

//------------------------------------------------------------------------------
ETextStyle CDrawGridSettings::labelsTextStyle() const
//------------------------------------------------------------------------------
{
    return m_textStyleLabels;
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
void CDrawGridSettings::setLabelsTextEffect(const ETextEffect i_textEffect)
//------------------------------------------------------------------------------
{
    m_textEffectLabels = i_textEffect;
}

//------------------------------------------------------------------------------
ETextEffect CDrawGridSettings::labelsTextEffect() const
//------------------------------------------------------------------------------
{
    return m_textEffectLabels;
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

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawGridSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str = m_strName
        + " Lines {Visible: " + bool2Str(m_bIsVisible)
        + ", Style: " + CEnumLineStyle(m_lineStyle).toString()
        + ", Color: " + m_penColor.name()
        + ", PenWidth: " + QString::number(m_iPenWidth_px) + "}"
        + ", Labels {Visible: " + bool2Str(m_bLabelsVisible)
        + ", Font: " + m_fntLabels.family()
        + ", TextStyle: " + CEnumTextStyle(m_textStyleLabels).toString()
        + ", TextSize: " + textSize2Str(m_textSizeLabels)
        + ", TextEffect: " + CEnumTextEffect().toString(m_textEffectLabels)
        + ", TextColor: " + m_colorLabelsText.name() + "}";
    return str;
}
