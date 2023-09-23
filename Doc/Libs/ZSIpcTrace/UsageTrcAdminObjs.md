@page _PAGE_Libs_ZSIpcTrace_Usage_TrcAdminObjs Creating Trace Admin Objects

Trace admin objects control what is traced and how.

A trace admin object can be associated with one or more modules, one or more
classes, or one or all instances of a class.

**Class and Module Tracer**

Usually you want to control trace outputs for a certain class and if tracing
for the class is enabled, how detailed these outputs should be.

To do this, a static class variable must be created and registered with the
Trace Server.

You cannot instantiate the trace admin object as a member by value as the order
in which static variables are initilized is undefined. And the trace admin object
can only be registered with the trace server if the trace server already exists.
This means that the trace admin object must be created on the heap at runtime after
the trace server has been created.

So you cannot define a static class variable like this:

    class CMyClass1
    {
    private: // class members
        // !!! DON'T DO THIS !!!
        static ZS::Trace::CTrcAdminObj s_trcAdminObj;
        ...
    };

Instead you need a pointer to the trace admin object and register it during
runtime at the server if needed. If the admin object is no longer needed it
must be released to avoid memory leaks.

    class CMyClass1
    {
    public:
        static void classMethod();
    private: // class members
        static ZS::Trace::CTrcAdminObj* s_pTrcAdminObj;
        ...
    };


    void classMethod()
    {
        if( s_pTrcAdminObj == nullptr )
        {
            s_pTrcAdminObj = CTrcServer::GetInstance("MyNameSpace", "MyClassName");
        }

        ....

        if( s_pTrcAdminObj != nullptr )
        {
            CTrcServer::ReleaseInstance(s_pTrcAdminObj);
            s_pTrcAdminObj = nullptr;
        }
    }

!!! The following description need to be revised. !!!
!!! The trace admin object already has a referene counter. !!!
!!! RefGuard and RefAnchor is a design pattern. !!!
!!! RefGuard to trace class methods to ensure that trace admin object is released if method returns. !!!

If you don't trace class methods but just instance methods you can implement a
reference counter to ensure that no memory leaks are left behind and that the
trace admin object is created exactly once and freed, if no longer needed.

There are different ways to implement such a reference counter.

E.g. in the constructor the class trace admin object is created if not existing and
the reference counter is incremented. In the destructor the reference counter
is decremented and if the reference counter reaches 0 the trace server is informed
that the trace admin object is no longer needed.

The two classes `CTrcAdminObjRefAnchor` and the guard class `CTrcAdminObjRefGuard`
have been invented to help you realizing the reference counting.

In addition using the guard class ensures that the trace admin object is also
released if the method to be traced exits on an exeption.

To trace class methods an instance of the `CTrcAdminObjRefGuard` has to be 
created on the stack.

To trace instance methods a reference guard instance is not needed and is
not recommended to be used. Instead use the reference anchor instance, allocate
the trace admin instance in the constructor and deallocate the instance in the
destructor of your class.

Header File:

    class CMyClass1
    {
    public: // class methods
        static QString NameSpace() { return "My::Name::Space"; }
        static QString ClassName() { return "CMyClass1"; }
    public: // class methods
        static QString classMethod(const QString& i_strMthInArgs);
    public: // ctor and dtor
        CMyClass1();
        ~CMyClass1();
    public: // instance methods
        QString instMethod(const QString& i_strMthInArgs);
    private: // class members
        static ZS::Trace::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchor;
    };

Source File:

    CTrcAdminObjRefAnchor CMyClass1::s_trcAdminObjRefAnchor(
        CMyClass1::NameSpace(), CMyClass1::ClassName());

    QString CMyClass1::classMethod(const QString& i_strMthInArgs)
    {
        ...
        // In the class method a reference guard is created ensuring that
        // trace admin object is released when leaving the method.
        CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchor);
        ...
    }

    CMyClass1::CMyClass1()
    {
        ...
        // In the constructor no reference guard is needed. But the
        // reference anchor may be used to access the trace admin object.
        s_trcAdminObjRefAnchor.allocTrcAdminObj();
        ...
    }

    CMyClass1::~CMyClass1()
    {
        ...
        // In the destructor also no reference guard is needed. But the
        // reference anchor may be used to release the trace admin object.
        s_trcAdminObjRefAnchor.releaseTrcAdminObj();
        ...
    }

    QString CMyClass1::instMethod(const QString& i_strMthInArgs)
    {
        // In the instance method a reference guard is not needed.
        // It is sufficient to check whether a trace admin object
        // is available to use it.
        if( s_trcAdminObjRefAnchor.trcAdminObj() != nullptr )
        {
            ...
        }
        ...
    }

