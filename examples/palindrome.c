#include <stdbool.h>

bool is_palindrome(const char* str) {
    return false;
}

#ifdef TEST
#include <test-harness.h>

void test_main() {
    TEST_IT("Empty string is a palindrome", {
        expect("palindrome", is_palindrome(""));
    });
}

TEST_HARNESS_MAIN
#endif
