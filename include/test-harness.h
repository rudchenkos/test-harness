#ifndef __TEST_HARNESS_H__
#define __TEST_HARNESS_H__

#include <stdbool.h>
#include <sys/errno.h>

/** Fail the current test case */
void test_harness_fail(const char* fmt, ...);

/** Check if value is true, fail the test otherwise */
void expect(const char* tag, bool value);

/** Check integer equality, report failure otherwise */
void expect_int(const char* tag, int actual, int expected);

/** Check string equality, report failure otherwise */
void expect_string(const char* tag, const char* actual, const char* expected);

/** Mark beginning of a test. If returns false, then tests should NOT be executed */
bool test_harness_begin_test(const char* name);
/** Mark the end of a test. Should only be called if a test has begun */
void test_harness_end_test();

/** Push test context description onto the stack */
void test_harness_push_context(const char* description);
/** Pop test context description from the stack */
void test_harness_pop_context();

/** Execute the test harness runner */
int test_harness_main(int argc, char** argv, const char* source_file);

#define TEST_HARNESS_MAIN int main(int argc, char** argv) \
    { return test_harness_main(argc, argv, __FILE__); }

#define TEST_CONTEXT(description, block) \
    do { \
        test_harness_push_context(description); \
        block; \
        test_harness_pop_context(); \
    } while(false);

#define TEST_IT(title, block) \
    do { \
        if (test_harness_begin_test(title)) { \
            block; \
            test_harness_end_test(); \
        } \
        errno = 0; \
    } while (false);

#endif
