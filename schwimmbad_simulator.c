/* Schwimmbadsimulator */
/* Ein Konsolen Programm dass die */
/* Tagesauslastung in einem Schwimmbad simuliert */
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
#define MAX_KINDERBECKEN	50
#define MAX_SCHWIMMERBECKEN	125
#define MAX_AUSSENBECKEN	175

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
int kinderbecken = 0;
int schwimmerbecken = 0;
int aussenbecken = 0;

int fehler = 0;

/* Struktur f�r einen Badegast */
typedef struct Badegast{
	int index;
	int ankunftsZeit;
	int ankunftsTyp;		/* 0 = Bus, 1 = Auto, 2 = Fu�g�nger */
	int kartenTyp;			/* 120 = 2h, 180 = 1h verl., 240 = 2h verl., 300 = Tageskarte */
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
int eintrittskarteKaufen(int menge);
void eintrittskarteVerlaengern(int simMinute);
int gehenOderBleiben(int simMinute);
void abreise(int simMinute);
void busHaltestellenVerwaltung(int simMinute);
int badegastHinzufuegen(int simMinute, int ankunftsTyp, int kartenTyp);
int badegaesteDurchsuchen();
void badegastFreilassen();
void schwimmbeckenWahl();
int zufallszahl(int maximum);
void eingabeVerarbeitung(int simMinute, int *simGeschwindigkeit);
void ausgabeVerarbeitung(int simMinute);

void fehlerAusgabe();

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
					
			/* Badeg�ste tretten die Heimreise an */
			abreise(simMinute);
			
			/* L�sst die Badeg�ste einzeln ins Schwimmbad */
			badegaesteEinlass(simMinute);
			
			/* Pr�ft ob ein Gast seine Eintrittskarte verl�ngern muss */
			eintrittskarteVerlaengern(simMinute);
		
			/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			/* Hier werden die Funktionen aufgerufen die jede Minute ausgef�hrt werden */
			
			
			
			
			
			
			
			
			
			/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			
			/* Schleifeninhalt wird jede simulierte Sekunde aufgerufen (entsprechen 1/60 Realsekunden) */
			for(simSekunde = 0; simSekunde < 60; simSekunde++) {
				/* ------------------------------------------------------------------------ */
				/* Hier werden die Funktionen aufgerufen die jede Sekunde ausgef�hrt werden */
			
			
			
			
			
			
			
			
			
				/* ------------------------------------------------------------------------ */
			}
					
			/* Badeg�ste suchen sich ein Schwimmbecken raus */
			schwimmbeckenWahl();
		}
		
		/* K�mmert sich um die Verwaltung der Bushaltestelle */
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