If you would use a reference guard instance also to control method tracing of
the instance methods the pointer to trace admin object will always be queried
from the trace server on entering the method and released on leaving the method.

For instance methods this is not necessary and getting the admin object in the
constructor and releasing it again in the destructor is sufficient and saves time.

**Additional Info**

On releasing a trace admin object from the trace server the admin object will
not be destroyed but only a reference counter is decremented. Even if the
reference counter for this trace admin object reaches 0 the object is not
destroyed but kept in memory indicating that the objects reference counter
is 0. This way the properties of the trace admin object (enabled state, trace
detail level) may be modified even if the trace admin object is not yet
(or no longer) referenced.

This has to be taken into account when recalling the tree of trace admin objects
during startup. The XML file may contain entries for trace admin objects which
no longer exists as the corresponding module, class or instance may have been
removed or renamed. So from time to time it might be necessary to delete the
trace admin objects XML file.

Only if the trace server is going to be destroyed during shutdown the tree of
trace admin objects will be destroyed together with all entries.

To ensure that during shutdown no trace admin objects are accessed which have
already been destroyed the trace server should be deleted after all classes
have been destroyed referencing trace admin objects.

Special care has to be taken when instantiating classes on the stack of the
'main' routine. If those classes would use trace admin objects they would
access the trace admin object after the trace server has been destroyed.
And if the trace server is destroyed also all trace admin objects will be
destroyed. So the class would access an already deleted object.

The same applies if the trace server is created in the constructor of the
class dervied from 'QCoreApplication'. Don't create members by value if they
use trace admin objects. You need to create them on the heap and destroy
those instances before the trace server is destroyed.

**Instance Tracer**

As already mentioned, a trace admin object can also be used to limit the output
to individual instances of a class. For this each instance should get a unique
object name.

In order not to lose the overview if many instances of the class are created during
runtime you may logically group the instances by their names. E.g. you may name an
instance 'MainPath::SubPath::ObjectName' (e.g. 'Sound::Level::Max').

If you use a different separator for the object path (e.g. "/") you need to replace
this separator by the separator used by the trace admin index tree ("::") as the
trace admin object tree uses the same node separator for name spaces, class names
and object names.

To do this, an instance variable must be created and registered with the
Trace Server.

You can neither use the reference anchor class nor the guard class for trace
admin objects defined as instance members. And this is not necessary as
accessing the admin object is only done once in the constructor and released
just once in the destructor.

Header File:

    class CMyClass2 : public QObject
    {
    public: // ctor and dtor
        CMyClass2();
        ~CMyClass2();
    public: // instance methods
        QString instMethod(const QString& i_strMthInArgs);
    private: // instance members
        ZS::Trace::CTrcAdminObj* m_pTrcAdminObj = nullptr;
    };

