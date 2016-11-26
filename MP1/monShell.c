#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define MAXCOMMANDE 4096

void afficheInvite();
void saisieCommande(char*);
void corrigeCommande(char*);

int main() {
	char commande[4096];
	
	while(1) {
		afficheInvite();
		saisieCommande(commande);
		
		printf("%s\n", commande);
	}
	
	return 0;
}

/*Affiche l'invite de commande*/
void afficheInvite() {
	printf("%% ");
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
	
	/*Supprime le saut de ligne final*/
	commande[i]=commande[i+1];
}