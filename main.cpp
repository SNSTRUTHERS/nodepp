#include <nodepp/nodepp.h>
#include <nodepp/json.h>
#include <nodepp/fs.h>
#include <nodepp/encoder.h>

using namespace nodepp;

void onMain() {

/*
    string_t val = "hello world";
    
    auto tmp1 = val.slice_view( 0, 6 );
    auto tmp2 = val.slice( 0, 6 );

    console::log( val, tmp1, tmp2 );
    console::log( encoder::base64::atob( val ) );
    console::log( encoder::base64::atob( tmp1 ) );
    console::log( encoder::base64::atob( tmp2 ) );
*/

    console::log( json::stringify( array_t<object_t>({
        object_t({ { "msg", encoder::base64::atob( fs::read_file( "LICENSE" ) ) }}),
        object_t({ { "msg", encoder::base64::atob( fs::read_file( "LICENSE" ) ) }}),
        object_t({ { "msg", encoder::base64::atob( fs::read_file( "LICENSE" ) ) }})
    }) ));

}

// g++ -o main main.cpp -I./include ; ./main ?mode=client