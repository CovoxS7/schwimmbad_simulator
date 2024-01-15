/* Schwimmbadsimulator */
/* Ein Konsolen Programm das die Auslastung */
/* eines Tages in einem Schwimmbad simuliert */
/* Autor: Hans Kuntsche */
/* Autor: S�ren Lehmann */
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
#define MAX_PARKPLATZ		30
#define MAX_FUSSGAENGER		55

/* Globale Variablen */
int badegaesteGesamtMenge = 0;
int badegaesteAktuelleMenge = 0;
int busFahrgaeste = 0;
int busHaltestelle = 0;
int autoBelegung = 0;
int autoParkplatz = 0;
int fussgaengerJa = 0;
int fussgaengerGesamt = 0;

/* Struktur f�r einen Badegast */
typedef struct Badegast{
	int index;
	int ankunftsZeit;
	int ankunftsTyp;		/* 0 = Bus, 1 = Auto, 2 = Fu�g�nger */
	int kartenTyp;			/* 0 = 2h, 1 = 1h verl., 2 = 2h verl., 3 = Tageskarte */
	int ereignisTyp;		/* 0 = Frei, 1 = Liege, 2 = Normal, 3 = Ring */
	/* Weitere Eigenschaften f�r einen Badegast hier eintragen */
	
	
	struct Badegast *davor;
	struct Badegast *danach;
} Badegast;

/* Zeiger f�r doppelt verkettete Liste */
Badegast *badegastAnfang = NULL;
Badegast *badegastEnde = NULL;
Badegast *badegastAktuell = NULL;

/* Funktionsdeklarationen */
void simulation();
void anreise(int simMinute);
void busAnreise(int simMinute);
void autoAnreise(int simMinute);
void zuFussAnreise(int simMinute);
void badegaesteEinlass(int simMinute);
int badegastHinzufuegen(int simMinute, int ankunftsTyp);
int badegaesteDurchsuchen();
void badegaesteFreilassen();
int zufallszahl(int maximum);
void eingabeVerarbeitung(int simMinute, int *simGeschwindigkeit);
void ausgabeVerarbeitung(int simMinute);

/* Hauptfunktion */
int main(void) {
	srand(time(0));
	simulation();
	return 0;
}

/* Funktion verwaltet den zeitlichen Ablauf der Simulation */
void simulation() {
	int simZeit = (11 * 60) + 1;	/* (10h �ffnungszeit + 1h Vorlauf) * 60min + 1min um auf 20:00 zu kommen */
	int simGeschwindigkeit = 1000;
	int simMinute; 
	int simSekunde;
	
	/* Schleifeninhalt wird jede simulierte Minute aufgerufen (entsprechend 1 Realsekunde) */
	/* simMinute = 50 l�sst die Simulation 09:50 starten f�r den ersten Bus */
	for(simMinute = 50; simMinute < simZeit; simMinute++) {
		
		/* Ruft die anreise-Funktion auf */
		anreise(simMinute);
			
		/* Funktionen ab �ffnung des Schwimmbads */
		if(simMinute > 59) {
			/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			/* Hier werden die Funktionen aufgerufen die jede Minute ausgef�hrt werden */
			
			
			
			
			
			
			
			
			
			/* L�sst die Badeg�ste einzeln ins Schwimmbad */
			badegaesteEinlass(simMinute);
			
			/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			
			/* Schleifeninhalt wird jede simulierte Sekunde aufgerufen (entsprechen 1/60 Realsekunden) */
			for(simSekunde = 0; simSekunde < 60; simSekunde++) {
				/* ------------------------------------------------------------------------ */
				/* Hier werden die Funktionen aufgerufen die jede Sekunde ausgef�hrt werden */
			
			
			
			
			
			
			
			
			
				/* ------------------------------------------------------------------------ */
			}
		}

		/* Eingabeverarbeitung */
		eingabeVerarbeitung(simMinute, &simGeschwindigkeit);
		
		/* Anzeigen der Ergebnisse */
		ausgabeVerarbeitung(simMinute);
		
		/* Steuert die Simulationsgeschwindigkeit */
		Sleep(simGeschwindigkeit);
	}
	badegaesteFreilassen();
}

