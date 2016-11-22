#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

static void search(char *chaine);

int main() {
	char commande[1000];
	char path[1000];
	char *ligne, *fin, *tmp;
	char *token[20];
	char *nomFichier;
	int fils, status, i, direction, fichier;
	int stdoutCopy=dup(1), stdinCopy=dup(0);
	
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
				direction=0;
				
				/*Sépare le reste de la commande en tokens*/
				while(tmp=strsep(&fin, " ")) {
					
					/*Test s'il y a une redirection*/
					if(!(strcmp(tmp, ">"))) {
						direction=1;
						break;
					} else if(!(strcmp(tmp, "<"))) {
						direction=2;
						break;
					} else if(!(strcmp(tmp, "2>"))) {
						direction=3;
						break;
					}
					
					token[i++]=tmp;
				}
				
				token[i]=NULL;
				
				/*Gestion de la redirection*/
				if(direction) {
					nomFichier=strsep(&fin, " ");
					
					switch(direction) {
						case 1:
							fichier=open(nomFichier, O_CREAT | O_WRONLY, S_IRWXU | S_IRGRP | S_IROTH);
							break;
						case 2:
							fichier=open(nomFichier, O_RDONLY);
							break;
						case 3:
							fichier=open(nomFichier, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH);
							break;
					}
					
					if(fichier<0) {
						printf("Erreur lors de l'ouverture du fichier\n");
					} else {
						switch(direction) {
							case 1:
								dup2(fichier, 1);
								break;
							case 2:
//								dup2(fichier, 0);
								token[i++]=nomFichier;
								token[i]=NULL;
								break;
							case 3:
								dup2(fichier, 1);
								break;
						}
						
						/*Crée un fils qui exécute la commande*/
						if((fils=fork())==0) {
							execvp(token[0], token);	
						}
						
						waitpid(fils, &status, WUNTRACED | WCONTINUED);
						
						dup2(stdoutCopy, 1);
//						dup2(stdinCopy, 0);
						
						close(fichier);
					}
				}
				
				else {
					/*Crée un fils qui exécute la commande*/
					if((fils=fork())==0) {
						execvp(token[0], token);	
					}
					
					waitpid(fils, &status, WUNTRACED | WCONTINUED);
				}
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