Source File:

    CMyClass2::CMyClass2(const QString& i_strObjName) :
        QObject(i_strObjName)
    {
        setObjectName(i_strObjName);

        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMyClass2::~CMyClass2()
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    QString CMyClass2::instMethod(const QString& i_strMthInArgs)
    {
        ...
        if( m_pTrcAdminObj != nullptr )
        {
            ...
        }
        ...
    }

**Several Class Tracers and Instance Tracers per Class**

You may combine several class tracers and instance tracers per class.

If you want to trace not always all methods of a class because you have
some interesting methods and others, which are invoked very often and just
would produce too many noise, you could use several class tracers to filter
the output for those methods. 'paint' or 'update' methods of the QWidget
class are methods which may produce too much noise if you would trace them
always. But if you want to search for problems concerning those noisy methods
you may wish to activate tracing for those methods.

But each trace admin objects must have a unique name and as only a namespace and
the class name may be assigned to class tracers using the anchor and anchor guard
class artificial names must be invented for subclasses.

Header File:

    class CMyClass3
    {
        ...
    private: // class members
        static ZS::Trace::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchor;
        static ZS::Trace::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchorNoisyMethods;
        static ZS::Trace::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchorVeryNoisyMethods;
    };

Source File:

    CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchor(
        CMyClass3::NameSpace(), CMyClass3::ClassName());

    CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchorNoisyMethods(
        CMyClass3::NameSpace(), CMyClass3::ClassName() + "::NoisyMethods") ;

    CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchorVeryNoisyMethods(
        CMyClass3::NameSpace(), CMyClass3::ClassName() + "::VeryNoisyMethods") ;

Same applies for instance traces which all need a unique name.

Header File:

    class CMyClass3
    {
    public: // ctor and dtor
        CMyClass3();
        ~CMyClass3();
        ...
    private: // instance members
        ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;
        ZS::Trace::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;
        ZS::Trace::CTrcAdminObj* m_pTrcAdminObjVeryNoisyMethods;
    };

Source File:

    CMyClass3::CMyClass3(const QString& i_strObjName) :
        QObject(i_strObjName)
    {
        setObjectName(i_strObjName);

        m_pTrcAdminObj =
            CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        m_pTrcAdminObjNoisyMethods =
            CTrcServer::GetTraceAdminObj(NameSpace(), ClassName() + "::NoisyMethods", objectName());
        m_pTrcAdminObjVeryNoisyMethods =
            CTrcServer::GetTraceAdminObj(NameSpace(), ClassName() + "::VeryNoisyMethods", objectName());
    }

    CMyClass3::~CMyClass3()
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
        m_pTrcAdminObjNoisyMethods = nullptr;

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjVeryNoisyMethods);
        m_pTrcAdminObjVeryNoisyMethods = nullptr;
    }

**Instance Tracer with logically Grouped Objects**

If you have a class for which many instances will be created it may be reasonable to logically
group the objects by their names.

You may have a class CParameter to adjust settings of different hardware components.
One instance of this CParameter class is used to control a specific setting of the hardware
component. Lets assume you have to adjust the power level so you create a PowerLevel instance
of class CParameter and of course you name the instance "PowerLevel".

It is easy to imagine that there are multiple hardware components in the system for which the
power level needs to be adjusted. For example the power level of a sound card or an equalizer.
To distinguish the parameters from each other you name one parameter "SoundCard::PowerLevel"
and the other one "Equalizer::PowerLevel". If you now also want to set the trace outputs
separately for each of these parameters, the object name with this path name must be passed
to the GetInstance method. The result is two different trace admin objects.

Header File:

    class CMyClass2
    {
    public: // ctor and dtor
        CMyClass2();
        ~CMyClass2();
        ...
    private: // instance members
        ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;
    };

