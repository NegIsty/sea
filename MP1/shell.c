#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void search(char *chaine);

int main() {
	char commande[200];
	char path[200];
	char *ligne, *token, *fin, *tmp;
	
	while(1) {
		getcwd(path, sizeof(path));
		printf("%s %% ", path);
		
		if(fgets(commande, sizeof(commande), stdin)==NULL) {
			printf("\n");
			return 0;
		} else {
			search(commande);
			ligne=strdup(commande);
			fin=ligne;
			
			token=strsep(&fin, " ");
			
			/*Commande saisie : exit*/
			if(!strcmp(token, "exit")) {
				return 0;
			} 
			
			/*Commande saisie : cd*/
			else if(!strcmp(token, "cd")) {
				token=strsep(&fin, " ");
				
				tmp=strcat(strcat(path, "/"), token);
				chdir(tmp);
			}
			
//			while(token=strsep(&fin, " ")) {
//				printf("%s\n", token);
//			}
			
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