/* Funktion Simuliert die Anreise der Badeg�ste */
void anreise(int simMinute) {
	int wahrscheinlichkeit;
	
	/* Alle 30 Minuten bringt der Bus neue Badeg�ste */
	/* Erster Bus 9:50 Uhr, letzter Bus 19:10 Uhr */
	if(((simMinute - 20) % 30 == 0 && simMinute < 611) || simMinute = 610) {
		/* Der Bus bringt nur soviele G�ste, dass die maximale */
		/* Auslastung des Schwimmbads nicht �berschritten werden kann */
		if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - MAX_BUS - MAX_AUTO - 1)) {
			busAnreise(simMinute);
		}
	}

	/* Anreise ab �ffnung des Schwimmbads */
	if(simMinute > 59) {
		
		/* Es besteht eine Wahrscheinlichkeit von 1:5 das in dieser Minute ein Auto ankommt */
		wahrscheinlichkeit = zufallszahl(6);
		if(wahrscheinlichkeit == 0 && autoParkplatz < MAX_PARKPLATZ) {
			/* Mit dem Auto kommen nur soviele G�ste, dass die maximale */
			/* Auslastung des Schwimmbads nicht �berschritten werden kann */
			if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - MAX_AUTO - 1)) {
				autoAnreise(simMinute);
			}
		}
		
		/* Es besteht eine Wahrscheinlichkeit von 1:10 das in dieser Minute ein Gast zu Fuss ankommt */
		wahrscheinlichkeit = zufallszahl(11);
		if(wahrscheinlichkeit == 0 && fussgaengerGesamt < MAX_FUSSGAENGER) {
			/* Zu Fu� kommen nur soviele G�ste, dass die maximale */
			/* Auslastung des Schwimmbads nicht �berschritten werden kann */
			if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - 1)) {
				zuFussAnreise(simMinute);
			}
		}
	}
}

/* Funktion zur Generieren der Anzahl der Fahrg�ste im Bus */
void busAnreise(int simMinute) {
	/* Je nach Uhrzeit bringt der Bus mehr oder weniger G�ste */
	/* Maximal 50 in den Zeiten der Hauptauslastung des Schwimmbads */
	/* In der letzten Stunde kommt kein Gast mehr zum Schwimmbad */
	if(simMinute < 60) busFahrgaeste = zufallszahl(20);
	if(simMinute > 60 && simMinute < 120) busFahrgaeste = zufallszahl(20) + 10;
	if(simMinute > 120 && simMinute < 180) busFahrgaeste = zufallszahl(20) + 20;
	if(simMinute > 180 && simMinute < 420) busFahrgaeste = zufallszahl(20) + 30;
	if(simMinute > 420 && simMinute < 480) busFahrgaeste = zufallszahl(20) + 20;
	if(simMinute > 480 && simMinute < 540) busFahrgaeste = zufallszahl(20) + 10;
	if(simMinute > 540 && simMinute < 600) busFahrgaeste = zufallszahl(20);
}

/* Funktion zur Generierung der Anzahl der im Auto anreisenden Badeg�ste */
void autoAnreise(int simMinute){
	/* Je nach Uhrzeit kommen mehr oder weniger G�ste mit dem Auto */
	/* Maximal 5 in den Zeiten der Hauptauslastung des Schwimmbads */
	/* In der letzten Stunde kommt kein Gast mehr zum Schwimmbad */
	if(simMinute < 60) autoBelegung = zufallszahl(2);
	if(simMinute > 60 && simMinute < 120) autoBelegung = zufallszahl(2) + 1;
	if(simMinute > 120 && simMinute < 180) autoBelegung = zufallszahl(2) + 2;
	if(simMinute > 180 && simMinute < 420) autoBelegung = zufallszahl(2) + 3;
	if(simMinute > 420 && simMinute < 480) autoBelegung = zufallszahl(2) + 2;
	if(simMinute > 480 && simMinute < 540) autoBelegung = zufallszahl(2) + 1;
	if(simMinute > 540 && simMinute < 600) autoBelegung = zufallszahl(2);
	
	/* Kommen Badeg�ste mit dem Auto wird ein Parkplatz belegt */
	if(autoBelegung > 0) {
		autoParkplatz++;
	}
}

/* Funktion der zu Fu� anreisenden Badeg�ste */
void zuFussAnreise(int simMinute){
	/* In der letzten Stunde kommt kein Gast mehr zum Schwimmbad */
	if(simMinute < 600) {
		fussgaengerJa++;
		fussgaengerGesamt++;
	}
}

/* Funktion f�gt die angereisten Personen den Badeg�sten hinzu */
void badegaesteEinlass(int simMinute) {
	for(; busFahrgaeste > 0; busFahrgaeste--) {
		badegastHinzufuegen(simMinute, 0);
	}	
	for(; autoBelegung > 0; autoBelegung--) {
		badegastHinzufuegen(simMinute, 1);
	}
	for(; fussgaengerJa > 0; fussgaengerJa--) {
		badegastHinzufuegen(simMinute, 2);
	}
}

