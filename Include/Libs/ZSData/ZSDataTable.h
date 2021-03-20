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

#ifndef ZSData_DataTable_h
#define ZSData_DataTable_h

#include <QtCore/qobject.h>
#include <QtCore/qlist.h>

#include "ZSData/ZSDataCommon.h"
#include "ZSData/ZSDataSetObject.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace Data
{
//******************************************************************************
class ZSDATADLL_API CDataTable : public CDataSetObject
/* The table is not a data set object group as neither rows nor columns are
   data set objects.
*******************************************************************************/
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::DS"; }
    static QString ClassName() { return "CDataTable"; }
public: // ctors and dtor
    CDataTable( const QString& i_strTblName );
    virtual ~CDataTable();
private: // copy ctor not allowed (use clone instead)
    CDataTable( const CDataTable& i_other );
private: // assignment operator not allowed (use clone instead)
    CDataTable& operator = ( const CDataTable& i_other );
public: // overridables
    virtual QString nameSpace() const { return CDataTable::NameSpace(); }
    virtual QString className() const { return CDataTable::ClassName(); }
protected: // instance members
    Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CDataTable

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DataTable_h
