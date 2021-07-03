#ifdef TEST
#include <test-harness.h>
#include <assert.h>
int main(int argc, char* argv[]) {
    assert(1 == 1);
    test_harness_start_watch(argv[0], __FILE__);
    return 0;
}
#endif
