// No copyright. Vladislav Aleinik, 2024
//=======================================
// Minimal test system
//=======================================
#ifndef HEADER_GUARD_TEST_SYSTEM_HPP_INCLUDED
#define HEADER_GUARD_TEST_SYSTEM_HPP_INCLUDED

namespace TestSystem
{
    typedef bool (*TestScenario)();

    enum TestResult
    {
        FAIL      = 0,
        OK        = 1,
        EXCEPTION = 2,
        ERROR     = 3,
        TIMEOUT   = 4
    };

    TestResult run_test(const char* name, TestScenario test, bool inspect = false);
};

#endif // HEADER_GUARD_TEST_SYSTEM_HPP_INCLUDED
