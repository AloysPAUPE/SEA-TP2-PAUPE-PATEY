#include "userlib/libnachos.h"
#include "userlib/syscall.h"

int shared_var=0;
LockId mutex;
SemId sem_plein;
SemId sem_vide;

int
main() {
  sem_plein=SemCreate("sem_plein",10);
  sem_vide=SemCreate("sem_vide",0);
  mutex=LockCreate("lock_prod_cons");
  ThreadId prog_cons_t=threadCreate("prog_cons",&prog_cons);
  ThreadId prog_prod_t=threadCreate("prog_prod",&prog_prod);
  t_error error_prog_cons = Join(prog_cons_t);
  t_error error_prog_prod = Join(prog_prod_t);
  n_printf("We have shared_var=%d",shared_var);
  return 0;
}

void prog_cons(){
	for(int i=0;i<25;i++){
		P(sem_plein);
		LockAcquire(mutex);
		shared_var-=1;
		LockRelease(mutex);
		V(sem_vide);
	}
}

void prog_prod(){
	for(int i=0;i<25;i++){
		P(sem_vide);
		LockAcquire(mutex);
		shared_var+=1;
		LockRelease(mutex);
		V(sem_plein);
	}
}