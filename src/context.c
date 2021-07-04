#include "internals.h"
#include <assert.h>

size_t __ncontexts = 0;
const char* __contexts[__MAX_TEST_CONTEXTS];

void test_harness_push_context(const char* description) {
    if (__ncontexts < __MAX_TEST_CONTEXTS - 1) {
        __contexts[__ncontexts] = description;
        ++__ncontexts;
    }
}

void test_harness_pop_context() {
    assert(__ncontexts > 0);
    --__ncontexts;
}

