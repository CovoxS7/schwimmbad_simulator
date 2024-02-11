/* Schwimmbadsimulator */
/* Ein Konsolen Programm dass die */
/* Tagesauslastung in einem Schwimmbad simuliert */
/* Autor: Hans Kuntsche */
/* Autor: Soeren Lehmann */
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
#define MAX_KINDERBECKEN	50
#define MAX_SCHWIMMERBECKEN	125
#define MAX_AUSSENBECKEN	175
#define MAX_LIEGEN			80

/* Globale Variablen */
int badegaesteGesamtMenge = 0;
int badegaesteAktuelleMenge = 0;
int busZustand = 0;
int busFahrgaeste = 0;
int busHaltestelle = 0;
int autoBelegung = 0;
int autoParkplatz = 0;
int fussgaengerJa = 0;
int fussgaengerGesamt = 0;
int zweiStundenkarte = 0;
int plusEineStunde = 0;
int plusZweiStunden = 0;
int tagesKarte = 0;
int liegenBelegt = 0;
int kinderbecken = 0;
int schwimmerbecken = 0;
int aussenbecken = 0;

int fehler = 0;

/* Struktur fuer einen Badegast */
typedef struct Badegast{
	int index;
	int ankunftsZeit;
	int ankunftsTyp;		/* 0 = Bus, 1 = Auto, 2 = Fussgaenger */
	int kartenTyp;			/* 120 = 2h, 180 = 1h verl., 240 = 2h verl., 300 = Tageskarte */
	int ereignisTyp;		/* 0 = Frei, 1 = Liege, 2 = Normal, 3 = Ring */
	int ereignisZeit;
	/* Weitere Eigenschaften fuer einen Badegast hier eintragen */
	
	
	struct Badegast *davor;
	struct Badegast *danach;
} Badegast;

/* Zeiger fuer doppelt verkettete Liste */
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
int eintrittskarteKaufen(int menge);
void eintrittskarteVerlaengern(int simMinute);
int gehenOderBleiben(int simMinute);
void abreise(int simMinute);
void busHaltestellenVerwaltung(int simMinute);
int badegastHinzufuegen(int simMinute, int ankunftsTyp, int kartenTyp);
int badegaesteDurchsuchen();
void badegastFreilassen();
void ereignisWahl(int simMinute);
void liegenAblauf();
void schwimmbeckenWahl();
int zufallszahl(int maximum);
void eingabeVerarbeitung(int simMinute, int *simGeschwindigkeit);
void ausgabeVerarbeitung(int simMinute);

void fehlerAusgabe();

/* TestFunktion */
void ereignisAblauf();

/* Hauptfunktion */
int main(void) {
	srand(time(0));
	simulation();
	return 0;
}

/* Funktion verwaltet den zeitlichen Ablauf der Simulation */
void simulation() {
	int simZeit = (11 * 60) + 1;	/* (10h oeffnungszeit + 1h Vorlauf) * 60min + 1min um auf 20:00 zu kommen */
	int simGeschwindigkeit = 1000;
	int simMinute; 
	int simSekunde;
	
	/* Schleifeninhalt wird jede simulierte Minute aufgerufen (entsprechend 1 Realsekunde) */
	/* simMinute = 50 laesst die Simulation 09:50 starten fuer den ersten Bus */
	for(simMinute = 50; simMinute < simZeit; simMinute++) {
				
		/* Ruft die anreise-Funktion auf */
		anreise(simMinute);
			
		/* Funktionen ab oeffnung des Schwimmbads */
		if(simMinute > 59) {
					
			/* Badegaeste tretten die Heimreise an */
			abreise(simMinute);
			
			/* Laesst die Badegaeste einzeln ins Schwimmbad */
			badegaesteEinlass(simMinute);
			
			/* Prueft ob ein Gast seine Eintrittskarte verlaengern muss */
			eintrittskarteVerlaengern(simMinute);
		
			/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			/* Hier werden die Funktionen aufgerufen die jede Minute ausgefuehrt werden */
			
				/* Laesst die Ereignisse ablaufen */
				ereignisAblauf();
			
			
			
			
			
			
			
			
			/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			
			/* Schleifeninhalt wird jede simulierte Sekunde aufgerufen (entsprechen 1/60 Realsekunden) */
			for(simSekunde = 0; simSekunde < 60; simSekunde++) {
				/* ------------------------------------------------------------------------ */
				/* Hier werden die Funktionen aufgerufen die jede Sekunde ausgefuehrt werden */
			
				/* Badegaeste entscheiden zufaellig ob sie Rutschen gehen */
				/* oder auf eine Liege oder in einem der Becken bleiben */
				ereignisWahl(simMinute);
			
			
			
			
			
				/* Laesst die Zeit auf der Liege ablaufen */
				liegenAblauf();
				
				/* Badegaeste suchen sich ein Schwimmbecken raus */
				schwimmbeckenWahl();
				/* ------------------------------------------------------------------------ */
			}
		}
		
		/* Kuemmert sich um die Verwaltung der Bushaltestelle */
		busHaltestellenVerwaltung(simMinute);

		/* Eingabeverarbeitung */
		eingabeVerarbeitung(simMinute, &simGeschwindigkeit);
		
		/* Anzeigen der Ergebnisse */
		ausgabeVerarbeitung(simMinute);
		
		/* Im Falle eines Fehlers wird eine Ausgabe erzeugt */
		fehlerAusgabe();
		
		/* Steuert die Simulationsgeschwindigkeit */
		Sleep(simGeschwindigkeit);
	}
}

