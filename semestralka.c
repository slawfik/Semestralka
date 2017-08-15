#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

FILE *fr;

struct mesiac   {
    int dlzka;
    int pocetPozn;
    struct organizer    {
        char* pozn[31];
    } Januar, Februar, Marec, April, Maj, Jun, Jul, August, September, Oktober, November, December;
    char* dni[];
};

int jePriestupny(int pa_rok) {
    if((pa_rok%4==0) || (pa_rok%400==0))   {
        return 29;
    } else {
        return 28;
    }
}


int dnivMesiaci(int pa_mesiac) {
    switch(pa_mesiac)    {
        case 1 :     
            return 31;
        case 2 :
            return 29;
        case 3 :
            return 31;
        case 4 :
            return 30;
        case 5 :
            return 31;
        case 6 :
            return 30;
        case 7 :
            return 31;
        case 8 :
            return 31;
        case 9 :
            return 30;
        case 10 :
            return 31;
        case 11 :
            return 30;
        case 12 :
           return 31;
    } 
}

int alocuj(struct mesiac *mes)   {
        int i;
            /* dynamicke vytvorenie druhej dimenzie pola */
    for (i = 0; i < 31; i++) {
        if ((mes->dni[i] = (char *) malloc(500)) == NULL) {
            printf("Malo pamati");
            return false;
        }
    }
        return true;
}

int nacitajMeniny(struct mesiac *mes, char* pa_subor, int pa_rok, int pa_mes)  {
    int i;
    /*Otvorenie vstupného kanálu súboru*/
    if ((fr = fopen(pa_subor, "r")) == NULL) {
        printf("\nSubor %s sa nepodarilo otvorit!\n", pa_subor);
        return 0;
        }
        
    /*test na priestupný rok*/ 
    if(pa_mes == 2)     {
        mes->dlzka = jePriestupny(pa_rok);
    }   else {
        mes->dlzka = dnivMesiaci(pa_mes);
    }
 
    /* vlozenie obsahu suboru do dvojrozmerneho pola po riadkoch */
    i = 0;
    while (fgets((mes->dni[i]), 499, fr) != NULL && i < mes->dlzka)
        i++;
    fclose(fr);
    return 1;
}

_Bool uvolniPam(struct mesiac *mes) {
    int i;
    for(i = 0;i < 31;i++) {
        free(mes->dni[i]);
    }
    return true;
}

_Bool alocujOrganizer(struct mesiac *mes)   {
    int b;    
    for(b = 0;b< 31;b++)    {
        mes->Januar.pozn[b] = (char *)malloc(600);
        mes->Februar.pozn[b] = (char *)malloc(600);
        mes->Marec.pozn[b] =(char *) malloc(600);
        mes->April.pozn[b] =(char *) malloc(600);
        mes->Maj.pozn[b] =(char *) malloc(600);
        mes->Jun.pozn[b] =(char *) malloc(600);
        mes->Jul.pozn[b] =(char *) malloc(600);
        mes->August.pozn[b] =(char *) malloc(600);
        mes->September.pozn[b] =(char *) malloc(600);
        mes->Oktober.pozn[b] =(char *) malloc(600);
        mes->November.pozn[b] =(char *) malloc(600);
        mes->December.pozn[b] =(char *) malloc(600);
    }
    mes->pocetPozn = 0;
}

int vyhladajMeno(struct mesiac *mes, char* meno, int mesiac)    {
    int b;
    for(b = 0; b< mes->dlzka; b++)      {
                if(strstr(mes->dni[b],meno)) {
                    ++b;
                    return b;
                }
    }
    return 100;
}

char vypisMenin(int g,struct mesiac *mes)    {
    printf(" %s  ", mes->dni[g]);
}


/*char* dajNazovSuboru(int pa_mesiac)   {
    switch(pa_mesiac)    {
        case 1 :     
            return "januar.txt";
        case 2 :
            return "februar.txt";
        case 3 :
            return "marec.txt";
        case 4 :
            return "april.txt";
        case 5 :
            return "maj.txt";
        case 6 :
            return "jun.txt";
        case 7 :
            return "jul.txt";
        case 8 :
            return "august.txt";
        case 9 :
            return "september.txt";
        case 10 :
            return "oktober.txt";
        case 11 :
            return "november.txt";
        case 12 :
           return "december.txt";
    }
}*/

_Bool spravnyDen(int pa_den, int pa_mes)    {
    if(pa_mes > 0 && pa_mes <13)    {
        if(pa_den <= dnivMesiaci(pa_mes))   {
            return true;
        }
    }
    return false;
}

