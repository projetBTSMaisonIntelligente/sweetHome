#ifndef TELEINFO_h
#define TELEINFO_h
#include "application.h"
#include "configuration.h"
#include "CDelestage.h"
class CTeleinfo
{
public:
    CTeleinfo(void);
    void lecture(void);
    void decodageTrame(char caractere);
    void affecterAttributs(char groupe_nom[8], char groupe_donnees[10]);
    void miseAuFormatJson(void);
private:
    CDelestage oDelestage;
    // Réception et décodage
    char caractere;
    int etape = 1;
    char teleinfo[250];
    char groupe_nom[8];
    char groupe_donnees[10];
    char index = 0;
    char cheksum = 0;
    // Télé-informations
    uint pApp;
    uint iInst = 0;
    uint indexHC = 0;
    uint indexHP = 0;
    uint pInst = 0;
    uint iSousc = 0;
    uint iMax = 0;
    uint pVA = 0;
    char* nCompteur;
};
#endif
