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

#ifndef ZSSys_TrcMthFile_h
#define ZSSys_TrcMthFile_h

#include <QtCore/qobject.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qhash.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysLogFile.h"

class QFile;
class QMutex;
class QTextStream;

namespace ZS
{
namespace System
{
//******************************************************************************
/*! @brief The class CTrcMthFile offers multithreaded applications the ability to write
    method trace output to text files limited to a maximum number of lines.

    The class encapsulates an instance of the CLogFile class and itself offers
    all methods of this encapsulated class. Some methods, such as "addEntry" have
    a slightly modified interface that is adapted to the tracing of method calls.

    For a method trace it is important to be able to easily determine when which
    method was called by which method and when the method is exited. To do this,
    it is necessary to indent the methods in the log file and to mark whether the
    line in the file is the entry or exit in the method.

    In order to profile the program execution it is important to determine how
    much time a method call took. For this a time stamp is added to each log entry.
    The timestamp consists of a humand readable time string contain date and time
    (2020-03-11 13:02:13) and the cpu time elapsed since the program started
    in microseconds (110.643863).

    In a multithreaded application, it is also important to know from which threads
    the method was called. Since each thread has its own call stack, the column
    indentations for the method calls must be managed separately per thread.

    Example:

        int CCalc::add( int i_iVal1, int i_iVal2, bool& bOk )
        {
            QString strClassName = "CCalc";
            QString strObjName = "myCalculator";
            QString strMth = "add";
            QString strMthCall = strClassName + "::" + strObjName + "." + strMth;
            QString strMthInArgs = "Val1: " + QString::number(i_iVal1) + ", Val2: " + QString::number(i_iVal2);
            QString strMthOutArgs;
            QString strMthAddInfo;
            QString strMthRet;

            m_pTrcMthFile->traceMethodEnter(strMthCall, strMthInArgs);

            strMthAddInfo = "Starting calculation ...";
            m_pTrcMthFile->traceMethod(strMthCall, strMthAddInfo);

            int iSum = i_iVal1 + i_iVal2;
            bool bOk = true;

            strMthAddInfo = "Calculation done";
            m_pTrcMthFile->traceMethod(strMthCall, strMthAddInfo);

            strMthRet = QString::number(iSum);
            strMthOutArgs = "Ok: " + bool2Str(bOk);
            m_pTrcMthFile->traceMethodLeave(strMthCall, strMthRet, strMthOutArgs);

            return iSum;
        }

        bool bOk;

        int iSum = CCalc::add(7, 8, bOk);

    Results in log entries as follows:

        <ThreadName> 2020-03-11 13:02:13 (     110.643863): -> CCalc::myCalculator.add(Val1: 7, Val2: 8)
        <ThreadName> 2020-03-11 13:02:13 (     110.643864):    CCalc::myCalculator.add(Starting calculation ...)
        <ThreadName> 2020-03-11 13:02:13 (     110.643865):    CCalc::myCalculator.add(Calculation done)
        <ThreadName> 2020-03-11 13:02:13 (     110.643866): <- CCalc::myCalculator.add(Ok: true): 15

    The following is another example of a trace method output if two threads are involved:

        <MainThread> 2020-03-11 13:02:13 (     110.643863): -> CWdgtCalc::Inst.onButtonClicked()
        <MainThread> 2020-03-11 13:02:13 (     110.643866):    -> CThreadCalc::ctor( Add, Arg1, Arg2 )
        <MainThread> 2020-03-11 13:02:13 (     110.643867):    <- CThreadCalc::ctor()
        <MainThread> 2020-03-11 13:02:13 (     110.643868):    -> CThreadCalc::Inst.startThread()
        <MainThread> 2020-03-11 13:02:13 (     110.643870):    <- CThreadCalc::Inst.startThread()
        <Thread_x>   2020-03-11 13:02:13 (     110.643872): -> CCalc::Inst.run()
        <Thread_x>   2020-03-11 13:02:13 (     110.643876):    -> CCalc::Inst.calculate()
        <Thread_x>   2020-03-11 13:02:13 (     110.643878):       -> CSum::Inst.add( Arg1, Arg2 )
        <MainThread> 2020-03-11 13:02:13 (     110.643879): <- CWdgtCalc::Inst.onButtonClicked()
        <Thread_x>   2020-03-11 13:02:13 (     110.643880):       <- CSum::Inst.add(): sum
        <Thread_x>   2020-03-11 13:02:13 (     110.643887):    <- CCalc::Inst.calculate(): result
        <Thread_x>   2020-03-11 13:02:13 (     110.643890): <- CCalc::Inst.run()

    In the methods trace above one can easily recognize that the "onButtonClicked" method
    returns while Thread_x is active and is currently processing the "add" method.

    For better readability it would be nice if each thread was displayed with a
    different text color. This can only be done by an appropriate editor adapted
    for reading the method trace file. Or the method trace client available
    within the ZSQtLib.
*/
class ZSSYSDLL_API CTrcMthFile
//******************************************************************************
{
public: // type definitions and constants
    static const int c_iStrLenThreadMax  = 24; /*!< Maximum number of characters used for thread names. */
    static const int c_iStrLenSysTimeMax = 15; /*!< Maximum number of characters used for the system time.
                                                    The maximum value is 99999999.999999 seconds (ca. 75 years). */
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CTrcMthFile"; }
public: // class methods
    static CTrcMthFile* Alloc( const QString& i_strAbsFilePath );
    static void Free( const QString& i_strAbsFilePath );
    static void Free( CTrcMthFile* i_pTrcMthFile );
    static CTrcMthFile* FindFile( const QString& i_strAbsFilePath );
    static int GetFilesCount();
protected: // ctors and dtor
    CTrcMthFile( const QString& i_strAbsFilePath );
    virtual ~CTrcMthFile();
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void close();
    void clear();
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods (log file wrapper)
    void setAbsoluteFilePath( const QString& i_strAbsFilePath );
    QString absoluteFilePath() const;
    QString completeBaseName() const;
    QString absolutePath() const;
public: // instance methods (log file wrapper)
    void setAutoSaveInterval( int i_iInterval_ms );
    int getAutoSaveInterval() const;
    void setCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getCloseFileAfterEachWrite() const;
public: // instance methods (log file wrapper)
    void setSubFileCountMax( int i_iCountMax );
    int getSubFileCountMax() const;
    void setSubFileLineCountMax( int i_iCountMax );
    int getSubFileLineCountMax() const;
public: // instance methods
    void traceMethodEnter(
        const QString& i_strMethod,
        const QString& i_strMthInArgs );
    void traceMethod(
        const QString& i_strMethod,
        const QString& i_strAddInfo );
    void traceMethodLeave(
        const QString& i_strMethod,
        const QString& i_strMthRet,
        const QString& i_strMthOutArgs = "" );
public: // instance methods
    void traceMethodEnter(
        const QString&   i_strMthThreadName,
        const QDateTime& i_dt,
        double           i_fSysTimeInSec,
        const QString&   i_strMethod,
        const QString&   i_strMthInArgs );
    void traceMethod(
        const QString&   i_strMthThreadName,
        const QDateTime& i_dt,
        double           i_fSysTimeInSec,
        const QString&   i_strMethod,
        const QString&   i_strAddInfo );
    void traceMethodLeave(
        const QString&   i_strMthThreadName,
        const QDateTime& i_dt,
        double           i_fSysTimeInSec,
        const QString&   i_strMethod,
        const QString&   i_strMthRet,
        const QString&   i_strMthOutArgs = "" );
public: // instance methods
    void addEntry(
        const QString&         i_strMthThreadName,
        const QDateTime&       i_dt,
        double                 i_fSysTimeInSec,
        ZS::System::EMethodDir i_mthDir,
        const QString&         i_strMethod,
        const QString&         i_strAddInfo = "",
        const QString&         i_strMethodOutArgs = "" );
protected: // instance methods
    int refCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // auxiliary instance methods
    QString currentThreadName() const;
protected: // class members
    static QMutex                      s_mtx;             /*!< Mutex to protect the static and instance methods of the class for multithreaded access. */
    static QMap<QString, CTrcMthFile*> s_mapTrcMthFiles;  /*!< Map containing all existing (allocated) method trace file instances.
                                                               The key of the entries is the absolute file path of the method trace files.
                                                               The value is the pointer to the method trace file instance. */
protected: // instance methods
    bool                  m_bEnabled;               /*!< By setting this flag to false writing method traces to the log file may be temporarily disabled. */
    ZS::System::CLogFile* m_pLogFile;               /*!< Log file instance used to write the method trace. */
    int                   m_iRefCount;              /*!< Number of allocs for this method trace file.
                                                         Will be decreased with each "free" call. If the ref count becomes 0 the method trace file instance will be
                                                         deleted (but the file of course remains on disk). */
    QHash<QString,int>    m_hashThreadCallDepths;   /*!< Map containing the call depth for each thread.
                                                         The key of the entries is the name of the thread (or the tread id converted into a string).
                                                         The value is the current call depth. */

}; // class CTrcMthFile

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_TrcMthFile_h
