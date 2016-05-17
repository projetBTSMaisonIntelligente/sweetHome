#include "regulation.h"


CRegulation::CRegulation(void)
{
      Particle.function("setReg", setRegulation);
      CFilPilote oFilPilote;
}

/* ======================================================================
méthode : setRegulation
rôle : méthode cloud permettant de recuperer les parametres necessaires
à la regulation via jeedom
Entrée   : un string du type suivant
"(numero de fp/consigne/temperatureCapteur/limiteHaute/limiteBasse)"
parenthèse pour delimiter la commande et des '/' pour delimiter les
différentes variables envoyées
Sortie : 0 si la regulation s'execute correctement
         1 si le numero de fil pilote est incorrect
         2 si la temperature du capteur est improbable
         3 si la consigne de temperature est improbable
====================================================================== */
int CRegulation::setRegulation(String command)
{
  uint8_t fp;
  double consigne,temp,ecart;
  int nombreSlash = 0,valeurRetour=0;

  Serial.println("setReg...");

  if(command[0]== '(' && command[command.length()-1] == ')')
  {
    Serial.println("parenthese OK...");
    String str="";
	   for(int i = 1 ; i < command.length()-1; i++)
		    {if(command[i] == '/')
          {
            switch(nombreSlash)
              {
                case 0: fp = str.toInt(); Serial.println(fp); break;

                case 1: consigne = str.toFloat(); Serial.println(consigne); break;

                case 2: temp = str.toFloat(), Serial.println(temp); break;

                case 3: ecart = str.toFloat(),Serial.println(ecart); break;

              }
            nombreSlash += 1;
            str ="";
          }
        else
            str += command[i];

          }
  }
  valeurRetour = setRegulationInterne(fp,consigne,temp,ecart);

  return valeurRetour;
}



/* ======================================================================
méthode : setRegulationInterne
rôle : execute l'agorithme de regulation TOR
Entrée   : numero du fil pilote, consigne de temperature,
la temperature actuelle, limite de regulation haute et basse par rapport
à la consigne

Sortie : 0 si la regulation s'execute correctement
         1 si le numero de fil pilote est incorrect
         2 si la temperature du capteur est improbable
         3 si la consigne de temperature est improbable
====================================================================== */
int CRegulation::setRegulationInterne(uint8_t fp,double consigne,double temp,double ecart)
{
int valeurRetour=0;

if ( (fp < 1 || fp > NB_FILS_PILOTES))
    valeurRetour = FP_INCORRECT; // numero de fp incorrect
    else if(temp < 0 || temp > 50)
          valeurRetour =TEMP_INCORRECT; // temperature improbable
          else if(consigne < 15 || consigne > 30)
                  valeurRetour =CONSIGNE_IMPROBABLE; // consigne improbable
                  else
    {

    if (temp >= consigne+ecart)
      {
        oFilPilote.controleFp(fp,ARRET);
      }
      else if(temp == consigne-ecart)
              {
                if (etatFP[fp-1] == ARRET)
                {
                  oFilPilote.controleFp(fp,CONFORT);
                }

              }
              else if(temp < consigne)
                    {
                      oFilPilote.controleFp(fp,CONFORT);
                    }
    }

    return valeurRetour;
}
