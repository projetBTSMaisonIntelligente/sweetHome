#include "CTeleinfo.h"

/* ======================================================================
Fonction : CTeleinfo
Objectif : constructeur
Entrée   : -
Sortie   : -
Infos +  : -
====================================================================== */

CTeleinfo::CTeleinfo(void)
{
    memset(teleinfo,0,sizeof(teleinfo));
    Particle.variable("teleinfo", teleinfo, STRING);
    CDelestage oDelestage;
}

/* ======================================================================
Fonction : lecture
Objectif : Lecture des trames reçues
Entrée   : -
Sortie   : -
Infos +  : -
====================================================================== */

void CTeleinfo::lecture(void)
{
    Serial1.begin(V_TELEINFO);
    if(Serial1.available())
    {
        caractere = Serial1.read() & BIT_8;
        if(caractere != NULL)
        {
            if(caractere != FIN_TRAME)
                RGB.color(0,128,0);
            else
                RGB.color(255,0,0);

            decodageTrame(caractere);
        }
    }
}

/* ======================================================================
Fonction : decodage
Objectif : Décode les trames reçues
Entrée   : char caractere : caractère reçu sur le Serial1
Sortie   : -
Infos +  : -
====================================================================== */

void CTeleinfo::decodageTrame(char caractere)
{
    switch(etape)
    {
        case 1: // Début de la trame
            if(caractere == DEBUT_TRAME)
            {
                etape++;
                memset(teleinfo,0,sizeof(teleinfo));
            }
        break;
        case 2: // Début du groupe de données
            if(caractere == DEBUT_GROUPE)
            {
                etape++;
                index = 0;
                cheksum = 0;
            }
            if(caractere == FIN_TRAME)
                etape = 1;
        break;
        case 3: // Récupération du nom du groupe
            cheksum += caractere;
            if(caractere == DEBUT_DONNEES)
            {
                etape++;
                groupe_nom[index] = 0;
                index = 0;
            }
            else
            {
                groupe_nom[index] = caractere;
                index++;
            }
        break;
        case 4: // Récupération des données
            cheksum += caractere;
            if(caractere == DEBUT_DONNEES)
            {
                etape++;
                groupe_donnees[index] = 0;
                index = 0;
            }
            else
            {
                groupe_donnees[index] = caractere;
                index++;
            }
        break;
        case 5: // Récupération et vérification du cheksum
            cheksum = ((cheksum + DEBUT_DONNEES) & FIN_DONNEES) + DEBUT_DONNEES;
            if(cheksum = caractere)
                affecterAttributs(groupe_nom, groupe_donnees);
            etape++;
        break;
        case 6: // Fin du groupe
            if(caractere == FIN_GROUPE)
            {
                etape = 2;
                this->miseAuFormatJson();
                oDelestage.controle(iSousc, iInst);
            }
        break;
    }
}

void CTeleinfo::affecterAttributs(char *groupe_nom, char *groupe_donnees)
{
    if(!strcmp(groupe_nom, "PAPP")) pApp = atoi(groupe_donnees);
    else if(!strcmp(groupe_nom, "IINST")) iInst = atoi(groupe_donnees);
    else if(!strcmp(groupe_nom, "HCHC")) indexHC = atoi(groupe_donnees);
    else if(!strcmp(groupe_nom, "HCHP")) indexHP = atoi(groupe_donnees);
    else if(!strcmp(groupe_nom, "ISOUSC")) iSousc  = atoi(groupe_donnees);
    else if(!strcmp(groupe_nom, "IMAX")) iMax = atoi(groupe_donnees);
    else if(!strcmp(groupe_nom, "ADCO")) nCompteur = groupe_donnees;
    pInst = iInst * 230;
    pVA = pInst * iInst;
}

void CTeleinfo::miseAuFormatJson(void)
{
    sprintf(teleinfo, "{\"papp\":%u,\"iinst\":%u,\"isousc\":%u,\"indexHP\":%u,\"indexHC\":%u,\"imax\":%u,\"ADCO\":%u,\"pinst\":%u,\"pVA\":%u}",pApp,iInst,iSousc,indexHP,indexHC,iMax,nCompteur,pInst,pVA);
}
