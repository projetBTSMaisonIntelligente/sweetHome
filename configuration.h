#define V_TELEINFO 1200
#define V_USB 9600
#define SERIAL_7E1 0x24
#define DEBUT_TRAME 0x02
#define FIN_TRAME 0x03
#define DEBUT_GROUPE 0x0A
#define FIN_GROUPE 0x0D
#define DEBUT_DONNEES 0x20
#define FIN_DONNEES 0x3F
#define BIT_8 0x7F
#define RATIO_DELESTAGE 0.9
#define RATIO_RELESTAGE 0.8
#define ETAT_NORMAL 0
#define ETAT_DELESTAGE 1
#define ETAT_RELESTAGE 2
#define ETAT_ERREUR 3
// Les fils pilotes sont connectés de la façon suivante sur l'I/O expander
// # Fil Pilote    MCP IO/Port   Digital Port
// FP1 (a et b) -> GPB6, GPB7  ->  14/15
// FP2 (a et b) -> GPB4, GPB5  ->  12/13
// FP3 (a et b) -> GPB2, GPB3  ->  10/11
// FP4 (a et b) -> GPA1, GPA0  ->  1/0
// FP5 (a et b) -> GPA3, GPA2  ->  3/2
// FP6 (a et b) -> GPA5, GPA4  ->  5/4
// FP7 (a et b) -> GPA7, GPA6  ->  7/6
// Relais       -> GPB1        ->  9
// LED          -> GPB0        ->  8
#define FP1 14,15
#define FP2 12,13
#define FP3 10,11
#define FP4  1,0
#define FP5  3,2
#define FP6  5,4
#define FP7  7,6
#define RELAIS_PIN  9
#define LED_PIN     8
#define DEBUG
#define NB_FILS_PILOTES 7
#define CONFORT 'C'
#define HORS_GEL 'H'
#define DELESTAGE 'D'
#define ECO 'E'
#define ARRET 'A'

///// retour méthodes ////////
#define SUCCES 0
#define FP_INCORRECT 1
//setfp
#define TAILLE_INCORRECT -1
#define ORDRE_INCORRECT 2
#define RADIATEUR_DELESTAGE 3
//setReg
#define TEMP_INCORRECT 2
#define CONSIGNE_IMPROBABLE 3
//setRelestage
#define PAS_DELESTAGE 2
//////////////////////////////
