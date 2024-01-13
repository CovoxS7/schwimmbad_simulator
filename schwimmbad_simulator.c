/* Schwimmbadsimulator */
/* Ein Konsolen Programm das die Auslastung */
/* eines Tages in einem Schwimmbad simuliert */
/* Autor: Hans Kuntsche */
/* Autor: Sören Lehmann */
/* Datum: 12.01.2024 */

/* Bibliotheken */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

/* Konstanten */
#define MAX_AUSLASTUNG		350
#define MAX_BUS				50
#define MAX_AUTO			5
#define MAX_FUSSGAENGER		55

/* Globale Variablen */
int busFahrgaeste = 0;
int busHaltestelle = 0;
int autoBelegung = 0;

/* Funktionsdeklarationen */
void simulation();
int zufallszahl(int maximum);
void eingabeVerarbeitung(int simMinute, int *simGeschwindigkeit);
void ausgabeVerarbeitung(int simMinute);

/* Hauptfunktion */
int main(void) {
	srand(time(0));
	simulation();
	return 0;
}

/* Funktion verwaltet die Simulation */
void simulation() {
	int simZeit = (11 * 60) + 1; /* (10h Öffnungszeit + 1h Vorlauf) * 60min + 1min um auf 20:00 zu kommen */
	int simGeschwindigkeit = 1000;
	int simMinute; 
	int simSekunde;
	
	/* Schleifeninhalt wird jede simulierte Minute aufgerufen (entsprechen 1 Realsekunde) */
	/* i = 50 lässt die Simulation 09:50 starten für den ersten Bus */
	for(simMinute = 50; simMinute < simZeit; simMinute++) {
		
		/* Schleifeninhalt wird jede simulierte Sekunde aufgerufen (entsprechen 1/60 Realsekunden) */
		for(simSekunde = 0; simSekunde < 60; simSekunde++) {
		
		/* Hier werden die Funktionen für die einzelnen Abläufe aufgerufen */
			
		}
		
		/* Eingabeverarbeitung */
		eingabeVerarbeitung(simMinute, &simGeschwindigkeit);
		
		/* Anzeigen der Ergebnisse */
		ausgabeVerarbeitung(simMinute);
		
		/* Lässt die Simulation nur alle 1 Sekunde durchlaufen */
		Sleep(simGeschwindigkeit);
		
	}
}

/* Funktion generiert eine Zufallszahl */
int zufallszahl(int maximum) {
	return rand() % maximum;
}

/* Funktion nimmt Eingaben ohne Enter entgegen und verarbeitet sie */
void eingabeVerarbeitung(int simMinute, int *simGeschwindigkeit) {
	char eingabe = '\0';
	char stopPause = '\0';
	int pauseZaehler = 0;
	
	/* _kbhit() prüft ob eine Taste gedrückt wurde */
	/* _getch() liest die Taste sofort ohne Eingabetaste */
	if(_kbhit()) {
		eingabe = _getch();
		
		/* Beschleunigt oder Verlangsamt die Simulation um das 10-fache */
		if(eingabe == 'T' || eingabe == 't') {
			if(*simGeschwindigkeit == 1000) {
				*simGeschwindigkeit = 100;
			}
			else {
				*simGeschwindigkeit = 1000;
			}
		}
		
		/* Pausiert die Simulation bis zum erneuten drücken der P-Taste */
		if(eingabe == 'P' || eingabe == 'p') {
			while(stopPause != 'P' && stopPause != 'p') {
				
				/* _kbhit() prüft ob eine Taste gedrückt wurde */
				/* _getch() liest die Taste sofort ohne Eingabetaste */
				if(_kbhit()) {
					stopPause = _getch();
				}
				
				/* Solange die Simulation pausiert ist wird eine Ladeanimation angezeigt */
				ausgabeVerarbeitung(simMinute);
				printf("\n\nSimulation pausiert: ");
				pauseZaehler++;
				switch(pauseZaehler) {
					case 1: printf("\\"); break;
					case 2: printf("|"); break;
					case 3: printf("/"); break;
					default: {
						printf("-");
						pauseZaehler = 0;
						break;
					}
				}
				Sleep(500);
			}
		}
	}
}

/* Funktion zur Ausgabe der simulierten Ergebnisse */
void ausgabeVerarbeitung(int simMinute) {
	system("cls");
	
	printf("Schwimmbad Wasserwesen");
	printf("\n\n2-Stunden-Karten: %10d", 0);
	printf("%*sRutschen", 23, "");
	printf("\n1 Stunde verlaengert: %6d", 0);
	printf("\n2 Stunden verlaengert: %5d", 0);
	printf("%*snormal", 8, "");
	printf("%*sSchwimmring", 19, "");
	printf("\nTageskarten: %15d", 0);
	printf("%*sauf Treppe: %4d", 8, "", 0);
	printf("%*sauf Treppe: %*s%3d", 9, "", 7, "", 0);
	printf("\nRutschennutzungen: %9d", 0);
	printf("%*sRinge im Automaten: %2d", 33, "", 0);
	printf("\n\nBistro: %2d/20", 0);
	printf("\nLiegen: %2d/80", 0);
	printf("%*sBecken", 38, "");
	printf("\n\n(H): %d", 0);
	printf("%*sKinder", 30, "");
	printf("%*sSchwimmer", 8, "");
	printf("%*sAussen", 8, "");
	printf("\n[P]: %d", 0);
	printf("%*s%3d", 33, "", 0);	/* Kinder */
	printf("%*s%3d", 14, "", 0);	/* Schwimmer */
	printf("%*s%3d", 11, "", 0);	/* Außen */
	
	/* Zeigt die simulierte Stunde an */
	/* Unter 10 Stunden wird eine 0 ergänzt */
	if(((simMinute / 60) + 9) < 10){
		printf("\n\n0%d:", (simMinute / 60) + 9);
	}
	else {
		printf("\n\n%d:", (simMinute / 60) + 9);
	}
	
	/* Zeigt die simulierte Minute an */
	/* Unter 10 Minuten wird eine 0 ergänzt */
	if((simMinute % 60) < 10){
		printf("0%d Uhr", simMinute % 60);
	}
	else {
		printf("%d Uhr", simMinute % 60);
	}
	
	printf("\nPersonen im Schwimmbad: %4d", 0);
	printf("\n...(T)urbo");
	printf("\n...(P)ause");
}
