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
/*! Instances of this class are used to filter method tracing.

    To debug and profile applications method tracing is used.
    Applikationen bestehen in der Regel aus einer Vielzahl von Module, Klassen
    und Instanzen von Klassen. Damit das Log File nicht mit Eintraegen voll
    geschrieben wird, die den Entwickler nicht interessieren, moechte man die
    Ausgabe auf die Teile beschraenken, die fuer den aktuellen Entwicklungsprozess
    bzw. die aktuelle Debug Sitzung von Interesse ist.

    Instanzen der Klasse CTrcAdminObj verwalten den Ausgabe Status einzelner
    Module, Klassen und Instanzen von Klassen, fuer die Method Trace Ausgaben
    definiert wurden.

    Trace Admin Instanzen werden in einem hierarchischen Baum verwaltet und
    erlaubt so die Sortierung nach Namespaces, Klassennamen und oder Instanznamen.
    Eine Trace Admin Instanz entspricht einem Blatt innerhalb des Baums.
    Der Einsatz einer Trace Admin Instanz macht nur in Verbindung eines solchen
    Baumes Sinn. Der Baum wird durch die Klasse CIdxTree realisiert. Diese
    Klasse erlaubt es, auf die Instanzen innerhalb des Baumes ueber einen
    Index zuzugreifen. Dies ist insbesondere dann sinnvoll, wenn man die
    Aenderungen an den Attributen eines Trace Admin Objekts von einer Applikation
    zu einer einen uebertragen will. Fuer einen online Mitschnitt der Trace
    Ausgaben aber auch um Embedded Geräte ohne oder nur mit sehr begrenztem
    Bildschirm zu debuggen wurde ein auf TCP/IP basierter Trace Server
    implementiert, der durch Verwendung von Indizes als Object Ids zum Einen
    den notwendigen Datenstrom reduziert aber auch das Finden der Trace Admin
    Objekte und somit den Online Mitschnitt der Trace Ausgaben beschleunigt.

    Zugriff auf den Trace Server erhaelt die Trace Admin Objekt Klasse ueber das
    Parent Object des Trace Admin Object Index Trees. Das ermoeglicht der Klasse
    CMethodTracer den methoden trace ueber den Trace Server auszugeben.

    Fuer weitere Details zur Verwendung der Trace Admin Klasse siehe Klassen
    CTrcServer und CIdxTreeTrcAdminObjs.
*/
class ZSSYSDLL_API CTrcAdminObj : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
friend class CIdxTreeTrcAdminObjs;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace"; }
    static QString ClassName() { return "CTrcAdminObj"; }
protected: // ctors and dtor
    CTrcAdminObj( const QString& i_strObjName, const QString& i_strObjThreadName = "" );
    virtual ~CTrcAdminObj();
signals:
    void changed( QObject* i_pTrcAdminObj ); /*!< Emitted if ObjState, Enabled, StateOnOff or DetailLevel has been changed */
    void aboutToBeDestroyed( ZS::Trace::CTrcAdminObj* i_pTrcAdminObj );
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree();
    CTrcServer* getTraceServer();
public: // instance methods
    QString getNameSpace() const;   // starting with "get" to distinguish from name space of CTrcAdminObj (ZS::Trace)
    QString getClassName() const;   // starting with "get" to distinguish from class name "CTrcAdminObj"
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
    bool isActive( int i_iDetailLevel ) const;
public: // instance methods
    virtual bool blockTreeEntryChangedSignal( bool i_bBlock );
    virtual bool isTreeEntryChangedSignalBlocked() const;
protected: // instance members
    int                   m_iBlockTreeEntryChangedSignalCounter;    /*!< Counts the number of times the tree entry changed signal has been blocked. */
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

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_TrcAdminObj_h
