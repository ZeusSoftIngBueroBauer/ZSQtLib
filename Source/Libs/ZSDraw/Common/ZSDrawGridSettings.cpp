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
    m_bLinesVisible(false),
    m_iLinesDistMin_px(20),
    m_eLinesStyle(ELineStyle::DotLine),
    m_iLinesWidth_px(1),
    m_colorLines(Qt::lightGray),
    m_bLabelsVisible(false),
    m_fntLabels(),
    m_textSizeLabels(ETextSize8),
    m_colorLabelsText(Qt::black),
    m_eTextStyleLabels(ETextStyle::Normal),
    m_eTextEffectLabels(ETextEffect::None)
{
}

//------------------------------------------------------------------------------
CDrawGridSettings::CDrawGridSettings(const QString& i_strName, const CDrawGridSettings& i_other) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
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
QString CDrawGridSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str = m_strName
        + " Lines {Visible: " + bool2Str(m_bLinesVisible)
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