/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestPhysVal_UnitModels_h
#define ZSApps_TestPhysVal_UnitModels_h

#include <QtCore/qglobal.h>

#include <QtCore/qabstractitemmodel.h>
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
class CUnit;
class CUnitGrp;
}

namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
struct SUnitsModelNode
//******************************************************************************
{
public: // ctors and dtor
    SUnitsModelNode();
    SUnitsModelNode( ZS::PhysVal::CUnitGrp* i_pUnitGrp );
    SUnitsModelNode( ZS::PhysVal::CUnit* i_pUnit );
    ~SUnitsModelNode();
public: // struct members
    ZS::PhysVal::CUnitGrp*  m_pUnitGrp; // Either root node, node for UnitGrp or node for Unit
    ZS::PhysVal::CUnit*     m_pUnit;    // (for root node both UnitGrp and Unit are nullptr, for other nodes either UnitGrp or Unit must be nullptr)
    QModelIndex             m_modelIdx;
    SUnitsModelNode*        m_pParentNode;
    QList<SUnitsModelNode*> m_arpChildNodes;

}; // struct SUnitsModelNode

//******************************************************************************
class CUnitsModel : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnNodeName = 0,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CUnitsModel( QObject* i_pObjParent = nullptr );
    virtual ~CUnitsModel();
protected: // instance methods
    void clear( SUnitsModelNode* i_pModelNode );
    void addNode( SUnitsModelNode* i_pParentNode, ZS::PhysVal::CUnitGrp* i_pUnitGrp );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
protected: // instance members
    SUnitsModelNode* m_pRootNode;

}; // class CUnitsModel


////******************************************************************************
//class CModelUnitGrpSIBase : public QAbstractTableModel
////******************************************************************************
//{
//    Q_OBJECT
//public: // type definitions and constants
//    typedef enum
//    {
//        EColumnId        = 0,
//        EColumnName      = 1,
//        EColumnSymbol    = 2,
//        EColumnMeasure   = 3,
//        EColumnDimension = 4,
//        EColumnCount
//    }   EColumn;
//public: // ctors and dtor
//    CModelUnitGrpSIBase( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
//    virtual ~CModelUnitGrpSIBase();
//public: // instance methods
//    void setObjName( const QString& i_strObjName );
//    QString getObjName() const;
//    unsigned int getUnitCount() const;
//    void setUnitGroup( ZS::PhysVal::CUnitGrp* i_pUnitGrp );
//    ZS::PhysVal::CUnitGrp* unitGroup();
//public: // must overridables of base class QAbstractItemModel
//    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
//    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
//    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
//public: // overridables of base class QAbstractItemModel
//    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
//private: // copy ctor not allowed
//    CModelUnitGrpSIBase( const CModelUnitGrpSIBase& );
//private: // assignment operator not allowed
//    CModelUnitGrpSIBase& operator = ( const CModelUnitGrpSIBase& );
//protected: // instance members
//    QString                m_strObjName;
//    ZS::PhysVal::CUnitGrp* m_pUnitGrp;
//
//}; // class CModelUnitGrpSIBase


//******************************************************************************
class CModelUnitGrpRatio : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnName   = 0,
        EColumnSymbol = 1,
        EColumnFactor = 2,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelUnitGrpRatio( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
    virtual ~CModelUnitGrpRatio();
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    unsigned int getUnitCount() const;
    void setUnitGroup( ZS::PhysVal::CUnitGrp* i_pUnitGrp );
    ZS::PhysVal::CUnitGrp* unitGroup();
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelUnitGrpRatio( const CModelUnitGrpRatio& );
private: // assignment operator not allowed
    CModelUnitGrpRatio& operator = ( const CModelUnitGrpRatio& );
protected: // instance members
    QString                m_strObjName;
    ZS::PhysVal::CUnitGrp* m_pUnitGrp;

}; // class CModelUnitGrpRatio


