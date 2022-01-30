@page _PAGE_Libs_ZSDraw Library ZSDraw

ZSDraw ist eine Bibliothek die benutzt werden kann, um ein 2D Vektor Mal Programm oder auch um einen grafischen Editor
aehnlich dem Qt Designer zu implementieren.

ZSAppDraw ist ein innerhalb der ZSQtLib verfuegbares 2D Vektor Mal Programm das auf der Bibliothek ZSDraw aufsetzt.

![ZSAppDraw](Libs/ZSDraw/ZSAppDraw.png)

ZSDraw verwendet die Klassen QGraphicsScene und QGraphicsView der Qt Klassenbibliothek.

Die Klasse ZS::Draw::CDrawingScene ist von QGraphicsScene abgeleitet.
Die Klasse ZS::Draw::CDrawingView is von QGraphicsView abgeleitet.

Grafische Draw Objekte sind sowohl von QQraphicsItems abgeleitet als auch von der Klassen CGraphObj abgeleitet.
CGraphObj wiederum ist von der Klasse CIdxTreeEntry abgeleitet, da die grafischen Draw Objekte nicht nur dem
Datencontainer QGraphicsScene sondern auch einem Index Tree der von QGraphicsScene abgeleiteten Klasse
CDrawingScene hinzugefügt werden. Über den Index Tree können die grafischen Objekte anhand ihres Namens
referenziert und hierarchisch sortiert werden.

Grafische Draw Objekte können auf drei verschiedene Arten erzeugt werden:

1. Programmatisch durch erzeugen einer von QGraphicsItem und CGraphObj abgeleiten Klasse.

2. Durch Drag & Drop über Meta Informationen auf die grafische Zeichenfläche von QGraphicsScene.

3. Durch Auswahl eines Zeichentools entsprechend dem zu erzeugenden grafischen Objekt und Malen
   des Objekts mit Maus Bewegungen auf der grafischen Zeichenfläche von QGraphicsScene.

Objekte können auf der Zeichenfläche mit der Maus selektiert, gruppiert, verschoben, gedreht oder in ihrer Form
geändert werden. Hierzu zeigt ein selektiertes Objekt Auswahlpunkte an, die von der Maus gepackt und verschoben
werden.

Um ein Objekt grafisch auf der Zeichenfläche zu erzeugen, muss ein Zeichentool ausgewählt werden.
Dabei muss der Drawing Scene auch mitgeteilt werden, dass im Folgenden ein grafisches Objekt
ueber Maus Event erzeugt werden soll. Dies geschieht durch Aufruf der CDrawingScene::setMode Methode,
die mehrere Argumente erwartet.

- Mode
  Enum mit den Werten Edit, Simulation und Ignore.
  Ignore ist zu übergeben, wenn der Mode nicht geändert werden soll und das Argument somit zu ignorieren ist.
  Nur im Edit Mode können Objekte auf der grafischen Oberfläche durch die Maus angelegt und verändert werden.
  Der Simulation Mode dient dazu, die Maus-Events an die Objekte selbst weiterzuleiten, die entsprechend darauf
  reagieren können. Ist z.B. eine ComboBox als Objekt der Empfänger der Maus-Events verarbeitet die ComboBox
  die Events, um z.B. die PullDown-List zu öffnen. Bei dem Objekt kann es sich aber auch um ein vollständig
  benutzerdefiniertes Objekt wie einen grafischen Schalter handeln, der über Maus-Klicks seinen On/Off Zustand
  ändert.

- EditTool
  Enum mit den Werten Select, CreateObjects, Ignore und Undefined.
  Ignore ist zu übergeben, wenn das EditTool nicht geändert werden soll und das Argument somit zu ignorieren ist.
  Undefined zeigt an, dass kein EditTool ausgewählt ist.
  Wird Select übergeben, sollen nachfolgende Maus-Events dazu dienen, Objekte zu selektieren.
  Mit CreateObjects wird angezeigt, dass über nachfolgende Maus-Events Objekte erzeugt werden sollen.

- EditMode
  Enum mit den Werten Creating, Move, Resize, Rotate, MoveShapePoint, EditText, Ignore und Undefined
  Ignore ist zu übergeben, wenn der EditMode nicht geändert werden soll und das Argument somit zu ignorieren ist.
  Undefined zeigt an, dass kein EditMode ausgewählt ist.
  Die anderen EditModes zeigen an, auf welche Art und Weise das Objekt zu modifizieren ist und hängen im wesentlichen
  davon ab, an welchem Eckpunkt (SelectionPoint) das Objekt mit der Maus gepackt wurde.

- EditResizeMode
  Enum mit den Werten ResizeAll, ResizeHor, ResizeVer, Ignore und Undefined.
  Ignore ist zu übergeben, wenn der ResizeMode nicht geändert werden soll und das Argument somit zu ignorieren ist.
  Undefined zeigt an, dass kein ResizeMode ausgewählt ist.

