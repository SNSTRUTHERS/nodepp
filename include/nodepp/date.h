/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_DATE
#define NODEPP_DATE

/*────────────────────────────────────────────────────────────────────────────*/

#include <cstring>
#include <ctime>
#include "macros.h"
#include "ptr.h"
#include "string.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class date_t {
protected:

    struct NODE {
        bool utc;
        uint day;
        uint year;
        uint hour;
        uint month;
        uint minute;
        uint second;
    };  ptr_t<NODE> obj;

    using TIME = struct tm;

    /*─······································································─*/

    void set_time( time_t time, bool utc ) const noexcept {
        TIME* info  = !utc ? localtime( &time ) : gmtime( &time );

        set_year  ( (uint)info->tm_year+1900 );
        set_month ( (uint)info->tm_mon +1    );
        set_second( (uint)info->tm_sec       );
        set_minute( (uint)info->tm_min       );
        set_day   ( (uint)info->tm_mday      );
        set_hour  ( (uint)info->tm_hour      );
        set_utc   ( utc );

    }

    time_t get_time() const noexcept {
        TIME info; memset( &info, 0, sizeof(TIME) );

        info.tm_sec  = (int)obj->second;
        info.tm_min  = (int)obj->minute;
        info.tm_mon  = (int)obj->month;
        info.tm_year = (int)obj->year;
        info.tm_hour = (int)obj->hour;
        info.tm_mday = (int)obj->day;

        return mktime( &info );
    }

public:

    template< class... V >
    date_t( const V&... args ) noexcept : obj( 0UL, NODE() ) { set_date( args... ); }

    date_t() noexcept : obj( 0UL, NODE() ) { set_date( false ); }

    /*─······································································─*/

    bool operator==( const date_t& other ) const noexcept { return get_stamp()==other.get_stamp(); }
    bool operator<=( const date_t& other ) const noexcept { return get_stamp()<=other.get_stamp(); }
    bool operator>=( const date_t& other ) const noexcept { return get_stamp()>=other.get_stamp(); }
    bool operator< ( const date_t& other ) const noexcept { return get_stamp()< other.get_stamp(); }
    bool operator> ( const date_t& other ) const noexcept { return get_stamp()> other.get_stamp(); }

    /*─······································································─*/

    void operator+=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() + other.get_stamp(), obj->utc );
    }

    void operator-=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() - other.get_stamp(), obj->utc );
    }

    void operator*=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() * other.get_stamp(), obj->utc );
    }

    void operator/=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() / other.get_stamp(), obj->utc );
    }

    /*─······································································─*/

    void set_stamp( const time_t& time, const bool& utc ) const noexcept {
         set_time ( time, utc );
    }

    /*─······································································─*/

    void set_date( const bool& utc ) const noexcept {
         set_utc(utc); set_time( ::time(nullptr), utc );
    }

    void set_date( const uint& year, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year);
    }

    void set_date( const uint& year, const uint& month, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const uint& hour, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day); set_hour(hour);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const uint& hour, const uint& min, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day); set_hour(hour); set_minute(min);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const uint& hour, const uint& min, const uint& second, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day); set_hour(hour); set_minute(min); set_second(second);
    }

    /*─······································································─*/

    void set_year  ( uint year  ) const noexcept { obj->year   = year-1900; }
    void set_month ( uint month ) const noexcept { obj->month  = month-1;   }
    void set_second( uint sec   ) const noexcept { obj->second = sec;       }
    void set_minute( uint min   ) const noexcept { obj->minute = min;       }
    void set_hour  ( uint hour  ) const noexcept { obj->hour   = hour;      }
    void set_day   ( uint day   ) const noexcept { obj->day    = day;       }
    void set_utc   ( bool utc   ) const noexcept { obj->utc    = utc;       }

    /*─······································································─*/

    string_t get_fulltime() const noexcept { time_t time = get_time();
        !obj->utc ? localtime( &time ) : gmtime( &time );
        return (string_t)ctime( &time );
    }

    [[nodiscard]] uint get_year() const noexcept { time_t time = get_time();
        TIME* info = !obj->utc ? localtime( &time ) : gmtime( &time );
        return (uint)info->tm_year+1900;
    }

    [[nodiscard]] uint get_month() const noexcept { time_t time = get_time();
        TIME* info = !obj->utc ? localtime( &time ) : gmtime( &time );
        return (uint)info->tm_mon+1;
    }

    [[nodiscard]] uint get_hour() const noexcept { time_t time = get_time();
        TIME* info = !obj->utc ? localtime( &time ) : gmtime( &time );
        return (uint)info->tm_hour;
    }

    [[nodiscard]] uint get_day() const noexcept { time_t time = get_time();
        TIME* info = !obj->utc ? localtime( &time ) : gmtime( &time );
        return (uint)info->tm_mday;
    }

    [[nodiscard]] uint get_minute() const noexcept { time_t time = get_time();
        TIME* info = !obj->utc ? localtime( &time ) : gmtime( &time );
        return (uint)info->tm_min;
    }

    [[nodiscard]] uint get_second() const noexcept { time_t time = get_time();
        TIME* info = !obj->utc ? localtime( &time ) : gmtime( &time );
        return (uint)info->tm_sec;
    }

    [[nodiscard]] uint get_stamp() const noexcept { return (uint)get_time(); }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

    inline date_t operator+( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() + B.get_stamp(), false );
        return C;
    }

    inline date_t operator-( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() - B.get_stamp(), false );
        return C;
    }

    inline date_t operator*( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() * B.get_stamp(), false );
        return C;
    }

    inline date_t operator/( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() / B.get_stamp(), false );
        return C;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp::date {

    [[nodiscard]] inline uint now(){ return date_t().get_stamp(); }

    [[nodiscard]] inline string_t fulltime(){ return date_t().get_fulltime(); }

    [[nodiscard]] inline uint day( const bool& utc ){ return date_t(utc).get_day(); }

    [[nodiscard]] inline uint year( const bool& utc ){ return date_t(utc).get_year(); }

    [[nodiscard]] inline uint hour( const bool& utc ){ return date_t(utc).get_hour(); }

    [[nodiscard]] inline uint month( const bool& utc ){ return date_t(utc).get_month(); }

    [[nodiscard]] inline uint minute( const bool& utc ){ return date_t(utc).get_minute(); }

    [[nodiscard]] inline uint second( const bool& utc ){ return date_t(utc).get_second(); }

}

/*────────────────────────────────────────────────────────────────────────────*/

#endif