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

#pragma once

#include <QObject>
#include "ZSDrawPluginInterface.h"

namespace ZS::Draw::Plugins::QtWidgets
{
class CObjFactoryWdgtCheckBox;
class CObjFactoryWdgtComboBox;
class CObjFactoryWdgtGroupBox;
class CObjFactoryWdgtLabel;
class CObjFactoryWdgtLineEdit;
class CObjFactoryWdgtPushButton;

class CDrawPluginQtWidgets : public QObject, public IDrawPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(ZS::Draw::Plugins::IDrawPluginInterface)
    Q_PLUGIN_METADATA(IID ZSDrawPluginInterface_iid)

public: // class methods
    static QString NameSpace() { return "ZS::Draw::Plugins::QtWidgets"; }
    static QString ClassName() { return "CDrawPluginQtWidgets"; }
public: // ctors and dtor
    CDrawPluginQtWidgets();
    ~CDrawPluginQtWidgets() override;
public: // interface methods of IDrawPluginInterface
    void createObjFactories() override;
private: // instance members
    CObjFactoryWdgtCheckBox* m_pObjFactoryWdgtCheckBox = nullptr;
    CObjFactoryWdgtComboBox* m_pObjFactoryWdgtComboBox = nullptr;
    CObjFactoryWdgtGroupBox* m_pObjFactoryWdgtGroupBox = nullptr;
    CObjFactoryWdgtLabel* m_pObjFactoryWdgtLabel = nullptr;
    CObjFactoryWdgtLineEdit* m_pObjFactoryWdgtLineEdit = nullptr;
    CObjFactoryWdgtPushButton* m_pObjFactoryWdgtPushButton = nullptr;
};

} // namespace ZS::Draw::Plugins::QtWidgets
