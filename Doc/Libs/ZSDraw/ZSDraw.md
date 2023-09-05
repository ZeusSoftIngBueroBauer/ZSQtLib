@page _PAGE_Libs_ZSDraw Library ZSDraw

ZSDraw ist eine Bibliothek die benutzt werden kann, um ein 2D Vektor Mal Programm oder auch um einen grafischen Editor
aehnlich dem Qt Designer zu implementieren.

ZSAppDraw ist ein innerhalb der ZSQtLib verfuegbares 2D Vektor Mal Programm das auf der Bibliothek ZSDraw aufsetzt.

![ZSAppDraw](Libs/ZSDraw/ZSAppDraw.png)

ZSDraw verwendet Klassen des Graphics View Frameworks der Qt Klassenbibliothek.

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

Weltkoordinaten
===============

Auf dem Bildschirm werden die grafischen Objekte immer in Pixel Koordinaten positioniert.
Soll die Zeichnung ein Abbild aus der realen Welt wie z.B. eine technische Zeichung oder eine Landkarte
darstellen, ist eine Transformation in eine Einheit aus dem metrischen System notwendig.

Für die Umrechnung müssen Breite und Höhe der real darzustellenden Fläche im metrischen System
als auch ein Massstab angegeben werden. Diese reale Fläche muss in das Pixel System übernommen werden.

Um Koordinaten aus dem metrischen System umrechnen zu können, muss ein Umrechnungsfaktor definiert werden,
der sich aus dem Verhältnis der realen Masse aus dem metrischen System zur Anzahl der Pixel des Bildes ergibt.

\f[
xFaktor = \frac {{width}_{world}} {{width}_{px}} \\
yFaktor = \frac {{height}_{world}} {{height}_{px}}
\f]

Damit das Bild nicht gestaucht wird, sollten xFaktor und yFaktor identisch sein.

Ein Pixel des Bildschirms hat ein metrische Dimension die in dpi (dots per inch) angegeben wird.
Ein möglicher Wert wäre \f${96.0 * 96.0}_{dpi}\f$.
In Millimeter angegeben entspräche dies \f${3.78 * 3.78}_{dpmm}\f$.
Ein Pixel entspricht also ca. \f$\simeq{0.26}_{mm}\f$.

Um ein DINA4 Blatt mit den Ausmaßen \f${297}_{mm} * {210}_{mm}\f$ exakt auf dem Bildschirm wiederzugeben
müsste das Bild folglich

- die Höhe \f${297}_{mm} * {3.78}_{dpmm} \simeq {1122.66}_{px}\f$ und
- die Breite \f${210}_{mm} * {3.78}_{dpmm} \simeq {793.8}_{px}\f$ besitzen.

\f[
xFaktor = \frac {{210}_{mm}} {{794}_{px}} \simeq 0.26 \frac {mm} {px} \\
yFaktor = \frac {{297}_{mm}} {{1123}_{px}} \simeq 0.26 \frac {mm} {px}
\f]

Oder anders ausgedrückt eine Linie auf dem Blatt Papier beginnend von \f${(100/100)}_{mm}\f$ bis \f${(110/110)}_{mm}\f$
entspräche auf dem Bildchirm einer Linie von \f${(378.0/378.0)}_{px}\f$ bis \f${(415.8/415.8)}_{px}\f$.

Eine Linie mit realen Weltkoordinaten kann niemals exakt auf dem Bildschirm wiedergegeben werden.
Das muss beim Malen auf dem Bilschirm mit der Maus berücksichtigt werden.

Wird ein Objekt malerisch erzeugt, besitzt es Pixel-Koordinaten, die in Welt-Koordinaten umgerechnet werden können,
diese entsprechen aber sehr wahrscheinlich nie den tatsächlich gewünschten Massen der realen Welt.

Da die grafischen Objekte z.B. für eine technische Zeichnung automatisch bemasst werden sollen, müssen die Positionen
und Abmessungen für das metrische System nach dem Zeichnen manuell eingegeben werden.

