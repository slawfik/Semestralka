#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "k_a_definitions.h"

//#include <fcntl.h>

char* _gets(char *s, int size) {
  int i, c;
  c = getchar();
  size--;
  if (size < 0) return NULL;
  for(i = 0; i < size; i++) {
    c = getchar();
    if (c == '\n') break; 
    s[i] = (char) c;
  }
  s[i] = 0;
  if (!i && c == EOF) return NULL;
  
  return s;
}

void data_init(DATA *data, const char* userName, const int socket) {
	data->socket = socket;
	data->userName[USER_LENGTH] = '\0';
	strncpy(data->userName, userName, USER_LENGTH);
}


int main(int argc, char *argv[]) {
    char *end = "exit";
    if (argc < 4) {
        printf("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port pouzivatel.");
        return 0;
    }
    
    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("Server neexistuje.");
        return 0;
    }
    int _port = atoi(argv[2]);
	if (_port <= 0) {
              //printf("Port musi byt cele cislo vacsie ako 0.");
                return 0;
        }
    char *userName = argv[3];
    
    //vytvorenie socketu <sys/socket.h>
    int Csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Csocket < 0) {
        printf("Chyba - socket.");   
        return 0;
    }
    
    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(_port);

    if (connect(Csocket,(struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
       return 0; 
    }

DATA data;
	data_init(&data, userName, Csocket);
	int pomi ,pomd, pomr, pomm;
	char vyhladat[15];	
	char buffer[600];
	//buffer[349] = '\n';
Navestie:
	//komunikácia výder voľby
	recv(data.socket,buffer,200,0);
	printf("%s\n",buffer);
	scanf("%s",buffer);
	int pom = atoi(buffer);
	write(data.socket,buffer,200);
	if(pom == 1)	{
			// načítanie dát pre voľbu
		recv(data.socket,buffer,200,0);
		printf("%s\n",buffer);
		scanf("%s",&buffer);
		write(data.socket,buffer,200);
			//return z metódy
		recv(data.socket,buffer,600,0);
		printf("%s",buffer);
	} else if(pom == 2)	{
		recv(data.socket,buffer,100,0);
		printf("%s\n",buffer);
		scanf("%s",&buffer);
		write(data.socket,buffer,15);
		recv(data.socket,buffer,100,0);
		printf("%s\n",buffer);
	} else if(pom == 3)	{
		recv(data.socket,buffer,200,0);
		printf("%s\n",buffer);
			//načítanie dát pre voľbu
		scanf("%s",&buffer);
		write(data.socket,buffer,200);
			//return z metódy
		recv(data.socket,buffer,200,0);
		printf("%s\n",buffer);
		char *poz = strchr(buffer,':');
		if(poz != NULL)	{
			_gets(buffer,500);
			write(data.socket,buffer,600);
			recv(data.socket,buffer,200,0);
			printf("%s\n",buffer);
		}
	} else if(pom == 4)	{
			// načítanie dát pre voľbu
		recv(data.socket,buffer,200,0);
		printf("%s\n",buffer);
		scanf("%s",buffer);
		write(data.socket,buffer,200);
			//return z metódy
		recv(data.socket,buffer,200,0);
		printf("%s",buffer);	
	}

	if(pom != 0) {
		goto Navestie;
	}
    //uzavretie socketu <unistd.h>
    close(data.socket);
	return 1;
}
