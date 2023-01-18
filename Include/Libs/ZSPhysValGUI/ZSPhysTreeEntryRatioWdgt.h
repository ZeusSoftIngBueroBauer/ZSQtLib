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

#ifndef ZSPhysValGUI_PhysTreeEntryRatioWdgt_h
#define ZSPhysValGUI_PhysTreeEntryRatioWdgt_h

#include "ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h"

class QLabel;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtRatio : public CWdgtAbstractTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtRatio"; }
public: // ctors and dtor
    CWdgtRatio( CIdxTreeUnits* i_pIdxTree, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtRatio();
public: // overridables
    virtual QString nameSpace() const { return CWdgtRatio::NameSpace(); }
    virtual QString className() const { return CWdgtRatio::ClassName(); }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree ) override;
protected: // instance members
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytSymbol;
    QLabel*      m_pLblSymbol;
    QLineEdit*   m_pEdtSymbol;
    QHBoxLayout* m_pLytFactor;
    QLabel*      m_pLblFactor;
    QLineEdit*   m_pEdtFactor;
    QHBoxLayout* m_pLytNextLower;
    QLabel*      m_pLblNextLower;
    QLineEdit*   m_pEdtNextLower;
    QHBoxLayout* m_pLytNextHigher;
    QLabel*      m_pLblNextHigher;
    QLineEdit*   m_pEdtNextHigher;

}; // class CWdgtRatio

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryRatioWdgt_h
