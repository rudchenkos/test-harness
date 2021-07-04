#include <stdbool.h>
#include <string.h>

bool is_palindrome(const char* str) {
	size_t len = strlen(str);
	if (len == 0) {
		return false;
	}
	for (size_t i = 0; i < len / 2; ++i) {
		if (str[i] != str[len - i - 1]) {
			return false;
		}
	}
    return true;
}

#ifdef TEST
#include <test-harness.h>

void test_main() {
    TEST_IT("Empty string is a palindrome", {
        expect("result is false", !is_palindrome(""));
    });

    TEST_IT("One letter is a palindrome", {
        expect("result is true", is_palindrome("a"));
    });

    TEST_IT("'ab' is not a palindrome", {
        expect("result is false", !is_palindrome("ab"));
    });

    TEST_IT("'aba' is a palindrome", {
        expect("result is true", is_palindrome("aba"));
    });

    TEST_IT("'abba' is a palindrome", {
        expect("result is true", is_palindrome("abba"));
    });
}

TEST_HARNESS_MAIN
#endif
