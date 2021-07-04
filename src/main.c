#include <test-harness.h>
#include "internals.h"

extern const char* test_source_file;
extern void test_main(); // To be defined in a client unit

int test_harness_main(int argc, char** argv, const char* source_file) {
    test_harness_report_begin(source_file);

    test_main();

    int rc = test_harness_report_end() > 0 ? 1 : 0;
    test_harness_start_watch(argv[0], source_file);
    return rc;
}
