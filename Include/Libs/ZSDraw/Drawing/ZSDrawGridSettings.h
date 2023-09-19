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

#ifndef ZSDraw_GridSettings_h
#define ZSDraw_GridSettings_h

#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Common/ZSDrawCommon.h"

class QSettings;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CDrawGridSettings
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDrawGridSettings"; }
public: // ctors and dtor
    CDrawGridSettings(const QString& i_strName);
    CDrawGridSettings(const CDrawGridSettings& i_other);
    ~CDrawGridSettings();
public: // operators
    bool operator == (const CDrawGridSettings& i_other) const;
    bool operator != (const CDrawGridSettings& i_other) const;
public: // instance methods
    void setLinesVisible(bool i_bVisible);
    bool areLinesVisible() const;
    void setLinesDistMin(int i_iDistMin_px);
    int linesDistMin() const;
    void setLinesStyle(const CEnumLineStyle& i_eLineStyle);
    CEnumLineStyle linesStyle() const;
    void setLinesWidth(int i_iWidth_px);
    int linesWidth() const;
    void setLinesColor(const QColor& i_color);
    QColor linesColor() const;
public: // instance methods
    void setLabelsVisible(bool i_bVisible);
    bool areLabelsVisible() const;
    void setLabelsFont(const QFont& i_fnt);
    QFont labelsFont() const;
    void setLabelsTextSize(ETextSize i_textSize);
    ETextSize labelsTextSize() const;
    void setLabelsTextColor(const QColor& i_color);
    QColor labelsTextColor() const;
    void setLabelsTextStyle(const CEnumTextStyle& i_eTextStyle);
    CEnumTextStyle labelsTextStyle() const;
    void setLabelsTextEffect(const CEnumTextEffect& i_eTextEffect);
    CEnumTextEffect labelsTextEffect() const;
public: // instance methods
    QString toString() const;
protected: // instance members
    QString m_strName;
    bool m_bLinesVisible;
    int m_iLinesDistMin_px;
    CEnumLineStyle m_eLinesStyle;
    int m_iLinesWidth_px;
    QColor m_colorLines;
    bool m_bLabelsVisible;
    QFont m_fntLabels;
    ETextSize m_textSizeLabels;
    QColor m_colorLabelsText;
    CEnumTextStyle m_eTextStyleLabels;
    CEnumTextEffect m_eTextEffectLabels;

}; // class CDrawGridSettings

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GridSettings_h
