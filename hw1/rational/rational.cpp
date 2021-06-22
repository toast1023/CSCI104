#include "rational.h"
#include <stdexcept>
#include <cmath>
#include <numeric>
#include <cstdlib>
// Libraries may be included as necessary

using namespace std;


Rational::Rational() : n(0), d(1) {}

Rational::Rational(int n, int d) {
    // Since the denominator cannot be zero an exception is thrown
    if (d == 0) {
        throw std::invalid_argument("division by zero");
    }

    // TODO: continue your implementation below.
    // You should likely set the member variables and then call this->reduce()
    // and this->normalize().
    this->n = n;
    this-> d = d;
    this->reduce();
    this->normalize();
}

void Rational::normalize() {
    // In C++ you don't always have to use this-> to denote a member variable.
    // If you omit it, the compiler will infer that n and d are indeed members
    // of Rational. There are two reasons you will see or use this->:
    //
    // 1. If you have a local variable that has the same name as a member
    //    variable, you need to use this-> to differentiate the two. If you
    //    created `int n` on this next line, the compiler would decide that
    //    any consequent uses of that variable refer to the local n, not our
    //    member variable. To specifically access the member n in this
    //    scenario, we'd use this->n.
    //
    // 2. Style. Some people (like me, Noah, the guy writing this) prefer to
    //    to use this-> to make it clear what variables are members when
    //    reading through the code. Others use a prefix (m_, i.e. m_member) or
    //    a suffix (_, i.e. member_) to denote it. In assignments where you
    //    write your own classes, this will be up to you. For now, do as you
    //    wish, but don't forget readability :)
    //
    // And feel free to delete this huge comment.
    if (n == 0 && d != 1) {
        d = 1;
    }
}

//ADDITION OPERATIONS
Rational Rational::operator+(const Rational& rhs) const {
    // TODO
    //make sure they have same base
    Rational result(0,1);
    //easiest greatest common factor is to multiply bases together
    result.n = (rhs.n * this->d) + (this->n * rhs.d);
    result.d = (rhs.d * this->d);
    result.reduce();
    result.normalize();
    return result; 
}
Rational operator+(int lhs, const Rational& rhs)
{
    Rational result(0,1);
    result.n = rhs.n + (lhs*rhs.d);
    result.d = rhs.d;
    result.reduce();
    result.normalize();
    return result;
}
Rational Rational::operator+(int rhs) const
{
    Rational result(0,1);
    result.n = this->n + (rhs * this->d);
    result.d = this->d;
    result.reduce();
    result.normalize();
    return result;
}
Rational& Rational::operator+=(const Rational& rhs) 
{
    // TODO
    Rational result(0,1);
    result.n = (rhs.n * this->d) + (this->n * rhs.d);
    result.d = (rhs.d * this->d);
    result.reduce();
    result.normalize();
    this->n = result.n;
    this->d = result.d;
    return *this; 
}
Rational& Rational::operator+=(int rhs)
{
    Rational result(0,1);
    result.n = this->n+(rhs* this->d);
    result.d = this->d;
    result.reduce();
    result.normalize();
    this->n = result.n;
    this->d = result.d;
    return *this;
}


//MULTIPLY OPERATIONS
Rational Rational::operator*(const Rational& rhs) const
{
    Rational result(0,1);
    result.n = this->n * rhs.n;
    result.d = this->d * rhs.d;
    result.reduce();
    result.normalize();
    return result; 
}
Rational Rational::operator*(int rhs) const 
{
    // TODO
    Rational result(0,1);
    result.n = this->n * rhs;
    result.d = this->d;
    result.reduce();
    result.normalize();
    return result;
}
Rational& Rational::operator*=(const Rational&rhs)
{
    Rational result(0,1);
    result.n = this->n * rhs.n;
    result.d = this->d * rhs.d;
    result.reduce();
    result.normalize();
    this->n = result.n;
    this->d = result.d;
    return *this;  
}
Rational& Rational::operator*=(int rhs)
{
    Rational result(0,1);
    result.n = this->n * rhs;
    result.d = this->d;
    result.reduce();
    result.normalize();
    this->n = result.n;
    this->d = result.d;
    return *this;
}

// This operator isn't prefixed by Rational:: because it's not actually a
// member of Rational. It just tells the compiler what to do when it sees
// multiplication between an int on the left and a Rational on the right. Our
// friend declaration simply enabled us to access private members on the const
// Rational& rhs in the function body.
//
// Note that we could implement any of the other operators above this same way
// using the corresponding global operator and passing a const Rational& lhs as
// the first argument, it's just a matter of convenience.
Rational operator*(int lhs, const Rational& rhs) {
    // TODO
    Rational result(0,1);
    result.n = lhs * rhs.n;
    result.d = rhs.d;
    result.reduce();
    result.normalize();
    return result;
}
//Bool operators
bool Rational::operator==(const Rational& other) const {
    // TODO
    if (this->n == other.n && this->d == other.d)
    {
        return true; 
    }
    return false; 
}
bool Rational::operator!=(const Rational& other) const
{
    if (this->n != other.n)
    {
        return true;
    }
    if (this->d != other.d)
    {
        return true; 
    }
    return false; 
}
bool Rational::operator<(const Rational& other) const
{
    //convert to same base then compare 
    Rational n1(0,1);
    Rational n2(0,1);
    n1.n = (this->n * other.d);
    n2.n = (other.n * this->d);
    if (n1.n < n2.n)
    {
        return true; 
    }
    return false; 
}
//Exponential
Rational Rational::operator^(int rhs) const
{
    //swap numbers around if negative
    Rational temp(0,1);
    if (rhs < 0)
    {
        rhs = rhs * -1;
        temp.n = pow(this->n, rhs);
        temp.d = pow(this->d, rhs);
        int temp2 = temp.n;
        temp.n = temp.d;
        temp.d = temp2;
    }
    else 
    {
        temp.n = pow(this->n, rhs);
        temp.d = pow(this->d, rhs);
    }
    temp.reduce();
    temp.normalize();
    return temp;
}
//input/output
std::ostream& operator<<(std::ostream& out, const Rational& r)
{
    Rational temp(0,1);
    temp.n = r.n;
    temp.d = r.d;
    temp.reduce();
    temp.normalize();
    out << temp.n << "/" << temp.d;
    return out;
}
std::istream& operator>>(std::istream& in, Rational& r)
{
    //read in first value
    //>> disregards space
    in >> r.n;
    char garbage;
    //assuming properly formatted, read in garbage
    in >> garbage;
    //if not properly formatted, set to 0/1
    if (garbage != '/')
    {
        r.n = 0;
        r.d = 1;
        return in;
    }
    //read in
    in >> r.d;
    r.reduce();
    r.normalize();
    return in; 
}

// TODO: implement the rest of your prototyped methods
void Rational::reduce()
{
    bool negN = false;
    bool negD = false; 
    int n1 = this-> n; 
    int d1 = this-> d;
    int n2 = this->n;
    int d2=this->d;
    if (n1 == 0)
    {
        return;
    }
    //flips to positive if negative
    if (n1 < 0)
    {
        n1 = n1 * -1;
        n2 = n1;
        negN = true;
    }
    if (d1 < 0)
    {
        d1 = d1* -1;
        d2 = d1;
        negD = true;
    }
    //GCD finder
    int common = gcd(n1,d1);
    this->n = n2/common;
    this->d = d2/common;
    //DEALS WITH NEGATIVE CASES
    if (negN == true && negD == false)
    {
        this-> n = this->n * -1;
    }
    else if (negD == true && negN == false)
    {
        this-> n = this->n * -1;
    }
}








