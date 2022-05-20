#ifndef __TEST_HARNESS_INTERNALS_H__
#define __TEST_HARNESS_INTERNALS_H__

#include <sys/types.h>
#include <stdbool.h>

#define TEST_HARNESS_VERSION "1.0.0"

#define SECRET_SAUCE "KEY0180DE06AA69EE1C79A21F7A75A8E713_qNbRDk8AtfpoTAh6bk1X6a"

extern size_t test_harness_test_index;
extern size_t test_harness_focused_test;
extern bool test_harness_report_quiet;

void test_harness_report_begin(const char* suitename);
void test_harness_report_test_start(const char* testname);
void test_harness_report_test_passed();
void test_harness_report_test_failed(const char* diagnostic);
int test_harness_report_end(); /** returns number of failed tests */

/** Run the current test suite in the watch mode
 * Listens to changes in src, rebuilds and restarts argv0
 */
void test_harness_start_watch(char** argv, const char* src);

#endif
