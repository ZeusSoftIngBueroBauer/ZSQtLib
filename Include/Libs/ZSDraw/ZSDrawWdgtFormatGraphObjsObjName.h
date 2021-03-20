/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_WdgtFormatGraphObjsObjName_h
#define ZSDraw_WdgtFormatGraphObjsObjName_h

#include <QtCore/qglobal.h>

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjs.h"

class QCheckBox;
class QLabel;
class QLineEdit;

class QLayout;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtFormatGraphObjsObjName : public CWdgtFormatGraphObjs
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtFormatGraphObjsObjName(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsObjName();
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges();
    virtual void resetChanges();
    virtual bool hasChanges() const;
protected slots:
    void onEdtObjNameTextChanged( const QString& i_strText );
    void onEdtObjIdTextChanged( const QString& i_strText );
private: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QHBoxLayout*             m_pLytObjName;
    QLabel*                  m_pLblObjName;
    QLineEdit*               m_pEdtObjName;
    QCheckBox*               m_pChkObjNameVisible;
    QHBoxLayout*             m_pLytObjId;
    QLabel*                  m_pLblObjId;
    QLineEdit*               m_pEdtObjId;
    QCheckBox*               m_pChkObjIdVisible;
    // Trace
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsObjName

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsObjName_h
