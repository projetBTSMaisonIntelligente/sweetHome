#include "CDelestage.h"

/* ======================================================================
Fonction : CDelestage
Objectif : constructeur
Entrée   : -
Sortie   : -
Infos +  : -
====================================================================== */

CDelestage::CDelestage(void)
{
    CFilPilote oFilPilote;
}

/* ======================================================================
Fonction : controle
Objectif : méthode appelant le délestage ou le relestage lorsque cela
           est nécessaire
Entrée   : - unsigned int iSouscrite : Intensitée souscrite
           - unsigned int iInstantanee : Intensitée instantanée
Sortie   : unsigned int : état résultant de la méthode de contrôle :
           - ETAT_NORMAL : si aucune action n'a eu lieu
           - ETAT_DELESTAGE : si le délestage c'est bien effectué
           - ETAT_RELESTAGE : si le relestage c'est bien effectué
           - ETAT_ERREUR : si une erreur à été détectée
Infos +  : méthode appelée lors de la réception des trames télé-infos
====================================================================== */

int CDelestage::controle(int iSouscrite, int iInstantanee)
{
    etat = ETAT_NORMAL;
    if(iSouscrite != NULL && iInstantanee >= 0)
    {
        // Mise à jours des données
        m_iSouscrite = iSouscrite;
        m_iInstantanee = iInstantanee;

        // Vérification si il y a besoin de délester ou de relester
        if(m_iInstantanee >= (RATIO_DELESTAGE * m_iSouscrite))
        {
            etat = this->delester();
        }
        else if(m_iInstantanee <= (RATIO_RELESTAGE * m_iSouscrite))
        {
            etat = this->relester();
        }
    }
    else
        etat = ETAT_ERREUR;

    return etat;
}

/* ======================================================================
Fonction : delester
Objectif : Appel de la fonction controleFp pour modifier l'état des
           fils pilotes
Entrée   : -
Sortie   : unsigned int : état résultant de la méthode de délestage :
           - ETAT_DELESTAGE : si le délestage c'est bien déroulé
           - ETAT_ERREUR : si une erreur à été détectée
Infos +  : -
====================================================================== */

int CDelestage::delester()
{
    etat = ETAT_DELESTAGE;
    oFilPilote.controleFp(1, DELESTAGE);
    return etat;
}

/* ======================================================================
Fonction : relester
Objectif : Appel de la fonction setRelestage pour modifier l'état des
           fils pilotes
Entrée   : -
Sortie   : unsigned int : état résultant de la méthode de relestage :
           - ETAT_RELESTAGE : si le relestage c'est bien déroulé
           - ETAT_ERREUR : si une erreur à été détectée
Infos +  : -
====================================================================== */

int CDelestage::relester()
{
    etat = ETAT_RELESTAGE;
    oFilPilote.setRelestage(1);
    return etat;
}
