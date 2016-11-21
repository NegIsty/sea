#include <stdio.h>
#include <unistd.h>

int main() {
	char commande[200];
	
	while(1) {
		printf("%% ");
		
		if(fgets(commande, sizeof(commande), stdin)==NULL) {
			printf("\n");
			return 0;
		} else {
			printf("%s", commande);
		}
	}
	
	return 0;
}