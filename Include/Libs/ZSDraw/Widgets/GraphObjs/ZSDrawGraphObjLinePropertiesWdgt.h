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

#ifndef ZSDraw_DrawGraphObjLinePropertiesWdgt_h
#define ZSDraw_DrawGraphObjLinePropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

namespace ZS
{
namespace Draw
{
class CDrawingScene;
class CGraphObjLine;
class CWdgtGraphObjLineGeometryProperties;
class CWdgtGraphObjLineStyleProperties;
class CWdgtGraphObjLabelsProperties;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjLineProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObjLineProperties"; }
    enum class EWidget {
        Labels,
        Geometry,
        LineStyle
    };
    static QString widgetName(EWidget i_widget);
public: // ctors and dtor
    CWdgtGraphObjLineProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strObjName,
        bool i_bAddApplyResetButtons,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjLineProperties();
public: // instance methods
    void expand(EWidget i_widget, bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    virtual bool setKeyInTree(const QString& i_strKeyInTree) override;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const override;
    bool hasChanges() const override;
    void acceptChanges() override;
    void rejectChanges() override;
protected slots:
    void onWdgtLabelsContentChanged();
    void onWdgtGeometryContentChanged();
    void onWdgtLineStyleContentChanged();
protected slots:
    void onGraphObjNameChanged(CGraphObj* i_pGraphObj, const QString& i_strNameNew, const QString& i_strNameOld);
protected: // instance members
    QWidget* m_pWdgtHeadLine;
    QHBoxLayout* m_pLytHeadLine;
    QLabel* m_pLblHeadLine;
    CWdgtGraphObjLabelsProperties* m_pWdgtLabels;
    CWdgtGraphObjLineGeometryProperties* m_pWdgtGeometry;
    CWdgtGraphObjLineStyleProperties* m_pWdgtLineStyle;

}; // class CWdgtGraphObjLineProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLinePropertiesWdgt_h
