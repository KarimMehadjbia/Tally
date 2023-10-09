/*  INF3173 - TP2
 *  Session : automne 2021
 *  Tous les groupes
 *  
 *  IDENTIFICATION.
 *
 *      Nom : MEHADJBIA DIA
 *      Prénom : KARIM
 *      Code permanent : MEHK92050004
 *      Groupe : 030
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


#include <sys/socket.h>
#include <errno.h>



#define READ_END 0
#define WRITE_END 1



int exec(char **tab,int* embois, int nr_commande, FILE *count){
    
    
    
    int pid = fork();
    
  
    
    if (pid == 0){ //Cas fils
        
        
        
        if (nr_commande == -1){ //Une seule commande donc on ne fait rien
            
        }
        else if (nr_commande == 0){//Cas pour la premiere commande
            
            dup2(embois[WRITE_END], 1);
            close(embois[READ_END]);
            close(embois[WRITE_END]);
            
        }
        else {//Autres cas
            
            int test = dup2(embois[READ_END], 0);
            close(embois[WRITE_END]);            
            close(embois[READ_END]);
            
            
        }
        execve(tab[0],tab,NULL);
        if (errno = ENOENT)//La commande n'existe pas
            exit(127);
        
        

        exit(1);
    }
    else if (pid > 0){ //Cas pére
        
        int status;
        waitpid(pid,&status, 0);//On attend que le fils termine
        
        if (nr_commande == 0){//Premiere commande
            
            //Creation et remplissage du fichier count
            
            char buf[5000];
            int octets_tube = read(embois[READ_END],buf,sizeof(buf));
            int octets_wrote = write(embois[WRITE_END],buf,octets_tube);
            count = fopen("count","w+");
            fprintf(count,"%d : %d\n",nr_commande+1,octets_tube);
            
        }
        else{
            if (WIFEXITED(status)){ //Cas succès
                exit(WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status)){ //La derniere commande s'est terminé à cause d'un signal
                
                exit(WTERMSIG(status)+128);
            } 
        }
        close(embois[WRITE_END]);
        return 0;
    }
}

void aux1 (char *string, int *embois, int nr_commande, FILE *count){
    
    char *commande[] = {"A","B","C","D","E","F","G","H", NULL}; //Tableau avec maximum 7 options par commande
    int i=0;
    char *tmp2 = strtok_r(string," ",&string);
    //On remplie le tableau pour chaque commande
    while (tmp2 != NULL ){
            if (i==0){
                char resultat[100] = "/bin/";
                strcat(resultat,tmp2);
                commande[0] = resultat;
            }
            else{
                commande[i] = tmp2;
            }
            tmp2 = strtok_r(NULL," ",&string);
            i++;
        }
        commande[i] = NULL;
        //On execute la commande
        exec(commande,embois,nr_commande,count);
}


void aux2 (char *string,char *ptr){
    
    
    FILE *count;
    int embois[2];
    int socket = socketpair(AF_UNIX,SOCK_STREAM,0,embois);
    
    int nr_commande = 0;
    
    //On vérifie s'il y a juste une commande
    char *retour = strchr(string,':');
    if (retour == NULL)
        nr_commande = -1;
       
    //Sinon on passe les commande a aux2 une à une
    char *tmp = strtok_r(string,":",&ptr);
    while (tmp != NULL){
        char *copie = tmp;
        aux1(copie,embois,nr_commande,count);
        
        tmp = strtok_r(NULL,":",&ptr);
        nr_commande++;
    }
    
    fclose(count);
    
}

int main(int argc, char **argv) {
    
    
    //Traitement des arguments
    
    char ligne[256]; 
    char *ptr = ligne;
    
    //On met toutes le commandes dans "ligne"
    for (int i=1;i<argc;i++){
        
        strcat(ligne,argv[i]);
        if (strcmp(argv[i],":") != 0){
            strcat(ligne," ");
        }
    }
    
    aux2(ligne,ptr);
    
    
    return 0;
}
