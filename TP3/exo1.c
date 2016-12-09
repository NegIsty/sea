#include <stdio.h>
#include <pthread.h>
#include <errno.h>

void* bonjour(void*);

int main(int argc, char *argv[]) {
	int i, n;
	
	if(argc>1)
		n=atoi(argv[1]);
	else {
		printf("Nombre de threads : ");
		if(scanf("%d", &n)!=1) {
			printf("Le nombre de threads doit être un entier\n");
			return 1;
		}
	}
	
	pthread_t tid[n];
	int tab[n];
	
	for(i=0; i<n; i++) {
		tab[i]=i;
		
		if(pthread_create(&tid[i], NULL, bonjour, &tab[i]))
			perror("pthread_create");
	}
	
	for(i=0; i<n; i++) {
		if(pthread_join(tid[i], NULL))
			perror("pthread_join");
	}
	
	return 0;
}

void* bonjour(void* n) {
	printf("Bonjour je suis le thread numéro %d\n", *((int*) n));
}