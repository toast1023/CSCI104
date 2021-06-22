#include "flexible.h"

using namespace std;


// The constructor creates a unique_ptr to a dynamically allocated array of two
// CharacterBlocks using the initialization list syntax. No need to change this
// unless you add fields to the FlexibleCharacterManager.
FlexibleCharacterManager::FlexibleCharacterManager() : blocks(new CharacterBlock[2] {}) {}

char* FlexibleCharacterManager::alloc_chars(size_t n) 
{
	//declaring a temp variables that store free block information
	size_t freeBuff = 0;
	size_t freeBloc = 0;  
	size_t freeSpace = 0;  
	char* freeAddress = buffer;      
	//very first block
	if (blocks_in_use == 0)
    {
        CharacterBlock firstBlock;
        firstBlock.address = buffer;
        firstBlock.size = n;
        blocks[0] = firstBlock;
        blocks_in_use++;
        return firstBlock.address;
    }
    //LOOKING FOR FREE SPACE
	for (unsigned int i = 0; i < blocks_in_use; i++)
	{
		//check is there is anything inside and if there is space
		//SPACE FOUND, no need to continue loop
		if (blocks[freeBloc].address[0] == '\0' && n<=blocks[freeBloc].size) 
        {
            freeAddress = freeBuff + blocks[freeBloc].address;
            freeSpace = freeBuff + blocks[freeBloc].size - n;
            blocks[freeBloc].size = n;
            break;
        }
        freeBloc++;
	}

	//last availiable space not big enough (end of buffer)
	if (freeBloc == blocks_in_use) 
	{
        if (BUFFER_SIZE < n)
    	{
    		return nullptr;
    	}
        if (blocks_in_use > 0) 
        {
        	//update before ending
        	freeAddress = blocks[blocks_in_use-1].address + blocks[blocks_in_use-1].size
        				+ freeBuff;
            CharacterBlock lastBlock = blocks[blocks_in_use-1];
            if ((lastBlock.address+lastBlock.size+n) > (buffer+BUFFER_SIZE))
            {
                return nullptr;
            }
        }
	}
	//double blocks used if too small
	if (blocks_in_use ==  blocks_size) 
	{
        doubleBlock();
	}
	//UPDATING BLOC
	if (freeBloc == blocks_in_use) 
	{
        blocks[freeBloc].address = freeAddress;
        blocks[freeBloc].size = n;
        blocks_in_use++;
	} 
	//SORTING/BLOCK SHIFTING
	else if (freeSpace > 0) 
	{
		//start at last valid block
        for (unsigned int i = blocks_in_use; i>=freeBloc; i--) 
        {
            blocks[i+1] = blocks[i];
        }
        //update
        blocks[freeBloc+1].address = freeBuff+blocks[freeBloc].address + blocks[freeBloc].size;
        blocks[freeBloc+1].size = freeBuff+freeSpace;
        blocks_in_use++;
	}
    return freeAddress;
}

void FlexibleCharacterManager::free_chars(char* p) {
	//INVAID ADDRESS
    if (p == nullptr)
    {
        return;
    }
    else if (p < buffer)
    {
    	return;
    }
    else if (p > buffer+BUFFER_SIZE)
    {
    	return;
    }
    //FREEING ADDRESSES
    //space freed
    unsigned int buffFree = 0;
    unsigned int spaceFree = 0;
    for (unsigned int i = 0; i < blocks_in_use; i++)
    {
    	spaceFree++;
    	if (blocks[i].address == p) 
    	{
	        fill(p, p+blocks[i].size, '\0');
            break;
        }
    }
    //BLOCK SHIFTING
	if (spaceFree < blocks_in_use) 
	{
		shiftBlock(spaceFree, buffFree);
    }
    //block Halving
    if (blocks_in_use < (blocks_size / 2)) 
    {
        halfBlock();
    }
}
//DOUBLE size of block if too small
void FlexibleCharacterManager::doubleBlock()
{
	//transfers ownership of blocks to temp
	CharacterBlock* temp = blocks.release();
    blocks_size *= 2;
    blocks = make_unique<CharacterBlock[]>(blocks_size);
    CharacterBlock* newBlocks = blocks.get();
    for (unsigned int i=0; i<blocks_in_use; i++) 
    {
        newBlocks[i] = temp[i];
    }
}
//similar code as above but for HALVING if too small
void FlexibleCharacterManager::halfBlock()
{
	CharacterBlock* temp = blocks.release();
    blocks_size /= 2;
    blocks = make_unique<CharacterBlock[]>(blocks_size);
    CharacterBlock* newBlocks = blocks.get();
    for (unsigned int i=0; i<blocks_in_use; i++) 
    {
        newBlocks[i] = temp[i];
    }
}
void FlexibleCharacterManager::shiftBlock(unsigned int &spaceFree, unsigned int &buffFree)
{
	//shifting cases
	if ((spaceFree+1) < blocks_in_use) 
    {
    	if (blocks[spaceFree+1].address[0] == '\0')
    	{
    		blocks[spaceFree].size = buffFree+ blocks[spaceFree+1].size 
    		+ blocks[spaceFree].size;
	        for (unsigned int t = spaceFree; t < blocks_in_use-1; t++)
	        {
	        	blocks[t] = blocks[t+1];
	        }
	        blocks_in_use--;
	        spaceFree++;
   		 }
	    else if (spaceFree > 1) 
	    {
	    	if (blocks[spaceFree-1].address[0] == '\0')
	    	{
	    		blocks[spaceFree-1].size = buffFree+ blocks[spaceFree].size
	            +blocks[spaceFree-1].size;
	            for (unsigned int k=spaceFree; k<blocks_in_use-1; k++)
	            {
	            	blocks[k] = blocks[k+1];
	            	k++;
	            }
	            blocks_in_use--;
	    	}
	    } 
	}
}




