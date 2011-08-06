//	Unit to simplify semaphore access in Critical Mass

#include "CMSemaphore.h"					//	the header

CMSemaphore::CMSemaphore(char *name, uint32 count)					//	constructor
	{
	theSemaphore = create_sem(count, name);							//	call system to create semaphore
	} // end of constructor

CMSemaphore::~CMSemaphore()										//	destructor
	{
	delete_sem(theSemaphore);									//	delete the semaphore
	} // end of destructor

void CMSemaphore::Acquire()										//	acquire the semaphore
	{
	acquire_sem(theSemaphore);									//	acquire the semaphore
	} // end of Acquire()
	
void CMSemaphore::Release()										//	release the semaphore
	{
	release_sem(theSemaphore);									//	release the semaphore
	} // end of Release()
	