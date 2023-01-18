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
class ZSSYSDLL_API CLogger : public QObject, public CIdxTreeEntry
//******************************************************************************
{
friend class CIdxTreeLoggers;
    Q_OBJECT
public: // class methods
    /*! Returns the name space of the class. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name of the class. */
    static QString ClassName() { return "CLogger"; }
protected: // ctors and dtor
    CLogger( const QString& i_strLeaveName );
    virtual ~CLogger();
signals:
    /*! @brief Emitted if ObjState, Enabled, StateOnOff or DetailLevel has been changed. */
    void changed( QObject* i_pLogger );
    /*! @brief Emitted if the object is going to be destroyed. */
    void aboutToBeDestroyed( QObject* i_pLogger );
public: // instance methods
    CIdxTreeLoggers* getLoggersIdxTree();
    CLogServer* getLogServer();
public: // instance methods
    void log( ELogDetailLevel i_eFilterDetailLevel, const QString& i_strLogEntry );
public: // instance methods
    void setEnabled( EEnabled i_enabled );
    EEnabled getEnabled() const;
    bool isEnabled() const;
public: // instance methods
    void setLogLevel( ELogDetailLevel i_eDetailLevel );
    ELogDetailLevel getLogLevel() const;
    bool isActive( ELogDetailLevel i_eFilterDetailLevel ) const;
public: // instance methods
    void setDataFilter( const QString& i_strFilter = "" );
    QString getDataFilter() const;
    bool isSuppressedByDataFilter( const QString& i_strLogEntry ) const;
public: // instance methods
    void setAddThreadName( bool i_bAdd );
    bool addThreadName() const;
    void setAddDateTime( bool i_bAdd );
    bool addDateTime() const;
    void setAddSystemTime( bool i_bAdd );
    bool addSystemTime() const;
    void setNameSpace( const QString& i_strNameSpace = "" );
    QString getNameSpace() const;
    void setClassName( const QString& i_strClassName = "" );
    QString getClassName() const;
    void setObjectName( const QString& i_strObjName = "" );
    QString getObjectName() const;
public: // instance methods
    virtual bool blockTreeEntryChangedSignal( bool i_bBlock );
    virtual bool isTreeEntryChangedSignalBlocked() const;
private: // Don't use QObject::objectName
    QString objectName() const;
protected: // instance members
    /*!< Counts the number of times the tree entry changed signal has been blocked. */
    int m_iBlockTreeEntryChangedSignalCounter;
    /*!< Flag to indicate whether the current thread name should be added to the log entry. */
    bool m_bAddThreadName;
    /*!< Flag to indicate whether the current date time should be added to the log entry. */
    bool m_bAddDateTime;
    /*!< Flag to indicate whether the current process time should be added to the log entry. */
    bool m_bAddSystemTime;
    /*!< Namespace of the class. May be empty. */
    QString m_strNameSpace;
    /*!< Class or module name. May be empty. */
    QString m_strClassName;
    /*!< Object name. May be empty. */
    QString m_strObjName;
    /*!< Logging cannot only be controlled via the detail level but logging can
         also be enabled or disabled by this flag. This is useful if a group of
         objects belonging to a namespace should be temporarily disabled and enabled
         later on restoring the previous detail level. */
    EEnabled m_enabled;
    /*!< Defines the current detail level of the log outputs for the
         module, class or instance referencing this object. If set to
         None log output is disabled. */
    ELogDetailLevel m_eDetailLevel;
    /*!< Data may also be suppressed by applying a filter.
         Filtering can be done in two ways:
         - Strings may be defined which must occur in the log entry.
         - Strings may be defined which may not occur in the log entry. */
    QString m_strDataFilter;
    /*!< When applying the data filter the data filter will be split into
         strings which must be included or excluded. The "Must Include"
         strings are stored in this string list. */
    QStringList m_strlstDataFilterInclude;
    /*!< When applying the data filter the data filter will be split into
         strings which must be included or excluded. The "Must Not Include"
         strings are stored in this string list. */
    QStringList m_strlstDataFilterExclude;

}; // class CLogger

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Logger_h
