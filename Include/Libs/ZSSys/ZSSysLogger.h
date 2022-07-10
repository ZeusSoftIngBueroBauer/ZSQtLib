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

#ifndef ZSSys_Logger_h
#define ZSSys_Logger_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

#include <QtCore/qobject.h>


class QMutex;


namespace ZS
{
namespace System
{
class CIdxTreeLoggers;
class CLogServer;

//******************************************************************************
/*! @brief Instances of this class are used to filter logging.

*/
class ZSSYSDLL_API CLogger : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
friend class CIdxTreeLoggers;
    Q_OBJECT
public: // class methods
    /*! Returns the name space of the class. */
    static QString NameSpace() { return "ZS::Log"; }
    /*! Returns the class name of the class. */
    static QString ClassName() { return "CLogger"; }
protected: // ctors and dtor
    CLogger(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strTreeEntryName );
    virtual ~CLogger();
signals:
    /*! @brief Emitted if ObjState, Enabled, StateOnOff or DetailLevel has been changed. */
    void changed( QObject* i_pLogger );
    /*! @brief Emitted if the object is going to be destroyed. */
    void aboutToBeDestroyed( ZS::System::CLogger* i_pLogger );
public: // instance methods
    CIdxTreeLoggers* getLoggersIdxTree();
    CLogServer* getLogServer();
public: // instance methods
    QString getNameSpace() const;
    QString getClassName() const;
public: // instance methods (reimplementing methods from base class QObject)
    void setObjectName( const QString& i_strObjName );
    QString getObjectName() const;
public: // instance methods
    void setObjectThreadName( const QString& i_strThreadName );
    QString getObjectThreadName() const;
public: // instance methods
    int lock();
    int unlock();
    bool isLocked() const;
    int getLockCount() const;
    void setDeleteOnUnlock( bool i_bDelete );
    bool deleteOnUnlock() const;
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
    void setDetailLevel( ZS::System::ELogDetailLevel i_eTrcDetailLevel );
    ZS::System::ELogDetailLevel getDetailLevel() const;
    bool isActive( ZS::System::ELogDetailLevel i_eFilterDetailLevel ) const;
public: // instance methods
    void setDataFilter( const QString& i_strFilter );
    QString getDataFilter() const;
    bool isDataSuppressedByFilter( const QString& i_strData ) const;
public: // instance methods
    virtual bool blockTreeEntryChangedSignal( bool i_bBlock );
    virtual bool isTreeEntryChangedSignalBlocked() const;
private: // Don't use QObject::objectName
    QString objectName() const;
protected: // instance members
    int     m_iBlockTreeEntryChangedSignalCounter; /*!< Counts the number of times the tree entry changed signal has been blocked. */
    QString m_strNameSpace;     /*!< Namespace of the class. May be empty. */
    QString m_strClassName;     /*!< Class or module name. */
    QString m_strObjName;       /*!< Object name. May be empty if this is a class tracer. */
    QString m_strObjThreadName; /*!< Name of the thread in which the object was created. */
    /*!< The logger may be locked so that it will not be deleted
         after e.g. renaming the object. */
    int m_iLockCount;
    /*!< Flag to indicate that the logger is no longer needed and
         should be deleted if it gets unlocked. */
    bool m_bDeleteOnUnlock;
    /*!< Usually loggers are only referenced by one specific module, class
         or instance of a class to control the detail level of log outputs.
         In certain circumstances or in case of a copy and paste error the
         same logger may be referenced by several modules, classes or
         instances of classes. If so the reference counter may become
         greater than 1. */
    int m_iRefCount;
    /*!< Logging cannot only be controlled via the detail level but logging can
         also be enabled or disabled by this flag. This is useful if a group of
         objects belonging to a namespace should be temporarily disabled and enabled
         later on restoring the previous detail level. */
    ZS::System::EEnabled  m_enabled;
    /*!< Defines the current detail level of the log outputs for the
         module, class or instance referencing this object. If set to
         None log output is disabled. */
    ZS::System::ELogDetailLevel m_eDetailLevel;
    /*!< Data may also be suppressed by applying a filter.
         This filter is a regular expression which allows to define a positive
         pattern where only the data will be logged which mets the expression
         or a negative pattern which supporessed the log output if the
         filter does not match. */
    QString m_strDataFilter;

}; // class CLogger

//******************************************************************************
/*! @brief Instances of this class are used in combination with class
           CLoggerRefGuard to ensure that static class loggers
           are freed and released if needed.
*******************************************************************************/
class ZSSYSDLL_API CLoggerRefAnchor : public QObject
{
    Q_OBJECT
public: // ctors and dtor
    CLoggerRefAnchor(const QString& i_strNameSpace, const QString& i_strClassName);
    virtual ~CLoggerRefAnchor();
public: // instance methods
    void allocLogger();
    void releaseLogger();
    CLogger* logger();
    void setDetailLevel(ZS::System::ELogDetailLevel i_eTrcDetailLevel);
    bool isActive(ZS::System::ELogDetailLevel i_eFilterDetailLevel) const;
private slots:
    void onLoggerDestroyed(QObject* i_pLogger);
private: // instance members
    mutable QMutex m_mtx;           /*!< Mutex to protect the instance members. */
    QString        m_strNameSpace;  /*!< Name space of the class to be logged. */
    QString        m_strClassName;  /*!< Name of class to be logged. */
    CLogger*       m_pLogger;       /*!< Pointer to logger. */
    int            m_idxInTree;     /*!< If once added the index in the tree is stored for faster access. */
    int            m_iRefCount;     /*!< Counts how often the logger is allocated but not released. */
};

//******************************************************************************
/*! @brief Instances of this class are used in combination with class
           CLoggerRefGuard to ensure that static class loggers
           are freed and released if needed.
*******************************************************************************/
class ZSSYSDLL_API CLoggerRefGuard
{
public: // ctors and dtor
    CLoggerRefGuard(CLoggerRefAnchor* i_pRefAnchor);
    ~CLoggerRefGuard();
public: // instance methods
    CLogger* logger();
    void setDetailLevel(ZS::System::ELogDetailLevel i_eTrcDetailLevel);
    bool isActive(ZS::System::ELogDetailLevel i_eFilterDetailLevel) const;
private: // instance members
    CLoggerRefAnchor* m_pRefAnchor;    /*!< Pointer to reference anchor which should be guarded. */
};

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Logger_h
