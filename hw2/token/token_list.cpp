#include "token_list.h"
#include <stdexcept>

TokenList::TokenList(TokenList const& other)
{
    if (other.head == nullptr)
    {
        head = nullptr;
    }
    else
    {
        std::shared_ptr<Item> current = other.head;
        for(unsigned int i=0; i < other.size(); i++)
        {
            this->push_back(current->value);
            current = current->next;
        }
    }
}
TokenList& TokenList::operator=(TokenList const& other)
{
    if (this->head == other.head)
    {
        return *this;
    }
    else 
    {
        //reset old list 
        for (unsigned int i=0; i<this->size(); i++)
        {
            remove(i);
        }
        //proceed with copying
        std::shared_ptr<Item> current = other.head;
        this->_size = other._size;
        for(unsigned int i=0; i < other.size(); i++)
        {
            this->push_back(current->value);
            current = current->next;
        }
    }
    return *this; 
}
TokenList::~TokenList()
{
    //iterate through and DELETE
    for (unsigned int i=0; i<this->size(); i++)
    {
        remove(i);
    }
    this->head.reset();
}

//done
size_t TokenList::size() const {
    return this->_size;
}

// TODO: implement the TokenList methods.

Token& TokenList::at(size_t index) {
    if (this->size() == 0) {
        throw std::out_of_range("no items");
    }

    // TODO: complete this function here
    std::shared_ptr<Item> current = this->head;
    for (size_t i =0; i<index%this->size(); i++)
    {
        current = current->next;
    }
    return(current->value);
}

const Token& TokenList::at(size_t index) const {
    if (this->size() == 0) {
        throw std::out_of_range("no items");
    }

    // TODO: complete this function here
    std::shared_ptr<Item> current = this->head;
    for (size_t i =0; i<index%this->size(); i++)
    {
        current = current->next;
    }
    return(current->value);
}
// TODO: insert (a copy of) the given Token at the end of list.
void TokenList::push_back(Token value)
{
    //edge, if nothing is in list
    if (this->head == nullptr)
    {
        //creates new shared pointer
        std::shared_ptr<Item> current = std::make_shared<Item> (value);
        head = current;
        head->next = head;
        head->previous = head;
        this->_size++;
    }
     //everything else normally
    else 
    {
        std::shared_ptr<Item> current = std::make_shared<Item> (value);
        std::shared_ptr<Item> last = head->previous;
        //head is next of new item
        current->next = head;
        //new item is previous of head
        head->previous = current;
        //last item is previous of new item
        current->previous = last; 
        //last item next is new node
        last->next = current; 
        this->_size++;
    }
}
// TODO: insert (a copy of) the given Token at the index given
// This function should first increase the size of the list by 1.
// Then it should insert the token at index%n where n is the new size of the list
// and assuming that the head of the list is at index 0.
void TokenList::insert(size_t index, Token value)
{
    if (this->size() == 0)
    {
        this->push_back(value);
        return;
    }
    std::shared_ptr<Item> newToken = std::make_shared<Item> (value);
    std::shared_ptr<Item> current = this->head;
    for (unsigned int i =0; i< index%this->size(); i++)
    {
        current = current->next;
    }
    std::shared_ptr<Item> prev = current->previous;
    prev->next = newToken;
    newToken->next = current;
    newToken->previous = prev;
    current->previous = newToken;
    this->_size++;
}
// TODO: removes the item at the given index from the list.
// If the list is empty, remove should just return without modifying the list.
// Otherwise, remove should remove the item at index%n where n is the current size of the list
// assuming that the head of the list is at index 0.
void TokenList::remove(size_t index)
{
    //base case if empty
    if (this->size()==0 || this->head == nullptr)
    {
        return;
    }
    std::shared_ptr<Item> current = this->head;
    std::shared_ptr<Item> prev = nullptr;
    //find node to delete
    for (unsigned int i=0; i < index%this->size(); i++)
    {
        prev = current; 
        current = current->next; 
    }
    std::shared_ptr<Item> nex = current->next;
    //check if there is only one node in list
    if (current->next == current)
    {
        this->head.reset();
        current->next.reset();
        current->previous.reset();
        this->_size--;
        return; 
    }
    //for normal items 
    else 
    {
        //if item to be deleted is head
        if (this->head == current)
        {
            prev = current; 
            head = current->next;
            prev->next = nex;
            nex->previous = prev;
            current.reset();
            this->_size--;
        }
        else 
        {
            prev->next = nex; 
            nex->previous = prev; 
            current.reset();
            this->_size--;
        }
    }
}
// TODO: reverse the list.
void TokenList::reverse()
{
    if (this->head == nullptr)
    {
        return; 
    }
    std::shared_ptr<Item> last = head->previous;
    this->head = last; 
    std::shared_ptr<Item> current = last->previous;
    std::shared_ptr<Item> prev = last;
    for (unsigned int i=this->size()-1; i >0; i--)
    {
        prev = current->previous;
        this->push_back(current->value);
        this->remove(i);
        current = prev; 
    }

}

std::ostream& operator<<(std::ostream& out, const TokenList& list)  {
    out << "{";
    std::shared_ptr<TokenList::Item> cursor = list.head;
    if (cursor != nullptr) {
        for (size_t i = 0; i < list.size() - 1; i++) {
            out << cursor->value << ", ";
            cursor = cursor->next;
        }
        out << cursor->value;
    }
    return out << "}";
}
