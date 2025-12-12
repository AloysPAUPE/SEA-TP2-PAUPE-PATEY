#include "userlib/libnachos.h"
#include "userlib/syscall.h"


LockId mutex;
CondId cv;
int is_data_ready = 0;

void producer()
{
    //Simulating data production
    for(int i=0;i<100000;i+=1){};

    //Variable to avoid spurious wakeup
    is_data_ready = 1;
    n_printf("Data has been produced\n");

    //Notify consumer when done
    CondSignal(cv);
}

void consumer()
{
    LockAcquire(mutex);
	//Waiting for data from producer
	CondWait(cv);
	if(is_data_ready==1){
	    n_printf("Data has been consumed\n");
	}
	LockRelease(mutex);
}

int main()
{
	mutex=LockCreate("lock_prod_cond");
	cv = CondCreate("cond_prod_cons");
	ThreadId prod_t=threadCreate("prog_prod",&producer);
	ThreadId cons_t=threadCreate("prog_cons",&consumer);
	t_error error_prod=Join(prod_t);
	t_error error_cons=Join(cons_t);
	return 0;
}