////******************************************************************************
//class CModelUnitGrpDataQuantity : public QAbstractTableModel
////******************************************************************************
//{
//    Q_OBJECT
//public: // type definitions and constants
//    typedef enum
//    {
//        EColumnName   = 0,
//        EColumnSymbol = 1,
//        EColumnFactor = 2,
//        EColumnCount
//    }   EColumn;
//public: // ctors and dtor
//    CModelUnitGrpDataQuantity( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
//    virtual ~CModelUnitGrpDataQuantity();
//public: // instance methods
//    void setObjName( const QString& i_strObjName );
//    QString getObjName() const;
//    unsigned int getUnitCount() const;
//    void setUnitGroup( ZS::PhysVal::CUnitGrp* i_pUnitGrp );
//    ZS::PhysVal::CUnitGrp* unitGroup();
//public: // must overridables of base class QAbstractItemModel
//    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
//    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
//    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
//public: // overridables of base class QAbstractItemModel
//    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
//private: // copy ctor not allowed
//    CModelUnitGrpDataQuantity( const CModelUnitGrpDataQuantity& );
//private: // assignment operator not allowed
//    CModelUnitGrpDataQuantity& operator = ( const CModelUnitGrpDataQuantity& );
//protected: // instance members
//    QString                m_strObjName;
//    ZS::PhysVal::CUnitGrp* m_pUnitGrp;
//
//}; // class CModelUnitGrpDataQuantity


//******************************************************************************
class CModelPhysSize : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EViewModeNormal           = 0,
        EViewModeContingencyTable = 1,
        EViewModeCount,
        EViewModeUndefined
    }   EViewMode;
    static QString viewMode2Str( int i_iViewMode );
    static EViewMode str2ViewMode( const QString& i_str );
    typedef enum
    {
        EColumnName                 = 0,
        EColumnSymbol               = 1,
        EColumnNextLower            = 2,
        EColumnNextHigher           = 3,
        EColumnFctConvertIntoSIUnit = 4,
        EColumnFctConvertFromSIUnit = 5,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelPhysSize( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
    virtual ~CModelPhysSize();
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    bool setViewMode( EViewMode i_viewMode );
    EViewMode getViewMode() const;
    unsigned int getUnitCount() const;
    void setUnitGroup( ZS::PhysVal::CUnitGrp* i_pUnitGrp );
    ZS::PhysVal::CUnitGrp* unitGroup();
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelPhysSize( const CModelPhysSize& );
private: // assignment operator not allowed
    CModelPhysSize& operator = ( const CModelPhysSize& );
protected: // instance members
    QString                m_strObjName;
    ZS::PhysVal::CUnitGrp* m_pUnitGrp;
    EViewMode              m_viewMode;

}; // class CModelPhysSize


//******************************************************************************
class CModelFctConvertInternal : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnFctConvert = 0,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelFctConvertInternal( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
    virtual ~CModelFctConvertInternal();
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    unsigned int getFctConvertsCount() const;
    void setPhysUnit( ZS::PhysVal::CPhysUnit* i_pPhysUnit );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelFctConvertInternal( const CModelFctConvertInternal& );
private: // assignment operator not allowed
    CModelFctConvertInternal& operator = ( const CModelFctConvertInternal& );
protected: // instance members
    QString                 m_strObjName;
    ZS::PhysVal::CPhysUnit* m_pPhysUnit;

}; // class CModelFctConvertInternal


//******************************************************************************
class CModelFctConvertExternal : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnUnitGrp    = 0,
        EColumnFctConvert = 1,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelFctConvertExternal( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
    virtual ~CModelFctConvertExternal();
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    unsigned int getFctConvertsCount() const;
    void setPhysUnit( ZS::PhysVal::CPhysUnit* i_pPhysUnit );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelFctConvertExternal( const CModelFctConvertExternal& );
private: // assignment operator not allowed
    CModelFctConvertExternal& operator = ( const CModelFctConvertExternal& );
protected: // instance members
    QString                 m_strObjName;
    ZS::PhysVal::CPhysUnit* m_pPhysUnit;

}; // class CModelFctConvertExternal

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_UnitModels_h
