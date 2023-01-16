@page _PAGE_Libs_ZSDiagram Library ZSDiagram

LayoutProcessing

DataProcessing

    - If new data is set at DiagTrace the most recent DataProcessing is invalidated
      by setting the bit EUpdateData for the DiagTrace object.
      The DiagTrace object informs the Diagram by calling "traceChanged" that data has
      been changed. Depending on the UpdateType of the Diagram the Diagram sets
      flags what has to be newly calculaled.
      In case of a Diagram visible on the screen (UpdateTypeWidget) all processing
      levels need to be done (Layout, Data, Pixmap and Widget).
      The diagram informs all diagram objects linked to the trace that data has been changed
      and that the corresponding processing levels need to be executed.
      For example Curves and Markes need to recalculate their positions depending on the
      changed trace data.
      If one object needs to recalculate the data to update its position all other objects
      need to be redrawn. Otherwise if the grid would be redrawn but not objects on top
      of the grid lines would overlay the lines of the curve object.

PixmapProcessing

WidgetProcessing


Update

If data has been changed and the flags have been set at all objects (incl. the diagram itself)
the Diagrams update method is called to process the changed data.