- ObjFactoryTypeChanged
  Dieses Boolsche Flag zeigt an, ob der grafische Object Type und damit die Objekt Factory zur Erzeugung des
  Objekt verändert wurde.

Da die Auswahl der verschiedenen Modis nicht unabhängig voneinender geschieht sondern nur in Kombination mit
anderen Modis Sinn macht, wurden nicht separate Methoden für jeden Mode implementiert sondern das Ändern der
Modes in einer Methode zusammengefasst.

Erzeugen von Objekten
=====================

Grafische Objekte werden über Objekt-Faktories erstellt, die in Gruppen zusammengefasst werden.
Mögliche Gruppen sind z.B.:

- "Standard Shapes" unter die die grafischen Objekt Klassen Point, Line, Rect, Ellipse, Polyline, Polygon und Text zählen.
- "Connections" zu denen die Grafischen Objekt Klassen ConnectionLines und ConnectionPoints zählen.
- Benutzerdefinierte Objekt Klassen wie "Widgets", deren Klassen von QGraphicsProxyWidget abgeleitetet werden können,
  und als Host für Qt Widget Klassen wie QComboBox, QGroupBox, QLineEdit etc. dienen können.
- Benutzerdefinierte Objekt Klassen, die Standard Shapes zu einer Gruppe zusammenfassen, um z.B. einen Transistor,
  einen elektrischen Widerstand, eine Diode, einen Kondensator oder eine Spule darzustellen.

Für jedes auf der Zeichenfläche zu erstellende Objekt muss eine entpsrechende Instanz einer Objekt-Faktory
registriert werden. Dies geschieht durch den Gruppen Namen (wie "Standard Shapes") und dem zu erstellen Element
(wie z.B. "Rect", "Line", "Ellipse"). Die Liste der registrierten Objekt Factories wird als Klassenvariable
innerhalb der Klass CObjFactory verwaltet.

Um den Code innerhalb der Klasse CDrawingScene besser zu verstehen wird im Folgenden beschrieben, wie
grafische Objekte auf der Zeichenfläche mittels Maus erzeugt werden.

1. Point
--------

Ein Point besteht nur aus einem Punkt, der nicht als Polygon behandelt wird.
Das Bounding Rectangle des Punktes ist der Punkt selbst.
Da alle Selection Points des Bounding Rectangles übereinander liegen würden,
wird der Center Point des des Bounding Rectangles verwendet.

Betrachten wir das Malen eines Punktes, das das einfachste, grafische Objekt darstellt.

Über den Aufruf CDrawingScene::setCurrentDrawingTool wird zunächst die Objekt Faktory aktiviert, über die
das Objekt auf der Zeichenfläche zu erstellen ist. Änderte sich die aktivierte Objekt Factory dabei,
wird der Mode der Drawing Scene auf EditTool = CreateObjects, EditMode = Undefined, EditResizeMode = Undefined
und das Flag ObjFactoryTypeChanged auf true gesetzt.

Um das Objekt per Maus zu erzeugen, wird die Maus an die gewünschte Position bewegt, ohne dabei einen
Maus Taste gedrückt zu halten. Da sich momentan kein Objekt under construction befindet, werden die
Maus Events ohne gedrückte Taste ignoriert.

Erreicht man die gewünschte Position und drückt die linke Maus Taste, wird das grafische Objekt "Point"
in der Methode "mousePressEvent" über den Factory Aufruf "createGraphObj" erzeugt und der graphics scene
als graphic item hinzugefügt. Die Drawing Scene fügt das Objekt daraufhin auch in den Index Tree seiner
grafischen Objekte ein.

In der Regel ist ein grafisches Objekt nach dem ersten Maus Click nicht fertig gestellt sondern wird durch
ziehen der Maus bei gedrückter Maustaste noch in der Form geändert und befindet sich somit noch "under construction".
Da das Objekt selbst am besten weiß, wie es über Maus-Events erzeugt wird und z.B. SelectionPoints temporär
erzeugen muss, wird ihm dieser Zustand durch den Aufruf "setEditMode(EEditMode::Creating)" mitgeteilt
und der Mouse Click Event an das Objekt weitergeleitet. Bei Verarbeitung dieses Events kann das Objekt selbst
entsprechend seinem Objekt Typ und seinem aktuellen Zustand darauf reagieren.

Im Falle des Point Objekts ist das Objekt mit dem ersten Maus Click fertig gestellt und in der
"mousePressEvent" Methode des Point Objekts wird bei Empfang des Mouse Clicks Events der Drawing Scene
durch Aufruf von "onGraphObjCreationFinished" mitgeteilt, dass das Objekt fertig gestellt ist.

Wird die Maus Taste nach Erzeugen des Point Objekts gedrückt gehalten und die Maus bewegt, werden
weitere Punkte innerhalb der "mouseMoveEvent" Methode erzeugt. Dies ist eine Besonderheit des grafischen
Point Objekts.

2. Line
-------

3. Rectangle
------------

4. Polyline
-----------

5. Connections
--------------