/* Funktion Simuliert die Anreise der Badegaeste */
void anreise(int simMinute) {
	int wahrscheinlichkeit;
	
	/* Alle 30 Minuten bringt der Bus neue Badegaeste */
	/* Erster Bus 9:50 Uhr, letzter Bus 19:10 Uhr */
	if(((simMinute - 20) % 30 == 0 && simMinute <= 610) || simMinute == 610) {
		/* Der Bus bringt nur soviele Gaeste, dass die maximale */
		/* Auslastung des Schwimmbads nicht ueberschritten werden kann */
		if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - MAX_BUS - MAX_AUTO - 1)) {
			busAnreise(simMinute);
		}
	}

	/* Anreise ab Oeffnung des Schwimmbads */
	if(simMinute >= 60) {
		
		/* Es besteht eine Wahrscheinlichkeit von 1:5 das in dieser Minute ein Auto ankommt */
		wahrscheinlichkeit = zufallszahl(6);
		if(wahrscheinlichkeit == 0 && autoParkplatz < MAX_PARKPLATZ) {
			/* Mit dem Auto kommen nur soviele Gaeste, dass die maximale */
			/* Auslastung des Schwimmbads nicht ueberschritten werden kann */
			if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - MAX_AUTO - 1)) {
				autoAnreise(simMinute);
			}
		}
		
		/* Es besteht eine Wahrscheinlichkeit von 1:10 das in dieser Minute ein Gast zu Fuss ankommt */
		/* Ausserdem wird mit einbezogen ob schon die Maximale Menge moeglicher Fussgaenger erreicht wurde */
		wahrscheinlichkeit = zufallszahl(11);
		if(wahrscheinlichkeit == 0 && fussgaengerGesamt < MAX_FUSSGAENGER) {
			/* Zu Fuss kommen nur soviele Gaeste, dass die maximale */
			/* Auslastung des Schwimmbads nicht ueberschritten werden kann */
			if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - 1)) {
				zuFussAnreise(simMinute);
			}
		}
	}
}

/* Funktion zur Generieren der Anzahl der Fahrgaeste im Bus */
void busAnreise(int simMinute) {
	/* Je nach Uhrzeit bringt der Bus mehr oder weniger Gaeste */
	/* Maximal 50 in den Zeiten von 11:00 Uhr und 16:00 Uhr */
	if(simMinute < 60) busFahrgaeste = zufallszahl(10) + 10;
	if(simMinute >= 60 && simMinute < 90) busFahrgaeste = zufallszahl(10) + 20;
	if(simMinute >= 90 && simMinute < 120) busFahrgaeste = zufallszahl(10) + 30;
	if(simMinute >= 120 && simMinute < 420) busFahrgaeste = zufallszahl(10) + 40;
	if(simMinute >= 420 && simMinute < 450) busFahrgaeste = zufallszahl(10) + 30;
	if(simMinute >= 450 && simMinute < 480) busFahrgaeste = zufallszahl(10) + 20;
	if(simMinute >= 480 && simMinute < 510) busFahrgaeste = zufallszahl(10) + 10;
}

/* Funktion zur Generierung der Anzahl der im Auto anreisenden Badegaeste */
void autoAnreise(int simMinute){
	/* Je nach Uhrzeit kommen mehr oder weniger Gaeste mit dem Auto */
	/* Maximal 5 in den Zeiten von 11:00 Uhr und 16:00 Uhr */
	if(simMinute >= 60 && simMinute < 90) autoBelegung = zufallszahl(1) + 2;
	if(simMinute >= 90 && simMinute < 120) autoBelegung = zufallszahl(1) + 3;
	if(simMinute >= 120 && simMinute < 420) autoBelegung = zufallszahl(1) + 4;
	if(simMinute >= 420 && simMinute < 450) autoBelegung = zufallszahl(1) + 3;
	if(simMinute >= 450 && simMinute < 480) autoBelegung = zufallszahl(1) + 2;
	if(simMinute >= 480 && simMinute < 570) autoBelegung = zufallszahl(1) + 1;
}

