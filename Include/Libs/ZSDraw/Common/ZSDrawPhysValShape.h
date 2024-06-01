/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifndef ZSDraw_PhysValShape_h
#define ZSDraw_PhysValShape_h

#include "ZSDrawDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"

class QPainter;
class QRectF;

namespace ZS
{
namespace Draw
{
class CDrawingScene;
class CDrawSettings;

//******************************************************************************
/*! @brief Represents a shape which can be drawn on the background or foreground
           of the drawing scene.
*/
class ZSDRAWDLL_API CPhysValShape
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValShape"; }
public: // ctors
    CPhysValShape(const CDrawingScene& i_drawingScene);
    CPhysValShape(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit);
//public: // ctors
//    CPhysValShape(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY);
//    CPhysValShape(
//        const CDrawingScene& i_drawingScene,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//        const ZS::System::GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//        const ZS::PhysVal::CUnit& i_unit);
public: // copy ctor
    CPhysValShape(const CPhysValShape& i_physValShapeOther);
public: // dtor
    virtual ~CPhysValShape();
public: // must overridable operators
    virtual CPhysValShape& operator = (const CPhysValShape& i_physValShapeOther) = 0;
public: // must overridable operators
    virtual bool operator == (const CPhysValShape& i_physValShapeOther) const = 0;
    virtual bool operator != (const CPhysValShape& i_physValShapeOther) const = 0;
public: // must overridables
    virtual bool isValid() const = 0;
    virtual bool isNull() const = 0;
    virtual void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) = 0;
    virtual QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ") const = 0;
public: // instance methods
    const CDrawingScene& drawingScene() const { return *m_pDrawingScene; }
    //const ZS::System::GUI::Math::CScaleDivLinesMetrics& divLinesMetricsX() const { return *m_pDivLinesMetricsX; }
    //const ZS::System::GUI::Math::CScaleDivLinesMetrics& divLinesMetricsY() const { return *m_pDivLinesMetricsY; }
    ZS::System::EYScaleAxisOrientation yScaleAxisOrientation() const;
public: // instance methods
    void setIndexInDrawingScene(int i_idx);
    int indexInDrawingScene() const;
protected: // instance members
    /*!< Reference to drawing scene. */
    const CDrawingScene* m_pDrawingScene;
    /*!< To speed up code and to improve readability a flag is set to define the YAxisScaleOrientation. */
    bool m_bYAxisTopDown;
    ///*!< Mathematic component to calculate the division lines of the X-Scale.
    //     Also used to convert pixel values into metric values and vice versa. */
    //const ZS::System::GUI::Math::CScaleDivLinesMetrics* m_pDivLinesMetricsX;
    ///*!< Mathematic component to calculate the division lines of the Y-Scale.
    //     Also used to convert pixel values into metric values and vice versa. */
    //const ZS::System::GUI::Math::CScaleDivLinesMetrics* m_pDivLinesMetricsY;
    /*!< Unit (either metric or pixels) in which the coordinates are stored. */
    ZS::PhysVal::CUnit m_unit;
    /*!< PhysVal shapes may be added to the drawing scene to be drawn in the background or foreground.
         If added to the drawing scene the index in the list of shapes is set here. Otherwise the index is -1. */
    int m_idxInDrawingScene;

}; // class CPhysValShape

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValShape_h
