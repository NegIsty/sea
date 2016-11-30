#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define MAXCOMMANDE 4096

char path[4096];

void afficheInvite();
void saisieCommande(char*);
void corrigeCommande(char*);
void traitement(char*);
void executeCommande(char**);
int diviseCommande(char**, int*, char**);
int executeAvecPipe(int, int, char**);

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
	int fils, in;
	int info[2];
	//info[0] : indice de parcours de token
	//info[1] : 0 si pas de pipe, 1 si pipe
	int fd[2];
	int stdOut=dup(1), stdIn=dup(0);
	char *commande[256];
	
	info[0]=0;
	in=0;
	
	do {
		info[1]=0;
		
		/*N'exécute pas la commande si une redirection est incorrecte*/
		if(!diviseCommande(token, info, commande)){
			break;
		}
		
		/*Exécution des commandes jusqu'à l'avant dernière*/
		else if(info[1]) {
			pipe(fd);
			if(!executeAvecPipe(in, fd[1], commande))
				break;
			close(fd[1]);
			in=fd[0];
		}
		
		/*Exécution de la dernière commande*/
		else {
			if(in!=0)
				dup2(in, 0);
			
			/*Crée un fils qui exécute la commande*/
			if((fils=fork())==0) {
				execvp(commande[0], commande);
				perror(commande[0]);
				break;
			} else if(fils==-1) {
				perror("fork");
				break;
			}
			waitpid(fils, NULL, 0);
		}
	} while(token[info[0]-1]);
	
	/*Rétablit les entrées et sorties standard*/
	dup2(stdIn, 0);
	dup2(stdOut, 1);
}

/*Sépare la commande à donner à execvp et établit les redirections fichier*/
int diviseCommande(char** token, int *info, char** commande) {
	int direction, fd0, fd1, i, j;
	
	i=info[0];
	direction=0;
	j=0;
	
	while(token[i] && strcmp(token[i], "|")) {
		/*Gère les redirections*/
		if(!(strcmp(token[i], "<"))) {
			direction=1;
			
			if(token[++i] && strcmp(token[i], "|")) {
				if((fd0=open(token[i], O_RDONLY))>0) {
					dup2(fd0, 0);
					close(fd0);
				} else if(fd0<0) {
					perror(token[i]);
					return 0;
				}
			} else {
				printf("< doit être suivie du nom du fichier source\n");
				return 0;
			}
		} else if(!(strcmp(token[i], ">"))) {
			direction=2;
			
			if(token[++i] && strcmp(token[i], "|")) {
				if((fd1=open(token[i], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH))>0) {
					dup2(fd1, 1);
					close(fd1);
				} else if(fd1<0) {
					perror(token[i]);
					return 0;
				}
			} else {
				printf("> doit être suivie du nom du fichier cible\n");
				return 0;
			}
		} else if(!(strcmp(token[i], "2>"))) {
			direction=2;
			
			if(token[++i] && strcmp(token[i], "|")) {
				if((fd1=open(token[i], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH))>0) {
					dup2(fd1, 1);
					close(fd1);
				} else if(fd1<0) {
					perror(token[i]);
					return 0;
				}
			} else {
				printf("2> doit être suivie du nom du fichier cible\n");
				return 0;
			}
		}
		
		/*Complète la commande à donner à execvp*/
		if(direction==0)
			commande[j++]=token[i];
		
		i++;
		
		/*Gère les pipes*/
		if(token[i] && direction!=2 && !strcmp(token[i], "|"))
			info[1]=1;
	}
	
	commande[j]=NULL;
	
	info[0]=++i;
}

/*Gère l'exécution des commandes suivies d'un pipe
Inspiré de stackoverflow : http://stackoverflow.com/questions/8082932/connecting-n-commands-with-pipes-in-a-shell*/
int executeAvecPipe(int in, int out, char** commande) {
	int fils;
	
	if((fils=fork())==0) {
		if(in!=0) {
			dup2(in, 0);
			close(in);
		}
		
		if(out!=1) {
			dup2(out, 1);
			close(out);
		}
		
		execvp(commande[0], commande);
		perror(commande[0]);
		return 0;
	} else if(fils==-1) {
		perror("fork");
		return 0;
	}
}