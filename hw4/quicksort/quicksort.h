#ifndef QSORT_H
#define QSORT_H

#include <vector>
#include <algorithm>


template<class T, class TComparator>
T& median_of_three(T& a, T& b, T& c, TComparator comparator) {
    // TODO: implement.
    //if b < a and a < c OR c < a and a < b
    //then a is MIDDLE
    if ((comparator(b,a)&& comparator(a,c)) || (comparator(c,a) && comparator(a,b)))
    {
    	return a; 
    }
    //if a < b and b < c OR c < b and b < a
    //then b is MIDDLE
    else if ((comparator(a,b) && comparator(b,c)) || (comparator(c,b) && comparator(b,a)))
    {
    	return b;
    }
    //if not a and b then c
    else 
    {
    	return c; 
    }
}

template<class T, class TComparator>
int partition(std::vector<T>& vec, TComparator& comparator, int low, int high) {
	  // TODO: implement.
	//gets value median
    T pivot = median_of_three(vec[low], vec[high], vec[(low+high)/2], comparator);
    //index of pivot
    int pivotIndex=0;
    //pivot is the low value
    if(vec[low] == pivot)
    {
    	pivotIndex = low;
    }
    //pivot already in place, 
    else if (vec[high] == pivot)
    {
    	pivotIndex = high;
    }
    //pivot is middle value
    else 
    {
    	pivotIndex = (low+high)/2;
    }
    std::swap(vec[pivotIndex], vec[high]);
    pivotIndex=high;
    int i = low-1;
    //i keeps track of pivot location
    //j goes through vector
    for(int j = low; j < high; j++)
    {
    	if(comparator(vec[j], vec[pivotIndex]))
    	{
    		i++;
    		std::swap(vec[i],vec[j]);
    	}
    }
    std::swap(vec[i+1], vec[pivotIndex]);
    pivotIndex = i+1;
   	// std::cout << "Pivot: " << pivotIndex << ": " << pivot << std::endl;
   	// for (unsigned k =0; k <vec.size(); k++)
   	// {
   	// 	std::cout << vec[k];
   	// }
    // std::cout << std::endl;
    return pivotIndex;
}

template<class T, class TComparator>
void quicksortHelper(std::vector<T>& vec, int low, int high, TComparator &comparator)
{
	if(low >= high)
	{
		return;
	}
	//if nothing to sort
	if ((vec.size() == 1) || (vec.size() ==0))
    {
    	return;
    }
    if(vec.size() == 2)
    {
    	if(comparator(vec[1],vec[0]))
    	{
    		std::swap(vec[1],vec[0]);
    		return; 
    	}
    }
    //gets location of pivot
    int new_partition = partition(vec, comparator, low, high);
    // std::cout << "New partition: " << new_partition << std::endl;
    quicksortHelper(vec, low, new_partition-1, comparator);
    quicksortHelper(vec, new_partition+1, high, comparator);


}

template<class T, class TComparator>
void quicksort(std::vector<T>& vec, TComparator comparator) {
    // TODO: implement.
    //immediately calls helper
    quicksortHelper(vec, 0 , (vec.size()-1),  comparator);
}

// template<class T, class TComparator>
// int doSwap(&vec, low, high, TComparator comparator)
// {

// }




#endif
