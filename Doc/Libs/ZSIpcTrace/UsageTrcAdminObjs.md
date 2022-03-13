@page _PAGE_Libs_ZSIpcTrace_Usage_TrcAdminObjs Creating Trace Admin Objects

Trace admin objects control what is traced and how.

A trace admin object can be associated with one or more modules, one or more
classes, or one or more instances of a class.

**Class and Module Tracer**

Usually you want to control trace outputs for a certain class and if tracing
for the class is enabled, how detailed these outputs should be.

To do this, a static class variable must be created and registered with the
Trace Server. The trace admin object can only be registered with the trace server
if the trace server already exists. This means that the trace admin object must
be created on the heap at runtime.

In order to ensure that no memory leaks are left behind and that the trace admin
object is created exactly once, a reference counter for the class is required,
which counts the number of created class objects.

In the constructor the class trace admin object is created if not existing and
the reference counter is incremented. In the destructor the reference counter
is decremented and if the reference counter reaches 0 the trace server is informed
that the trace admin object is no longer needed.

If you also want to trace static class methods a reference counter for the
class instances is not sufficient. Instead you would need a reference counter for
the trace admin object itself. Or you provide static class methods to initialize
and release the class variables and invoke `init` once during startup and `release`
during shutdown.

Using a reference counter for the static trace admin object of the class
implies the following:

- On entering a method check if the reference counter is 0. If it is 0 
  alloc the trace admin object and increment the reference counter.
- On leaving the method decrement the reference counter. If it becomes 0
  release the trace admin object.

The steps above have to be repeated for all class and instance methods.
In addition it must be ensured that the reference counter is decremented
and the trace admin object is released if an exception is thrown while
the method is processed.

To ensure and simplify the tasks above the guard class CTrcAdminObjRefGuard has
been invented which has to be used in combination with CTrcAdminObjRefAnchor class.

In the class methods an instance of the CTrcAdminObjRefGuard has to be 
created on the stack. In instance methods a reference guard instance should
not be instantiated. Instead use the reference anchor instance, allocate
the instance in the constructor and deallocate the instance in the destructor.
If you would use a reference guard instance also to control method tracing of
the instance methods the trace admin object will always be get on entering
the method and released when leaving the method. For instance methods this is
not necessary and getting the admin object in the constructor and releasing
it again in the destructor is sufficient and saves time.

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

Header File:

    class CMyClass1
    {
    public: // class methods
        static QString NameSpace() { return "MyNameSpace"; }
        static QString ClassName() { return "CMyClass1"; }
        ...
    public: // class methods
        ...
        static QString testClassMethodTracing(const QString& i_strMthInArgs);
        ...
    private: // class members
        static ZS::Trace::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchor;
        ...
    };

Source File:

    CTrcAdminObjRefAnchor CTestModule1::s_trcAdminObjRefAnchor(
        CTestModule1::NameSpace(), CTestModule1::ClassName());

    QString CTestModule1::classMethod(const QString& i_strMthInArgs)
    {
        ...
        CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchor);
        ...
    }

    CMyClass1::CMyClass1()
    {
        ...
        s_trcAdminObjRefAnchor.allocTrcAdminObj();
        ...
    }

    CMyClass1::~CMyClass1()
    {
        ...
        s_trcAdminObjRefAnchor.releaseTrcAdminObj();
        ...
    }

**Instance Tracer**

