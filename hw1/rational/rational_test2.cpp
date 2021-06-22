#include "rational.h"
#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;


int main() {

    // IMPORTANT: add `friend int main();` in your rational.h
    // before running this (and remove it before submitting)

    // Testing initialization
    Rational zero;
    Rational half(2, 4);
    Rational third(30, 90);
    Rational quarter(12, 48);

    assert(zero.n == 0);    assert(zero.d == 1);
    assert(half.n == 1);    assert(half.d == 2);
    assert(third.n == 1);   assert(third.d == 3);
    assert(quarter.n == 1); assert(quarter.d == 4);

    // Testing extraction (>>)
    stringstream inputs("1/2 2/4  -4   /  -8");
    Rational i1, i2, i3;
    inputs >> i1 >> i2 >> i3;

    for (Rational i : {i1, i2, i3}) {
        assert(i.n == 1);
        assert(i.d == 2);
    }

    // Testing insertion (<<)
    for (Rational i : {half, third, quarter, i1, i2, i3}) {
        stringstream test_out; test_out << i;
        string exp = to_string(i.n) + "/" + to_string(i.d);
        assert(test_out.str() == exp);
    }

    // Testing addition
    Rational d1 = half + 1;
    Rational d2 = quarter + third;
    Rational d3 = 4 + third;
    assert(d1.n == 3); assert(d1.d == 2);
    assert(d2.n == 7); assert(d2.d == 12);
    assert(d3.n == 13); assert(d3.d == 3);

    // Testing multiplication
    Rational s1 = half * 1;
    Rational s2 = zero * third;
    Rational s3 = 4 * third;
    assert(s1.n == 1); assert(s1.d == 2);
    assert(s2.n == 0); assert(s2.d == 1);
    assert(s3.n == 4); assert(s3.d == 3);

    // Testing exponentiation
    Rational e1 = half ^ 2;
    Rational e2 = half ^ 10;
    Rational e3 = half ^ 0;
    assert(e1.n == 1); assert(e1.d == 4);
    assert(e2.n == 1); assert(e2.d == 1024);
    assert(e3.n == 1); assert(e3.d == 1);

    // Testing equality and inequality
    Rational q1(5, 2), q2(10, 4), q3(-10, -4);
    assert(q1 == q2);
    assert(q1 == q3);
    assert(half != q1);
    assert(quarter != q2);

    // Testing less-than
    Rational l1(1, 2), l2(49, 100), l3(4999, 10000);
    assert(l2 < l1);
    assert(l2 < l3);

    // Testing assignment
    Rational a1, a2(4, 5), a3(1, 5);
    a1 *= 2; a1 += 1;
    a2 += a3; a2 *= 2;
    a3 *= 3; a3 += 2;
    assert(a1 == Rational(1, 1));
    assert(a2 == Rational(2, 1));
    assert(a3 == Rational(13, 5));

    cout << "All tests passed successfully!" << endl;

}