/* Funktion der zu Fuss anreisenden Badegaeste */
void zuFussAnreise(int simMinute){
	/* Fussgaenger kommen je nach belieben */
	if(simMinute < 570) {
		fussgaengerJa++;
		fussgaengerGesamt++;
	}
}

/* Funktion fuegt die angereisten Personen den Badegaesten hinzu */
void badegaesteEinlass(int simMinute) {
	int kartenTyp;
	
	/* Fuer jeden angereisten Gast wird die badegastHinzufuegen Funktion aufgerufen */
	/* und ihr die Anreisezeit, den Anreisetyp sowie die gekaufte Karte mitgegeben */
	
	/* Busgaeste erhalten Anreisetyp 0 */
	for(; busFahrgaeste > 0; busFahrgaeste--) {
		kartenTyp = eintrittskarteKaufen(1);
		badegastHinzufuegen(simMinute, 0, kartenTyp);
	}
	
	/* Kommen Badegaeste mit dem Auto wird ein Parkplatz belegt, */
	/* alle erhalten die gleiche Karte und Anreisetyp 1 */
	if(autoBelegung > 0) {
		kartenTyp = eintrittskarteKaufen(autoBelegung);
		autoParkplatz++;
		for(; autoBelegung > 0; autoBelegung--) {
			badegastHinzufuegen(simMinute, 1, kartenTyp);
		}
	}
	
	/* Fussgaenger erhalten den Anreisetyp 2 */
	for(; fussgaengerJa > 0; fussgaengerJa--) {
		kartenTyp = eintrittskarteKaufen(1);
		badegastHinzufuegen(simMinute, 2, kartenTyp);
	}
}

/* Funktion ermittelt welche Eintrittskarte die Gaeste kaufen */
int eintrittskarteKaufen(int menge) {
	int wahrscheinlichkeit;
	
	/* Es besteht eine Wahrscheinlichkeit von 1:5 fuer Tageskarte/2H-Karte */
	wahrscheinlichkeit = zufallszahl(6);
	/* Tageskarten haben den Wert 300 zugeordnet */
	if(wahrscheinlichkeit == 0) {
		tagesKarte += menge;
		return 300;
	}
	/* 2H-Karten haben den Wert 120 zugeordnet */
	else {
		zweiStundenkarte += menge;
		return 120;
	}
}

/* Funktion sorgt dafuer das alle Gaeste die laenger bleiben als ihre Karte gueltig ist, ihre Karte verlaengern */
void eintrittskarteVerlaengern(int simMinute) {
	int vergangeneZeit;
	
	badegastAktuell = badegastAnfang;
	
	/* Schleife laeuft einmal durch alle Badegaeste */
	while(badegastAktuell != NULL) {
		
		/* Wenn der Gast keine Tageskarte(300) hat wird geprueft ob er verlaengern muss */
		if(badegastAktuell->kartenTyp != 300) {
			
			/* Ermittelt die seit betreten des Schwimmbads vergangene Zeit */
			vergangeneZeit = simMinute - badegastAktuell->ankunftsZeit;
			
			/* Es wird geprueft ob die Aktuelle Karte abgelaufen ist */
			if(badegastAktuell->kartenTyp < vergangeneZeit) {
				
				/* Ist die Karte abgelaufen werden die jeweiligen Zaehler fuer die Ausgabe angepasst */
				switch(badegastAktuell->kartenTyp) {
					case 120: {
						zweiStundenkarte--;
						plusEineStunde++;
						break;
					}
					case 180: {
						plusEineStunde--;
						plusZweiStunden++;
						break;
					}
					case 240: {
						plusZweiStunden--;
						tagesKarte++;
						break;
					}
				}
				
				/* Der neue Kartentyp wird gespeichert */
				badegastAktuell->kartenTyp += 60;
			}
		}
		
		/* Anschliessend wird zum naechsten Badegast gewechselt */
		badegastAktuell = badegastAktuell->danach;
	}
}

