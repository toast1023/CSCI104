#include "simple.h"
#include <algorithm>

using namespace std;


char* SimpleCharacterManager::alloc_chars(size_t size) {
	// TODO: your implementation here
	// will return a pointer to a memory address in the buffer that can hold n characters. If there is not enough space left in the buffer, return nullptr.

	//check for spaces left in buffer
	unsigned int remain = 0;;
	int temp = first_available_address -buffer;
	for (unsigned int i = temp; i < BUFFER_SIZE; i++)
	{
		//buffer is not yet allocated
		if (buffer[i] == '\0') 
		{
			remain = -1; 
		}
		else if (buffer[i] != '\0')
		{
			remain++;
		}
	}
	//within bounds, memory allocated
	//return address
	if (size < remain)
	{
		first_available_address = first_available_address + size;
		return (first_available_address-size);
	}

	//not enough space left in buffer
	return nullptr;
}

void SimpleCharacterManager::free_chars(char* address) {
	// TODO: your implementation here
	// function given a pointer into the buffer, will free all at that address until the end of the buffer. If the pointer is nullptr or an address not in the buffer, this function should do nothing. This is not the ideal way to free memory; it is only to help us learn an easier way to manage memory to start

	//function is nullptr, do nothing
	if (address == nullptr)
	{
		return; 
	}

	for(unsigned int i = 0; i < BUFFER_SIZE; i++)
    {
        if(&buffer[i] == address)
        {
        	//fill(start, end, '\0')
        	std::fill(&buffer[i], &buffer[BUFFER_SIZE-1], '\0');
            first_available_address = address;
            return;
        }
    }
    return;
}
