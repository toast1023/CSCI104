#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>


template<typename T, typename TComparator = std::less<T> >
class Heap {
public:
    // Constructs an m-ary heap for any m >= 2
    Heap(size_t m, TComparator comparator = TComparator());

    // Destructor as needed
    ~Heap();

    // Adds an item
    void push(const T& item);

    // returns the element at the top of the heap
    //  max (if max-heap) or min (if min-heap)
    const T& top() const;

    // Removes the top element
    void pop();

    // returns true if the heap is empty
    bool empty() const;

private:
    // Add whatever helper functions and data members you need below
    void trickleUp(size_t loc);
    // heapify() helper function
    void heapify(size_t loc);

    // Data
    std::vector<T> store;
    size_t m;
    TComparator comparator;
};

template<class T, class TComparator>
const T& Heap<T, TComparator>::top() const {
    // If nothing to top:
    // throw std::logic_error("can't top an empty heap");
    if(this->empty())
    {
        throw std::logic_error("can't top an empty heap");
    }
    return store.front();
}

template<class T, class TComparator>
void Heap<T, TComparator>::pop() {
    // If nothing to pop
    // throw std::logic_error("can't pop an empty heap");
    if(this->empty())
    {
        throw std::logic_error("can't pop an empty heap");
    }
    //use pop_back on vector
    store[0] = store.back();
    store.pop_back();
    // std::cout << "Before " << std::endl;
    // for(unsigned int n = 0; n < store.size(); n++)
    // {
    //     std::cout<<  store[n] << " ";
    // }
    // std::cout << std::endl;
    //call heapify on root
    heapify(0);
    // std::cout << "After " <<  std::endl;
    // for(unsigned int n = 0; n < store.size(); n++)
    // {
    //     std::cout << store[n] << " ";
    // }
    // std::cout << std::endl;
}

template<class T, class TComparator>
void Heap<T, TComparator>::push(const T& item)
{
    store.push_back(item);
    // std::cout << "Item: "<< item << std::endl;
    trickleUp(store.size()-1);
}

template<class T, class TComparator>
void Heap<T, TComparator>::trickleUp(size_t loc)
{
    //base case
    if(loc == 0 || this->empty())
    {
        return;
    }
    else
    {
        int parent = (loc-1)/m;
        //promote up if better than child
        while(comparator(store[loc], store[parent]) && (parent >= 0))
        {
            if(loc > 0)
            {
                std::swap(store[parent],store[loc]);
                // std::cout << "loc: " << loc <<  " with value " << store[loc]<< std::endl;
                // std::cout << "parent: " << parent << " with value " << store[parent]<< std::endl;
                // std::cout << std::endl;
                loc = parent;
                //Because loc is size_t it cannot be negative or else seg faults
                if (loc == 0)
                {
                    break;
                }
                parent = (loc-1)/m;
            }
        }
        // for (unsigned k=0; k < store.size(); k++)
        // {
        //     std::cout << store[k] << " ";
        // }
        // std::cout << std::endl;
    }
}

template<class T, class TComparator>
bool Heap<T, TComparator>::empty() const
{
    return store.empty();
}

template<class T, class TComparator>
void Heap<T, TComparator>::heapify(size_t loc)
{
    //recursive or loop
    //base case: no children
    //look through all child nodes use comparator to find the smallest (or largest)
        //has to be SMALLEST child
    //if no swap happens then you're done
    //if swap does happen then recurse again
    //TRICKLE DOWN
    if(loc*m+1 > store.size()-1 || this->empty())
    {
        return;
    }
    else
    {
        //finding smallest/largest child
        //left child
        int child = loc*m+1;
        //first child to last child
        for(unsigned int i=child; i < child+m; i++)
        {
            if(i<store.size())
            {
                //find smallest
                if(this->comparator(store[i], store[child]))
                {
                    child = i;
                }
            }
        }
        //if smallest child is better than loc
        if(this->comparator(store[child], store[loc]))
        {
            std::swap(store[loc],store[child]);
            heapify(child);
        }
    }
}
template<class T, class TComparator>
Heap<T,TComparator>::Heap(size_t m, TComparator comparator)
{
    this->m = m; 
    this->comparator = comparator; 
}
template<class T, class TComparator>
Heap<T,TComparator>::~Heap()
{
    
}
#endif
