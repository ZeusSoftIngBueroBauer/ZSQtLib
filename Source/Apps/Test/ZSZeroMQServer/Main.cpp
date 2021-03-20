/*******************************************************************************

Copyright 2004 - 2017 by ZeusSoft, Ing. Buero Bauer, Germany
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#include <zmq.h>
#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#include <process.h>
#else
#include <unistd.h>
#endif
#include <string.h>
#include <assert.h>


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    //  Socket to talk to clients
    void* context = zmq_ctx_new();

    void* pvSocket = zmq_socket(context, ZMQ_ROUTER);
    printf("zmq_bind(pvSocket, tcp://*:5559)\n");
    int rc = zmq_bind(pvSocket, "tcp://*:5559");
    assert(rc == 0);

    enum EMsgPart {
        EMsgPartId = 0,
        EMsgPartFrameSep = 1,
        EMsgPartData     = 2,
        EMsgPartCount
    };

    int       ariMsgPartLens[EMsgPartCount];
    zmq_msg_t arMsgPartsRcv[EMsgPartCount];
    zmq_msg_t arMsgPartsSnd[EMsgPartCount];
    int       iMsgPart;
    int       iMsgPartsRcv;
    int       iMoreParts;

    while( true )
    {
        iMsgPart = 0;
        iMsgPartsRcv = 0;

        for( iMsgPart = 0; iMsgPart < EMsgPartCount; iMsgPart++ )
        {
            //  Process all parts of the message
            zmq_msg_init(&arMsgPartsRcv[iMsgPart]);
            ariMsgPartLens[iMsgPart] = zmq_msg_recv(&arMsgPartsRcv[iMsgPart], pvSocket, 0);
            iMsgPartsRcv++;
            iMoreParts = zmq_msg_more(&arMsgPartsRcv[iMsgPart]);
            if( !iMoreParts )
            {
                break;      //  Last message part
            }
        }

        if( iMsgPartsRcv != EMsgPartCount )
        {
            // kack dreck
        }
        else
        {
            for( iMsgPart = 0; iMsgPart < EMsgPartCount; iMsgPart++ )
            {
                if( iMsgPart < (EMsgPartCount-1) )
                {
                    zmq_msg_init(&arMsgPartsSnd[iMsgPart]);
                    zmq_msg_copy(&arMsgPartsSnd[iMsgPart], &arMsgPartsRcv[iMsgPart]);
                    zmq_msg_send(&arMsgPartsSnd[iMsgPart], pvSocket, ZMQ_SNDMORE);
                }
                else
                {
                    zmq_msg_init_size(&arMsgPartsSnd[iMsgPart], 5);
                    memcpy(zmq_msg_data(&arMsgPartsSnd[iMsgPart]), "World", 5);
                    zmq_msg_send(&arMsgPartsSnd[iMsgPart], pvSocket, 0);
                }
                zmq_msg_close(&arMsgPartsSnd[iMsgPart]);
            }
        }

        for( iMsgPart = 0; iMsgPart < iMsgPartsRcv; iMsgPart++ )
        {
            zmq_msg_close(&arMsgPartsRcv[iMsgPart]);
        }

        #ifdef _WIN32
        Sleep(1000);       //  Do some 'work'
        #else
        sleep(1);          //  Do some 'work'
        #endif
    }

    //void* frontend = zmq_socket(context, ZMQ_ROUTER);
    //printf("zmq_bind(frontend, tcp://*:5559)\n");
    //int rc = zmq_bind(frontend, "tcp://*:5559");
    //assert(rc == 0);

    //void* backend = zmq_socket(context, ZMQ_DEALER);
    //printf("zmq_bind(backend, tcp://*:5560)\n");
    //rc = zmq_bind(backend, "tcp://*:5560");
    //assert(rc == 0);

    //zmq_proxy(frontend, backend, NULL);

    //while( true )
    //{
    //    char arcBufId[10];      memset(arcBufId, 0x00, sizeof(arcBufId));
    //    char arcBufFrm[10];     memset(arcBufFrm, 0x00, sizeof(arcBufFrm));
    //    char arcBufMsg[10];     memset(arcBufMsg, 0x00, sizeof(arcBufMsg));

    //    printf("zmq_recv(responder, arcBufId, 10, 0)\n");
    //    int iIdLen = zmq_recv(responder, arcBufId, 10, 0);
    //    printf("Received %s\n", arcBufId);

    //    printf("zmq_recv(responder, arcBufFrm, 10, 0)\n");
    //    int iFrmLen = zmq_recv(responder, arcBufFrm, 10, 0);
    //    printf("Received %s\n", arcBufFrm);

    //    printf("zmq_recv(responder, arcBufMsg, 10, 0)\n");
    //    int iMsgLen = zmq_recv(responder, arcBufMsg, 10, 0);
    //    printf("Received %s\n", arcBufMsg);

    //    #ifdef _WIN32
    //    Sleep(1000);       //  Do some 'work'
    //    #else
    //    sleep(1);          //  Do some 'work'
    //    #endif

    //    //printf("zmq_send(responder, arcBufId, %d, 0)\n", iIdLen);
    //    //zmq_send(responder, arcBufId, iIdLen, 0);

    //    printf("zmq_send(responder, arcBufFrm, %d, 0)\n", iFrmLen);
    //    zmq_send(responder, arcBufFrm, iFrmLen, 0);

    //    printf("zmq_send(responder, World, 5, 0)"\n);
    //    zmq_send(responder, "World", 5, 0);
    //}

    //  We never get here…
    zmq_close(pvSocket);
    //zmq_close(frontend);
    //zmq_close(backend);

    zmq_ctx_destroy(context);

    return 0;

} // main
