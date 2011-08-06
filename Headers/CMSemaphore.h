//	Unit to simplify semaphore access in Critical Mass

#pragma once
#include "CMGUIDefs.h"

#include <OS.h>

class CMSemaphore						//	class for easy access to semaphores with error-handling
	{								//	errors are assumed to be fatal to the program
	sem_id theSemaphore;				//	the system semaphore
	
	public:
	CMSemaphore(char *name = "CMSemaphore", uint32 count = 1);
									//	constructor
	~CMSemaphore();					//	destructor
		
	void Acquire();					//	acquire the semaphore
	void Release();					//	release the semaphore
	}; // end of class CMSemaphore
	