Nach jeder grafischen Operation am Objekt mit der Maus (verschieben, skalieren, rotieren) müssen ggf. Position,
Höhe und Breite sowie Drehwinkel manuell für das metrische Einheiten-System korrigiert werden.

Das PageSetup definiert die Ausmasse der Zeichnung. Es kann hier die Bilgröße angegeben werden und - falls eine
Transformation in das metrische System mit Weltkoordinaten erforderlich ist - Breite und Höhe in einer Einheit
aus dem metrischen System (z.B. mm, cm, m, km, etc.).

Über ein Flag kann gesteuert werden, ob nach Änderung eines Wertes versucht werden soll, x und y Faktor identisch zu halten.


Erzeugen von Objekten
=====================

Um die Erstellung benutzerdefinierter grafischer Objekte zu erlauben, wird das Design Pattern der Objekt Faktories verwendet.
Für jeden grafischen Objekt Typ muss eine Objekt Faktory implementiert und mit einem eindeutigen Namen registriert werden.
Objekt Faktories werden wie die grafischen Objekt Typen in logische Gruppen geordnet.

Auch für die innerhalb der ZSDraw Bibliothek vordefinierten grafischen Objekt Typen werden Objekt Faktories registriert,
die in die beiden Gruppen "Standard Shapes" und "Connections" unterteilt werden.

Zu den "Standard Shapes" gehören die grafischen Objekt Klassen Point, Line, Rect, Ellipse, Polyline, Polygon und Text.
Zur Gruppe "Connections" zählen die Klassen ConnectionLines und ConnectionPoints.

Jede registrierte Objekt Faktory wird mit Gruppen und Typ Name in einer Klassenvariable der Klasse CObjFactory gespeichert.

Um ein Objekt auf der Zeichenfläche zu erstellen, muss zunächst das Zeichen Werkzeug ausgewählt werden. Die Auswahl eines
Zeichen Werkzeugs entspricht dabei der Auswahl einer Objekt Factory. Wird z.B. das Werkzeug "Linie Zeichnen" ausgewählt,
wird über die Methode CDrawingScene::setCurrentDrawingTool("Standard Shapes", "Line") die zugehörige Objekt Faktory aktiviert.

Implizit werden für die Faktory "Standard Shapes - Line" folgende Modi in der Drawing Scene eingestellt:

- Mode: Edit
- EditTool: CreateObjects
- EditMode: Creating
- EditResizeMode: None

Mit einem Maus Klick in die Zeichenfläche wird die Methode "createGraphObj" der Objekt Factory aufgerufen und dabei
die aktuelle Maus Position und die zu verwendenden Mal Attribute wie Liniendicke, Linienfarbe etc. übergeben.
Da in der Regel das neu erzeugte grafische Objekt durch weitere Maus Bewegungen noch in der Form angepasst werden
(Linien End Punkt wird durch Mouse Move verschoben und mit Maus Release endgültig festgelegt) wird das neu erzeugte
Objekt als Objekt "under construction" in den Instanzvariablen "m_pGraphObjCreating" und "m_pGraphicsItemCreating"
gespeichert. Beide Instanzvariablen verweisen auf das selbe Objekt, werden aber doppelt gehalten, um "dynamic_cast"
Aufrufe zwischen den Klassen "CGraphObj" und "QGrahicsItem" überflüssig zu machen.

Nachfolgende Maus Events werden and das "Creating" Objekt geschickt werden.

Über den Methodenaufruf "CDrawingScene::addGraphObj" wird das neu erzeugte Objekt über "QGraphicsScene::addItem" der
GrahicsScene und außerdem mit "CIdxTree::add" das "CGraphObj" dem Index Tree grafischer Objekte hinzugefügt.

Über die Methodenaufrufe "CGraphObj::setEditMode(EEditMode::Creating)" wird dem Objekt mitgeteilt, dass es
"under construction" ist und entsprechend auf nachfolgende Maus Events reagieren kann.

Des weiteren wird das Grafik Item selektiert über den Methoden Aufruf "QGrahicsItem::setSelected".


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
