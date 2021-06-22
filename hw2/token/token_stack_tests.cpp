
#include "token_stack.h"
#include <iostream>
#include <string>

using namespace std;


int main() {
	TokenStack test; 
	test.pop();
	cout << test << endl; 


	test.push(Token(TokenKind::NUMBER, 1));
    test.push(Token(TokenKind::NUMBER, 2));
    test.push(Token(TokenKind::NUMBER, 3));
    test.push(Token(TokenKind::NUMBER, 4));
    test.push(Token(TokenKind::NUMBER, 5));

    cout << "Push test I: " << test << endl;

    Token(top1) = test.top();
    cout << "Top test I: " << top1 << endl;

    test.reverse();
    cout << "Reverse test I: " << test << endl;

    Token(top2) = test.top();
    cout << "Top test II: " << top2 << endl;

    test.pop();
    test.pop();
    cout << "Pop test: " << test << endl; 

    test.reverse();
    cout << "Reverse test II: " << test << endl;

    test.push(Token(TokenKind::NUMBER, 1));
    test.push(Token(TokenKind::NUMBER, 2));
    cout << "Push test II: " << test << endl; 

    Token(top3) = test.top();
    cout << "Top test III: " << top3 << endl;

    test.pop();
    test.pop();
    test.pop();
    test.pop();
    test.pop();

    test.push(Token(TokenKind::LP));
    test.push(Token(TokenKind::NUMBER, 1));
    test.push(Token(TokenKind::PLUS));
    test.push(Token(TokenKind::NUMBER, 2));
    test.push(Token(TokenKind::RP));

    cout << "Equation test: " << test << endl;

    test.reverse();
    cout << "Size: " << test.size() << endl;
    cout << "Equation reverse: " << test << endl;

    test.pop();
    cout << "Top test IV: " << test.top() << endl;
    cout << "Size: " << test.size() << endl;

    test.pop();
    cout << "Top test V: " << test.top() << endl;
    cout << "Size: " << test.size() << endl;

    test.pop();
    cout << "Top test VI: " << test.top() << endl;
    cout << "Size: " << test.size() << endl;

    test.pop();
    cout << "Top test VII: " << test.top() << endl;
    cout << "Size: " << test.size() << endl;

    test.pop();
    cout << "POP ERROR TEST" << endl;
    cout << "Size: " << test.size() << endl;

    test.pop();
    cout << "POP ERROR TEST II" << endl;

    cerr << "Nice Nice NICE" << endl;
    return 0;
}
