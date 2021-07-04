# Simple unit test harness in C

## Quick start

```
$ make examples
$ ./examples/palindrome.test
```

Edit `examples/palindrome.c` and observe test results.

## How to use

In your C module, add a section behind `#ifdef TEST` which defines function
`test_main` and instantiates test harness runner. Now, this source file,
regardless of its original role, can be compiled as an executable which runs
own tests.

In watch mode, the runner will attemt to rebuild the test by executing `make <SELF>`.
