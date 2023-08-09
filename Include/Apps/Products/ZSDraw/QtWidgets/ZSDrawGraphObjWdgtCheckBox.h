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

#ifndef ZSDraw_GraphObjWdgtCheckBox_h
#define ZSDraw_GraphObjWdgtCheckBox_h

#include "ZSDrawGraphObjWdgt.h"

class QCheckBox;

namespace ZS
{
namespace Draw
{
class CDrawSettings;
class CGraphObjSelectionPoint;

namespace QtWidgets
{
//******************************************************************************
class CGraphObjWdgtCheckBox : public CGraphObjWdgt
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to.
        Please note that the static class functions name must be different
        from the non static virtual member function "nameSpace" */
    static QString NameSpace() { return "ZS::Draw::QtWidgets"; }
    /*! Returns the class name.
        Please note that the static class functions name must be different
        from the non static virtual member function "nameSpace" */
    static QString ClassName() { return "CGraphObjWdgtCheckBox"; }
public: // ctors and dtor
    CGraphObjWdgtCheckBox(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "" );
    virtual ~CGraphObjWdgtCheckBox();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
    static qint64 s_iInstCount;
protected: // instance members
    QCheckBox* m_pCheckBox;

}; // class CGraphObjWdgtCheckBox

} // namespace QtWidgets

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjWdgtCheckBox_h
