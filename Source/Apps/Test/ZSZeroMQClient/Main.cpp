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
//#include <stdio.h>
//#ifdef _WIN32
//#include <io.h>
//#include <process.h>
//#else
//#include <unistd.h>
//#endif
//#include <string.h>
//#include <assert.h>


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    printf("Connecting to hello world server\n");

    void* context = zmq_ctx_new();
    void* requester = zmq_socket(context, ZMQ_REQ);

    printf("zmq_connect(requester, tcp://localhost:5559\n");
    int rc = zmq_connect(requester, "tcp://localhost:5559");

    if( rc < 0 )
    {
        printf("%s", zmq_strerror(zmq_errno()));
    }
    else
    {
        zmq_msg_t msg;

        rc = zmq_msg_init(&msg);

        int request_nbr;

        for( request_nbr = 0; request_nbr != 10; request_nbr++ )
        {
            char buffer [10];
            memset(buffer, 0x00, sizeof(buffer));
            printf("zmq_send(requester, Hello, 5, 0)\n");
            zmq_send(requester, "Hello", 5, 0);
            printf("zmq_recv(requester, buffer, 10, 0)\n");
            zmq_recv(requester, buffer, 10, 0);
            printf("Received: %s\n", buffer);
        }
    }

    zmq_close(requester);
    zmq_ctx_destroy(context);

    return 0;

} // main
