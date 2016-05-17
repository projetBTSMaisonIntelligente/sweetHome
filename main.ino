#include "CTeleinfo.h"
#include "FilPilote.h"
#include "regulation.h"
#include "ADAFRUIT_MCP23017.h"

Adafruit_MCP23017 mcp;
CFilPilote oFilPilote;
CTeleinfo oTeleinfo;
CRegulation oRegulation;

void setup()
{
    RGB.control(true);
    RGB.color(255,0,0);
    oFilPilote.initialisationFp();
}

void loop()
{

    oTeleinfo.lecture();
}
