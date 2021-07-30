#include <test-harness.h>
#include "internals.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern const char* test_source_file;
extern void test_main(); // To be defined in a client unit

enum RunMode {
	RunOnce,
	RunWatch,
};

static void usage() {
	printf("Test harness version %s\n", TEST_HARNESS_VERSION);
	puts("  -fN  focus on test N");
	puts("  -w   run in watch mode");
	puts("  -q   silence all messages except test failures");
}

int test_harness_main(int argc, char** argv, const char* source_file) {
	int ch = 0;
	enum RunMode mode = RunOnce;
	char** argv0 = argv;

	while ((ch = getopt(argc, argv, "f:wq")) != -1) {
		switch (ch) {
            case 'f':
                test_harness_focused_test = atoi(optarg);
                break;
			case 'w':
				mode = RunWatch;
				break;
			case 'q':
				test_harness_report_quiet = true;
				break;
			case '?':
				usage();
				exit(1);
		}
	}
	argc -= optind;
	argv += optind;

    test_harness_report_begin(source_file);

    test_main();

    int rc = test_harness_report_end() > 0 ? 1 : 0;

	if (mode == RunWatch) {
		test_harness_start_watch(argv0, source_file);
	}
    return rc;
}
