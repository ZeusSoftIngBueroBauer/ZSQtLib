@page _PAGE_BasicConcepts_MemLeakDetection Detecting memory leaks

To detect memory leaks the header file "ZSSysMemLeakDump.h" must be included.
The include statement for "ZSSysMemLeakDump" should be after all necessary
standard libraries and Qt header files have been included as there a lot of
macros and preprocessor symbols within the CRT and Qt header files defining
and redefining memory allocation functions leading to compile errors.

The following are example code lines on how to use this module together with the
CRT heap reporting functions:

    #include "ZSAppExample\Include\ZSExampleApp.h"

    #include "ZSSys\ZSSysMemLeakDump.h"

    int main( int argc, char *argv[] )
    {
        CApplication* pApp = new CApplication(argc, argv);

        int iAppResult = pAppDbExample->exec();

        delete pApp;
        pApp = nullptr;

        #ifdef _WINDOWS
        #ifdef _DEBUG
        _CrtDoForAllClientObjects(ZS::dumpClientHook, nullptr);
        #endif
        #endif

        return iAppResult;
    }

How to exclude memory blocks from being dumped:
-----------------------------------------------

Sometimes memory blocks allocated with the new operator should not be freed during
runtime of the system (e.g. internal heap objects or static class variables). But
those memory blocks would also be reported as detected memory leaks. The example
code lines below exclude the memory block from the memory leak dump:

    #ifdef _MEMLEAK_DUMP
    #pragma push_macro("_SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE")
    #pragma warning( disable : 4005 )
    #define _SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE 0
    #endif

    pi = new int[10];

    #pragma warning( default : 4005 )
    #pragma pop_macro("_SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE")
