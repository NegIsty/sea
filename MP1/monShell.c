#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAXCOMMANDE 4096

char path[4096];

void afficheInvite();
void saisieCommande(char*);
void corrigeCommande(char*);
void traitement(char*);

int main() {
	char commande[4096];
	
	while(1) {
		afficheInvite();
		saisieCommande(commande);
		traitement(commande);
	}
	
	return 0;
}

/*Affiche l'invite de commande*/
void afficheInvite() {
	getcwd(path, sizeof(path));
	printf("%s %% ", path);
}

/*Gère la saisie de la commande utilisateur*/
void saisieCommande(char* commande) {
	/*Quitte le programme si ctrl+D*/
	if(fgets(commande, MAXCOMMANDE, stdin)==NULL) {
		printf("\n");
		exit(0);
	}
	
	corrigeCommande(commande);
}

/*Corrige la commande utilisateur*/
void corrigeCommande(char* commande) {
	int i=0, j=0;
	
	/*Supprime les blancs avant la commande*/
	while(commande[i]=='\t' || commande[i]==' ') {
		j=i;
		
		while(commande[j]!='\0') {
			commande[j]=commande[j+1];
			j++;
		}
	}
	
	i++;
	
	/*Supprime les répétitions de blancs au milieu de la commande*/
	while(commande[i]!='\n') {
		if(commande[i]=='\t' || commande[i]==' ') {
			commande[i]=' ';
			
			while(commande[i+1]=='\t' || commande[i+1]==' ') {
				j=i+1;
				
				while(commande[j]!='\0') {
					commande[j]=commande[j+1];
					j++;
				}
			}
		}
		
		i++;
	}
	
	/*Supprime le saut de ligne et l'éventuel espace final*/
	if(commande[i-1]==' ')
		commande[i-1]=commande[i+1];
	else
		commande[i]=commande[i+1];
}

/*Gère le traitement de la commande saisie*/
void traitement(char* commande) {
	char *ligne, *fin, *tmp;
	char *token[256];
	int i=0;
	
	ligne=strdup(commande);
	fin=ligne;
	
	/*Division en tokens de la commande*/
	while(token[i++]=strsep(&fin, " "));
	
	/*Commande saisie : exit*/
	if(!strcmp(token[0], "exit")) {
		exit(0);
	}
	
	/*Commande saisie : cd*/
	else if(!strcmp(token[0], "cd")) {
		tmp=strcat(strcat(path, "/"), token[1]);
		chdir(tmp);
	}
	
	/*Autre commande*/
	else {
		i=0;
		
		while(token[i])
			printf("%s\n", token[i++]);
	}
	
	free(ligne);
}