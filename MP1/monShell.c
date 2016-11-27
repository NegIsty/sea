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
void executeCommande(char**);
void diviseCommande(char**, int, char**);

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
		executeCommande(token);
	}
	
	free(ligne);
}

/*Gère l'exécution des commandes autres que exit et cd*/
void executeCommande(char** token) {
	int fils, i;
	int stdOut=dup(1), stdIn=dup(0);
	char *commande[256];
	
	i=0;
	
	diviseCommande(token, i, commande);
	
	/*Crée un fils qui exécute la commande*/
	if((fils=fork())==0) {
		execvp(commande[0], commande);
		perror(commande[0]);
		exit(1);
	} else if(fils==-1)
		perror("fork");
	waitpid(fils, NULL, 0);
	
	dup2(stdIn, 0);
	dup2(stdOut, 1);
}

/*Sépare la commande à donner à execvp et établit les redirections*/
void diviseCommande(char** token, int i, char** commande) {
	int direction, fd0, fd1, j;
	
	direction=0;
	j=0;
	
	while(token[i]) {
		/*Gère les redirections*/
		if(!(strcmp(token[i], "<"))) {
			direction=1;
			
			if(token[++i]) {
				if((fd0=open(token[i], O_RDONLY))!=0) {
					dup2(fd0, 0);
					close(fd0);
				} else if(fd0<0) {
					perror(token[i]);
					exit(1);
				}
			}
		} else if(!(strcmp(token[i], ">"))) {
			direction=1;
			
			if(token[++i]) {
				if((fd1=open(token[i], O_CREAT | O_WRONLY, S_IRWXU | S_IRGRP | S_IROTH))!=0) {
					dup2(fd1, 1);
					close(fd1);
				} else if(fd1<0) {
					perror(token[i]);
					exit(1);
				}
			}
		} else if(!(strcmp(token[i], "2>"))) {
			direction=1;
			
			if(token[++i]) {
				if((fd1=open(token[i], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH))!=0) {
					dup2(fd1, 1);
					close(fd1);
				} else if(fd1<0) {
					perror(token[i]);
					exit(1);
				}
			}
		}
		
		if(direction==0)
			commande[j++]=token[i];
		
		i++;
	}
	
	commande[j]=NULL;
}