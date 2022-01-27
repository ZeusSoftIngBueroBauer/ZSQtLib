/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsObjName"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CWdgtFormatGraphObjsObjName(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsObjName();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges() override;
    virtual void resetChanges() override;
    virtual bool hasChanges() const override;
protected: // must overridables of base class CWdgtFormatGraphObjs
    virtual void onGraphObjChanged() override;
private: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QHBoxLayout*             m_pLytName;
    QLabel*                  m_pLblName;
    QLineEdit*               m_pEdtName;
    QHBoxLayout*             m_pLytPath;
    QLabel*                  m_pLblPath;
    QLineEdit*               m_pEdtPath;
    // Trace
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsObjName

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsObjName_h
