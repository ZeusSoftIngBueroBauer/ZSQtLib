@page _PAGE_Libs_ZSIpcTrace_UseCase_ReadingTraceOutput Reading Method Trace Outputs

**Wait until a thread has been started and the worker class is available**

Within a multi-threaded application, a recurring task is to create a worker instance in a thread.
The creator of the worker instance in the thread then wants to communicate with this worker instance.
For this the creator has to wait until the thread has been started and the worker instance has
been created.

An easy way to synchronize with the "worker instance created" event would be a for loop that is run
until the worker instance is created. In order that the thread of the creator does not take up too
much cpu time, a thread sleep is to be added to this for loop.

The code may look like follows:

@code
void CMyClass1::startClass2Thread()
{
    ...
    CMethoTracer mthTracer(...);
    ...

    CMyClass1* pWorker = nullptr;
    CMyClass2Thread* pThread = new CMyClass2Thread("Inst1");

    pThread->start();

    while( pThread->getWorker() == nullptr )
    {
        sleep(10);
    }
    pWorker = pThread->getWorker();
}
@endcode

**Assuming that a method tracer is used to trace all methods the method trace output
for the code snipped above will look like:**

![MthTrcLog1](ZSIpcTrace/MthTrcLog1.png)

Another option, and at first glance a better one, would be to use a wait condition to
synchronize with the started thread. After starting the thread the creator waits on the
wait condition to be signalled. The thread creating the worker instance will signal the
wait condition after the worker instance has been created.

The thread must know about the wait condition. Now you could pass the wait condition to
the constructor of the thread or - even a better way - after the worker instance has
been created the thread emits a 'running' signal. This way the thread doesn't need to
know anything about the creator.

'The creator connects a slot method to this signal and when the slot is called the
wait condition is signalled. But ensure to used 'DirectConnection' and not 'QueuedConnection'
for the signal/slot connection. Otherwise the slot will never be called.

The code may look like follows:

@code
void CMyClass1::startClass2Thread()
{
    ...
    CMethoTracer mthTracer(...);
    ...

    CMyClass2Thread* pThread = new CMyClass2Thread("Inst1");

    QObject::connect(
        pThread, &CMyClass2Thread::running,
        this, &CMyClass1::onClass2ThreadRunning,
        Qt::DirectConnection);

    pThread->start();

    m_pMtxWaitClass2ThreadRunning->lock();
    m_pWaitClass2ThreadRunning->wait(m_pMtxWaitClass2ThreadRunning);
    m_pMtxWaitClass2ThreadRunning->unlock();

    CMyClass1* pWorker = pThread->getWorker();
}

void CMyClass1::onClass2ThreadRunning()
{
    ...
    CMethoTracer mthTracer(...);
    ...
    m_pWaitClass2ThreadRunning->notify_all();
}
@endcode

**Assuming that a method tracer is used to trace all methods the method trace output
for the code snipped above will look like:**

![MthTrcLog2](ZSIpcTrace/MthTrcLog2.png)

**But using the code above the application may end up in a deadlock. Why?
By reading the method trace output in case of a deadlock it is easy to find the reason:**

As the QTcpServer of the trace method server and also the QTcpSocket connected to the
client is created in a gateway thread the method trace output will be sent to the client
even if the main thread is deadlocked.

![MthTrcLog3](ZSIpcTrace/MthTrcLog3.png)

- The thread is created and start has been called.
- The thread has been (run method entered).
- The worker instance is created.
- The signal 'running' is emitted and the slot 'onClass2ThreadRunning' is called.
- The Wait Condition is signalled (notify_all).
- The threads enteres its event loop (exec).
- The creator locks the Wait Mutex and waits until the thread signals the Wait Condition.

But the thread will not signal the Wait Condition anymore as thats already been done before
while the creator was not waiting at the Wait Condition.

As you can see Remote Method Tracing can really help to find problems in existing code.
