#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void search(char *chaine);

int main() {
	char commande[200];
	char path[200];
	char *ligne, *fin, *tmp;
	char *token[20];
	int fils, status, i;
	
	while(1) {
		/*Affiche le répertoire courant*/
		getcwd(path, sizeof(path));
		printf("%s %% ", path);
		
		/*Quitte le programme si ctrl+D*/
		if(fgets(commande, sizeof(commande), stdin)==NULL) {
			printf("\n");
			return 0;
		} 
		
		/*Gère les commandes saisies par l'utilisateur*/
		else {
			search(commande);
			ligne=strdup(commande);
			fin=ligne;
			
			/*Récupère le premier token de la commande*/
			token[0]=strsep(&fin, " ");
			
			/*Commande saisie : exit*/
			if(!strcmp(token[0], "exit")) {
				return 0;
			} 
			
			/*Commande saisie : cd*/
			else if(!strcmp(token[0], "cd")) {
				token[1]=strsep(&fin, " ");
				
				tmp=strcat(strcat(path, "/"), token[1]);
				chdir(tmp);
			}
			
			/*Autre commande*/
			else {
				i=1;
				
				/*Sépare le reste de la commande en tokens*/
				while(tmp=strsep(&fin, " ")) {
					token[i++]=tmp;
				}
				token[i]=NULL;
				
				/*Crée un fils qui exécute la commande*/
				if((fils=fork())==0) {
					execvp(token[0], token);	
				}
				waitpid(fils, &status, WUNTRACED | WCONTINUED);
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
