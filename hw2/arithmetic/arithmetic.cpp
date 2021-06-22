#include "token.h"
#include "token_stack.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <cctype>

using namespace std;

// THROW ALL EXCEPTIONS EXACTLY AS GIVEN IN THE COMMENTS
// You need only uncomment them to throw the exceptions
// under the correct conditons
// You can check the conditions using an if statement or
// switch statement
// For any problems with the expression format as well as attempting
// division by zero, throw the exception invalid_argument("Malformed");

TokenStack tokenize(std::string expression) {
    TokenStack stack;
    stringstream stream(expression);

    // Loop while there are still characters in the stream.
    while (stream.rdbuf()->in_avail()) {
        // TODO: push your tokens to the stack.
        int temp;
        char temp2; 
        //come back and simplify later 
        //White-space skipper
        if (stream.peek() == ' ')
        {
            stream.get();
            continue;
        }
        //goes through all the signs
        else if (stream.peek() == '(')
        {
            stream >> temp2;
            stack.push(Token(TokenKind::LP));
        }
        else if (stream.peek() == ')')
        {
            stream >> temp2;
            stack.push(Token(TokenKind::RP));
        }
        else if (stream.peek() == '+')
        {
            stream >> temp2;
            stack.push(Token(TokenKind::PLUS));
        }
        else if (stream.peek() == '*')
        {
            stream >> temp2;
            stack.push(Token(TokenKind::TIMES));
        }
        else if (stream.peek() == '-')
        {
            stream >> temp2;
            stack.push(Token(TokenKind::MINUS));
        }
        else if (stream.peek() == '/')
        {
            stream >> temp2;
            stack.push(Token(TokenKind::DIVIDE));
        }
        //NUMBER INPUT
        else if (stream >> temp)
        {
            stack.push(Token(TokenKind::NUMBER, temp));
        }
        //if not any of those, then EXCEPTION
        else
        {
            throw invalid_argument("Malformed");
        }
        // If we find an invalid token, we can simply:
        // throw invalid_argument("Malformed");
    }

    // We pushed the tokens into the stack from left to right, which means
    // we'll pop them back out backwards. For convenience, we'll return the
    // reverse of our current stack so that we can pop the tokens out from
    // left to right, as you'd read the expression.
    stack.reverse();
    return stack;
}

