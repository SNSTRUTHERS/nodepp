/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_EXCEPT
#define NODEPP_EXCEPT

#include "macros.h"
#include "console.h"
#include "iterator.h"
#include "ptr.h"
#include "signal.h"
#include "string.h"
#include "task.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class except_t {
protected:

    struct NODE {
        ptr_t<task_t> ev; string_t msg;
    };  ptr_t<NODE> obj;

public:

   ~except_t() noexcept {
        if( obj->ev == nullptr ){ return; }
   	    process::onSIGERROR.off( obj->ev );
    }

    except_t() noexcept : obj( new NODE() ) {}

    /*─······································································─*/

    template< class T >
    except_t( const T& except_type )
    noexcept requires(type::is_class<T>::value) : obj( new NODE() ) {
        obj->msg = except_type.what(); auto inp = type::bind( this );
        obj->ev  = process::onSIGERROR.once([=](int){ inp->print(); });
    }

    /*─······································································─*/

    template< class... T >
    except_t( const T&... msg ) noexcept : obj( new NODE() ) {
        obj->msg = string::join( " ", msg... );
        auto inp = type::bind( this );
        obj->ev  = process::onSIGERROR.once([=](int){ inp->print(); });
    }

    /*─······································································─*/

    except_t( const string_t& msg ) noexcept : obj( new NODE() ) {
        obj->msg = msg; auto inp = type::bind( this );
        obj->ev  = process::onSIGERROR.once([=](int){ inp->print(); });
    }

    /*─······································································─*/

    void       print() const noexcept { console::error(obj->msg); }
    bool       empty() const noexcept { return obj->msg.empty(); }
    const char* what() const noexcept { return obj->msg.c_str(); }
    operator char const*() const noexcept { return what(); }
    string_t    data() const noexcept { return obj->msg; }
    string_t   value() const noexcept { return obj->msg; }

};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