Selektieren von Objekten
========================

Dies geschieht durch einen Mouse Click auf das Objekt oder durch expliziten Aufruf von QGraphicsItem::select.
Fuer die selektierten Objekte muessen Mouse Button Events gesetzt werden, die es akzeptieren soll, damit die
Qt Graphics Scene den Mouse Click Event unmittelbar als auch nachfolgende Mouse Events an das Item schickt.

Selection Points:
-----------------

Selection Points werden zum einen für jeden Polygon Punkt eines Objekts erzeugt.
Zum Anderen auch für die markanten Punkte des Bounding Rectangles sowie zwei
Rotation Points zentriert jeweils oberhalb und unterhalt des Bounding Rectangles.

Folgende Tabelle zeigt, welche Selection Points erzeugt werden, wenn das jeweilige
grafische Objekt selektiert wird.

Objekt-Type | Polygon | BoundingRectangle | RotationPoints
------------+---------+-------------------+---------------
Point       | -       | Center            | -
Line        | +       | -                 | -
            |         |                   | -

Verschieben von Objekten
========================

Um ein Objekt verschieben zu koennen, muss es vorher selektiert und als Mouse Event Grabber der Scene bekannt
gegeben werden. Damit werden die Mouse Events, die an der Position des Objekt eintreffen, an das Objekt weitergeleitet.
Das Objekt muss die Mouse Move Events entsprechend verarbeiten.

Form Änderungen
===============

Drehen der Objekte
==================

Gruppieren von Objekten
=======================

Löschen der Objekte
===================

Labels
======

Jedem grafischen Objekt können verschiedene Labels zugewiesen werden.
Die Labels werden nach einem Selektion Point ausgerichtet. Welcher Selection Point sinnvoll ist,
hängt zum einen vom Labeltyp als auch vom Typ des grafischen Objekts ab.

Labels selbst haben keine Selektion Points.

Folgende Label-Typen können an ein grafisches Objekt gebunden werden:

- Name und Path Labels zeigen den Namen des Objektes an, wobei das Path-Label auch den Namen des Objektes anzeigt.
  Auch wenn es möglich ist, macht es wenig Sinn, Name und Path Label gleichzeitig anzuzeigen.
  Name und Path Labels können je nach Objekt Typ an folgende Selection Points gebunden werden:
  Point ... BoundingRectangle.CenterPoint
  Group ... BoundingRectangle.All

- Position Labels zeigen die Position von Selection Points des Objektes an.
  Jedem Selection Point(bis auf die Rotation Points, falls existent) kann ein Position Label zugewiesen werden.

- Dimension Line Labels zeigen die Ausmasse des Objektes wie Breite oder Höhe des Bounding Rectangles des Objekts an.
  Die Breite des Bounding Rectangles wird entweder an die obere oder untere Begrenzungslinie des Bounding Rectangles gebunden.
  Die Hoehe des Bounding Rectangles wird entweder an die linke oder rechte Begrenzungslinie des Bounding Rectangles gebunden.
  Der Rotationswinkel ist immer an den Center Point des Bounding Rectangles und an einen der Center Points der Begrenzungslinien gebunden.

- User Defined Labels zeigen benutzerdefinierten Text an und können an jeden existierenden Selection Point
  gebunden werden.

Zwischen Label und Selektion Point existiert eine virtuelle, normalerweise nicht sichtbare Verbindungslinie.
Diese Verbindungslinie wird sichtbar, wenn über das Label "gehoovert" wird oder wenn das Label selektiert wird.
Durch Aufruf von "show<LabelType>" und "hide<LabelType>" können diese Verbindungslinien dauerhaft ein- und
ausgeschaltet werden. Die Verbindungsline wird durch das Label gezeichnet (nicht durch das Objekt, an das das
Label gebunden ist).

Wird das grafische Objekt verschoben, an das das Label gebunden ist, muss der Abstand zwischen Label und
dem grafischen Objekt beibehalten werden. Hierzu speichert das Label den aktuellen Abstand zum Selection
Point des Objekts ab, mit dem das Label verankert ist. Das verschobene Objekt aktualisiert unter Beibehaltung
des Abstands die Position all seiner Labels (updateLabelPositionsAndContents).

Benutzerdefinierte Objekte
==========================

Code-Beispiele
==============

Beide Klassen werden in der Regel im MainWindow der Applikation instanziiert.

Zuvor wird dem MainWindow ein Widget als CentralWidget zugewiesen. Dem Layout des CentralWidget
wird die Drawing View Instanz hinzugefuegt.

    m_pLyt = new QHBoxLayout();
    m_pWdgtCentral = new QWidget();
    m_pWdgtCentral->setLayout(m_pLyt);
    setCentralWidget(m_pWdgtCentral);

    m_pDrawingScene = new CDrawingScene(this);
    m_pDrawingView = new CDrawingView(m_pDrawingScene);
    m_pDrawingView->setMouseTracking(true);
    m_pLyt->addWidget(m_pDrawingView);
