#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAXBUFFER 100

void qerror(char *mess) { perror(mess); exit(1); }


int main(int argc, char* argv[]) {

    int sockfd, size_addr;
     struct sockaddr_in remote_addr;
     struct sockaddr_in local_addr;
     char ip_s[INET_ADDRSTRLEN]; //IP em dotted decimal notation
     int bytes,r,v=1;
     char buf[MAXBUFFER];


char enviar[MAXBUFFER];
char receber[MAXBUFFER];


 //verifica argumentos
    if (    (argc != 4) && (atoi(argv[1]) && atoi(argv[3]) > 2000)   )  {
        printf("Erro , argumentos = %s <portOrador> <ipBroadcast> <portOuvinte>\n",argv[0]);
        exit(1);
    }

 //cria socket UDP
 sockfd = socket(AF_INET,SOCK_DGRAM,0);
 //exit_on_error(sockfd,"Error:socket()");
 //permite broadcast
 r=setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &v, sizeof(v));
 //exit_on_error(r,"Error:setsocketopt()");


 //associa socket localmente (IP+porto)
 bzero((char *)&local_addr, sizeof(local_addr));
 local_addr.sin_family = AF_INET;
 local_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP Local
 local_addr.sin_port = htons(atoi(argv[1])); //porto

 r=bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr));
 //exit_on_error(r,"Error:bind()");


    while(1){


        //ORADOR->INTERLOCUTOR

      printf("Aguarda UDP no porto %d\n",atoi(argv[1]));
          size_addr=sizeof(remote_addr);
          bytes = recvfrom(sockfd, receber, MAXBUFFER-1, 0, (struct sockaddr *)&remote_addr, &size_addr);
          // escreve origem e dados recebidos
          inet_ntop(AF_INET,&(remote_addr.sin_addr),ip_s, sizeof ip_s);
          printf("Recebido de: %s\n",ip_s);
          printf("Porto: %d\n",ntohs(remote_addr.sin_port));
          printf("Bytes lidos: %d\n", bytes);
          buf[bytes]=0; //Null terminated string
          printf("Dados: %s\n", buf);


        //printf("\nEscrever algo para enviar:");
        //fgets(enviar, sizeof(enviar), stdin);

            //INTERLOCUTOR->ORADOR


        //envia dados=argv[3] por datagrama UDP
        bytes = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
        printf("Bytes enviados: %d\n", bytes);
        //close(sockfd);

        //GUARDA DADOS ORADOR PARA MANDAR MAIS TARDE

        int orador_ip= remote_addr.sin_addr.s_addr;
        int orador_porto = remote_addr.sin_port;


//INTERLOCUTOR->OUVINTE


//prepara endereço IP+porto do destino
 bzero((char *)&remote_addr, sizeof(remote_addr));
 remote_addr.sin_family = AF_INET;
 remote_addr.sin_addr.s_addr = inet_addr(argv[2]); //IP Destino
 remote_addr.sin_port = htons(atoi(argv[3])); //Porto Destino


 //envia dados=argv[3] por datagrama UDP
        bytes = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
        printf("Bytes enviados: %d\n", bytes);
        //close(sockfd);


//OUVINTE->INTERLOCUTOR

              printf("Aguarda UDP \n");
          size_addr=sizeof(remote_addr);
          bytes = recvfrom(sockfd, receber, MAXBUFFER-1, 0, (struct sockaddr *)&remote_addr, &size_addr);
          // escreve origem e dados recebidos
          inet_ntop(AF_INET,&(remote_addr.sin_addr),ip_s, sizeof ip_s);
          printf("Recebido de: %s\n",ip_s);
          printf("Porto: %d\n",ntohs(remote_addr.sin_port));
          printf("Bytes lidos: %d\n", bytes);
          buf[bytes]=0; //Null terminated string
          printf("Dados: %s\n", buf);

//INTERLOCUTOR->ORADOR

//prepara endereço IP+porto do destino
 bzero((char *)&remote_addr, sizeof(remote_addr));
 remote_addr.sin_family = AF_INET;
 remote_addr.sin_addr.s_addr = orador_ip= remote_addr.sin_addr.s_addr;
 remote_addr.sin_port = orador_porto;


 //envia dados=argv[3] por datagrama UDP
        bytes = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
        printf("Bytes enviados: %d\n", bytes);
        //close(sockfd);


bzero((char *)&local_addr, sizeof(local_addr));



    }
}

