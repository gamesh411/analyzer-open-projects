// RUN: %clang_cc1 -fsyntax-only -analyze \
// RUN:   -analyzer-checker=core,debug.ExprInspection %s -verify

// Handle constructors for lambda captures
// Variables which are captured by value into a lambda require a call to a copy
// constructor.
void clang_analyzer_eval(bool);
void clang_analyzer_warnIfReached();

void reached_function() {
  clang_analyzer_warnIfReached(); // expected-warning{{REACHABLE}}
};

struct incr_on_copy {
  int &i;
  incr_on_copy(int &i) : i(i) {}
  incr_on_copy(const incr_on_copy &o) : i(++o.i) { reached_function(); }
  incr_on_copy &operator=(const incr_on_copy &o) = delete;
  incr_on_copy &operator=(incr_on_copy &&o) = delete;
  ~incr_on_copy() = default;
};

void test_simple_copy() {
  int a = 0;

  incr_on_copy ioc1(a);

  clang_analyzer_eval(ioc1.i == 0); // expected-warning{{TRUE}}
  clang_analyzer_eval(a == 0);      // expected-warning{{TRUE}}

  // Explicit copy constructor call.
  incr_on_copy ioc2(ioc1);

  clang_analyzer_eval(ioc2.i == 1); // expected-warning{{TRUE}}
  clang_analyzer_eval(a == 1);      // expected-warning{{TRUE}}
}

void test_lambda_capture() {
  int a = 0;

  incr_on_copy ioc(a);

  clang_analyzer_eval(ioc.i == 0); // expected-warning{{TRUE}}
  clang_analyzer_eval(a == 0);     // expected-warning{{TRUE}}

  // Implicitly call copy constructor in case of capture-by-value.
  [ioc]() {
    clang_analyzer_warnIfReached(); // expected-warning{{REACHABLE}}
  }();

  clang_analyzer_eval(ioc.i == 1); // expected-warning{{TRUE}}
  clang_analyzer_eval(a == 1);     // expected-warning{{TRUE}}
}