/* Funktion berechnet die Wahrscheinlichkeit ob ein Badegast in dieser Minute sich entscheidet zu gehen */
int gehenOderBleiben(int simMinute) {
	int wahrscheinlichkeit;
	int vergleichswert;
	int entscheidung;
	
	/* Ist der Badegast gerade nicht auf der Rutsche oder einer Liege kann er sich entscheiden zu gehen */
	if(badegastAktuell->ereignisTyp == 0) {
		/* Der Vergleichwert fuer die Wahrscheinlichkeit das ein Gast geht steigt mit fortschreitender Stunde */
		if(simMinute <= 420) vergleichswert = badegastAktuell->kartenTyp - 15;
		if(simMinute > 420) vergleichswert = badegastAktuell->kartenTyp - 30;
		if(simMinute > 480) vergleichswert = badegastAktuell->kartenTyp - 45;
		if(simMinute > 540) vergleichswert = badegastAktuell->kartenTyp - 60;
		
		/* Es wird eine Zufallszahl bestimmt die maximal der Verweildauer des Badegastes entspricht */
		wahrscheinlichkeit = zufallszahl(simMinute - badegastAktuell->ankunftsZeit + 1);
		
		/* Da Badegaeste, die mit dem Bus kamen, bedenken haben das der letzte Bus zu voll werden koennte */
		/* entscheiden sich alle schon dafuer zum vorletzten Bus an die Haltestelle zu gehen */
		if(simMinute == 593 && badegastAktuell->ankunftsTyp == 0) {
			entscheidung = 1;
		}
		
		/* Wenn das Schwimmbad schliesst entscheiden sich auch die letzten Gaeste dafuer zu gehen */
		else if(simMinute == 660) {
			entscheidung = 1;
		}
		
		/* Ansonsten wird im Tagesverlauf einfach der Wahrscheinlichkeitswert mit dem Vergleichswert abgeglichen */
		/* Ist die Wahrscheinlichkeit hoeher als der Vergleichswert, entscheidet sich der Badegast dafuer zu gehen */
		else {
			entscheidung = (wahrscheinlichkeit > vergleichswert) ? 1 : 0;
		}
	}
	else {
		entscheidung = 0;
	}
	
	return entscheidung;
}

/* Funktion simuliert die Abreise der Badegaeste */
void abreise(int simMinute) {
	int ankunftsZeit;
	
	badegastAktuell = badegastAnfang;
	
	/* Schleife laeuft einmal durch alle Badegaeste */
	while(badegastAktuell != NULL) {
		
		/* Die Wahrscheinlichkeit dass ein Gast gehen moechte wird von der gehenOderBleiben Funktion ermittelt */
		if(gehenOderBleiben(simMinute)) {
			
			/* Gaeste die mit dem Bus kamen, gehen zurueck zur Bushaltestelle */
			if(badegastAktuell->ankunftsTyp == 0) {
				busHaltestelle++;
				badegastFreilassen();
				badegaesteAktuelleMenge--;
			}
			
			/* Gaeste die mit dem selben Auto kamen, gehen gemeinsam und machen einen Parkplatz frei */
			/* Dafuer wird die Ankunftszeit zwischengespeichert und mit den anderen Gaesten abgeglichen */
			else if(badegastAktuell->ankunftsTyp == 1) {
				ankunftsZeit = badegastAktuell->ankunftsZeit;
				if(autoParkplatz > 0) {
					autoParkplatz--;
				}
				
				/* Hat sich ein Gast der mit Auto kam dazu entschieden zu gehen, wird anhand des Ankunfttyps und der Ankunftszeit geprueft ob vor ihm in */
				/* der Liste Mitfahrer sind und die Liste zurueckgelaufen. Da nur ein Auto pro Minute auf dem Parkplatz erscheint ist der Abgleich eindeutig */
				while(badegastAktuell->davor != NULL && (badegastAktuell->davor)->ankunftsTyp == 1 && (badegastAktuell->davor)->ankunftsZeit == ankunftsZeit) {
					badegastAktuell = badegastAktuell->davor;
				}
				
				/* Beim ersten Mitfahrer angekommen laeuft die Schleife in der Liste wieder vorwaerts */
				while(badegastAktuell != NULL) {
					
					/* Gehoert der Aktuelle Badegast zum jeweiligen Auto wird er aus der Liste entfernt, ansonsten wird die Schleife abgebrochen */
					/* Das ist wichtig da durch das entfernen des ersten Elements in der Liste die Schleife schon auf einem Gast stehen koennte der nicht zum Auto gehoert */
					if(badegastAktuell->ankunftsTyp == 1 && badegastAktuell->ankunftsZeit == ankunftsZeit) {
						
						/* Lag der Gast gerade auf einer Liege wird die Liege wieder frei */
						if(badegastAktuell->ereignisTyp == 1) {
							liegenBelegt--;
						}
						badegastFreilassen();
						badegaesteAktuelleMenge--;
					}
					else {
						break;
					}
					
					/* Sollte der entfernte Badegast der erste in der Liste gewesen sein, wird kein Element weiter gesprungen da durch das entfernen schon der */
					/* naechste Badegast ausgewaehlt ist. Ist der Aktuelle Badegast nicht der Anfang wird noch geprueft ob der naechste Badegast zu dem Auto gehoert */
					if(badegastAktuell != badegastAnfang) {
						if(badegastAktuell->danach != NULL && (badegastAktuell->danach)->ankunftsTyp == 1 && (badegastAktuell->danach)->ankunftsZeit == ankunftsZeit) {
							badegastAktuell = badegastAktuell->danach;
						}
					}
				}
			}
			
			/* Fussgaenger gehen direkt nach Hause */
			else if(badegastAktuell->ankunftsTyp == 2) {
				badegastFreilassen();
				badegaesteAktuelleMenge--;
			}
			
			/* Auch hier wird geprueft ob der Anktuelle Badegast auf dem Anfang steht */
			/* Wenn ja wurde der urspruengliche Anfang entfernt und das naechste Element ist schon ausgewaehlt */
			if(badegastAktuell != badegastAnfang) {
				badegastAktuell = badegastAktuell->danach;
			}
		}
		
		/* Hat sich der Aktuelle Gast nicht dazu entschieden zu gehen wird einfach zum naechsten Gast weitergegangen */
		else {
			badegastAktuell = badegastAktuell->danach;
		}
	}
}

