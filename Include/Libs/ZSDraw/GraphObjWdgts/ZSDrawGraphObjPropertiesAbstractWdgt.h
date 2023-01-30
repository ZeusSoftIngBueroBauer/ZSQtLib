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

#ifndef ZSDraw_GraphObjPropertiesAbstractWdgt_h
#define ZSDraw_GraphObjPropertiesAbstractWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/Common/ZSDrawCommon.h"

class QSettings;

namespace ZS
{
namespace System
{
class CIdxTree;
}

namespace Draw
{
class CDrawingScene;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjPropertiesAbstract : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObjPropertiesAbstract"; }
public: // ctors and dtor
    CWdgtGraphObjPropertiesAbstract(
        CDrawingScene* i_pDrawingScene,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjPropertiesAbstract();
public: // overridables
    virtual QString nameSpace() const { return CWdgtGraphObjPropertiesAbstract::NameSpace(); }
    virtual QString className() const { return CWdgtGraphObjPropertiesAbstract::ClassName(); }
public: // overridables
    virtual void saveState(QSettings& i_settings) const;
    virtual void restoreState(const QSettings& i_settings);
public: // overridables
    virtual void setKeyInTree( const QString& i_strKeyInTree );
    QString getKeyInTree() const;
public: // overridables
    virtual void resizeToContents();
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    ZS::System::CIdxTree* m_pIdxTree;
    QString m_strKeyInTree;
    int m_cxLblWidthClm1;
    int m_cxEdtWidthClm1;
    int m_cxLblWidthClm2;
    int m_cxEdtWidthClm2;
    int m_cxClmSpacing;

}; // class CWdgtGraphObjPropertiesAbstract

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjPropertiesAbstractWdgt_h
