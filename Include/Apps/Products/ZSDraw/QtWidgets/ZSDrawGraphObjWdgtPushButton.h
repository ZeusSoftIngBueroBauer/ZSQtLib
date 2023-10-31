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

#ifndef ZSDraw_GraphObjWdgtPushButton_h
#define ZSDraw_GraphObjWdgtPushButton_h

#include "ZSDrawGraphObjWdgt.h"

class QPushButton;

namespace ZS
{
namespace Draw
{
class CDrawSettings;
class CGraphObjSelectionPoint;

namespace QtWidgets
{
//******************************************************************************
class CGraphObjWdgtPushButton : public CGraphObjWdgt
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::QtWidgets"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjWdgtPushButton"; }
public: // ctors and dtor
    CGraphObjWdgtPushButton(
        CDrawingScene* i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString& i_strObjName = "");
    virtual ~CGraphObjWdgtPushButton();
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
    static qint64 s_iInstCount;
protected: // instance members
    QPushButton* m_pPushButton;

}; // class CGraphObjWdgtPushButton

} // namespace QtWidgets

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjWdgtPushButton_h