/* Funktion bestimmt wo sich der Bus befindet und nimmt zu den jeweiligen Zeiten die Fahrgaeste mit */
void busHaltestellenVerwaltung(int simMinute) {
	
	/* Uebergibt je nach Uhrzeit den aktuellen Zustand vom Bus */
	busZustand = 1;
	if(((simMinute - 20) % 30 == 0 && simMinute <= 610) || simMinute == 610) busZustand = 2;
	if(((simMinute - 21) % 30 == 0 && simMinute <= 611) || simMinute == 611) busZustand = 3;
	if(((simMinute - 22) % 30 == 0 && simMinute <= 612) || simMinute == 612) busZustand = 3;
	if(((simMinute - 23) % 30 == 0 && simMinute <= 613) || simMinute == 613) busZustand = 4;
	
	/* laesst aller 30 Minuten maximal 50 Personen mit dem Bus abholen, letzte Runde 19:13 Uhr */
	if(((simMinute - 23) % 30 == 0 && simMinute <= 613) || simMinute == 613) {
		if(busHaltestelle > MAX_BUS) {
			busHaltestelle -= 50;
		}
		else {
			busHaltestelle = 0;
		}
	}
}

/* Funktion fuegt die eingelassenen Badeg�ste zur Liste der Badegaeste hinzu */
int badegastHinzufuegen(int simMinute, int ankunftsTyp, int kartenTyp) {
	
	/* Reserviert Speicher fuer einen neuen Badegasteintrag */
	if(!(badegastAktuell = (Badegast* ) malloc(sizeof(Badegast)))) {
		/* Hier folgt die Fehlerbehandlung */
		fehler = 2;
		return 0;
	}
	
	/* Erhoeht die Zaehler fuer die aktuelle Auslastung und die Gesamtgaestemenge */
	badegaesteGesamtMenge++;
	badegaesteAktuelleMenge++;
	
	/* Uebergibt die Startwerte dem Badegasteintrag */
	badegastAktuell->index = badegaesteGesamtMenge;
	badegastAktuell->ankunftsZeit = simMinute;
	badegastAktuell->ankunftsTyp = ankunftsTyp;
	badegastAktuell->kartenTyp = kartenTyp;
	badegastAktuell->ereignisTyp = 0;
	badegastAktuell->ereignisZeit = 0;
	badegastAktuell->davor = badegastEnde;
	badegastAktuell->danach = NULL;
	
	/* Haengt den Badegast ans Ende der Liste */
	if(badegastAnfang == NULL) {
		badegastAnfang = badegastAktuell;
	}
	else {
		badegastEnde->danach = badegastAktuell;
	}
	badegastEnde = badegastAktuell;
	
	return 0;
}

/* Funktion loescht den aktuellen Badegast aus der doppelt verketteten Liste */
void badegastFreilassen() {	
	Badegast *hilfsZeiger = NULL;
	
	/* Ist ueberhaupt ein Element in der Liste */
	if(badegastAnfang != NULL) {
			
		/* Ist das Element der Anfang */
		if(badegastAktuell->davor == NULL) {
			badegastAktuell = badegastAnfang->danach;
			
			/* Wenn es kein Folgeelement gibt, Anfang freigeben und Zeiger auf NULL setzen */
			if(badegastAktuell == NULL) {
				free(badegastAnfang);
				badegastAnfang = NULL;
				badegastEnde = NULL;
			}
			
			/* Ansonsten Anfang freigeben und aktuell aufs naechste Element setzen */
			else {
				badegastAktuell->davor = NULL;
				free(badegastAnfang);
				badegastAnfang = badegastAktuell;
			}
		}
		
		/* Ist das Element das Ende der Liste, Ende Freigeben und neu belegen */
		else if(badegastAktuell->danach == NULL) {
			badegastAktuell = badegastEnde->davor;
			badegastAktuell->danach = NULL;
			hilfsZeiger = badegastEnde;
			badegastEnde = badegastAktuell;
			free(hilfsZeiger);
		}
		
		/* Ist das Element irgendwo in der Mitte, Element freigeben und Zeiger umbiegen */
		else {
			(badegastAktuell->davor)->danach = badegastAktuell->danach;
			(badegastAktuell->danach)->davor = badegastAktuell->davor;
			hilfsZeiger = badegastAktuell;
			badegastAktuell = badegastAktuell->davor;
			free(hilfsZeiger);
		}
	}
}