// Reduce is called when we encounter closing parentheses. If the expression
// is validly formed, we can expect there to be either a binary operation on
// the work stack. This method should pop the tokens in that operation,
// compute the result, and push it back onto the work stack as a new token.
void reduce(TokenStack& work) {
    TokenStack immediate;

    // TODO: pop tokens off of work into immediate until we hit TokenKind::LP.
    // TODO: if there's no TokenKind::LP, throw:
    // throw invalid_argument("Malformed");
    bool LPexists = false;
    while(!work.empty())
    {
        if (work.top().kind == TokenKind::LP)
        {
            LPexists = true; 
            // cout << "Immediate 3: " << immediate << endl;
            break;
        }
        immediate.push(work.top());
        work.pop();
        // cout << "Immediate 2: " << immediate << endl;
    }
    //gets rid of remaining LP
    work.pop();
    if (LPexists == false)
    {
        throw invalid_argument("Malformed");
    }
    if (immediate.empty())
    {
        throw invalid_argument("Malformed");
    }
    // cout << "Immediate: " << immediate << endl;
    // TODO: check whether we have "-p" or "p @ q @ ..." and handle accordingly
    // TODO: if you are expecting a token but immediate is empty, throw:
    // throw invalid_argument("Malformed");
    // TODO: if you are expecting a token of a different kind, throw:
    // throw invalid_argument("Malformed");
    // TODO: if you encounter a differing chained binary operation, throw:
    // throw invalid_argument("Malformed");
    // cout << "Reduced Work: " << work << endl;
    if (immediate.top().kind == TokenKind::MINUS)
    {
        immediate.pop();
        work.push(Token(TokenKind::NUMBER, (immediate.top().value * -1)));
        immediate.pop();
        if (!immediate.empty())
        {
            throw invalid_argument("Malformed");
        }
        // cout << "Negative immediate: " << immediate << endl; 
        // cout << "Reduced Work: " << work << endl;
    }
    // TODO: use integer math to do your computations.
    // TODO: if the operator is not of kind +, -, *, or /, throw:
    // throw invalid_argument("Malformed");
    // TODO: if dividing by zero, throw:
    // throw invalid_argument("Malformed");
    // TODO: push the result of either branch back onto work
    else
    {
        if (immediate.top().kind != TokenKind::NUMBER)
        {
            // cout << "ERROR" << endl;
            throw invalid_argument("Malformed");
        }
        int calc = immediate.top().value;
        immediate.pop();
        //constantly check if its empty, otherwise top() fails
        if (!immediate.empty())
        {
            //ALL THINGS PLUS GO HERE
            //basically repeat of code for multiple, divide and 
            //subtract, with additional checks for division by
            //0 for subtract
            if (immediate.top().kind == TokenKind::PLUS)
            {
                while (!immediate.empty())
                {
                    if (immediate.top().kind == TokenKind::PLUS)
                    {
                        immediate.pop();
                    }
                    else
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.empty())
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.top().kind == TokenKind::NUMBER)
                    {
                        //avoid top() == empty error
                        if (!work.empty())
                        {
                            if (work.top().kind == TokenKind::NUMBER)
                            {
                                calc = work.top().value; 
                                work.pop();
                                work.push(Token(TokenKind::NUMBER, (calc + immediate.top().value)));
                            }
                            else
                            {
                                work.push(Token(TokenKind::NUMBER, (calc + immediate.top().value)));
                            }
                        immediate.pop();
                        }
                        else 
                        {
                            work.push(Token(TokenKind::NUMBER, (calc + immediate.top().value)));
                            immediate.pop();
                        }
                    }
                    else 
                    {
                        throw invalid_argument("Malformed");
                    }
                }
            }
            //ALL THINGS MULTIPLY GO HERE
            else if (immediate.top().kind == TokenKind::TIMES)
            {
                while (!immediate.empty())
                {
                    if (immediate.top().kind == TokenKind::TIMES)
                    {
                        immediate.pop();
                    }
                    else
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.empty())
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.top().kind == TokenKind::NUMBER)
                    {
                        if (!work.empty())
                        {
                            if (work.top().kind == TokenKind::NUMBER)
                            {
                                calc = work.top().value; 
                                work.pop();
                                work.push(Token(TokenKind::NUMBER, (calc * immediate.top().value)));
                            }
                            else
                            {
                                work.push(Token(TokenKind::NUMBER, (calc * immediate.top().value)));
                            }
                        immediate.pop();
                        }
                        else 
                        {
                            work.push(Token(TokenKind::NUMBER, (calc * immediate.top().value)));
                            immediate.pop();
                        }
                    }
                    else 
                    {
                        throw invalid_argument("Malformed");
                    }
                }
            }
            //ALL THINGS SUBTRACT GO HERE
            else if (immediate.top().kind == TokenKind::MINUS)
            {
                while (!immediate.empty())
                {
                    if (immediate.top().kind == TokenKind::MINUS)
                    {
                        immediate.pop();
                    }
                    else
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.empty())
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.top().kind == TokenKind::NUMBER)
                    {
                        if (!work.empty())
                        {
                            if (work.top().kind == TokenKind::NUMBER)
                            {
                                calc = work.top().value; 
                                work.pop();
                                //for division and subtraction, order matters!!!
                                work.push(Token(TokenKind::NUMBER, (calc - immediate.top().value)));
                            }
                            else
                            {
                                work.push(Token(TokenKind::NUMBER, (calc-immediate.top().value)));
                            }
                        immediate.pop();
                        }
                        else 
                        {
                            work.push(Token(TokenKind::NUMBER, (calc-immediate.top().value)));
                            immediate.pop();
                        }
                    }
                    else 
                    {
                        throw invalid_argument("Malformed");
                    }
                }
            }
            //ALL THINGS DIVIDE GO HERE
            else if (immediate.top().kind == TokenKind::DIVIDE)
            {
                while (!immediate.empty())
                {
                    if (immediate.top().kind == TokenKind::DIVIDE)
                    {
                        immediate.pop();
                    }
                    else
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.empty())
                    {
                        throw invalid_argument("Malformed");
                    }
                    if (immediate.top().kind == TokenKind::NUMBER)
                    {
                        if (!work.empty())
                        {
                            if (work.top().kind == TokenKind::NUMBER)
                            {
                                calc = work.top().value; 
                                work.pop();
                                //check for division by 0!
                                if (immediate.top().value == 0)
                                {
                                    throw invalid_argument("Malformed");
                                }
                                work.push(Token(TokenKind::NUMBER, (calc / immediate.top().value)));
                            }
                            else
                            {
                                if (immediate.top().value == 0)
                                {
                                    throw invalid_argument("Malformed");
                                }
                                work.push(Token(TokenKind::NUMBER, (calc/immediate.top().value)));
                            }
                        immediate.pop();
                        }
                        else 
                        {
                            if (immediate.top().value == 0)
                            {
                                throw invalid_argument("Malformed");
                            }
                            work.push(Token(TokenKind::NUMBER, (calc/immediate.top().value)));
                            immediate.pop();
                        }
                    }
                    else 
                    {
                        throw invalid_argument("Malformed");
                    }
                }
            }
        }
        //case of single number
        else 
        {
            work.push(Token(TokenKind::NUMBER, calc));
        }
    }
    // cout <<"hi: " << work << endl;
}   

int evaluate(std::string expression) {
    TokenStack tokens = tokenize(expression);
    TokenStack work;

    // TODO: while the stack is not empty, pop the top token.
    // TODO: if the top token is of TokenKind::RP, reduce(work).
    // TODO: otherwise, push the top token onto work.
    while(!tokens.empty())
    {
        if(tokens.top().kind == TokenKind::RP)
        {
            // cout << "work1:" << work << endl;
            reduce(work);
        }
        else 
        {
            work.push(tokens.top());
            // cout << "work2: " << work << endl;
        }
        tokens.pop();
    }
    // cout <<"Work: " << work << endl; 
    // TODO: once done, check there is exactly one TokenKind::NUMBER token.
    // TODO: if so, return its value, otherwise throw:
    // throw invalid_argument("Malformed");
    if (work.size()== 1 && work.top().kind == TokenKind::NUMBER)
    {
        return work.top().value;
    }
    else
    {
        throw invalid_argument("Malformed");
    }
}


// Please DO NOT CHANGE MAIN
int main(int argc, char** argv) {
    if (argc <= 1) {
        cerr << "Expect an expression..." << endl;
        return -1;
    }

    try {
        cout << evaluate(argv[1]) << endl;
    } catch (invalid_argument &e) {
        cerr << "error: " << e.what() << endl;
    }
    return 0;
}


// If you're interested in extending your parser to support things like order
// of operations, unary operators, and single values, feel free to ping me
// on Discord (noahbkim) once you've finished the assignment as is. This will
// not give you any extra credit or improve your score in any way; it's just
// something fun and cool.