char* vypisPoznamk(struct mesiac *mes, int pa_den, int pa_mes)  {
    switch(pa_mes)  {
        case 1:     return mes->Januar.pozn[pa_den];
        case 2:     return mes->Februar.pozn[pa_den];
        case 3:     return mes->Marec.pozn[pa_den];
        case 4:     return mes->April.pozn[pa_den];
        case 5:     return mes->Maj.pozn[pa_den];
        case 6:     return mes->Jun.pozn[pa_den];
        case 7:     return mes->Jul.pozn[pa_den];
        case 8:     return mes->August.pozn[pa_den];
        case 9:     return mes->September.pozn[pa_den];
        case 10:     return mes->Oktober.pozn[pa_den];
        case 11:     return mes->November.pozn[pa_den];
        case 12:     return mes->December.pozn[pa_den];
    }
}
/*
char* dajSuborsPozn(int pa_rok)   {
    switch(pa_rok)  {
        case 2017:  return "pozn17.txt";
        case 2018:  return "pozn18.txt";
        case 2019:  return "pozn19.txt";
        default:    return NULL;
    }
}*/

void vypazPozn(struct mesiac *mes,int pa_mes,int pa_den)	{
	--pa_den;
	   switch(pa_mes)    {
        case 1:
            strcpy(mes->Januar.pozn[pa_den],"");
            break;
        case 2:
            strcpy(mes->Februar.pozn[pa_den],"");
            break;
        case 3:
            strcpy(mes->Marec.pozn[pa_den],"");
            break;
        case 4:
            strcpy(mes->April.pozn[pa_den],"");
            break;
        case 5:
            strcpy(mes->Maj.pozn[pa_den],"");
            break;
        case 6:
            strcpy(mes->Jun.pozn[pa_den],"");
            break;
        case 7:
            strcpy(mes->Jul.pozn[pa_den],"");
            break;
        case 8:
            strcpy(mes->August.pozn[pa_den],"");
            break;
        case 9:
            strcpy(mes->September.pozn[pa_den],"");
            break;
        case 10:
            strcpy(mes->Oktober.pozn[pa_den],"");
            break;
        case 11:
            strcpy(mes->November.pozn[pa_den],"");
            break;
        case 12:
            strcpy(mes->December.pozn[pa_den],"");
            break;
	}
}

_Bool nacitajSuborsPzon(char* pa_file, struct mesiac *mes)  {
    int mesc, den, i, pom;
    char* pos;
    char buf[500];
    fr = fopen(pa_file,"r");
    fscanf(fr,"%df",&pom);
    mes->pocetPozn = pom;
    for(i = 0;i<pom;i++)    {
    fscanf(fr,"%df",&den);
    fgets(buf,7,fr);
    pos = strchr(buf,';');
    mesc = atoi(pos+1);
    --den;
    switch(mesc)    {
        case 1:
            fgets(mes->Januar.pozn[den],500,fr);
            break;
        case 2:
            fgets(mes->Februar.pozn[den],500,fr);
            break;
        case 3:
            fgets(mes->Marec.pozn[den],500,fr);
            break;
        case 4:
            fgets(mes->April.pozn[den],500,fr);
            break;
        case 5:
            fgets(mes->Maj.pozn[den],500,fr);
            break;
        case 6:
            fgets(mes->Jun.pozn[den],500,fr);
            break;
        case 7:
            fgets(mes->Jul.pozn[den],500,fr);
            break;
        case 8:
            fgets(mes->August.pozn[den],500,fr);
            break;
        case 9:
            fgets(mes->September.pozn[den],500,fr);
            break;
        case 10:
            fgets(mes->Oktober.pozn[den],500,fr);
            break;
        case 11:
            fgets(mes->November.pozn[den],500,fr);
            break;
        case 12:
            fgets(mes->December.pozn[den],500,fr);
            break;
        default:
            printf("Neznámi mesiac! Skontrolujete obsah súboru %s",pa_file);
            fclose(fr);
            return false;
      }
    }
    fclose(fr);
    return true;
}

_Bool zapisPoznDoSub(char* pa_file, struct mesiac *mes,int pa_mes, int pa_den,char* buff) {
    int me = 0,dn = 0,a = 0;
    fr = fopen(pa_file,"w+");
    if(buff != NULL)    {
        ++mes->pocetPozn;
    }	else {
	--mes->pocetPozn;
    }
    fprintf(fr,"%d\n",mes->pocetPozn);
    for(me = 1;me<13;me++)  {
        a = dnivMesiaci(me);
        for(dn = 0; dn < a;dn++)    {
            char* pozn = vypisPoznamk(mes,dn,me);
            if(strlen(pozn) != 0)   {
                fprintf(fr,"%d;%d\n%s",dn+1,me,pozn);
		continue;
            }
        }
    }
    if(buff != NULL)    {
        fprintf(fr,"%d;%d\n%s\n",pa_den,pa_mes,buff);
    }    
    fclose(fr);
    return true;
}

