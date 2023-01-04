@page _PAGE_Libs_ZSPhysVal_CreatePhysSizes Creation of Physical Sizes

When creating the physical units, we want to take into account that you want to
access the individual instances as intuitively as possible.

It would be nice if you could access the "MilliSeconds" unit as follows:

- Kinematics.Time.ms
- Kinematics.Time.MilliSeconds

For this it would be necessary

- implement a science field class `CPhysScienceFieldKinematiks` which inherits
  from class `CPhysScienceField` and
- create an instance of this class called `Kinematics` as a global variable so
  that it can be accessed by all modules.

Die Klasse `CPhysScienceFieldKinematiks` definiert Instanz Variablen mit Public
Accessors für jeden zu implementierenden Einheitenring.

Für den Einheitenring der physikalischen Größe Time wäre eine Klasse `CPhysSizeKinematicsTime`
zu implementieren, die wiederum eine Instanzvariable `MilliSeconds` definiert und diese mit
Public Accessor von außen zugänglich macht. Damit die Einheit `MilliSeconds` auch über das
Einheiten Symbol `ms` erreicht werden kann, müsste zusätzlich eine Instanzvariable `ms` mit Public
Accessor definiert werden, die als Referenz auf `MilliSeconds` zu initialisieren ist.

Leider funktioniert das nicht so, wie gewünscht, da die Reihenfolge, in der globale, namespace und
Klassenvariable initialisiert werden, nicht genau vorhersagbar und nicht durch den Programmierer
vorgegeben werden kann.

Wird die globale Variable `Kinematics` initialisiert und die Konstruktoren der Klassen `CPhysSize`
usw. aufgerufen, werden Klassenvariable wie z.B. String Konstanten referenziert, die zu diesem
Zeitpunkt noch nicht angelegt erzeugt wurden. Das Programm wird mit einer Zugriffsverletztung beim
Anlegen der Instanz `Kinematics` terminieren.

Die Instanziierung der globalen `Kinematics` Variable "by value" hätte außerdem noch weitere Nachteile.
Zum Einen wird der Konstruktor durchlaufen, noch bevor z.B. der Trace Server instanziiert ist und damit
wäre eine Methoden Trace über die Klasse `CIpcTrcServer` nicht möglich.
Zum Anderen würde die Instanz erst nach Beendigung der main Routine zerstört. Etwaiige auf dem Heap
dynamisch durch `Kinematics` erzeugte Verweise wären damit vor Beending der main Routine noch nicht
zerstört und würde duch den Mem Leak Dumper als Speicherlöcher angezeigt.

Deshalb muss ein Kompromiss zwischen der gewünschten `schönen` und einer auch zu realisierenden
intuitiven Schreibweise gefunden werden.

Man könnte mit namespaces und Klassennamen experimentieren, um folgende Zugriffssyntax zu realiseren:

- Kinematics::Time::ms
- Kinematics::Time::MilliSeconds

Ein Mischmasch wie folgt sollte vermieden werden:

- Kinematics->Time.ms
- Kinematics->Time.MilliSeconds

Ich habe mich für die Beispielimplementierung auf folende Zugriffssyntax festgelegt:

- Kinematics->Time->ms
- Kinematics->Time->MilliSeconds

Wichtig für mich war, dass zur Dereferenzierung der Einheiten innerhalb der Substrings im Code immer
derselbe Separator verwendet werden kann. Statt `.` ist dies nun der Pointer Operator `->`.

Wie bereits oben beschrieben werden die Klassen `CPhysScienceFieldKinematiks` und `CPhysSizeKinematicsTime`
definiert und implementiert und eine globale Variable `Kinematics` definiert. Diese Variable ist jedoch
ein Pointer auf Instanz der Klasse `CPhysScienceFieldKinematiks`, die erst nach Aufruf der main Funktion
angelegt wird. In der Test Applikation für physikalische Größen erledigt dies der Konstruktor der Klasse
`CApplication`. Im Destruktor wird diese Instanz wieder zerstört.