Source File:

    CMyClass2::CMyClass2(const QString& i_strObjName) :
        QObject(i_strObjName)
    {
        setObjectName(i_strObjName);

        m_pTrcAdminObj =
            CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

Usage:

    CMyClass2 soundCardPowerLevel("SoundCard::PowerLevel");
    CMyClass2 equalizerPowerLevel("Equalizer::PowerLevel");

**Trace Admin Objects for Child Objects**

There may be classes whose trace output should be checked for each instance.
However, since there will be many of these instances distributed throughout the system,
it will be difficult to find unique object names for these instances.

A good use case for this would be e.g. Mutex and WaitConditions.

The prerequisite is, of course, that the classes support method tracing. Within the ZSQtLib,
in the name space 'ZS::System' wrappers were implemented around the classes QMutex, QMutexLocker
and QWaitCondition. This allows one to locate race conditions or deadlocks associated with
these classes by inspecting the trace output.

As a recommendation extend the object name of the mutexes and wait condition with the
name space, class name and object of the class instance creating the mutex or wait condition
and add their usages.

Example:

    m_pMtxWaitClass2ThreadRunning = new CMutex(ClassName() + "::" + i_strObjName + "::WaitClass2ThreadRunning");
    m_pWaitClass2ThreadRunning = new CWaitCondition(ClassName() + "::" + i_strObjName + "::Class2ThreadRunning");

Header File:

    class CMyClass2
    {
    public: // ctor and dtor
        CMyClass2();
        ~CMyClass2();
        ...
    private: // instance members
        ZS::System::CMutex*         m_pMtxCounters;
        ZS::System::CMutex*         m_pMtxWaitClass3ThreadRunning;
        ZS::System::CWaitCondition* m_pWaitClass3ThreadRunning;
        ZS::Trace::CTrcAdminObj*    m_pTrcAdminObj;
    };

Source File:

    CMyClass2::CMyClass2(const QString& i_strObjName) :
        QObject(i_strObjName)
    {
        setObjectName(i_strObjName);

        m_pTrcAdminObj =
            CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

        m_pMtxCounters = new CMutex(
            QMutex::Recursive, ClassName() + "::" + i_strObjName + "::Counters");
        m_pMtxWaitClass3ThreadRunning = new CMutex(
            ClassName() + "::" + i_strObjName + "::WaitClass3ThreadRunning");
        m_pWaitClass3ThreadRunning = new CWaitCondition(
            ClassName() + "::" + i_strObjName + "::Class3ThreadRunning");
    }

**Renaming Objects**

It often happens that instances are created with a default name such as "Undefined" or "Object-N".
This name should then later be adjusted by the user.

If instance tracers are used for the classes, you naturally also want this name change of the objects
to be reflected in the trace output.

In order for this to be accomplished, the new object name must also be passed to the trace admin object instances.

If trace admin objects are renamed, the reference counter must be taken into account and either the same trace
admin object, a newly created trace admin object or the reference to another, already existing trace admin object
will be returned.

- If the trace admin object is only referenced once or not at all the trace admin object may be simply renamed.
  There is no need to create a new trace admin object.
- If the trace admin object is referenced more than once (GetTraceAdminObj has been called with the same
  name space, class name and object name several times) a new trace admin object has to be created and returned
  to the code in which the renaming took place. All other modules must still refer to the unchanged trace admin
  object - only the reference counter got to be decremented by one.
- If already a trace admin object with the name space, class name and new object name is existing, a reference
  to this already existing trace admin object got to be returned to the code in which the renaming took place.
  The reference counter of the previously referenced trace admin object got to be decremented, the reference
  counter of the newly referenced trace admin object got to be incremented.

The following code snippet shows renaming of an object together with child objects and the trace admin object:

    void CMyClass2::setObjectName(const QString& i_strObjName)
    {
        // Please note that the method will not be traced if called in the ctor.
        // The method is called before the trace admin object is created.
        // But if the method is called to rename an already existing object the
        // method will be traced as the trace admin object is then existing.
        QString strMthInArgs;
        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            strMthInArgs = i_strObjName;
        }
        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "setObjectName",
            /* strAddInfo   */ strMthInArgs );

        QObject::setObjectName(i_strObjName);

        if( m_pTrcAdminObj != nullptr )
        {
            m_pTrcAdminObj = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObj, objectName());
        }

        if( m_pMtxCounters != nullptr )
        {
            m_pMtxCounters->setObjectName(ClassName() + "::" + objectName() + "::Counters");
        }
        if( m_pMtxWaitClass3ThreadRunning != nullptr )
        {
            m_pMtxWaitClass3ThreadRunning->setObjectName(ClassName() + "::" + objectName() + "::WaitClass3ThreadRunning");
        }
        if( m_pWaitClass3ThreadRunning != nullptr )
        {
            m_pWaitClass3ThreadRunning->setObjectName(ClassName() + "::" + objectName() + "::Class3ThreadRunning");
        }
    }


**Summary**

If you have created all trace admin objects as described in the examples above, you will get a tree with
the following structure.

**Mutexes and Wait Conditions:**

![Mutexes and Wait Conditions](ZSIpcTrace/TraceAdminObjectsTree-MutexAndWaitConditions.png)

**Classes to be Investigated**

![Classes to be investigated](ZSIpcTrace/TraceAdminObjectsTree-ClassesToBeInvestigated.png)
