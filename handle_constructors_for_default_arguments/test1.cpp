// RUN: %clang_cc1 -fsyntax-only -analyze \
// RUN:   -analyzer-checker=core,debug.ExprInspection %s -verify

void clang_analyzer_eval(bool);
void clang_analyzer_warnIfReached();

struct init_with_list {
    int a;
    init_with_list(): a(1){}
};

struct init_in_body {
    int a;
    init_in_body() { a = 1; }
};

struct init_default_member {
    int a = 1;
};

struct basic_struct{
  int a;
};

void f(init_with_list l = init_with_list()){
  clang_analyzer_eval(l.a==1); // expected-warning {{TRUE}}
}

void g(init_in_body l = init_in_body()){
  clang_analyzer_eval(l.a==1); // expected-warning {{TRUE}}
}

void h(init_default_member l = init_default_member()){
  clang_analyzer_eval(l.a==1); // expected-warning {{TRUE}}
}

void f2(){
  init_with_list l;
  clang_analyzer_eval(l.a==1); // expected-warning {{TRUE}}
}

void f3(init_with_list l){
  clang_analyzer_eval(l.a==1); //expected-warning {{TRUE}}
}

void f4(basic_struct l){
  clang_analyzer_warnIfReached(); //expected-warning {{REACHABLE}}
  clang_analyzer_eval(l.a==1); //expected-warning {{FALSE}}
  clang_analyzer_warnIfReached(); //expected-warning {{REACHABLE}}

}


void test1 (){
// f3(init_with_list());
}

void test2(){
  g();
}

void test3(){
  h();
}

void test4(){
  init_with_list l;
  clang_analyzer_eval(l.a==1); //expected-warning {{TRUE}}
  f3(l);
}

void test5(){
  basic_struct s;
  s.a=1;
  f4(s);
  clang_analyzer_warnIfReached(); //expected-warning {{REACHABLE}}

}
