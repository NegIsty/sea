#include <stdio.h>
#include <pthread.h>
#include <errno.h>

void* affCompteur();

int compteur=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

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
	
	for(i=0; i<n; i++) {
		if(pthread_create(&tid[i], NULL, affCompteur, NULL))
			perror("pthread_create");
	}
	
	for(i=0; i<n; i++) {
		if(pthread_join(tid[i], NULL))
			perror("pthread_join");
	}
	
	printf("Le compteur vaut %d\n", compteur);
	
	pthread_mutex_destroy(&mutex);
	
	return 0;
}

void* affCompteur() {
	pthread_mutex_lock(&mutex);
	compteur++;
	pthread_mutex_unlock(&mutex);
	//printf("%d\n", compteur);
}