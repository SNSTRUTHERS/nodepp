/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_SIGNAL
#define NODEPP_SIGNAL

/*────────────────────────────────────────────────────────────────────────────*/

#ifdef uint
#   undef uint
#endif
#ifdef ulong
#   undef ulong
#endif
#include <csignal>
#include <cstdlib>
#ifndef uint
#   define uint unsigned int
#endif
#include "conio.h"
#include "console.h"
#include "event.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp::process {

    inline event_t<int> onSIGERROR;
    inline event_t<> onSIGEXIT;
    inline event_t<> onSIGCLOSE;

    namespace signal {

        inline void start() {
            ::signal( SIGFPE,  []( int ){ onSIGERROR.emit(SIGFPE ); conio::error("SIGFPE: ");  console::log("Floating Point Exception"); onSIGEXIT .emit(); });
            ::signal( SIGSEGV, []( int ){ onSIGERROR.emit(SIGSEGV); conio::error("SIGSEGV: "); console::log("Segmentation Violation");   onSIGEXIT .emit(); });
            ::signal( SIGILL,  []( int ){ onSIGERROR.emit(SIGILL ); conio::error("SIGILL: ");  console::log("Illegal Instruction");      onSIGEXIT .emit(); });
            ::signal( SIGTERM, []( int ){ onSIGERROR.emit(SIGTERM); conio::error("SIGTERM: "); console::log("Process Terminated");       onSIGEXIT .emit(); });
            ::signal( SIGINT,  []( int ){ onSIGERROR.emit(SIGINT ); conio::error("SIGINT: ");  console::log("Signal Interrupt");         onSIGEXIT .emit(); });
        #ifdef SIGPIPE
            ::signal( SIGPIPE, []( int ){ onSIGERROR.emit(SIGPIPE); conio::error("SIGPIPE: "); console::log("Broked Pipeline");          onSIGEXIT .emit(); });
            ::signal( SIGKILL, []( int ){ onSIGERROR.emit(SIGKILL); conio::error("SIGKILL: "); console::log("Process Killed");           onSIGEXIT .emit(); });
        #endif
            ::signal( SIGABRT, []( int ){ onSIGERROR.emit(SIGABRT); conio::error("SIGABRT: "); console::log("Process Abort");            onSIGEXIT .emit(); });
            ::atexit( /*----*/ []( /*-----*/ ){ /*------------------------------------------------------------------------------------------*/ onSIGCLOSE.emit(); });
        #ifdef SIGPIPE
            ::signal( SIGPIPE, SIG_IGN );
        #endif
        }

        inline void unignore( int signal ){ ::signal( signal, SIG_DFL ); }
        inline void   ignore( int signal ){ ::signal( signal, SIG_IGN ); }
	    inline void     emit( int signal ){ ::raise ( signal );          }

    }

}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