/* Funktion Simuliert die Anreise der Badeg�ste */
void anreise(int simMinute) {
	int wahrscheinlichkeit;
	
	/* Alle 30 Minuten bringt der Bus neue Badeg�ste */
	/* Erster Bus 9:50 Uhr, letzter Bus 19:10 Uhr */
	if(((simMinute - 20) % 30 == 0 && simMinute <= 610) || simMinute == 610) {
		/* Der Bus bringt nur soviele G�ste, dass die maximale */
		/* Auslastung des Schwimmbads nicht �berschritten werden kann */
		if(badegaesteAktuelleMenge <= (MAX_AUSLASTUNG - MAX_BUS - MAX_AUTO - 1)) {
			busAnreise(simMinute);
		}
	}

	/* Anreise ab �ffnung des Schwimmbads */
	if(simMinute >= 60) {
		
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
		/* Au�erdem wird mit einbezogen ob schon die Maximale Menge m�glicher Fussg�nger erreicht wurde */
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
	/* Maximal 50 in den Zeiten von 11:00 Uhr und 16:00 Uhr */
	if(simMinute < 60) busFahrgaeste = zufallszahl(10) + 10;
	if(simMinute >= 60 && simMinute < 90) busFahrgaeste = zufallszahl(10) + 20;
	if(simMinute >= 90 && simMinute < 120) busFahrgaeste = zufallszahl(10) + 30;
	if(simMinute >= 120 && simMinute < 420) busFahrgaeste = zufallszahl(10) + 40;
	if(simMinute >= 420 && simMinute < 450) busFahrgaeste = zufallszahl(10) + 30;
	if(simMinute >= 450 && simMinute < 480) busFahrgaeste = zufallszahl(10) + 20;
	if(simMinute >= 480 && simMinute < 510) busFahrgaeste = zufallszahl(10) + 10;
}

/* Funktion zur Generierung der Anzahl der im Auto anreisenden Badeg�ste */
void autoAnreise(int simMinute){
	/* Je nach Uhrzeit kommen mehr oder weniger G�ste mit dem Auto */
	/* Maximal 5 in den Zeiten von 11:00 Uhr und 16:00 Uhr */
	if(simMinute >= 60 && simMinute < 90) autoBelegung = zufallszahl(1) + 2;
	if(simMinute >= 90 && simMinute < 120) autoBelegung = zufallszahl(1) + 3;
	if(simMinute >= 120 && simMinute < 420) autoBelegung = zufallszahl(1) + 4;
	if(simMinute >= 420 && simMinute < 450) autoBelegung = zufallszahl(1) + 3;
	if(simMinute >= 450 && simMinute < 480) autoBelegung = zufallszahl(1) + 2;
	if(simMinute >= 480 && simMinute < 570) autoBelegung = zufallszahl(1) + 1;
}

/* Funktion der zu Fuss anreisenden Badeg�ste */
void zuFussAnreise(int simMinute){
	/* Fussg�nger kommen je nach belieben */
	if(simMinute < 570) {
		fussgaengerJa++;
		fussgaengerGesamt++;
	}
}

/* Funktion f�gt die angereisten Personen den Badeg�sten hinzu */
void badegaesteEinlass(int simMinute) {
	int kartenTyp;
	
	/* F�r jeden angereisten Gast wird die badegastHinzuf�gen Funktion aufgerufen */
	/* und ihr die Anreisezeit, den Anreisetyp sowie die gekaufte Karte mitgegeben */
	
	/* Busg�ste erhalten Anreisetyp 0 */
	for(; busFahrgaeste > 0; busFahrgaeste--) {
		kartenTyp = eintrittskarteKaufen(1);
		badegastHinzufuegen(simMinute, 0, kartenTyp);
	}
	
	/* Kommen Badeg�ste mit dem Auto wird ein Parkplatz belegt, */
	/* alle erhalten die gleiche Karte und Anreisetyp 1 */
	if(autoBelegung > 0) {
		kartenTyp = eintrittskarteKaufen(autoBelegung);
		autoParkplatz++;
		for(; autoBelegung > 0; autoBelegung--) {
			badegastHinzufuegen(simMinute, 1, kartenTyp);
		}
	}
	
	/* Fussg�nger erhalten den Anreisetyp 2 */
	for(; fussgaengerJa > 0; fussgaengerJa--) {
		kartenTyp = eintrittskarteKaufen(1);
		badegastHinzufuegen(simMinute, 2, kartenTyp);
	}
}

/* Funktion ermittelt welche Eintrittskarte die G�ste kaufen */
int eintrittskarteKaufen(int menge) {
	int wahrscheinlichkeit;
	
	/* Es besteht eine Wahrscheinlichkeit von 1:5 f�r Tageskarte/2H-Karte */
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

/* Funktion sorgt daf�r das alle G�ste die l�nger bleiben als ihre Karte g�ltig ist, ihre Karte verl�ngern */
void eintrittskarteVerlaengern(int simMinute) {
	int vergangeneZeit;
	
	badegastAktuell = badegastAnfang;
	
	/* Schleife l�uft einmal durch alle Badeg�ste */
	while(badegastAktuell != NULL) {
		
		/* Wenn der Gast keine Tageskarte(300) hat wird gepr�ft ob er verl�ngern muss */
		if(badegastAktuell->kartenTyp != 300) {
			
			/* Ermittelt die seit betreten des Schwimmbads vergangene Zeit */
			vergangeneZeit = simMinute - badegastAktuell->ankunftsZeit;
			
			/* Es wird gepr�ft ob die Aktuelle Karte abgelaufen ist */
			if(badegastAktuell->kartenTyp < vergangeneZeit) {
				
				/* Ist die Karte abgelaufen werden die jeweiligen Z�hler f�r die Ausgabe angepasst */
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
		
		/* Anschlie�end wird zum n�chsten Badegast gewechselt */
		badegastAktuell = badegastAktuell->danach;
	}
}

/* Funktion berechnet die Wahrscheinlichkeit ob ein Badegast in dieser Minute sich entscheidet zu gehen */
int gehenOderBleiben(int simMinute) {
	int wahrscheinlichkeit;
	int vergleichswert;
	int entscheidung;
	
	/* Der Vergleichwert f�r die Wahrscheinlichkeit das ein Gast geht steigt mit fortschreitender Stunde */
	if(simMinute <= 420) vergleichswert = badegastAktuell->kartenTyp - 15;
	if(simMinute > 420) vergleichswert = badegastAktuell->kartenTyp - 30;
	if(simMinute > 480) vergleichswert = badegastAktuell->kartenTyp - 45;
	if(simMinute > 540) vergleichswert = badegastAktuell->kartenTyp - 60;
	
	/* Es wird eine Zufallszahl bestimmt die maximal der Verweildauer des Badegastes entspricht */
	wahrscheinlichkeit = zufallszahl(simMinute - badegastAktuell->ankunftsZeit + 1);
	
	/* Da Badeg�ste, die mit dem Bus kamen, bedenken haben das der letzte Bus zu voll werden k�nnte */
	/* entscheiden sich alle schon daf�r zum vorletzten Bus an die Haltestelle zu gehen */
	if(simMinute == 593 && badegastAktuell->ankunftsTyp == 0) {
		entscheidung = 1;
	}
	
	/* Wenn das Schwimmbad schlie�t entscheiden sich auch die letzten G�ste daf�r zu gehen */
	else if(simMinute == 660) {
		entscheidung = 1;
	}
	
	/* Ansonsten wird im Tagesverlauf einfach der Wahrscheinlichkeitswert mit dem Vergleichswert abgeglichen */
	/* Ist die Wahrscheinlichkeit h�her als der Vergleichswert, entscheidet sich der Badegast daf�r zu gehen */
	else {
		entscheidung = (wahrscheinlichkeit > vergleichswert) ? 1 : 0;
	}
	
	return entscheidung;
}

/* Funktion simuliert die Abreise der Badeg�ste */
void abreise(int simMinute) {
	int ankunftsZeit;
	
	badegastAktuell = badegastAnfang;
	
	/* Schleife l�uft einmal durch alle Badeg�ste */
	while(badegastAktuell != NULL) {
		
		/* Die Wahrscheinlichkeit dass ein Gast gehen m�chte wird von der gehenOderBleiben Funktion ermittelt */
		if(gehenOderBleiben(simMinute)) {
			
			/* G�ste die mit dem Bus kamen, gehen zur�ck zur Bushaltestelle */
			if(badegastAktuell->ankunftsTyp == 0) {
				busHaltestelle++;
				badegastFreilassen();
				badegaesteAktuelleMenge--;
			}
			
			/* G�ste die mit dem selben Auto kamen, gehen gemeinsam und machen einen Parkplatz frei */
			/* Daf�r wird die Ankunftszeit zwischengespeichert und mit den anderen G�sten abgeglichen */
			else if(badegastAktuell->ankunftsTyp == 1) {
				ankunftsZeit = badegastAktuell->ankunftsZeit;
				if(autoParkplatz > 0) {
					autoParkplatz--;
				}
				
				/* Hat sich ein Gast der mit Auto kam dazu entschieden zu gehen, wird anhand des Ankunfttyps und der Ankunftszeit gepr�ft ob vor ihm in */
				/* der Liste Mitfahrer sind und die Liste zur�ckgelaufen. Da nur ein Auto pro Minute auf dem Parkplatz erscheint ist der Abgleich eindeutig */
				while(badegastAktuell->davor != NULL && (badegastAktuell->davor)->ankunftsTyp == 1 && (badegastAktuell->davor)->ankunftsZeit == ankunftsZeit) {
					badegastAktuell = badegastAktuell->davor;
				}
				
				/* Beim ersten Mitfahrer angekommen l�uft die Schleife in der Liste wieder vorw�rts */
				while(badegastAktuell != NULL) {
					
					/* Geh�rt der Aktuelle Badegast zum jeweiligen Auto wird er aus der Liste entfernt, ansonsten wird die Schleife abgebrochen */
					/* Das ist wichtig da durch das entfernen des ersten Elements in der Liste die Schleife schon auf einem Gast stehen k�nnte der nicht zum Auto geh�rt */
					if(badegastAktuell->ankunftsTyp == 1 && badegastAktuell->ankunftsZeit == ankunftsZeit) {
						badegastFreilassen();
						badegaesteAktuelleMenge--;
					}
					else {
						break;
					}
					
					/* Sollte der entfernte Badegast der erste in der Liste gewesen sein, wird kein Element weiter gesprungen da durch das entfernen schon der */
					/* n�chste Badegast ausgew�hlt ist. Ist der Aktuelle Badegast nicht der Anfang wird noch gepr�ft ob der n�chste Badegast zu dem Auto geh�rt */
					if(badegastAktuell != badegastAnfang) {
						if(badegastAktuell->danach != NULL && (badegastAktuell->danach)->ankunftsTyp == 1 && (badegastAktuell->danach)->ankunftsZeit == ankunftsZeit) {
							badegastAktuell = badegastAktuell->danach;
						}
					}
				}
			}
			
			/* Fussg�nger gehen direkt nach Hause */
			else if(badegastAktuell->ankunftsTyp == 2) {
				badegastFreilassen();
				badegaesteAktuelleMenge--;
			}
			
			/* Auch hier wird gepr�ft ob der Anktuelle Badegast auf dem Anfang steht */
			/* Wenn ja wurde der urspr�ngliche Anfang entfernt und das n�chste Element ist schon ausgew�hlt */
			if(badegastAktuell != badegastAnfang) {
				badegastAktuell = badegastAktuell->danach;
			}
		}
		
		/* Hat sich der Aktuelle Gast nicht dazu entschieden zu gehen wird einfach zum n�chsten Gast weitergegangen */
		else {
			badegastAktuell = badegastAktuell->danach;
		}
	}
}

/* Funktion bestimmt wo sich der Bus befindet und nimmt zu den jeweiligen Zeiten die Fahrg�ste mit */
void busHaltestellenVerwaltung(int simMinute) {
	
	/* �bergibt je nach Uhrzeit den aktuellen Zustand vom Bus */
	busZustand = 1;
	if(((simMinute - 20) % 30 == 0 && simMinute <= 610) || simMinute == 610) busZustand = 2;
	if(((simMinute - 21) % 30 == 0 && simMinute <= 611) || simMinute == 611) busZustand = 3;
	if(((simMinute - 22) % 30 == 0 && simMinute <= 612) || simMinute == 612) busZustand = 3;
	if(((simMinute - 23) % 30 == 0 && simMinute <= 613) || simMinute == 613) busZustand = 4;
	
	/* l�sst aller 30 Minuten maximal 50 Personen mit dem Bus abholen, letzte Runde 19:13 Uhr */
	if(((simMinute - 23) % 30 == 0 && simMinute <= 613) || simMinute == 613) {
		if(busHaltestelle > MAX_BUS) {
			busHaltestelle -= 50;
		}
		else {
			busHaltestelle = 0;
		}
	}
}

/* Funktion f�gt die eingelassenen Badeg�ste zur Liste der Badeg�ste hinzu */
int badegastHinzufuegen(int simMinute, int ankunftsTyp, int kartenTyp) {
	
	/* Reserviert Speicher f�r einen neuen Badegasteintrag */
	if(!(badegastAktuell = (Badegast* ) malloc(sizeof(Badegast)))) {
		/* Hier folgt die Fehlerbehandlung */
		fehler = 2;
		return 0;
	}
	
	/* Erh�ht die Z�hler f�r die aktuelle Auslastung und die Gesamtg�stemenge */
	badegaesteGesamtMenge++;
	badegaesteAktuelleMenge++;
	
	/* �bergibt die Startwerte dem Badegasteintrag */
	badegastAktuell->index = badegaesteGesamtMenge;
	badegastAktuell->ankunftsZeit = simMinute;
	badegastAktuell->ankunftsTyp = ankunftsTyp;
	badegastAktuell->kartenTyp = kartenTyp;
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

/* Funktion l�scht den aktuellen Badegast aus der doppelt verketteten Liste */
void badegastFreilassen() {	
	Badegast *hilfsZeiger = NULL;
	
	/* Ist �berhaupt ein Element in der Liste */
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
			
			/* Ansonsten Anfang freigeben und aktuell aufs n�chste Element setzen */
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

/* Funktion l�sst die Badeg�ste ein Becken ausw�hlen */
void schwimmbeckenWahl() {
	int auswahl, wahrscheinlichkeit;
	
	/* Becken werden zu Beginn wieder auf 0 gesetzt */
	kinderbecken = 0;
	schwimmerbecken = 0;
	aussenbecken = 0;
	
	badegastAktuell = badegastAnfang;
	
	/* Schleife l�uft einmal �ber alle Badeg�ste */
	while(badegastAktuell != NULL) {
		
		/* Wenn der Badegast noch keinem anderen Ereignis zugeordnet ist, sucht er sich ein Becken aus */
		if(badegastAktuell->ereignisTyp == 0) {
			
			/* Die Gesamtkapazit�t der Becken (350) wurde durch 25 geteilt */
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
			
			/* Anschlie�end wird die Auswahl auf die Vordefinierten Becken gelegt und der Z�hler erh�ht */
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
		
	/* Anschlie�end geht die Schleife weiter zum n�chsten Gast */
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
	
	/* _kbhit() pr�ft ob eine Taste gedr�ckt wurde */
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
		
		/* Pausiert die Simulation bis zum erneuten dr�cken der P-Taste */
		if(eingabe == 'P' || eingabe == 'p') {
			while(stopPause != 'P' && stopPause != 'p') {
				
				/* _kbhit() pr�ft ob eine Taste gedr�ckt wurde */
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
								
				/* Wird E eingegeben, wird die Pause unterbrochen und das Programm beendet sich im n�chsten Schritt */				
				if(stopPause == 'e' || stopPause == 'E') {
					break;
				}
			}
		}
		
		/* Beendet das Programm */
		if(eingabe == 'e' || eingabe == 'E' || stopPause == 'e' || stopPause == 'E') {
			badegastAktuell = badegastAnfang;
			
			/* Der reservierte Speicher f�r die Elemente der doppelt Verkettetn Liste wird wieder freigegeben */
			if(badegastAnfang != NULL) {
				while(badegastAktuell != NULL) {	
				badegastAktuell = badegastAktuell->danach;
				free(badegastAnfang);
				badegastAnfang = badegastAktuell;
				}
				badegastEnde = NULL;
			}
			/* Anschlie�end wird das Programm beendet */
			exit(0);
		}
		
		/* Pr�ft ob die Eingabe mit einem Zeichen der erlaubtenEingabe �bereinstimmt, wenn nicht wird darauf hingewiesen */
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
	printf("%*sRinge im Automaten: %-2d", 40, "", 0);
	printf("\n\nBistro: %2d/20", 0);
	printf("\nLiegen: %2d/80", 0);
	printf("%*sBecken", 46, "");
	printf("\n\n[P]: %-3d", autoParkplatz);
	printf("%*sKinder", 28, "");
	printf("%*sSchwimmer", 16, "");
	printf("%*sAussen", 16, "");
	printf("\n(H): %-3d", busHaltestelle);
	printf("%*s%-3d", 28, "", kinderbecken);	/* Kinder */
	printf("%*s%-3d", 19, "", schwimmerbecken);	/* Schwimmer */
	printf("%*s%-3d", 22, "", aussenbecken);	/* Au�en */
	
	/* pr�ft wo sich der Bus befindet und macht die entsprechende Ausgabe */
	switch(busZustand) {
		case 1: printf("\nBus: Unterwegs"); break;
		case 2: printf("\nBus: Ankunft"); break;
		case 3: printf("\nBus: Wartet"); break;
		case 4: printf("\nBus: Abfahrt"); break;
		default: printf("\nBus: Faellt aus"); break;
	}
	
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
		case 2: printf("\n\nDatensatz f�r einen weiteren Badegast konnte nicht angelegt werden."); break;
		case 3: printf("Die Maximalbelastung aller Becken wurde �berstiegen!"); break;
	}
	fehler = 0;
}
