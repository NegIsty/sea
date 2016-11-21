#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void search(char *chaine);

int main() {
	char commande[200];
	char *ligne, *token, *fin;
	
	while(1) {
		printf("%% ");
		
		if(fgets(commande, sizeof(commande), stdin)==NULL) {
			printf("\n");
			return 0;
		} else {
			search(commande);
			ligne=strdup(commande);
			fin=ligne;
			
			while(token=strsep(&fin, " ")) {
				printf("%s\n", token);
			}
			
			free(ligne);
		}
	}
	
	return 0;
}

/*Elimine le saut de ligne du fgets
Source : https://openclassrooms.com/courses/realiser-des-saisies-securisees-grace-a-fgets*/
static void search(char *chaine) {
    char *p=strchr(chaine, '\n');

    if (p) {
        *p = 0;
    }
}
