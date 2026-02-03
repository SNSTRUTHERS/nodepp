/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_POSIX_SLEEP
#define NODEPP_POSIX_SLEEP

/*────────────────────────────────────────────────────────────────────────────*/

#ifdef uint
#   undef uint
#endif
#ifdef ulong
#   undef ulong
#endif
#include <unistd.h>
#include <sys/time.h>
#include "../macros.h"
#ifndef uint
#   define uint unsigned int
#endif
#ifndef ulong
#   define ulong unsigned long
#endif

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp::process { using NODE_INTERVAL = struct timeval; }
namespace nodepp::process {

    inline NODE_INTERVAL& get_time_interval(){
        thread_local static NODE_INTERVAL interval;
        gettimeofday( &interval, NULL );
        return interval;
    }

    inline ulong micros(){ NODE_INTERVAL time = get_time_interval();
        return (ulong)(time.tv_sec * 1000000 + time.tv_usec);
    }

    inline ulong seconds(){ NODE_INTERVAL time = get_time_interval();
        return (ulong)(time.tv_sec + time.tv_usec / 1000000);
    }

    inline ulong millis(){ NODE_INTERVAL time = get_time_interval();
        return (ulong)(time.tv_sec * 1000 + time.tv_usec / 1000);
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp::process {

    inline ulong& get_timeout( bool reset=false ) {
    thread_local static ulong stamp=0;
        if( reset ){ stamp = 60000; }
    return stamp; }

    inline void clear_timeout() { get_timeout(true); }

    inline ulong set_timeout( int time=0 ) {
        if( time < 0 ){ /*--------------*/ return 1; }
        auto stamp=&get_timeout(); ulong out=*stamp;
        if( *stamp>(ulong)time ){ *stamp=(ulong)time; }
    return out==0 ? 1 : out; }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp::process {

    inline void delay( ulong time ){ ::usleep((uint)time*1000); }

    inline void yield(){ delay(TIMEOUT); }

    inline ulong now(){ return millis(); }

}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/