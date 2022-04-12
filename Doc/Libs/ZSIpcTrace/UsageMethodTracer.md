@page _PAGE_Libs_ZSIpcTrace_Usage_MethodTracer Generating Method Trace Outputs

Now that we know how to create a trace server and how to create trace admin objects,
we can turn to the actual task - generating trace output.

First it is described which methods of the trace server are used to generate the trace
output and how the indentation is done according to the call stack for the active threads.
However, this is only intended as background information. The CMethodTracer class should
be used for the trace outputs, as this ensures that the exit from a method is also logged
if the method is exited at an unexpected point - e.g. if an exception is thrown.

Using Methods of the Trace Server directly
==========================================

The trace outputs are made via the trace server, which provides the following three
methods with different signatures (arguments):

- traceMethodEnter<br/>
  Entry into a method is to be logged with 'traceMethodEnter'.
- traceMethodLeave<br/>
  If a method is left, 'traceMethodLeave' must be called.
- traceMethod<br/>
  If additional trace outputs are to take place within the method,
  this is possible with 'traceMethod'.

Trace output is only created

- if the trace admin objects detail level is not set to None,
- if the trace server outputs are enabled and active.

Trace output may be disabled and enabled for the trace server at all using
the 'setEnabled' method.

Trace output is active if either output to local method trace file is activated,
if a client is connected or - if no client is connected - caching of the trace
output is enabled.

The name of the current thread is inserted at the beginning of the log entry.

For each thread (defined by its name) the call depth is maintained.
Spaces are inserted according to the current call depth of the thread.

The Trace Server determines the current thread in whose context the methods are called
and tries to determine the name of the thread. If no name is assigned to the thread,
a name is generated from the thread id.

The call depth of the method call stack is managed based on the thread name and the method
calls per thread are indented accordingly. On entering a method the call depth is incremented
after the trace string has been output. Further calls for the same thread will use the new
indentation. On leaving a method the call depth is decremented before the trace string is output.

In order for this to work properly, each thread should have a meaningful and unique name.

Thread IDs are assigned dynamically by the operating system, which means that the "same"
thread can have different thread IDs and thus different names when it is created and
destroyed dynamically. This makes it difficult to read the trace output.

Furthermore, the Method Trace Client application allows each thread to be assigned its
own color. This usually only leads to the desired trace output if the threads have been
assigned meaningful and unique names.

For safety, the class name of the thread class can be prepended to the object name of the thread:

@code
class MyThread : public QThread {
public:
    MyThread() : QThread() {
        setObjectName("MyThread-" + m_strMyClass2ObjName);
    }
};
@endcode

After the name of the current thread, the current date time and the time in seconds
since the applications has been started is added to the trace output string.

After the thread and time information and the call stack indentation by
thread the tag '->' is added to indicate that the method is entered.

At next the name space and class name is output encapsulated in the tags '<' and '>'.

After the class information the method name is output.

- For instance tracers the name of the instance for which the method is called
    is added followed by the method name. The object name is separted by a '.'
    from the method name.
- For class tracers only the method name is output.

 At last the method input arguments are added encapsulated in the tags '(' and ')'.

It is up to the caller what is included in the method arguments string.

If only one argument is passed the name of the argument may be omitted.
In some cases adding the argument name makes no real sense. E.g. for a method
'setEnabled' it doesn't really make sense to add "Enabled: " before the
arguments value. If there are several input arguments it is useful to
add the name of the argument.

The following rule for type of arguments may be used:

- For values with units add the unit symbol at the end of the value.
  @code
  'Freq: 56.0 kHz'
  @endcode

- For structures encapsulate the members in '{' and '}'.
  @code
  'Settings: {Freq: 45 Hz, Level: 6 dBm}'
  @endcode

- For lists start with the length of the list encapsulated in '[' and ']'.
  If the list contains elements encapsulate the list in '(' and ')' and
  each element in '{' and '}'. For each element - if desired - start with
  the index (or key) followed by ':' and the value. If the value is a
  structure or a list again follow the rules for structures and lists.
  @code
  'Frequencies: [3]({0: 45 kHz}, {1: 12 Hz}, {2: 20.0 MHz})
  'Settings: [2]({Input: {Freq: 45 kHz, Level: 5 W}}, {Output: {Freq: 12 Hz, Level: 2 W}})
  'Arbitrary: [2]({Elem1: {Val1: uups}}, {Elem2: [2]({0: argh}, {1: aha})})
  @endcode

Typical log outputs will look like:

**Class Tracer**

@code
<GUIMain > 2022-04-04 22:17:30:569 ( 27.720377): -> <ZS::Apps::Test::IpcTrace::CMyClass1> classMethod(Hello Class)
<GUIMain > 2022-04-04 22:17:30:569 ( 27.720377): <- <ZS::Apps::Test::IpcTrace::CMyClass1> classMethod(): Hello World
@endcode

**Instance Tracer**

@code
<GUIMain > 2022-04-04 22:17:30:569 ( 27.720377): -> <ZS::Apps::Test::IpcTrace::CMyClass1> Inst1.instMethod(Hello Class)
<GUIMain > 2022-04-04 22:17:30:569 ( 27.720377): <- <ZS::Apps::Test::IpcTrace::CMyClass1> Inst1.instMethod(): Hello World
@endcode

<br/>

Using the Guard class CMethodTracer
===================================

The guard class CMethodTracer should be used instead of using the trace methods of
the trace server. An instance of the class is created on the stack of the method to
be traced. The constructor traces entering the method. The destructor - which is also
called if the method is unexpectedly exited - traces leaving the method.

To trace entering and leaving a method create an instance at the beginning of the
method as follows:

@code
QString strMthInArgs;

if( s_trcAdminObjRefAnchor.isActive(ETraceDetailLevelMethodArgs) )
{
    strMthInArgs  = "Arg1: " + convertArg1ToString(i_arg1);
    strMthInArgs += ", Arg2: " + convertArg1ToString(i_arg2);
    strMthInArgs += ", Arg3: " + convertArg1ToString(i_arg3);
}

CMethodTracer mthTracer(
    /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
    /* iDetailLevel */ ETraceDetailLevelMethodCalls,
    /* strObjName   */ objectName(),
    /* strMethod    */ "ctor",
    /* strAddInfo   */ strMthInArgs );
@endcode

First it will be checked whether tracing is enabled by querying the detail level
of the trace admin object. Only if tracing is enabled the string containing the
method input arguments is created.

On leaving the method the destructor of 'mthTracer' is called tracing the return
of the method.

If the method has a return value and/or returns values via method output arguments
add the return value an the output arguments at the end of the method call to the
method tracer as follows:

@code
QString strMthRet;
QString strMthOutArgs;

if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
{
    strMthRet = convertResultToString(o_arg);
    strMthOutArgs  = "Arg1: " + convertArg1ToString(o_arg1);
    strMthOutArgs += ", Arg2: " + convertArg1ToString(o_arg2);
    mthTracer.setMethodReturn(strMthRet);
    mthTracer.setMethodOutArgs(strMthOutArgs);
}
@endcode

For convenience the method tracer class provides methods with different signatures
to directly assign the return value for the most common data types like bool,
int, EResult and SErrResultInfo.

There are different constructors with a different set of arguments implement.
Which constructor to use depends on the use case.
Usually you will need only the two constructor with a trace admin object as the
first input argument. When tracing class methods or tracing instance methods using
a trace admin object created as an instance tracer there is no need to pass the
object name of the instance to the constructor.

When using a class trace admin object which has no valid object name assigned and
if you want to output the name of the instace for which the method was applied
the constructor with the object name as argument is provided.
