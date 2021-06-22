#include "token_stack.h"
#include <stdexcept>

TokenStack::TokenStack()
{

}
TokenStack::~TokenStack()
{
	while(list.size() != 0)
	{
		pop();
	}
}
// TODO: return the top value on the stack.
const Token& TokenStack::top() const{
    if (this->empty()) 
    {
        throw std::out_of_range("empty stack");
    }
    // TODO: complete function here
    else
    {
    	return list.at(list.size()-1);
    }
}
// TODO: implement the TokenStack methods.
size_t TokenStack::size() const
{
	return list.size();
}

// TODO: check whether the stack is empty
bool TokenStack::empty() const
{
	if (list.size() == 0)
	{
		return true; 
	}
	return false; 
}

// TODO: push a new token on top of the stack.
void TokenStack::push(const Token& t)
{
	list.push_back(t);
}
// TODO: remove the top element on the stack.
// If the stack is empty, this function should do nothing
void TokenStack::pop()
{
	if (list.size() == 0)
	{
		return;
	}
	list.remove(list.size()-1);
}

// TODO: reverse the contents of the stack.
void TokenStack::reverse()
{
	list.reverse();
}

// Help debugging
std::ostream& operator<<(std::ostream& out, const TokenStack& stack)
{
	for (unsigned int i=0; i<stack.list.size(); i++)
	{
		out << stack.list.at(i);
	}
	return out; 
}









