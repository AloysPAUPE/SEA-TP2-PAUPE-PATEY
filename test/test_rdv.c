#include "userlib/libnachos.h"
#include "userlib/syscall.h"

int var_1=0;
int var_2=0;
SemId sem_1;
SemId sem_2;

void prog_1(){
	var_1=1;
	V(sem_2);
	P(sem_1);
	n_printf("We leave prog_1 with var_1=%d and var_2=%d\n", var_1,var_2);
}

void prog_2(){
	var_2=1;
	V(sem_1);
	P(sem_2);
	n_printf("We leave prog_2 with var_1=%d and var_2=%d\n", var_1, var_2);
}

int
main() {
  sem_1=SemCreate("sem_rdv_1",0);
  sem_2=SemCreate("sem_rdv_2",0);
  ThreadId prog_1_t=threadCreate("prog_rdv_1",&prog_1);
  ThreadId prog_2_t=threadCreate("prog_rdv_2",&prog_2);
  return 0;
}