/* Funktion ermittelt ob ein Badegast ein bestimmtes Ereignis erhaelt */
void ereignisWahl(int simMinute) {
	int wahrscheinlichkeit, ereignis;
	int liegeZeit;
	
	/* Ist ueberhaupt ein Element in der Liste */
	if(badegastAnfang != NULL) {
		
		/* Eine halbe Stunde vor Schliessung des Schwimmbads beginnt das Personal die Liegen */
		/* abzuwischen und nur noch die schon an der Rutsche anstehenden Badegaeste durchzuwinken */
		if(simMinute <= 630) {
			badegastAktuell = badegastAnfang;
	
			/* Schleife laeuft einmal ueber alle Badegaeste */
			while(badegastAktuell != NULL) {
				
				/* Gaeste die mit dem Bus fahren muessen wollen ab 18:30 Uhr kein */
				/* Ereignis mehr antreten damit sie puenktlich zu ihrem Bus kommen */
				if(badegastAktuell->ankunftsTyp == 0 && simMinute >= 570) {
					return;
				}
				else {
					/* Wenn der Badegast keinem Ereignis (1,2,3) zugeordnet ist besteht */
					/* eine Wahrscheinlichkeit von 1/1000 dass er ein Ereignis zugeordnet bekommt */
					if(badegastAktuell->ereignisTyp == 0) {
						wahrscheinlichkeit = zufallszahl(1001);
						
						/* Hat der Badegast ein Ereignis gezogen wird ermittelt welches Ereignis eintritt */
						if(wahrscheinlichkeit == 0) {
							ereignis = zufallszahl(3) + 1;
						}
						else {
							ereignis = 0;
						}
						
						/* Sind alle Liegen belegt kann der Gast keine Liege besetzen */
						if(ereignis == 1 && liegenBelegt >= MAX_LIEGEN) {
							return;
						}
						
						/* Ist eine Liege frei wird eine zufaellige Liegedauer */
						/* 10-30 Minuten(600-1800 Sekunden) bestimmt und zugewiesen */
						else if(ereignis == 1) {
							liegeZeit = zufallszahl(1200) + 600;
							badegastAktuell->ereignisTyp = ereignis;
							badegastAktuell->ereignisZeit = liegeZeit;
							liegenBelegt++;
						}
						
						/* Ansonsonsten wird ihm ein Rutschereignis zugeordnet */
						else {
							badegastAktuell->ereignisTyp = ereignis;
							badegastAktuell->ereignisZeit = 10;
						}
					}
				}
				
			/* Anschliessend geht die Schleife weiter zum naechsten Gast */
			badegastAktuell = badegastAktuell->danach;	
			}
		}
	}
}

/*++++++++++++++++++++++++++++++++++++++++++++++*/
/* Uebergangsfunktion zum beenden der Ereignisse */
void ereignisAblauf() {
	if(badegastAnfang != NULL) {
		badegastAktuell = badegastAnfang;

		while(badegastAktuell != NULL) {
					
			if(badegastAktuell->ereignisTyp == 2 || badegastAktuell->ereignisTyp == 3) {
				if(badegastAktuell->ereignisZeit > 0) {
					badegastAktuell->ereignisZeit -= 1;
				}
				else if(badegastAktuell->ereignisZeit == 0) {
					badegastAktuell->ereignisTyp = 0;
				}
			}
			badegastAktuell = badegastAktuell->danach;
		}
	}
}

/* Funktion laesst die Gaeste fuer eine bestimmte Zeit auf den Liegen verweilen */
void liegenAblauf() {
	
	/* Ist ueberhaupt ein Element in der Liste */
	if(badegastAnfang != NULL) {
		badegastAktuell = badegastAnfang;
		
		/* Schleife laeuft einmal ueber alle Badegaeste */
		while(badegastAktuell != NULL) {
			
			/* Prueft ob der Gast auf einer Liege liegt */
			if(badegastAktuell->ereignisTyp == 1) {
				
				/* Prueft ob er noch verbleibende Liegezeit hat, wenn ja wird eine Sekunde */
				/* abgezogen wen nicht wird der Ereignistyp wieder auf 0 gesetzt */
				if(badegastAktuell->ereignisZeit > 0) {
					badegastAktuell->ereignisZeit -= 1;
				}
				else if(badegastAktuell->ereignisZeit == 0) {
					badegastAktuell->ereignisTyp = 0;
					liegenBelegt--;
				}
			}
			
			/* Anschliessend geht die Schleife weiter zum naechsten Gast */
			badegastAktuell = badegastAktuell->danach;
		}
	}
}

