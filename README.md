# schwimmbad_simulator
Ein Programm das den Tagesablauf in einem Schwimmbad simuliert.
Entwickelt in der Programmiersprache C unter dem Standard von ANSI-C.

## Ausführung
Das Programm wurde für Windows Betriebssysteme entwickelt.
Zur Ausführung wird keine zusätzliche Installation benötigt.
Der Start erfolgt über die schwimmbad_simulator.exe

## Annahmen
Für die Simulation wurden bestimmte Annahmen getroffen wie sich die
Badegäste in verschiedenen Situationen verhalten. Die Simulation selber 
läuft von 09:50 Uhr (für den ersten Bus) und 20:00 Uhr. Da die Hauptauslastung 
des Schwimmbads zwischen 12:00 Uhr und 17:00 Uhr liegt, sind die Hauptzeiten 
für die Anreise zwischen 11:00 Uhr und 16:30 Uhr. Davor und danach steigt und 
sinkt die Auslastung linear. 
Da die die "durchschnittliche" Tagesauslastung 800 Gäste beträgt ist dies keine 
Festgeschriebene Zahl. Stattdessen nähert sich die Tagesauslastung immer der 800
an.

### Busfahrgäste
Der erste Bus kommt schon 09:50 Uhr demzufolge stehen ab dieser Zeit die ersten 
Badegäste vor der Tür, werden aber erst 10:00 Uhr reingelassen. Da es im 
beschaulichen Wasserswesen schon vorgekommen sein soll das Badegäste an der 
Bushaltestelle übernachten mussten, gehen "fast alle" Busfahrgäste vorsorglich 
zum vorletzten Bus um 18:53 Uhr. Sollte dieser überfüllt sein bleibt ihnen immer 
noch der letzte Bus der nur 20 Minuten später um 19:13 Uhr fährt. Vereinzelte
Gäste die noch in einer Schlange stehen oder auf einer Liege liegen, gehen spätestens 
zum letzten Bus. Bus 17:50 Uhr der letzte in dem noch Gäste sitzen die zum Schwimmbad
wollen. 

### Autofahrer
Autofahrer müssen immer auf ihre Mitfahrer achten. Möchte einer Mitfahrer gehen,
müssen alle gehen. Natürlich wird noch gewartet bis jeder seinen letzten Rutsch-
durchlauf beendet hat, bzw von der Liege runter ist. Ab 19:00 Uhr kommen keine 
Gäste mehr mit dem Auto

### Fussgänger
Fussgänger haben es am einfachsten. Sie kommen und gehen wann sie wollen ohne 
auf andere zu schauen. Die Letzten Fußgänger treffen spätestens 19:00 Uhr ein.

### Eintrittskarten
Eintrittskarten werden zufällig gekauft. Ab 16:00 Uhr kauft kein Gast mehr eine 
Tageskarte, da es sich nicht mehr lohnt.

### Ereignisse
Rutschdurchgänge und Liegenbenutzungen werden im Programm als Ereignis bezeichnet.
Ab 19:30 Uhr beginnt das Personal die Liegen abzuwischen, so das nurnoch gewartet
wird bis die liegenden aufstehen, sich aber keiner mehr hinlegen darf. Auch an 
der Rutsche lässt der Bademeister ab 19:30 Uhr nurnoch die schon anstehenden 
Badegäste rutschen.

## CodeStyle (Hinweise)
- Zur Einrückung werden Tabulatoren mit 4 Leerzeichen verwendet
- Sprechend Deutsch deklarieren
- Deutsche Umlaute werden durch bspw. ae, ue oder oe ersetzt
- ß wird zu ss
- Keine Leerzeichen zwischen Aufruf und Argumenten bspw.: main(), if(), ...
- Immer Leerzeichen zwischen Argumenten und Codeblock bspw.: main() {}, if() {}, ...
- Anweisung immer in geschweiften Klammer außer wenn Aufruf und Anweisung auf 
  einer Zeile stehen bspw.: if() Anweisung;
- Kommentieren, kommentieren, kommentieren