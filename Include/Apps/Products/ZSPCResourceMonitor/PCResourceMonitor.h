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

#ifndef ZSApps_PCResourceMonitor_h
#define ZSApps_PCResourceMonitor_h

#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>

#include <QtCore/qobject.h>

#include "ZSPhysVal/ZSPhysValArr.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysAux.h"

class QSettings;
class QTimer;

namespace ZS
{
namespace System
{
struct SErrResultInfo;
class CErrLog;
}

namespace Trace
{
class CTrcAdminObj;
}

namespace Apps
{
namespace Products
{
namespace PCResourceMonitor
{
class CModelProcessors;
class CModelProcessorsValues;
class CModelMemory;
class CModelMemoryValues;
class CModelLogicalDisks;
class CModelLogicalDisksValues;

//******************************************************************************
struct SAutoUpdateIntervals
//******************************************************************************
{
public: // default ctor
    SAutoUpdateIntervals();
public: // operators
    bool operator == ( const SAutoUpdateIntervals& i_intervalsOther );
    bool operator != ( const SAutoUpdateIntervals& i_intervalsOther );
public: // struct members
    int m_iComputerSystem_ms;
    int m_iOperatingSystem_ms;
    int m_iProcessor_ms;
    int m_iProcessorValues_ms;
    int m_iMemory_ms;
    int m_iMemoryValues_ms;
    int m_iLogicalDisks_ms;
    int m_iLogicalDisksValues_ms;

}; // struct SAutoUpdateIntervals

//******************************************************************************
class CPCResourceMonitor : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Products::PCResourceMonitor"; }
    static QString ClassName() { return "CPCResourceMonitor"; }
public: // type definitions and constants
    typedef enum {
        EStateIdle    = 0,
        EStateRunning = 1,
        EStateCount
    }   EState;
    static QString state2Str( int i_iState );
    typedef enum {
        EDriveTypeUnknown       = 0,
        EDriveTypeNoRootDir     = 1,
        EDriveTypeRemovableDisk = 2,
        EDriveTypeLocalDisk     = 3,
        EDriveTypeNetworkDrive  = 4,
        EDriveTypeCompactDisk   = 5,
        EDriveTypeRAMDisk       = 6,
        EDriveTypeCount
    }   EDriveType;
    static QString driveType2Str( int i_iDriveType );
    //static CA::StdPC::EStorageType driveType2StorageType( int i_iDriveType );
public: // ctors and dtor
    CPCResourceMonitor( const QString& i_strObjName );
    virtual ~CPCResourceMonitor();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    // Computer System
    void autoUpdateIntervalComputerSystemChanged();
    void autoUpdateComputerSystemStateChanged();
    void computerSystemChanged();
    // Operating System
    void autoUpdateIntervalOperatingSystemChanged();
    void autoUpdateOperatingSystemStateChanged();
    void operatingSystemChanged();
    // Processor Equipment
    void autoUpdateIntervalProcessorChanged();
    void autoUpdateProcessorStateChanged();
    void processorChanged();
    // Processor Load Values
    void autoUpdateIntervalProcessorValuesChanged();
    void autoUpdateProcessorValuesStateChanged();
    void processorValuesChanged();
    // Memory Equipment
    void autoUpdateIntervalMemoryChanged();
    void autoUpdateMemoryStateChanged();
    void memoryChanged();
    // Memory Load Values
    void autoUpdateIntervalMemoryValuesChanged();
    void autoUpdateMemoryValuesStateChanged();
    void memoryValuesChanged();
    // Logical Disks Equipment
    void autoUpdateIntervalLogicalDisksChanged();
    void autoUpdateLogicalDisksStateChanged();
    void logicalDisksChanged();
    // Logical Disks Load Values
    void autoUpdateIntervalLogicalDisksValuesChanged();
    void autoUpdateLogicalDisksValuesStateChanged();
    void logicalDisksValuesChanged();
protected:
    void initWbem();
    void releaseWbem();
public: // instance methods retrieving values from the host
    void saveQueryValues( const QString& i_strWinClass );
    // Computer System
    void updateComputerSystem();
    void startAutoUpdateComputerSystem();
    void stopAutoUpdateComputerSystem();
    EState getAutoUpdateComputerSystemState() const;
    void setAutoUpdateIntervalComputerSystemInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalComputerSystemInMs() const;
    // Operating System
    void updateOperatingSystem();
    void startAutoUpdateOperatingSystem();
    void stopAutoUpdateOperatingSystem();
    EState getAutoUpdateOperatingSystemState() const;
    void setAutoUpdateIntervalOperatingSystemInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalOperatingSystemInMs() const;
    // Processor Equipment
    void updateProcessor();
    void startAutoUpdateProcessor();
    void stopAutoUpdateProcessor();
    EState getAutoUpdateProcessorState() const;
    void setAutoUpdateIntervalProcessorInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalProcessorInMs() const;
    // Processor Load Values
    void updateProcessorValues();
    void startAutoUpdateProcessorValues();
    void stopAutoUpdateProcessorValues();
    EState getAutoUpdateProcessorValuesState() const;
    void setAutoUpdateIntervalProcessorValuesInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalProcessorValuesInMs() const;
    // Memory Equipment
    void updateMemory();
    void startAutoUpdateMemory();
    void stopAutoUpdateMemory();
    EState getAutoUpdateMemoryState() const;
    void setAutoUpdateIntervalMemoryInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalMemoryInMs() const;
    // Memory Load Values
    void updateMemoryValues();
    void startAutoUpdateMemoryValues();
    void stopAutoUpdateMemoryValues();
    EState getAutoUpdateMemoryValuesState() const;
    void setAutoUpdateIntervalMemoryValuesInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalMemoryValuesInMs() const;
    // Logical Disks Equipment
    void updateLogicalDisks();
    void startAutoUpdateLogicalDisks();
    void stopAutoUpdateLogicalDisks();
    EState getAutoUpdateLogicalDisksState() const;
    void setAutoUpdateIntervalLogicalDisksInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalLogicalDisksInMs() const;
    // Logical Disks Load Values
    void updateLogicalDisksValues();
    void startAutoUpdateLogicalDisksValues();
    void stopAutoUpdateLogicalDisksValues();
    EState getAutoUpdateLogicalDisksValuesState() const;
    void setAutoUpdateIntervalLogicalDisksValuesInMs( int i_iInterval_ms );
    int getAutoUpdateIntervalLogicalDisksValuesInMs() const;
protected:
    void saveAutoUpdateIntervals();
    void readAutoUpdateIntervals();
protected slots:
    void onAutoUpdateOperatingSystemTimeout();
    void onAutoUpdateComputerSystemTimeout();
    void onAutoUpdateProcessorTimeout();
    void onAutoUpdateProcessorValuesTimeout();
    void onAutoUpdateMemoryTimeout();
    void onAutoUpdateMemoryValuesTimeout();
    void onAutoUpdateLogicalDisksTimeout();
    void onAutoUpdateLogicalDisksValuesTimeout();
public: // instance methods
    // Computer System
    ZS::PhysVal::CPhysVal getManufacturerUpdSysTime() const;
    QString getManufacturer() const;
    ZS::PhysVal::CPhysVal getModelUpdSysTime() const;
    QString getModel() const;
    ZS::PhysVal::CPhysVal getSerialNrUpdSysTime() const;
    QString getSerialNr() const;
    ZS::PhysVal::CPhysVal getDNSHostNameUpdSysTime() const;
    QString getDNSHostName() const;
    ZS::PhysVal::CPhysVal getDomainUpdSysTime() const;
    QString getDomain() const;
    // Operating System
    ZS::PhysVal::CPhysVal getOSUpdSysTime() const;
    QString getOS() const;
    ZS::PhysVal::CPhysVal getOSServicePackUpdSysTime() const;
    QString getOSServicePack() const;
    // Processor Equipment
    ZS::PhysVal::CPhysVal getProcessorsCountUpdSysTime() const;
    unsigned int getProcessorsCount() const;
    ZS::PhysVal::CPhysVal getLogicalProcessorsCountUpdSysTime() const;
    unsigned int getLogicalProcessorsCount() const;
    CModelProcessors* getModelProcessors();
    // Processor Load Values
    CModelProcessorsValues* getModelProcessorsValues();
    // Memory Equipment
    CModelMemory* getModelMemory();
    // Memory Load Values
    CModelMemoryValues* getModelMemoryValues();
    // Logical Disks Equipment
    CModelLogicalDisks* getModelLogicalDisks();
    // Logical Disks Values
    CModelLogicalDisksValues* getModelLogicalDisksValues();
protected: // instance members
    QString                   m_strObjName;
    ZS::System::CErrLog*              m_pErrLog;
    SAutoUpdateIntervals      m_autoUpdateIntervals;
    // Wbem Interface (web base environment management via dcom)
    bool                      m_bWbemInitialized;
    IWbemServices*            m_pIWbemServices;
    IWbemRefresher*           m_pIWbemRefresher;
    // Win32_PerfFormattedData_PerfOS_Processor
    IWbemHiPerfEnum*          m_pIWbemHiPerfEnumOSProc;
    bool                      m_bIWbemHiPerfEnumOSProcGotHandles;
    IWbemObjectAccess**       m_arpIWbemObjAccessOSProc;
    ULONG                     m_uObjAccessOSProcCount;
    CIMTYPE                   m_cimTypeOSProcLoad;
    LONG                      m_lhndOSProcLoad;
    // Computer System
    EState                    m_stateAutoUpdateComputerSystem;
    QTimer*                   m_pTimerAutoUpdateComputerSystem;
    ZS::PhysVal::CPhysVal     m_physValComputerSystemManufacturerUpdSysTime;
    QString                   m_strComputerSystemManufacturer;
    ZS::PhysVal::CPhysVal     m_physValComputerSystemModelUpdSysTime;
    QString                   m_strComputerSystemModel;
    ZS::PhysVal::CPhysVal     m_physValComputerSerialNrUpdSysTime;
    QString                   m_strComputerSerialNr;
    ZS::PhysVal::CPhysVal     m_physValComputerSystemDNSHostNameUpdSysTime;
    QString                   m_strComputerSystemDNSHostName;
    ZS::PhysVal::CPhysVal     m_physValComputerSystemDomainUpdSysTime;
    QString                   m_strComputerSystemDomain;
    // Processor Equipment
    EState                    m_stateAutoUpdateProcessor;
    QTimer*                   m_pTimerAutoUpdateProcessor;
    ZS::PhysVal::CPhysVal     m_physValComputerSystemNumberOfProcessorsUpdSysTime;
    unsigned int              m_uComputerSystemNumberOfProcessors;
    ZS::PhysVal::CPhysVal     m_physValComputerSystemNumberOfLogicalProcessorsUpdSysTime;
    unsigned int              m_uComputerSystemNumberOfLogicalProcessors;
    CModelProcessors*         m_pModelProcessors;
    // Operating System
    EState                    m_stateAutoUpdateOperatingSystem;
    QTimer*                   m_pTimerAutoUpdateOperatingSystem;
    ZS::PhysVal::CPhysVal     m_physValOSCaptionUpdSysTime;
    QString                   m_strOSCaption;
    ZS::PhysVal::CPhysVal     m_physValOSServicePackUpdSysTime;
    QString                   m_strOSServicePack;
    // Processors Load Values
    EState                    m_stateAutoUpdateProcessorValues;
    QTimer*                   m_pTimerAutoUpdateProcessorValues;
    CModelProcessorsValues*   m_pModelProcessorsValues;
    // Memory Equipment
    EState                    m_stateAutoUpdateMemory;
    QTimer*                   m_pTimerAutoUpdateMemory;
    CModelMemory*             m_pModelMemory;
    // Memory Load Values
    EState                    m_stateAutoUpdateMemoryValues;
    QTimer*                   m_pTimerAutoUpdateMemoryValues;
    CModelMemoryValues*       m_pModelMemoryValues;
    // Logical Disks Equipment
    EState                    m_stateAutoUpdateLogicalDisks;
    QTimer*                   m_pTimerAutoUpdateLogicalDisks;
    CModelLogicalDisks*       m_pModelLogicalDisks;
    // Logical Disks Load Values
    EState                    m_stateAutoUpdateLogicalDisksValues;
    QTimer*                   m_pTimerAutoUpdateLogicalDisksValues;
    CModelLogicalDisksValues* m_pModelLogicalDisksValues;
private: // instance members
    ZS::Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CPCResourceMonitor

} // namespace PCResourceMonitor

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_PCResourceMonitor_h
