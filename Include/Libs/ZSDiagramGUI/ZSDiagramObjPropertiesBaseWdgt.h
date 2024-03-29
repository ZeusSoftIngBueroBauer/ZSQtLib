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

#ifndef ZSDiagram_ObjPropertiesBaseWdgt_h
#define ZSDiagram_ObjPropertiesBaseWdgt_h

#include "ZSDiagramGUI/ZSDiagramItemPropertiesBaseWdgt.h"

namespace ZS
{
namespace Diagram
{
namespace GUI
{
//******************************************************************************
class ZSDIAGRAMGUIDLL_API CWdgtDiagramObjPropertiesBase : public CWdgtDiagramItemPropertiesBase
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString ClassName() { return "CWdgtDiagramObjPropertiesBase"; }
public: // ctors and dtor
    CWdgtDiagramObjPropertiesBase(
        const QString& i_strWdgtClassName,
        CWdgtDiagram* i_pDiagram,
        const QString& i_strDiagItemClassName,
        ZS::System::EMode i_mode = ZS::System::EMode::View,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDiagramObjPropertiesBase();
protected: // instance members

}; // class CWdgtDiagramObjPropertiesBase

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ObjPropertiesBaseWdgt_h
