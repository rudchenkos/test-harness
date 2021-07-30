# Simple unit test harness in C

## Quick start

Build the example and run its tests:
```
$ make examples
$ ./examples/palindrome.test
```

Now, let's run the tests in the watch mode, when the tests are automatically
recompiled and re-run every time the source file is changed:

```
$ ./examples/palindrome.test -w
```
Edit `examples/palindrome.c` and observe test results.

## Focusing

When debugging a test failure, it helps to focus on a single test case:
```
$ lldb -- ./examples/palindrome.test -f 3
```

## How to use

In your C module, add a section behind `#ifdef TEST` which defines function
`test_main` and instantiates test harness runner. Now, this source file,
regardless of its original role, can be compiled as an executable which runs
own tests.

In watch mode, the runner will attemt to rebuild the test by executing `make <SELF>`.

# FAQ

## Vim highlights lambda-like expressions passed to TEST_IT and TEST_CONTEXT

Add this to your vimrc:
```
let c_no_curly_error=1
```