/* Funktion laesst die Badegaeste ein Becken auswaehlen */
void schwimmbeckenWahl() {
	int auswahl, wahrscheinlichkeit;
	
	/* Becken werden zu Beginn wieder auf 0 gesetzt */
	kinderbecken = 0;
	schwimmerbecken = 0;
	aussenbecken = 0;
	
	badegastAktuell = badegastAnfang;
	
	/* Schleife laeuft einmal ueber alle Badegaeste */
	while(badegastAktuell != NULL) {
		
		/* Wenn der Badegast noch keinem anderen Ereignis zugeordnet ist, sucht er sich ein Becken aus */
		if(badegastAktuell->ereignisTyp == 0) {
			
			/* Die Gesamtkapazitaet der Becken (350) wurde durch 25 geteilt */
			/* Davon wird eine Zufallszahl generiert */
			wahrscheinlichkeit = zufallszahl(MAX_AUSLASTUNG / 25);
			
			/* 2 Zahlen (0/1) haben die Wahrscheinlichkeit auf Auswahl 0 (Kinderbecken) */
			if(wahrscheinlichkeit < 2) {
				 auswahl = 0;
			}
			
			/* 7 Zahlen (7-13) haben die Wahrscheinlichkeit auf Auswahl 2 (Aussenbecken) */
			else if(wahrscheinlichkeit > 6) {
				auswahl = 2;
			}
			
			/* Die restlichen 5 Zahlen (2-6) haben die Wahrscheinlichkeit auf Auswahl 1 (Schwimmerbecken) */
			else {
				auswahl = 1;
			}
			
			/* Anschliessend wird die Auswahl auf die Vordefinierten Becken gelegt und der Zaehler erhoeht */
			/* Ist ein Becken schon voll geht der Switch in den Default-Fall bis der Gast ein freies Becken findet */
			switch(auswahl) {
				case 0: if(kinderbecken < MAX_KINDERBECKEN) kinderbecken++; break;
				case 1: if(schwimmerbecken < MAX_SCHWIMMERBECKEN) schwimmerbecken++; break;
				case 2: if(aussenbecken < MAX_AUSSENBECKEN) aussenbecken++; break;
				default: {
					switch(auswahl) {
						case 1: if(kinderbecken < MAX_KINDERBECKEN) kinderbecken++; break;
						case 2: if(schwimmerbecken < MAX_SCHWIMMERBECKEN) schwimmerbecken++; break;
						case 0: if(aussenbecken < MAX_AUSSENBECKEN) aussenbecken++; break;
						default: {
							switch(auswahl) {
								case 2: if(kinderbecken < MAX_KINDERBECKEN) kinderbecken++; break;
								case 0: if(schwimmerbecken < MAX_SCHWIMMERBECKEN) schwimmerbecken++; break;
								case 1: if(aussenbecken < MAX_AUSSENBECKEN) aussenbecken++; break;
								default: fehler = 3; break;
							}
							break;
						}
					}
					break;
				}
			}
		}
		
	/* Anschliessend geht die Schleife weiter zum naechsten Gast */
	badegastAktuell = badegastAktuell->danach;	
	}
}

/* Funktion generiert eine Zufallszahl */
int zufallszahl(int maximum) {
	return rand() % maximum;
}

