# Icaro: Dedalo's Tester

![Icaro's logo](logo.png)

Minimal testing framework for C++ >= 20, designed as a companion for the build system [Dedalo](https://github.com/javiersalcedopuyo/dedalo).

---

## Usage
Tests are added with the macro `TEST( name, test );`  
`test` being the body of a non-capturing lambda that returns a boolean indicating success or failure.  
To perform the tests 3 macros are provided:
- `VERIFY` for boolean expressions
- `VERIFY_EQ` to compare equality
- `VERIFY_NOT_EQ` to compare non equality

Each of them uses a templated implementation `ImplVerify[Eq|NotEq]`. If you have custom types that require specific behaviour (like different error logging, or more complex validation) you'll have to provide your own specialisation of said templates.

Tests are added to a static list in `Icaro::tests`, and can be run with `Icaro::run()`.  
The `run` function takes 3 optional arguments:
 - `test_name_filter`: Only the tests with names containing this string will run
 - `setup`: Function pointer to an optional setup function to be run before *each* test

 - `takedown`: Function pointer to an optional teardown function to be run after *each* test

### Example
```cpp
#include "icaro.hpp"

// ...

// Add new tests like this:
TEST( test_name,
{
    // Your test logic goes here
    VERIFY( boolean_condition );
    VERIFY_EQ( var_1, var_2 );
    VERIFY_NOT_EQ( var_1, var_2 );

    return true; // This is mandatory
});

// ...

Icaro::run({
    .test_name_filter = "",         // Only tests which contain this will run
    .setup            = nullptr,    // Function pointer to an optional setup function to be run before *each* test
    .teardown         = nullptr,    // Function pointer to an optional teardown function to be run after *each* test
});

```

## Example output
```bash
# Output from the examples in test.cpp intentionally breaking `verify_eq` and applying the name filter `eq`
🧪 verify_eq:
❌ FAILED: Expected 'var_1 == var_2'
           Found: 43, and 42
---
🧪 verify_not_eq:
✅ PASSED
---
TEST RESULTS:
        📋 2/3 tests run.
        ✅ 1 tests passed.
        ❌ 1 tests failed:
                - verify_eq
```

## Features
- [x] Single header, no 3rd party dependencies.
- [x] Detailed error logging displaying the expressions as they are in the code, and their values.
- [x] Final report indicating:
    - [x] Number of total tests run out of the total number of existing ones.
    - [x] Number of succesful tests.
    - [x] Number of failed tests and a list of their names.
- [x] Name-based filtering of the tests to be run.
- [ ] Organisation in test suites.
- [ ] Integration with CI/CD pipelines.