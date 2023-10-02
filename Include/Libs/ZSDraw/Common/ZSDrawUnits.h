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

#ifndef ZSDraw_Units_h
#define ZSDraw_Units_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSPhysVal/ZSPhysUnit.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Physical size for lengths in drawings.

    To position graphical objects in QGraphicsScene the positions got to be
    defined in pixel coordinates.

    ZS::Draw also supports some kind of "wysiwyg" drawings where the size of the
    drawing is not defined as an image size with width and height in pixels but
    also with width and height in a metric unit like "mm", "m" or even "km".

    The class CUnitsLength defines the metric and pixel units and the corresponding
    conversion routines to convert coordinates between pixels and metric units
    and vice versa.

    In order to convert a length in metric units into screen pixels, the screen
    resolution defined as a factor in dots per inches or dots per milli meter
    must be known. The screen resolution can be automatically detected through
    a system call, can be calculated by the user using the number of pixels
    available in horizontal and vertical direction and the screen size in metric
    unit or can just be manually defined by the user to a suitable value.

    A resolution of approximately 4 pixels per millimeter is a value that can
    occur in practice. With a resolution of 2560 by 1440 pixels, the monitor
    would be 64 cm wide and 36 cm high and the monitors screen diagonal would
    be approximately 73.43 cm.

    The screen resolution is applied with the method

    - "setPxpi" in pixels per inches.

    For print devices the resolution has to be applied using the method

    - "setDpi" in dots per inches or

    In order to support landscape sketches or to draw very small objects,
    it is possible to define a scale so that large lengths such as kilometers
    or very small lengths like nano meters can also be displayed on the monitor.

    The scale factor is applied with the method

    - "setScaleFactor" providing the dividend and the divisor.

    The unit conversion routines between pixels and metric units must take both,
    the screen resolution of a pixel and the scale factor into account.

    The following scenario should make this clear.

    - An area whose width is 50 meters and whose height is 20 meters should be
      represented in a drawing.
    - The monitor's is approximately 64 cm width and 36 cm high.
    - To have an area of 50 * 20 meters visible on the screen, a scale factor
      of 1/100 would be a good choice (the resulting "wysiwyg" image would be
      approx. 50 cm width and 20 cm in high).
    - The calculated image size in pixels would become
      ImageWidth/px  = AreaWidth/m * ScaleFactor * Resolution/(px/m)
                     = 50 m * 1/100 * 4000.0 px/m = 2000 px
      ImageHeight/px = AreaHeight/m * ScaleFactor * Resolution/(px/m)
                     = 20 m * 1/100 * 4000.0 px/m = 800 px

    !Additional notes!

    - When saving a drawing whose dimension unit is set to metrics the image size in pixels is
      not stored together with the drawings. The image size is always recalculated using the
      current screen resolution on loading the drawing. This means that on one PC the image
      shown in the figure above may have a width and height of 2150 px * 860 px or may have
      a different width and height depending on the screen resolution as retrieved on the
      other PC (maybe 2180 px * 880 px).

    - When setting up the drawing size the automatically detected screen resolution may be
      manually set by the user - if supported by the application. The DrawingSizeProperties
      widget allows editing the screen resolution by double clicking on the edit control
      "Resolution - Pixels/mm".
*/
class ZSDRAWDLL_API CUnitsLength : public QObject, public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CUnitsLength(ZS::System::CIdxTreeEntry* i_pParentBranch);
    ~CUnitsLength();
signals:
    void screenResolutionInPxPerMMChanged();
public: // instance methods (resolution of monitor, pixels per inches)
    void setScreenResolutionInPxPerMM(double i_fRes_px_mm);
    double screenResolutionInPxPerMM() const;
    ZS::PhysVal::CPhysVal screenPixelWidth(const ZS::PhysVal::CUnit& i_unit) const;
public: // instance methods (scale factor of drawing in metric dimension)
    void setScaleFactor(int i_iDividend, int i_iDivisor);
    double scaleFactor() const;
    int scaleFactorDividend() const;
    int scaleFactorDivisor() const;
public: // instance methods (distinguish between metric system and pixels or dots)
    bool isMetricUnit(const QString& i_strSymbolOrName) const;
    bool isMetricUnit(const ZS::PhysVal::CUnit& i_unit) const;
private: // instance methods
    /*!< Unit tree entry for metric unit micro meter */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroMeter;
    /*!< Unit tree entry for metric unit milli meter */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliMeter;
    /*!< Unit tree entry for metric unit centi meter */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryCentiMeter;
    /*!< Unit tree entry for metric unit dezi meter */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDeziMeter;
    /*!< Unit tree entry for metric unit meter (SI Unit of this unit ring) */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMeter;
    /*!< Unit tree entry for metric unit kilo meter */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloMeter;
    /*!< Unit tree entry for metric unit inches.
         1 Inch = 25.4 mm = 2.54cm = 0.0254m
         MToSI: 0.0254 -> MFromSI: 1.0/MToSI */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryInch;
    /*!< Unit tree entry for pixels unit screen pixels */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPixel;
    /*!< Unit tree entry for pixels unit print device dots */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDots;
public: // instance members
    /*!< Metric unit: micro meter */
    ZS::PhysVal::CUnit um;
    /*!< Metric unit: milli meter */
    ZS::PhysVal::CUnit mm;
    /*!< Metric unit: centi meter */
    ZS::PhysVal::CUnit cm;
    /*!< Metric unit: dezi meter */
    ZS::PhysVal::CUnit dm;
    /*!< Metric unit: meter (SI unit of this unit ring) */
    ZS::PhysVal::CUnit m;
    /*!< Metric unit: kilo meter */
    ZS::PhysVal::CUnit km;
    /*!< Metric unit: inches */
    ZS::PhysVal::CUnit in;
    /*!< Pixels unit: screen pixels */
    ZS::PhysVal::CUnit px;
    /*!< Pixels unit: print device dots */
    ZS::PhysVal::CUnit dots;

}; // CUnitsLength

//******************************************************************************
class CUnitsAngle : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CUnitsAngle(ZS::System::CIdxTreeEntry* i_pParentBranch);
    virtual ~CUnitsAngle();
public: // class members
    static const QString c_strSymbolPhi;
private: // instance members
    /*!< Unit tree entry for angles in radiants */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryRad;
    /*!< Unit tree entry for angles in degrees */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDegree;
public: // instance members
    /*!< Angle unit: Radiant */
    ZS::PhysVal::CUnit Rad;
    /*!< Angle unit: Degree */
    ZS::PhysVal::CUnit Degree;

}; // CUnitsAngle

//******************************************************************************
class ZSDRAWDLL_API CUnits : public ZS::PhysVal::CIdxTreeUnits
//******************************************************************************
{
public: // ctors and dtor
    CUnits();
    ~CUnits();
public: // instance members
    /*!< Unit circle for lengths. */
    CUnitsLength Length;
    /*!< Unit circle for angles. */
    CUnitsAngle Angle;

}; // CUnits

extern ZSDRAWDLL_API CUnits Units;

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Units_h
