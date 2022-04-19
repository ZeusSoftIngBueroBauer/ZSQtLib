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

#ifndef ZSSys_TrcAdminObj_h
#define ZSSys_TrcAdminObj_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

#include <QtCore/qobject.h>


class QMutex;


namespace ZS
{
namespace Trace
{
class CIdxTreeTrcAdminObjs;
class CTrcServer;

//******************************************************************************
/*! @brief Instances of this class are used to filter method tracing.

To debug and profile applications method tracing is used.

Applications usually consist of a large number of modules, classes and instances
of classes. So that the log file is not filled up with entries that are not of
interest to the developer, one would like to restrict the output to those parts
that are of interest for the current development process or the current debug session.

Instances of the CTrcAdminObj class manage the output status of individual modules,
classes and instances of classes for which Method Trace outputs have been defined.

Trace Admin instances are managed in a hierarchical tree, allowing sorting by
namespaces, class names and/or instance names.

A Trace Admin instance corresponds to a leaf within the tree. Using a Trace Admin
instance only makes sense in connection with such a tree. The tree is realized by
the CIdxTree class. This class allows to access the instances not just by a key but
also by an index within the tree. This is particularly useful if you want to
transfer changes to the attributes of a Trace Admin object from one application
to another as this reduces the amount of data to be transferred and accessing
the object via index is faster than a lookup by key.

A TCP/IP-based trace server was implemented for an online recording of the trace
outputs, but also for debugging embedded devices without or only with a very
limited screen.
*/
class ZSSYSDLL_API CTrcAdminObj : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
friend class CIdxTreeTrcAdminObjs;
    Q_OBJECT
public: // class methods
    /*! Returns the name space of the class. */
    static QString NameSpace() { return "ZS::Trace"; }
    /*! Returns the class name of the class. */
    static QString ClassName() { return "CTrcAdminObj"; }
protected: // ctors and dtor
    CTrcAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strTreeEntryName );
    virtual ~CTrcAdminObj();
signals:
    /*! @brief Emitted if ObjState, Enabled, StateOnOff or DetailLevel has been changed. */
    void changed( QObject* i_pTrcAdminObj );
    /*! @brief Emitted if the object is going to be destroyed. */
    void aboutToBeDestroyed( ZS::Trace::CTrcAdminObj* i_pTrcAdminObj );
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree();
    CTrcServer* getTraceServer();
public: // instance methods
    QString getNameSpace() const;
    QString getClassName() const;
    QString getObjectName() const;  // starting with "get" to distinguish from QObject::objectName)
public: // instance methods
    void setObjectThreadName( const QString& i_strThreadName );
    QString getObjectThreadName() const;
public: // instance methods
    int incrementRefCount();
    int decrementRefCount();
    void setRefCount( int i_iRefCount );
    int getRefCount() const;
public: // instance methods
    void setEnabled( ZS::System::EEnabled i_enabled );
    ZS::System::EEnabled getEnabled() const;
    bool isEnabled() const;
public: // instance methods
    void setTraceDetailLevel( int i_iTrcDetailLevel );
    int getTraceDetailLevel() const;
public: // instance methods
    bool isActive( int i_iFilterDetailLevel ) const;
public: // instance methods
    virtual bool blockTreeEntryChangedSignal( bool i_bBlock );
    virtual bool isTreeEntryChangedSignalBlocked() const;
protected: // instance members
    int                   m_iBlockTreeEntryChangedSignalCounter;    /*!< Counts the number of times the tree entry changed signal has been blocked. */
    QString               m_strNameSpace;       /*!< Namespace of the class. May be empty. */
    QString               m_strClassName;       /*!< Class or module name. */
    QString               m_strObjName;         /*!< Object name. May be empty if this is a class tracer. */
    QString               m_strObjThreadName;   /*!< Name of the thread in which the object was created. */
    int                   m_iRefCount;          /*!< Usually trace admin objects are only referenced by one specific module,
                                                     class or instance of a class to control the detail level of method
                                                     trace outputs. In certain circumstances or in case of a copy and paste
                                                     error the same trace admin object may be referenced by several modules,
                                                     classes or instances of classes. If so the reference counter may become
                                                     greater than 1. */
    ZS::System::EEnabled  m_enabled;            /*!< Tracing cannot only be controlled via the detail level but tracing can
                                                     also be enabled or disabled by this flag. This is useful if a group of
                                                     objects belonging to a namespace should be temporarily disabled and enabled
                                                     later on restoring the previous detail level. */
    int                   m_iTrcDetailLevel;    /*!< Defines the current detail level of the method trace outputs for the
                                                     module, class or instance referencing this object. If set to
                                                     None method trace output is disabled. */

}; // class CTrcAdminObj

//******************************************************************************
/*! @brief Instances of this class are used in combination with class
           CTrcAdminObjRefGuard to ensure that static class trace admin objects
           are freed and released if needed.
*******************************************************************************/
class ZSSYSDLL_API CTrcAdminObjRefAnchor : public QObject
{
    Q_OBJECT
public: // ctors and dtor
    CTrcAdminObjRefAnchor(const QString& i_strNameSpace, const QString& i_strClassName);
    virtual ~CTrcAdminObjRefAnchor();
public: // instance methods
    void allocTrcAdminObj();
    void releaseTrcAdminObj();
    CTrcAdminObj* trcAdminObj();
    void setTraceDetailLevel(int i_iTrcDetailLevel);
    bool isActive(int i_iFilterDetailLevel) const;
private slots:
    void onTrcAdminObjDestroyed(QObject* i_pTrcAdminObj);
private: // instance members
    QString       m_strNameSpace;
    QString       m_strClassName;
    int           m_idxInTree;
    CTrcAdminObj* m_pTrcAdminObj;
};

//******************************************************************************
/*! @brief Instances of this class are used in combination with class
           CTrcAdminObjRefGuard to ensure that static class trace admin objects
           are freed and released if needed.
*******************************************************************************/
class ZSSYSDLL_API CTrcAdminObjRefGuard
{
public: // ctors and dtor
    CTrcAdminObjRefGuard(CTrcAdminObjRefAnchor* i_pRefAnchor);
    ~CTrcAdminObjRefGuard();
public: // instance methods
    CTrcAdminObj* trcAdminObj();
    void setTraceDetailLevel(int i_iTrcDetailLevel);
    bool isActive(int i_iFilterDetailLevel) const;
private: // instance members
    CTrcAdminObjRefAnchor* m_pRefAnchor;
};

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_TrcAdminObj_h
