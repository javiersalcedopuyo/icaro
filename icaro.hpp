#pragma once

#if __cplusplus < 202002L
    #error Unsupported C++ version. C++20 or newer is required.
#endif

#include <cstdio>
#include <string>
#include <vector> // TODO: Replace with my own
#include <format>

#define TEST( name, test ) Icaro::tests.emplace_back( #name, []()-> bool { test }  );

// NOTE:
//  For specific types like optionals or result/expected unions, you'll need to define your own
//  template specialisations for ImplVerify
#define VERIFY( condition )     if( !Icaro::ImplVerify( (condition), #condition ) ) return false;
#define VERIFY_EQ( a, b )       if( !Icaro::ImplVerifyEq( (a), (b), #a, #b ) )      return false;
#define VERIFY_NOT_EQ( a, b )   if( !Icaro::ImplVerifyNotEq( (a), (b), #a, #b ) )   return false;


namespace Icaro
{
    using String = std::string;

    template< typename T >
    using List = std::vector<T>;

    using std::format;
    static void println( const String& str ) { printf( "%s\n", str.c_str() ); }

    struct Test
    {
        Test() = delete;
        Test( const char* _name, bool (*lambda)() ): name( _name ), test_fun( lambda ) {}

        // TODO: Print how long it took
        auto operator()() const -> bool
        {
            println( format( "🧪 {}:", name ) );
            if( test_fun == nullptr )
            {
                println( "❌ FAILED: No test with this name!" );
                println( "---" );
                return false;
            }
            const auto passed = test_fun();
            if( passed )
            {
                println( "✅ PASSED" );
            }
            return passed;
        }

        String name;
        bool (*test_fun)();
    };


    // TODO: Group these into test suites
    static List< Test > tests{};

    struct RunArgs
    {
        String filter       = "";
        void (*setup)()     = nullptr;
        void (*teardown)()  = nullptr;
    };
    static void run( const RunArgs& args )
    {
        auto passed = 0u;
        auto failed = List<String>{};
        failed.reserve( tests.size() );

        for( const auto& test: tests )
        {
            if( args.filter.empty()
                or test.name.find( args.filter ) != String::npos )
            {
                if( args.setup ) args.setup();

                if( test() )
                    ++passed;
                else
                    failed.emplace_back( test.name );

                if( args.teardown ) args.teardown();

                println( "---" );
            }
        }

        // TODO: Print how long it took
        println( "🏁 TEST RESULTS:" );
        if( not args.filter.empty() )
        {
            println( format( "\tℹ️ Tests ran with filter: '{}'.", args.filter ) );
        }
        println( format( "\t📋 {}/{} tests run.", passed + failed.size(), tests.size() ) );
        println( format( "\t✅ {} tests passed.", passed ) );
        println( format( "\t❌ {} tests failed.", failed.size() ) );

        for( const auto& name: failed )
            println( format( "\t\t- {}", name ) );
    }


    template< typename T >
    constexpr auto ImplVerify( const T& condition, const String& text ) -> bool
    {
        if( condition == false )
        {
            println( format( "❌ FAILED: Expected '{}' to be true.", text ) );
            return false;
        }
        return true;
    }


    template<typename A, typename B>
    constexpr auto ImplVerifyEq( const A& a, const B& b, const String& a_text, const String& b_text ) -> bool
    {
        if( a == b ) return true;

        println( format( "❌ FAILED: Expected '{} == {}'", a_text, b_text ) );
        println( format( "           Found: {}, and {}", a, b ) );
        return false;
    }


    template<typename A, typename B>
    constexpr auto ImplVerifyNotEq( const A& a, const B& b, const String& a_text, const String& b_text ) -> bool
    {
        if( a != b ) return true;

        println( format( "❌ FAILED: Expected '{} != {}'", a_text, b_text ) );
        println( format( "           Found: {}, and {}", a, b ) );
        return false;
    }
}
