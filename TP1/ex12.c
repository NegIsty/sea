#include <stdio.h>
#include <unistd.h>

int main() {
    int tab[10000];
	int i;
	
	srand(time(NULL));
	
	for(i=0; i<10000; i++) {
	    tab[i]=(rand()%1000)+1;
	}
	printf("Cases contenant 42 :\n");
	for(i=0; i<10000; i++) {
	    if(tab[i]==42)
	        printf("%d\n", i);
	}
    
    return 0;
}