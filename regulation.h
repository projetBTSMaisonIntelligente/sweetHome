#ifndef REGULATION_h
#define REGULATION_h
#include "application.h"
#include "pid.h"
#include "configuration.h"
#include "FilPilote.h"
extern CFilPilote oFilPilote;

class CRegulation {
public:
  CRegulation(void);
  static int setRegulation(String command);
  static int setRegulationInterne(uint8_t,double,double,double);

};
#endif
