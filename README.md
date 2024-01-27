# schwimmbad_simulator
Ein Programm das den Tagesablauf in einem Schwimmbad simuliert.
Entwickelt in der Programmiersprache C unter dem Standart von ANSI-C.

## Ausführung
Das Programm wurde für Windows Betriebssysteme entwickelt.
Zur Ausführung wird keine zusätzliche Installation benötigt.
Der Start erfolgt über die schwimmbad_simulator.exe

## Annahmen
Für die Simulation wurden bestimmte Annahmen getroffen wie sich die
Badegäste in verschiedenen Situationen verhalten. Die Simulation selber 
läuft von 09:50 Uhr (für den ersten Bus) und 20:00 Uhr. Da die Hauptauslastung 
des Schwimmbads zwischen 12:00 Uhr und 17:00 Uhr liegt, sind die Hauptzeiten 
für die Anreise zwischen 11:00 Uhr und 16:00 Uhr. Davor und danach steigt und 
sinkt die Auslastung linear.

### Busfahrgäste
Der erste Bus kommt schon 09:50 Uhr demzufolge stehen ab dieser Zeit die ersten 
Badegäste vor der Tür, werden aber erst 10:00 Uhr reingelassen. Da es im 
beschaulichen Wasserswesen schon vorgekommen sein soll das Badegäste an der 
Bushaltestelle übernachten mussten, gehen alle Gäste vorsorglich schon zum
vorletzten Bus um 18:53 Uhr. Sollte dieser überfüllt sein bleibt ihnen immer 
noch der letzte Bus der nur 20 Minuten später um 19:13 Uhr fährt. Aus diesem 
Grund ist auch der Bus 17:20 Uhr der letzte in dem noch Gäste sitzen die zum 
Schwimmbad wollen. Schließlich bezahlt man 2 Stunden, da will man wenigstens 
ein einhalb Stunden Spaß für haben.

### Autofahrer
Autofahrer sind relativ flexibel. Hier hängt es auch nicht zwingend am Fahrer,
möchte ein Mitfahrer gehen, müssen alle gehen. Abgesehen davon wollen auch sie 
wenigstens ein einhalb Stunden Zeit zum Baden haben. Die Gäste erscheinen 
also bis maximal 18:30 Uhr mit dem Auto.

### Fussgänger
Fussgänger haben es am einfachsten. Sie kommen und gehen wann sie wollen ohne 
auf andere zu schauen. Aber auch sie wollen wenigstens ein einhalb Stunden Zeit
zum baden und kommen bis maximal 18:30 Uhr zum Schwimmbad.
