#include <stdio.h>
#include <unistd.h>

int main() {
	int i;
	
	if(fork()==0) {
		for(i=0; i<10000; i++) {
			write(1, "b", 1);
		}
	} else {
		for(i=0; i<10000; i++) {
			write(1, "a", 1);
		}
		wait();
		write(1, "\n", 1);
	}
    
    return 0;
}

/*
On constate que les "a" et "b" sont mélangés selon l'ordre dans lequel l'ordonnanceur a exécuté les write();
*/