/* Funktion nimmt Eingaben ohne Enter entgegen und verarbeitet sie */
void eingabeVerarbeitung(int simMinute, int *simGeschwindigkeit) {
	const char *erlaubteEingabe = "tTpPeE";
	char eingabe = '\0';
	char stopPause = '\0';
	int pauseZaehler = 0;
	
	/* _kbhit() prueft ob eine Taste gedrueckt wurde */
	/* _getch() liest die Taste sofort ohne Eingabetaste */
	if(_kbhit()) {
		eingabe = _getch();
		
		/* Eingabepuffer leeren */
		while(_kbhit()) {
			_getch();
		}
		
		/* Beschleunigt oder Verlangsamt die Simulation um das 10-fache */
		if(eingabe == 'T' || eingabe == 't') {
			if(*simGeschwindigkeit == 1000) {
				*simGeschwindigkeit = 100;
			}
			else {
				*simGeschwindigkeit = 1000;
			}
		}
		
		/* Pausiert die Simulation bis zum erneuten druecken der P-Taste */
		if(eingabe == 'P' || eingabe == 'p') {
			while(stopPause != 'P' && stopPause != 'p') {
				
				/* _kbhit() prueft ob eine Taste gedrueckt wurde */
				/* _getch() liest die Taste sofort ohne Eingabetaste */
				if(_kbhit()) {
					stopPause = _getch();
					
					/* Eingabepuffer leeren */
					while(_kbhit()) {
						_getch();
					}
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
				printf("\nWeiter durch ...(P)ause druecken");
				Sleep(500);
								
				/* Wird E eingegeben, wird die Pause unterbrochen und das Programm beendet sich im naechsten Schritt */				
				if(stopPause == 'e' || stopPause == 'E') {
					break;
				}
			}
		}
		
		/* Beendet das Programm */
		if(eingabe == 'e' || eingabe == 'E' || stopPause == 'e' || stopPause == 'E') {
			badegastAktuell = badegastAnfang;
			
			/* Der reservierte Speicher fuer die Elemente der doppelt Verkettetn Liste wird wieder freigegeben */
			if(badegastAnfang != NULL) {
				while(badegastAktuell != NULL) {	
				badegastAktuell = badegastAktuell->danach;
				free(badegastAnfang);
				badegastAnfang = badegastAktuell;
				}
				badegastEnde = NULL;
			}
			/* Anschliessend wird das Programm beendet */
			exit(0);
		}
		
		/* Prueft ob die Eingabe mit einem Zeichen der erlaubtenEingabe uebereinstimmt, wenn nicht wird darauf hingewiesen */
		if(strchr(erlaubteEingabe, eingabe) == NULL) {
			fehler = 1;
		}
	}
}

/* Funktion zur Ausgabe der simulierten Ergebnisse */
void ausgabeVerarbeitung(int simMinute) {
	system("cls");
	
	printf("Schwimmbad Wasserwesen");
	printf("\n\n2-Stunden-Karten: %10d", zweiStundenkarte);
	printf("%*sRutschen", 30, "");
	printf("\n1 Stunde verlaengert: %6d", plusEineStunde);
	printf("\n2 Stunden verlaengert: %5d", plusZweiStunden);
	printf("%*snormal", 8, "");
	printf("%*sSchwimmring", 26, "");
	printf("\nTageskarten: %15d", tagesKarte);
	printf("%*sauf Treppe: %-3d", 8, "", 0);
	printf("%*sauf Treppe: %*s%-3d", 17, "", 8, "", 0);
	printf("\nRutschennutzungen: %9d", 0);
	printf("%*sRinge im Automaten: %2d", 40, "", 0);
	printf("\n\nLiegen: %2d/80", liegenBelegt);
	printf("%*sBecken", 46, "");
	printf("\n\n[P]: %-3d", autoParkplatz);
	printf("%*sKinder", 28, "");
	printf("%*sSchwimmer", 16, "");
	printf("%*sAussen", 16, "");
	printf("\n(H): %-3d", busHaltestelle);
	printf("%*s%-3d", 28, "", kinderbecken);	/* Kinder */
	printf("%*s%-3d", 19, "", schwimmerbecken);	/* Schwimmer */
	printf("%*s%-3d", 22, "", aussenbecken);	/* Aussen */
	
	/* prueft wo sich der Bus befindet und macht die entsprechende Ausgabe */
	switch(busZustand) {
		case 1: printf("\nBus: Unterwegs"); break;
		case 2: printf("\nBus: Ankunft"); break;
		case 3: printf("\nBus: Wartet"); break;
		case 4: printf("\nBus: Abfahrt"); break;
		default: printf("\nBus: Faellt aus"); break;
	}
	
	/* Zeigt die simulierte Stunde an */
	/* Unter 10 Stunden wird eine 0 ergaenzt */
	if(((simMinute / 60) + 9) < 10){
		printf("\n\n0%d:", (simMinute / 60) + 9);
	}
	else {
		printf("\n\n%d:", (simMinute / 60) + 9);
	}
	
	/* Zeigt die simulierte Minute an */
	/* Unter 10 Minuten wird eine 0 ergaenzt */
	if((simMinute % 60) < 10){
		printf("0%d Uhr", simMinute % 60);
	}
	else {
		printf("%d Uhr", simMinute % 60);
	}
	
	printf("\nAktuelle Auslastung: %7d", badegaesteAktuelleMenge);
	printf("\nTagesauslastung: %11d", badegaesteGesamtMenge);
	printf("\n...(T)urbo");
	printf("\n...(P)ause");
	printf("\n...(E)nde");
}

/* Funktion erzeugt bei Fehlern eine Ausgabe in der Konsole */
void fehlerAusgabe() {
	switch(fehler) {
		case 1: printf("\n\nKeine Gueltige Eingabe"); break;
		case 2: printf("\n\nDatensatz fuer einen weiteren Badegast konnte nicht angelegt werden."); break;
		case 3: printf("Die Maximalbelastung aller Becken wurde ueberstiegen!"); break;
	}
	fehler = 0;
}
