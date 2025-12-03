#include "userlib/libnachos.h"
#include "userlib/syscall.h"

int shared_var=0;
LockId mutex;

void prog(){
	for(int i=0;i<50;i++){
		LockAcquire(mutex);
		shared_var+=1;
		LockRelease(mutex);
	}
}

int
main() {
  mutex=LockCreate("sem_mutex");
  ThreadId prog_t1=threadCreate("prog_mutex_1", &prog);
  ThreadId prog_t2=threadCreate("prog_mutex_2",&prog);
  t_error error_t1=Join(prog_t1);
  t_error error_t2=Join(prog_t2);
  n_printf("We now have shared_var=%d", shared_var);
  return 0;
}

