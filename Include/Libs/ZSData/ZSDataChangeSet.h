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

#error Module not used anymore

#ifndef ZSData_ChangeEvents_h
#define ZSData_ChangeEvents_h

#include <QtCore/qobject.h>

#include "ZSData/ZSDataCommon.h"
#include "ZSData/ZSDataSetObject.h"
#include "ZSSys/ZSSysTrcAdminObj.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDataSet;
//class CDESchemaChgDscr;
//class CDTSchemaChgDscr;
//class CDPSchemaChgDscr;
//class CGrpSchemaChgDscr;
//class CDSSchemaChgDscr;
//class CDTDataChgDscr;
//class CDPDataChgDscr;

//******************************************************************************
class ZSDATADLL_API CChangeDataSet : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CChangeDataSet( CDataSet* i_pDS, ZS::System::ERowVersion i_version, const SDbClientRegSpec& i_dbCltRegSpec );
    CChangeDataSet( const QString& i_strObjName, ZS::System::ERowVersion i_version );
    virtual ~CChangeDataSet();
public: // instance methods
    CChangeDataSet* clone( ZS::Data::ECloneCopyDepth i_copyDepth ) const;
public: // instance methods
    SCnctId getConnectionKey() const { return m_dbCltRegSpec.m_cnctId; }
public: // instance methods
    void addObject( CDataSetObject* i_pDSObj );
    void removeObject( CDataSetObject* i_pDSObj );
public: // instance methods
    //bool hasSchemaChanges() const;
    //QHash<QString,CDSObjSchemaChgDscr*> getSchemaChanges( EDataSetObjType i_dsObjType = EDataSetObjTypeCount ) const;
    //QHash<QString,CDSObjSchemaChgDscr*> takeSchemaChanges( EDataSetObjType i_dsObjType = EDataSetObjTypeCount ); // Releases ownership.
    //void addSchemaChanges( EDataSetObjType i_dsObjType, CDSObjSchemaChgDscr* i_pChgEvArgs );    // Hands over ownership.
    //void addSchemaChanges( CDESchemaChgDscr* i_pChgEvArgs );                                    // Hands over ownership.
    //void addSchemaChanges( CDTSchemaChgDscr* i_pChgEvArgs );                                    // Hands over ownership.
    //void addSchemaChanges( CDPSchemaChgDscr* i_pChgEvArgs );                                    // Hands over ownership.
    //void addSchemaChanges( CGrpSchemaChgDscr* i_pChgEvArgs );                                   // Hands over ownership.
    //void addSchemaChanges( CDSSchemaChgDscr* i_pChgEvArgs );                                    // Hands over ownership.
public: // instance methods
    //bool hasDataChanges() const;
    //QHash<QString,CDSObjDataChgDscr*> getDataChanges( EDataSetObjType i_dsObjType = EDataSetObjTypeCount ) const;
    //QHash<QString,CDSObjDataChgDscr*> takeDataChanges( EDataSetObjType i_dsObjType = EDataSetObjTypeCount ); // Releases ownership.
    //void addDataChanges( EDataSetObjType i_dsObjType, CDSObjDataChgDscr* i_pChgEvArgs );        // Hands over ownership.
    //void addDataChanges( CDTDataChgDscr* i_pChgEvArgs );                                        // Hands over ownership.
    //void addDataChanges( CDPDataChgDscr* i_pChgEvArgs );                                        // Hands over ownership.
public: // instance methods
    ZS::System::SErrResultInfo serialize( QByteArray& io_byteArr ) const;
    ZS::System::SErrResultInfo deserialize( const QByteArray& i_byteArr );
public: // instance methods
    void resolveObjectReferences( CDataSet* i_pDS );
    void invalidateObjectReferences();
public: // instance methods
    QString getAddTrcInfo( int i_iDetailLevel ) const;
protected slots:
    //void onDSOrginalSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pChgEvArgs );
    //void onDSObjOrginalSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pChgEvArgs );
    //void onDSObjOrginalDataChanged( ZS::Data::CDSObjDataChgDscr* i_pChgEvArgs );
protected: // instance members
    CDataSet*                           m_pDS;
    ZS::System::ERowVersion             m_version;
    SDbClientRegSpec                    m_dbCltRegSpec;
    // Depending on which site (server or client) the change tracker is used
    // either accepted (server (database) or current (client) changes are stored.
    //QHash<QString,CDSObjSchemaChgDscr*> m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeCount];  // Key the object's UId.
    //QHash<QString,CDSObjDataChgDscr*>   m_arhshpDSObjDataChgDscr[EDataSetObjTypeCount];    // Key the object's UId.
    // Trace
    Trace::CTrcAdminObj*                m_pTrcAdminObj;

}; // class CChangeDataSet

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_ChangeEvents_h
