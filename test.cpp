#include "icaro.hpp"
#include <cstdio>

static void setup()    { printf( "🏗️ SETUP\n"    ); }
static void teardown() { printf( "⚒️ TEARDOWN\n" ); }

auto main() -> int
{
    TEST( verify_true,
    {
        VERIFY( true );
        return true;
    });

    TEST( verify_eq,
    {
        auto var_1 = 43;
        auto var_2 = 42;
        VERIFY_EQ( var_1, var_2 );
        return true;
    });

    TEST( verify_not_eq,
    {
        auto var_1 = 67;
        auto var_2 = 69;
        VERIFY_NOT_EQ( var_1, var_2 );
        return true;
    });

    printf( "# TESTING VERIFY\n" );
    Icaro::run({});
    printf( "\n---\n\n" );

    printf( "# TESTING NAME FILTER 'eq'\n" );
    Icaro::run({ .filter = "eq" });
    printf( "\n---\n\n" );

    printf( "# TESTING SETUP AND TEARDOWN\n" );
    Icaro::run({ .setup = setup, .teardown = teardown });
    printf( "\n---\n\n" );
}
