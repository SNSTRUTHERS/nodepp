/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_POSIX_ATOMIC
#define NODEPP_POSIX_ATOMIC

/*────────────────────────────────────────────────────────────────────────────*/

#include <cstddef>
#include "../type.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {
template< class T >
    requires(type::is_trivially_copyable<T>::value)
class atomic_t   { private: T value; protected:

    void cpy( const atomic_t& other ) noexcept {
         emcpy( &value, &other.value, sizeof( T ) );
    }

    static constexpr size_t scale = type::is_pointer<T>::value
        ? sizeof( typename type::remove_pointer<T>::type )
        : 1;

public:

    atomic_t( atomic_t&& other ) noexcept { cpy(type::move(other)); }

    atomic_t( const atomic_t& other ) noexcept { cpy(other); }

    atomic_t( T _val_ ) noexcept : value( _val_ ) {}

    atomic_t() noexcept : value( T{} ) {}

public:

    T get() const noexcept {
        return __atomic_load_n( &value, __ATOMIC_ACQUIRE );
    }

    void set( T new_val ) noexcept {
        __atomic_store_n( &value, new_val, __ATOMIC_RELEASE );
    }

    /*─······································································─*/

    T _and( T new_val ) noexcept {
        return __atomic_fetch_and( &value, new_val, __ATOMIC_SEQ_CST );
    }

    T _xor( T new_val ) noexcept {
        return __atomic_fetch_xor( &value, new_val, __ATOMIC_SEQ_CST );
    }

    T _or( T new_val ) noexcept {
        return __atomic_fetch_or( &value, new_val, __ATOMIC_SEQ_CST );
    }

    /*─······································································─*/

    T add( T new_val ) noexcept {
        return __atomic_fetch_add( &value, new_val * scale, __ATOMIC_SEQ_CST );
    }

    T sub( T new_val ) noexcept {
        return __atomic_fetch_sub( &value, new_val * scale, __ATOMIC_SEQ_CST );
    }

    /*─······································································─*/

    T swap( T new_val ) noexcept {
        return __atomic_exchange_n( &value, new_val, __ATOMIC_SEQ_CST );
    }

    bool compare( T& expected, T desired ) const noexcept {
        return __atomic_compare_exchange_n( &value, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST );
    }

public:

    template< typename U = T >
    atomic_t& operator&=( T value ) noexcept
    requires(!type::is_pointer<U>::value) { _and(value); return *this; }

    template< typename U = T >
    atomic_t& operator|=( T value ) noexcept
    requires(!type::is_pointer<U>::value) { _or (value); return *this; }

    template< typename U = T >
    atomic_t& operator^=( T value ) noexcept
    requires(!type::is_pointer<U>::value) { _xor(value); return *this; }

    template< typename U = T >
    atomic_t& operator-=( T value ) noexcept
    requires(!type::is_pointer<U>::value) { sub(value); return *this;  }

    template< typename U = T >
    atomic_t& operator+=( T value ) noexcept
    requires(!type::is_pointer<U>::value) { add(value); return *this;  }

    /*─······································································─*/

    template< typename U = T >
    T operator--() /*-------------*/ noexcept
    requires(!type::is_pointer<U>::value) { return sub(1) - 1; }

    template< typename U = T >
    T operator++() /*-------------*/ noexcept
    requires(!type::is_pointer<U>::value) { return add(1) + 1; }

    template< typename U = T >
    T operator--(int) /*----------*/ noexcept
    requires(!type::is_pointer<U>::value) { return sub(1); }

    template< typename U = T >
    T operator++(int) /*----------*/ noexcept
    requires(!type::is_pointer<U>::value) { return add(1); }

    /*─······································································─*/

    atomic_t& operator =( T value ) noexcept { set(value); return *this; }

    /*─······································································─*/

    bool operator==( T value ) const noexcept { return get() == value; }
    bool operator>=( T value ) const noexcept { return get() >= value; }
    bool operator<=( T value ) const noexcept { return get() <= value; }
    bool operator> ( T value ) const noexcept { return get() >  value; }
    bool operator< ( T value ) const noexcept { return get() <  value; }
    bool operator!=( T value ) const noexcept { return get() != value; }

    explicit operator T() /**/ const noexcept { return get(); }

}; }

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/