//	Unit to simplify semaphore access in Critical Mass

#ifndef __CMSEMAPHORE_H__
#define __CMSEMAPHORE_H__

#include "CMGUIDefs.h"

#include <OS.h>

class CMSemaphore
{								//	errors are assumed to be fatal to the program
	sem_id theSemaphore;				//	the system semaphore
	
	public:
	CMSemaphore(char *name = "CMSemaphore", uint32 count = 1);
									//	constructor
	~CMSemaphore();					//	destructor
		
	void Acquire();					//	acquire the semaphore
	void Release();					//	release the semaphore
};

#endif
