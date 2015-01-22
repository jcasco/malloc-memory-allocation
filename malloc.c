/* malloc.c
*  Josue Casco
*  CS360
*  11/6/14
*
*/
#include "malloc.h"
#include <strings.h>
//initial head of free list
void *malloc_head = NULL;

//free list
typedef struct flist{
	int size;
	struct flist *flink;
	struct flist *blink;
} *Flist;


//allocates given size, in bytes, of memory, and returns pointer to memory
void *malloc(size_t size){
	int multSize = size;
	int *blk;
	int totSize; 
	Flist frLst;	
	Flist tmp;
	Flist tmp2;
	
	//calculate padding to multiple of 8
	if(size % 8 != 0){
		//size plus rem
		multSize = size + (8 - (size % 8));
	}
	totSize = multSize + 8;

	//free list is empty, no memory in free list
	if(malloc_head == NULL){
		//if total size is left than min block minus 12bytes
		if(size <= 8176){
			//set new head 
			malloc_head = sbrk(8192);

			//create free list
			frLst = (Flist) malloc_head;
			//total free size - user allocated
			frLst->size =  8192 - (multSize + 8);
			frLst->flink = NULL;
			frLst->flink = NULL;
	
			blk = (int *)(malloc_head + frLst->size);
			*blk = multSize +8;
			
			return (void *)blk+8;
		}
		//
		else{
			blk = (int*) sbrk(multSize + 8);
			//write the size to begining
			*blk = multSize + 8;
			
			return (void*)blk + 8; 
		}
	}
	//free list exists
	else{
		tmp2 = NULL;
		//find large enough chunk in free list
		tmp = malloc_head;
		while(tmp != NULL){
			//check size of blk
			if(tmp->size >= multSize +8){
				tmp2 = tmp;
				tmp = NULL;				
			}
			else
				tmp = tmp->flink;
		}

		//add block to free list
		if(tmp2 != NULL){
			//enough for mem + free list node
			if((tmp2->size - (multSize +8)) >= 12){
				tmp2->size = tmp2->size - (multSize + 8);
				
				blk = (int*)((void*)tmp2 + tmp2->size);
				*blk = multSize + 8;

				return (void *)blk + 8;
			}
			//free list node won't fit 
			else{
				Flist whlBlk;
				whlBlk = tmp2;
				//delete node, joins prev and next link
				//node is begining
				if(tmp2 == malloc_head){
					malloc_head = tmp2->flink;
					if(tmp2->flink != NULL)
							tmp2->flink->blink = NULL;
				}
				//end of list
				else if(tmp2->flink == NULL && tmp2->blink !=NULL){
					tmp2->blink->flink = NULL;
				}
				else{
					Flist pnode;
					pnode = tmp2->blink;
					pnode->flink = tmp2->flink;
					pnode->flink->blink = pnode;
				}
				
				return whlBlk + 8;
			}
		}
		//no block size found
		else{ 
			//call sbrk for size greater than 8176
			if(size > 8176){
				blk= (int *) sbrk(multSize + 8);
				*blk = multSize +8;

				return (void *) blk + 8;
			}
			//call sbrk, add remaining mem to list
			else{
				//store old begging of list
				Flist tmpHead;
				tmpHead = (Flist) malloc_head;
			
				malloc_head = sbrk(8192);
				
				//new node w/allocated memory 
				frLst = (Flist) malloc_head;
				frLst->size = 8192 - multSize +8;
				frLst->flink = tmpHead;
				frLst->blink = NULL;

				tmpHead->blink = frLst;

				blk = (int*)(malloc_head + frLst->size);
				*blk = multSize + 8;

				return (void *)blk + 8;
			}	
		}
	}
}

void free(void *ptr){
	Flist temp, prevH; 
	temp = (Flist) (ptr-8);
	if(malloc_head == NULL){
	//	malloc_head = temp;
	//	malloc_head->flink = NULL;
	//	malloc_head->blink = NULL;
	}
	else{
		//move node to head link previous free nodes
		prevH = malloc_head;
		prevH->blink = temp;
		//temp->flink = prevH;
		//malloc_head = temp;
	}
}

void *calloc(size_t nmemb, size_t size){
	void *p;
	//return NULL if nmemb or size is 0
	if(nmemb ==0 || size ==0)
		return NULL;
	
	//allocate total memory needed
	p = malloc(nmemb * size);

	//initialize to 0
	bzero(p, nmemb * size);

	return p;
}
void *realloc(void *pts, size_t size){
	void *pdest;
	pdest = malloc(size);
	bcopy(pts, pdest, size);
	//free(pts);
	return pdest;
}


