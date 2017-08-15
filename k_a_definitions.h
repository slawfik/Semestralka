#ifndef K_DEFINITIONS_H
#define	K_DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define USER_LENGTH 10

typedef struct data {
    char userName[USER_LENGTH + 1];
    int socket;
} DATA;

struct mesiac   {
    int dlzka;
    struct organizer    {
        char* pozn[31];
    } Januar, Februar, Marec, April, Maj, Jun, Jul, August, September, Oktober, November, December;
    char* dni[];
};

_Bool zapisPoznDoSub(char* pa_file, struct mesiac *mes,int pa_mes, int pa_den,char* buff);
char* idNaDatum(int pa_pomd,int pa_pomm,int pa_pomr);
void data_init(DATA *data, const char* userName, const int socket);
_Bool nacitajSuborsPzon(char* pa_file, struct mesiac *mes);
char* dajSuborsPozn(int pa_rok);
char* vypisPoznamk(struct mesiac *mes, int pa_den, int pa_mes);
_Bool spravnyDen(int pa_den, int pa_mes);
char* dajNazovSuboru(int pa_mesiac);
char vypisMenin(int g,struct mesiac *mes);
int vyhladajMeno(struct mesiac *mes, char* meno, int mesiac);
_Bool alocujOrganizer(struct mesiac *mes);
_Bool uvolniPam(struct mesiac *mes);
int nacitajMeniny(struct mesiac *mes, char* pa_subor, int pa_rok, int pa_mes);
int alocuj(struct mesiac *mes);
int dnivMesiaci(int pa_mesiac);
int jePriestupny(int pa_rok);


#ifdef	__cplusplus
}
#endif

#endif	/* K_DEFINITIONS_H */

