#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int tab[10000];
	int i, j, proc;
	
	if(argc>1)
		proc=atoi(argv[1]);
	else {
		printf("Nombre de processus : ");
		if(scanf("%d", &proc)!=1) {
			printf("Le nombre de processus doit être un entier\n");
			return 1;
		}
	}
	
	srand(time(NULL));
	
	for(i=0; i<10000; i++) {
	    tab[i]=(rand()%1000)+1;
	}
	
	printf("Cases contenant 42 :\n");
	
	for(j=1; j<proc; j++) {
		if(fork()==0) {
			for(i=(j-1)*10000/proc; i<j*10000/proc; i++) {
	    	    if(tab[i]==42)
	    	        printf("%d\n", i);
	    	}
	    	return 0;
		}
	}
	
	for(i=(proc-1)*10000/proc; i<10000; i++) {
	    if(tab[i]==42)
	        printf("%d\n", i);
	}
	
	for(j=0; j<proc-1; j++)
		wait();
    
    return 0;
}