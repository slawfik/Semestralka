#include "k_a_definitions.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

char* denVtyzdni(int pa_den, int pa_mes, int pa_rok)  {           /* Zelerov algoritmus pre výpočet dňa v týždni*/
    int f ,g;
    int pom = 0;
    int mesiac[12] = {0,3,3,6,1,4,6,2,5,0,3,5};
    int ab = 0;
    int rk = pa_rok;
    pom += (pa_den%7);
    pom += mesiac[(pa_mes-1)];
    for(f = 9;f>0;f--)  {
        if(pa_rok> (f*1000))    {
            pa_rok = (pa_rok-(f*1000));
            ab = (f*10);
            break;
        }
    }
    for(g = 9; g>0; g--)    {
        if(pa_rok>(g*100))  {
            pa_rok = (pa_rok-(g*100));
            ab = (ab+g);
            break;
        }
    }
    pom +=((pa_rok+(pa_rok/4))%7);
    pom += (((ab%4)-3)*(-2));
    if(jePriestupny(rk) == 1 && pa_mes<3)    {
        --pom;
    }
    switch(pom%7) {
        case 0:
            return " je Nedeľa ";
        case 1:
            return " je Pondelok ";
        case 2:
             return " je Utorok ";
        case 3:
           return " je Streda ";
        case 4:
            return " je Štvrtok ";
        case 5:
            return " je Piatok ";
        case 6:
            return " je Sobota ";
    }
}

void data_init(DATA *data, const char* userName, const int socket) {
	data->socket = socket;
	data->userName[USER_LENGTH] = '\0';
	strncpy(data->userName, userName, USER_LENGTH);
}

