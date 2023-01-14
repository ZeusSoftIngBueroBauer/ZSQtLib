@page _PAGE_Libs_ZSPhysVal_CreatePhysSizes Creation of Physical Sizes

When creating the physical units, we want to take into account that you want to
access the individual instances as intuitively as possible.

It would be nice if you could access the "MilliSeconds" unit as follows:

- Kinematics.Time.ms
- Kinematics.Time.MilliSeconds

For this the following steps would be necessary:

1. Implement a class maybe called `CPhysSizeKinematiksTime` which inherits from class `CPhysSize`
   and provides an instance member `MilliSeconds` of type `CPhysUnit` with public access.
   This class also need to provide an instance member `ms` with public access which is a reference
   to `MilliSeconds`.
2. Implement a science field class maybe called `CPhysScienceFieldKinematiks` which inherits
   from class `CPhysScienceField` and provides a member `Time` of type `CPhysSizeKinematiksTime`
   with public access.
3. Create an instance of class `CPhysSizeKinematiksTime` called `Kinematics` as a global variable so
   that it can be accessed by all modules.

Punkt 3 ist dabei problematisch. Die physikalischen Einheitenringe und ihre Einheiten müssen einem
Index Baum hinzugefügt werden mit entsprechenden Parent/Child Verkettungen.

Wird `Kinematics` als globale Variable definiert, muss der Index Baum bereits vorher angelegt worden sein,
damit die phyisalikischen Einheitenringe, die innerhalb `Kinematics` definiert wurden, auch dem Index Baum
hinzugefügt werden können. Das könnte man dadurch erreichen, dass man den Index Tree vor `Kinematics`
definiert. Werden neben `Kinematics` auch weitere Einheiten Größen wie `Geometry` oder `Electricity` benötigt,
müsste man die zugehörigen Klassen `CPhysScienceFieldGeometry` und `CPhysScienceFieldElectricity` innerhalb
eines Moduls instanziieren, nachdem man den Index Baum instanziiert hat.

Dies könnte in einem Modul `Units` wie folgt geschehen:

Header File Units.h:

    extern CIdxTreePhysSizes IdxTreePhysSizes;
    extern CPhysScienceFieldKinematics Kinematics;
    extern CPhysScienceFieldGeometry Geometry;
    extern CPhysScienceFieldElectricity Electricity;

Source File Units.cpp:

    CIdxTreePhysSizes IdxTreePhysSizes;
    CPhysScienceFieldKinematics Kinematics(&IdxTreePhysSizes);
    CPhysScienceFieldGeometry Geometry(&IdxTreePhysSizes);
    CPhysScienceFieldElectricity Electricity(&IdxTreePhysSizes);


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

