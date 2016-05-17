

#include "FilPilote.h"

int SortiesFP[NB_FILS_PILOTES*2] = { FP1,FP2,FP3,FP4,FP5,FP6,FP7 };
char etatFP[NB_FILS_PILOTES] = "";

CFilPilote::CFilPilote(void)
{
      Particle.function("setfp", setfp);
      Particle.variable("etatfp", etatFP, STRING);
}

/* ======================================================================
méthode : setfp
rôle : selectionne le mode d'un des fils pilotes
Entrée   : ordre à envoyer (numéro fil pilote et ordre)
          C=Confort, A=Arrêt, E=Eco, H=Hors gel
          exemple: 1A => FP1 Arrêt
                   6C => FP6 confort
Sortie : 0 si l'ordre a été envoyé
         -1 si l'ordre n'est pas de la taille attendu
         1 si le numero de fil pilote est incorrect
         2 si l'ordre est incorrect
         3 si le radiateur concerné par l'ordre est actuellement delesté
====================================================================== */
int CFilPilote::setfp(String ordre)
{
uint8_t nFp; // numero de fp a commander
char cOrdre; // ordre à envoyer
int valeurRetour=0;

if (ordre.length() == 2)
    {
      String fp = "";
      fp += ordre[0];
      nFp = fp.toInt(); // stock le numero du FP
      //nFp = ordre[0]-'0';
      cOrdre = ordre[1]; //stock la commande à envoyer

      if ( (nFp < 1 || nFp > NB_FILS_PILOTES))
          valeurRetour=FP_INCORRECT;
          else if (cOrdre!=CONFORT && cOrdre!=ECO && cOrdre!=HORS_GEL &&
                    cOrdre!=ARRET && cOrdre!=DELESTAGE)
                    valeurRetour=ORDRE_INCORRECT;
                    else if (etatFP[nFp-1]==DELESTAGE)
                        valeurRetour=RADIATEUR_DELESTAGE;
                          else
                            {
                                valeurRetour=controleFp(nFp,cOrdre);
                            }
    }
    else
      valeurRetour=TAILLE_INCORRECT;

      #ifdef DEBUG
      Serial.println("setfp()...");
      switch(valeurRetour)
      {
        case -1: Serial.println("Erreur, taille de la commande incorrect"); break;
      }
      #endif

      return valeurRetour;
}


/* ======================================================================
méthode : controleFp
rôle : Envoie des ordres physiquement vers les fils pilotes
Entrée   : numero du fil pilote, l'ordre à envoyer
          C=Confort, A=Arrêt, E=Eco, H=Hors gel
          exemple: 1,A => FP1 Arrêt
                   6,C => FP6 confort
Sortie : 0 si l'ordre a été envoyé
         1 si le numero de fil pilote est incorrect
         2 si l'ordre est incorrect
         3 si le radiateur concerné par l'ordre est actuellement delesté
====================================================================== */
int CFilPilote::controleFp(uint8_t nFp,char cOrdre)
{
  // Commande à passer
  uint8_t fpcmd1, fpcmd2;
  uint8_t valeurRetour=0;

  if ( (nFp < 1 || nFp > NB_FILS_PILOTES))
      valeurRetour = FP_INCORRECT; // numero de fp incorrect
      else if(cOrdre!=CONFORT && cOrdre!=ECO && cOrdre!=HORS_GEL &&
              cOrdre!=ARRET && cOrdre!=DELESTAGE)
        valeurRetour = ORDRE_INCORRECT; // ordre incorrect
          else if(etatFP[nFp-1]==DELESTAGE)
              valeurRetour = RADIATEUR_DELESTAGE; // radiateur en mode delestage

     // on change l'état du Fil pilote (après test des parametres)
     else
     {
     etatFP[nFp-1]=cOrdre;

     switch (cOrdre)
     {
         // Confort => Commande 0/0
         case CONFORT: fpcmd1=LOW;  fpcmd2=LOW;  break;
         // Eco => Commande 1/1
         case ECO: fpcmd1=HIGH; fpcmd2=HIGH; break;
         // Hors gel => Commande 1/0
         case HORS_GEL: fpcmd1=HIGH; fpcmd2=LOW;  break;
         // Arrêt => Commande 0/1
         case ARRET: fpcmd1=LOW;  fpcmd2=HIGH; break;
         // Delestage => Commande 0/1
         case DELESTAGE: fpcmd1=LOW; fpcmd2=HIGH; break;
     }

     // Envoie des ordres physiquement
     mcp.digitalWrite(SortiesFP[2*(nFp-1)], fpcmd1); //pin 1
     mcp.digitalWrite(SortiesFP[2*(nFp-1)+1], fpcmd2);// pin 2

     }

     #ifdef DEBUG
     Serial.print("controleFp()...");
     switch(valeurRetour)
     {

       case 0: Serial.print("Ordre envoye avec succes -> zone: ");
               Serial.print(nFp); Serial.print(" ,Ordre: ");
               Serial.println(cOrdre); break;
       case 1: Serial.println("Erreur, numero de fil pilote incorrect."); break;
       case 2: Serial.println("Erreur, Ordre incorrect."); break;
       case 3: Serial.println("Erreur, radiateur en delestage"); break;
     }
     #endif


     return (valeurRetour);

}


/* ======================================================================
méthode : initialisationFp
rôle : Initialisation des pins du mcp en sortie et
initialisation des fils pilotes à hors gel
Entrée  : void
Sortie : 0 si l'initialisation a réussi -1 sinon

====================================================================== */
int CFilPilote::initialisationFp(void)
{
    #ifdef DEBUG
    Serial.println("-------INITIALISATION-------");
    #endif

    uint8_t valeurRetour = 0;
    int i=0;

    mcp.begin();

    for(i=0; i<=15; i++)          // initialisation
      {                             // des pins du mcp
        mcp.pinMode(i, OUTPUT);   // en sortie
      }

    for(i=0; i<NB_FILS_PILOTES; i++)
      {
      if(valeurRetour == 0)
      {
        valeurRetour=controleFp(i+1,HORS_GEL); // il n'y a pas de fil pilote 0
      }
        else
        valeurRetour = -1;
      }
    #ifdef DEBUG
    Serial.println("-----FIN DE L'INITIALISATION-----\n");
    #endif
    return (valeurRetour);
}
/* ======================================================================
méthode : setRelestage
rôle : passer le radiateur delesté en mode arret afin qu'il soit à
nouveau accessible par setFp()
Entrée  : numero du fil pilote à relester
Sortie : 0 si le relestage a reussi
         1 si le numero de fp est incorrect
         2 si la zone concerné n'est pas delesté (pas de relestage possible)
====================================================================== */
int CFilPilote::setRelestage(uint8_t fp)
{
uint8_t valeurRetour=0;

if ( (fp < 1 || fp > NB_FILS_PILOTES))
  valeurRetour = FP_INCORRECT;
  else if (etatFP[fp-1] != DELESTAGE)
        valeurRetour =PAS_DELESTAGE;
        else
          // on repasse le radiateur dans le modfe 'arret' afin
          // de pouvoir a nouveau le controler via setfp
          etatFP[fp-1] = ARRET;

#ifdef DEBUG
Serial.print("setRelestage()...");
switch(valeurRetour)
{

case 0: Serial.print("Relestage reussi -> zone: ");
        Serial.print(fp); break;
case 1: Serial.println("Erreur, numero de fil pilote incorrect."); break;
case 2: Serial.println("Erreur, La zone indiqué n'est pas actuellement delestee"); break;
}
#endif

return valeurRetour;
}
