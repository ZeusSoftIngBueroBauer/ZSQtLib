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

#ifndef ZSDraw_DrawGraphObjPolygonPropertiesWdgt_h
#define ZSDraw_DrawGraphObjPolygonPropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

namespace ZS
{
namespace Draw
{
class CDrawingScene;
class CGraphObjPolygon;
class CWdgtGraphObjPolygonGeometryProperties;
class CWdgtGraphObjFillStyleProperties;
class CWdgtGraphObjLineStyleProperties;
class CWdgtGraphObjLabelsProperties;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjPolygonProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObjPolygonProperties"; }
    enum class EWidget {
        Labels,
        Geometry,
        LineStyle,
        FillStyle
    };
    static QString widgetName(EWidget i_widget);
public: // ctors and dtor
    CWdgtGraphObjPolygonProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strObjName,
        bool i_bAddApplyResetButtons,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjPolygonProperties();
public: // instance methods
    void expand(EWidget i_widget, bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    virtual bool setKeyInTree(const QString& i_strKeyInTree) override;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const override;
    bool hasChanges() const override;
    void acceptChanges() override;
    void rejectChanges() override;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
protected slots:
    void onCmbGraphObjTypeCurrentIndexChanged(int i_idx);
    void onWdgtLabelsContentChanged();
    void onWdgtGeometryContentChanged();
    void onWdgtLineStyleContentChanged();
    void onWdgtFillStyleContentChanged();
protected slots:
    void onGraphObjNameChanged(CGraphObj* i_pGraphObj, const QString& i_strNameNew, const QString& i_strNameOld);
protected: // instance members
    QWidget* m_pWdgtHeadLine = nullptr;
    QHBoxLayout* m_pLytHeadLine = nullptr;
    QLabel* m_pLblHeadLine = nullptr;
    QWidget* m_pWdgtGraphObjType = nullptr;
    QHBoxLayout* m_pLytWdgtGraphObjType = nullptr;
    QLabel* m_pLblGraphObjType = nullptr;
    QComboBox* m_pCmbGraphObjType = nullptr;
    CWdgtGraphObjLabelsProperties* m_pWdgtLabels = nullptr;
    CWdgtGraphObjPolygonGeometryProperties* m_pWdgtGeometry = nullptr;
    CWdgtGraphObjLineStyleProperties* m_pWdgtLineStyle = nullptr;
    CWdgtGraphObjFillStyleProperties* m_pWdgtFillStyle = nullptr;

}; // class CWdgtGraphObjPolygonProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjPolygonPropertiesWdgt_h
