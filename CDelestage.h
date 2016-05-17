#ifndef DELESTAGE_h
#define DELESTAGE_h
#include "application.h"
#include "configuration.h"
#include "FilPilote.h"
extern CFilPilote oFilPilote;

class CDelestage
{
public:
    CDelestage(void);
    int delester(void);
    int relester(void);
    int controle(int iSouscrite, int iInstantanee);
private:
    int etat;
    int m_iSouscrite;
    int m_iInstantanee;
};
#endif
