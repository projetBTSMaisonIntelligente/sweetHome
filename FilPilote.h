#ifndef FILPILOTE_h
#define FILPILOTE_h
#include "configuration.h"
#include "application.h"
#include "ADAFRUIT_MCP23017.h"

extern char etatFP[];
extern Adafruit_MCP23017 mcp;

class CFilPilote
{
  public:
      CFilPilote(void);
      static int setfp(String);
      static int controleFp(uint8_t,char);
      int setRelestage(uint8_t);
      int initialisationFp(void);
};
#endif