/* Funktion f�gt die eingelassenen Badeg�ste zur Liste der Badeg�ste hinzu */
int badegastHinzufuegen(int simMinute, int ankunftsTyp) {
	
	/* Reserviert Speicher f�r einen neuen Badegasteintrag */
	if(!(badegastAktuell = (Badegast* ) malloc(sizeof(Badegast)))) {
		/* Hier folgt die Fehlerbehandlung */
		return 0;
	}
	
	/* Erh�ht die Z�hler f�r die aktuelle Auslastung und die Gesamtg�stemenge */
	badegaesteGesamtMenge++;
	badegaesteAktuelleMenge++;
	
	/* �bergibt die Startwerte dem Badegasteintrag */
	badegastAktuell->index = badegaesteGesamtMenge;
	badegastAktuell->ankunftsZeit = simMinute;
	badegastAktuell->ankunftsTyp = ankunftsTyp;
	badegastAktuell->kartenTyp = 0;
	badegastAktuell->ereignisTyp = 0;
	badegastAktuell->davor = badegastEnde;
	badegastAktuell->danach = NULL;
	
	/* H�ngt den Badegast ans Ende der Liste */
	if(badegastAnfang == NULL) {
		badegastAnfang = badegastAktuell;
	}
	else {
		badegastEnde->danach = badegastAktuell;
	}
	badegastEnde = badegastAktuell;
	
	return 0;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/*       Funktion ist nur ein Beispiel und gilt zu Testzwecken       */
/* Es wird gepr�ft ob alle Badeg�ste in die Liste eingetragen wurden */
int badegaesteDurchsuchen() {
	int i = 0;
	
	badegastAktuell = badegastAnfang;
	while(badegastAktuell != NULL) {
		badegastAktuell = badegastAktuell->danach;
		i++;
	}
	return i;
}

/* Funktion gibt den kompletten reservierten Speicher wieder frei */
void badegaesteFreilassen() {
	badegastAktuell = badegastAnfang->danach;
	
	while(badegastAktuell != NULL) {
		badegastAnfang->danach = badegastAktuell->danach;
		badegastAktuell->danach->davor = badegastAnfang;
		free(badegastAktuell);
		badegastAktuell = badegastAnfang->danach;
	}
	free(badegastAnfang);
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
	
	/* _kbhit() pr�ft ob eine Taste gedr�ckt wurde */
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
		
		/* Pausiert die Simulation bis zum erneuten dr�cken der P-Taste */
		if(eingabe == 'P' || eingabe == 'p') {
			while(stopPause != 'P' && stopPause != 'p') {
				
				/* _kbhit() pr�ft ob eine Taste gedr�ckt wurde */
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
	printf("%*sRutschen", 30, "");
	printf("\n1 Stunde verlaengert: %6d", 0);
	printf("\n2 Stunden verlaengert: %5d", 0);
	printf("%*snormal", 8, "");
	printf("%*sSchwimmring", 26, "");
	printf("\nTageskarten: %15d", 0);
	printf("%*sauf Treppe: %3d", 8, "", 0);
	printf("%*sauf Treppe: %*s%3d", 17, "", 7, "", 0);
	printf("\nRutschennutzungen: %9d", 0);
	printf("%*sRinge im Automaten: %2d", 40, "", 0);
	printf("\n\nBistro: %2d/20", 0);
	printf("\nLiegen: %2d/80", 0);
	printf("%*sBecken", 46, "");
	printf("\n\n(H): %2d", 0);
	printf("%*sKinder", 29, "");
	printf("%*sSchwimmer", 16, "");
	printf("%*sAussen", 17, "");
	printf("\n[P]: %2d", autoParkplatz);
	printf("%*s%3d", 32, "", 0);	/* Kinder */
	printf("%*s%3d", 22, "", 0);	/* Schwimmer */
	printf("%*s%3d", 20, "", 0);	/* Au�en */
	
	/* Zeigt die simulierte Stunde an */
	/* Unter 10 Stunden wird eine 0 erg�nzt */
	if(((simMinute / 60) + 9) < 10){
		printf("\n\n0%d:", (simMinute / 60) + 9);
	}
	else {
		printf("\n\n%d:", (simMinute / 60) + 9);
	}
	
	/* Zeigt die simulierte Minute an */
	/* Unter 10 Minuten wird eine 0 erg�nzt */
	if((simMinute % 60) < 10){
		printf("0%d Uhr", simMinute % 60);
	}
	else {
		printf("%d Uhr", simMinute % 60);
	}
	
	printf("\nPersonen im Schwimmbad: %4d", badegaesteGesamtMenge);
	printf("\nPersonen in der Liste: %5d", badegaesteDurchsuchen());
	printf("\n...(T)urbo");
	printf("\n...(P)ause");
}