int main(int argc, char* argv[],struct mesiac *mes)    {
    char *endd = "exit";
    int pomi ,pomd, pomr, pomm, nas ,i;
    char *nazovSubMes;
    char buf[200];
    buf[199] = '\n';

    int port = atoi(argv[1]);
    char *userName = argv[2];
    //Vytvorenie socketu
    int serverSocket;
    if((serverSocket = socket(AF_INET,SOCK_STREAM,0)) < 0)  {
        printf("Chyba socketu");  
        return 0;
    }
    //definovanie adresy servera
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;         //internetove sockety
    serverAddress.sin_addr.s_addr = INADDR_ANY; //prijimame spojenia z celeho internetu
    serverAddress.sin_port = htons(port);  
    
    //prepojenie adresy servera zo socketom
    if(bind(serverSocket,(struct sockaddr *)&serverAddress,sizeof(serverAddress))) {
        printf("Chyba - bind");
        return 0;
    }
    
    //počet rozpracovaných pripojení
    listen(serverSocket,7);
    
    struct sockaddr_in clientAdress;
    socklen_t clientAddresLength = sizeof(clientAdress);
    int clientSock = accept(serverSocket,(struct sockaddr *)&clientAdress,&clientAddresLength);
	
	DATA data;
	data_init(&data, userName, clientSock);
	
	alocuj(mes);
	alocujOrganizer(mes);
	
	char buffer[600];
	buffer[599] = '\n';

Navestie:
	write(data.socket,"\n\nVítajte stlačte číslo pre vykonanie programu. \n\n1. Isť na dátum\n2. Vyhľadať meno\n3. Pridať poznámku na deň\n4. Vymazať poznámku\n0.Ukončiť-(exit)",200);
	recv(data.socket,buffer,200,0);
	printf("%s\n",buffer);
	pomi = atoi(buffer);
	printf("%d\n",pomi);
	endd = strstr(buffer,"exit");
	if(pomi ==  1)	{
		write(data.socket,"\nZadajte dátum vo formáte deň mesiac a rok napr. 02.09.1991\n",200);
		recv(data.socket,buffer,200,0);
		char *poz = strchr(buffer,'.');
		pomd = atoi(poz-2);
		pomm = atoi(poz+1);
		pomr = atoi(poz+4);
		if(spravnyDen(pomd,pomm) == true) {
			strcat(buffer, denVtyzdni(pomd,pomm,pomr));
			nazovSubMes = dajNazovSuboru(pomm);
			nacitajMeniny(mes,nazovSubMes,pomr, pomm);
			strcat(buffer, " a meniny má -->  ");
			strcat(buffer, mes->dni[--pomd]);
			nazovSubMes = dajSuborsPozn(pomr);	//nazov suboru ale pre poznámky rok
			if(nazovSubMes != NULL)  {  
                		nacitajSuborsPzon(nazovSubMes,mes);
				char *poznamka = vypisPoznamk(mes,pomd,pomm);
				strcat(buffer,"\n Poznámky na tento deň:\n  ");
				strcat(buffer,poznamka);
				strcat(buffer,"\0");
				write(data.socket,buffer,600);
				//goto Navestie;
			} else { 
				strcat(buffer,"\nPoznámky pre tento rok nevedieme!\0");
				write(data.socket,buffer,600);
				//goto Navestie;
			}
		} else	{ 
			write(data.socket,"Zadali ste nesprávny deň alebo mensiac! ",600);
			//goto Navestie;
		}	
	} else if(pomi == 2)	{
		strcpy(buffer,"Zadajte meno pre vyhľadanie:  ");
		write(data.socket,buffer,100);
		recv(data.socket,buffer,15,0);
		for(i = 1;i<13;i++)     {
            		nazovSubMes = dajNazovSuboru(i);
            		nacitajMeniny(mes,nazovSubMes,2016,i);
			nas = vyhladajMeno(mes,buffer,i);
            		if(nas != 100)    {
				char str[30];
				sprintf(str, " má meniny %d.%d", nas,i);
				strcat(buffer,str);
				write(data.socket,buffer,100);
				break;
				//goto Navestie;
            		}
        	}
		if(nas == 100)	{
			strcpy(buffer,"Hľadané meno sa nenašlo, skontrolujte či ste ho napísali správne aj s diakritikou.");
			write(data.socket,buffer,100);
			//goto Navestie;
		}
	} else if(pomi == 3)	{
		write(data.socket,"\nZadajte dátum vo formáte deň mesiac a rok napr. 02.09.1991\n",200);
		recv(data.socket,buffer,200,0);
		char *poz = strchr(buffer,'.');
		pomd = atoi(poz-2);
		pomm = atoi(poz+1);
		pomr = atoi(poz+4);
		if(spravnyDen(pomd,pomm) == true) {
			nazovSubMes = dajSuborsPozn(pomr);
			if(nazovSubMes != NULL)  { 
				nacitajSuborsPzon(nazovSubMes,mes);
				write(data.socket,"Teraz možete napísať poznámku:\n",200);
				recv(data.socket,buffer,600,0);
				zapisPoznDoSub(nazovSubMes,mes,pomm,pomd,buffer);
				write(data.socket,"Vaša poznámka bola zaznamenaná.",200);
				//goto Navestie;
			} else { 
				write(data.socket,"Poznámky pre tento rok nevedieme!",200);
				//goto Navestie;
			}
		} else { 
			write(data.socket,"Zadali ste nesprávny deň alebo mesiac! ",200);
			//goto Navestie;
		}
	} else if(pomi == 4) {
		write(data.socket,"Vymazanie poznámky zadajte dátum\n",200);
		recv(data.socket,buffer,200,0);
		char *poz = strchr(buffer,'.');
		pomd = atoi(poz-2);
		pomm = atoi(poz+1);
		pomr = atoi(poz+4);
		if(spravnyDen(pomd,pomm) == true) {
			nazovSubMes = dajSuborsPozn(pomr);
			if(nazovSubMes != NULL)  { 
				nacitajSuborsPzon(nazovSubMes,mes);
				vypazPozn(mes,pomm,pomd);
				zapisPoznDoSub(nazovSubMes,mes,pomm,pomd,NULL);
				write(data.socket,"Poznámka bola vymazaná",200);
				//printf("niečo som tu \n");
				//goto Navestie;
			} else {
				write(data.socket,"Poznámky pre tento rok nevedieme!",200);
				//goto Navestie;
			}
		} else {
			write(data.socket,"Zadali ste nesprávny deň alebo mesiac! ",200);
			//goto Navestie;
		}
	}
	if(pomi != 0)	{
		goto Navestie;
	}
    uvolniPam(mes);
    for(i = 0;i< 31;i++)    {
        free(mes->Januar.pozn[i]);
        free(mes->Februar.pozn[i]);
        free(mes->Marec.pozn[i]);
        free(mes->April.pozn[i]);
        free(mes->Maj.pozn[i]);
        free(mes->Jun.pozn[i]);
        free(mes->Jul.pozn[i]);
        free(mes->August.pozn[i]);
        free(mes->September.pozn[i]);
        free(mes->Oktober.pozn[i]);
        free(mes->November.pozn[i]);
        free(mes->December.pozn[i]);
    }
    close(data.socket);
    close(clientSock);
    return 1;
}
