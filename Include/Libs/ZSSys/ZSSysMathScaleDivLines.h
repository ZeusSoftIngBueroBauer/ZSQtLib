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

#ifndef ZSSys_MathScaleDivLines_h
#define ZSSys_MathScaleDivLines_h

#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace Math
{
//******************************************************************************
/*! @brief Class calculating division lines for a given scale.

    The division lines are rounded to a whole number of a decimal power.

        Example:

            Input:

                          DistMinVal (optional = 1.0, used to calculate logarithmic spacing)
                |         |<--  -->|                              |
                |         |<- 1.0->|                              |
            ScaleMinVal                                      ScaleMaxVal
               0.0                                               10.0
                +-------------------------------------------------+
                |<--------------------- 500 --------------------->|
                |<-------------------ScaleRangePix -------------->|
                |       |<50>|                                    |
                |       |<-->|                                    |
                |     DistMinPix                                  |

            Output:

                |<DistVal>|
                |<- 10.0->|
             FirstVal
               0.0                                                |
                +---------|---------|---------|---------|---------+
               0.0       10.0      20.0      30.0      40.0      50.0
             FirstPix                                             |
                |<- 100 ->|                                       |
                |<DistPix>|                                       |

    Pure pixel coordinate system (no world transformation)

        For example used by bitmap paintings.

        Example 1:

            Input:

                Min_px:         0 px
                Max_px:        19 px
                ScaleRange_px: Max/px - Min/px + 1 = 20 px
                DistMinPix:    10 px
    
            Result:
    
                  |<-                                 Range: 20 px                          ->|
                  |<-           DistMinPix: 10 px       ->|
                | 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 | px
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   |
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                  |<-             DistPix: 10 px        ->|<-             DistPix: 10 px        ->|
                DivLineCount: 2
                !! There will be no division line at position 20 px. !!

        Example 2:

            Input:

                Min_px:         0 px
                Max_px:        20 px
                ScaleRange_px: Max/px - Min/px + 1 = 21 px
                DistMinPix:    10 px

            Result:

                  |<-                                 Range: 21 px                              ->|
                  |<-           DistMinPix: 10 px       ->|
                | 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 | px
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                  |<-             DistPix: 10 px        ->|<-             DistPix: 10 px        ->|
                DivLineCount: 3
                !! There will be a division line at position 20 px. !!

    Scalings with world transformation (screen coordinate system in pixels, resulting scale in another unit)

        For example used by drawings in metric system or by diagrams.

        Example 1:

            Input:

                ScaleMinVal:  0.0 mm (or any other unit like Hz)
                ScaleMaxVal: 10.0 mm
                ScaleRange:  10.0 mm
                Min_px:         0 px
                Max_px:        19 px
                ScaleRange_px: Max/px - Min/px + 1 = 20 px
                DistMinPix:    10 px

            Result:

                  |<-                                Range: 20 px                           ->|
                  |<-           DistMinPix: 10 px       ->|
                | 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19 |
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   | X |
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                 0.0                                   5.0                                   10.0 mm
                  |<-                            DistPix: 19.0 px                           ->|
                  |<-                            DistVal: 10.0 mm                           ->|
                DivLineCount: 2

            Lines can only be drawn on whole number pixel positions but not between two pixels.
            One pixel must be truncated from the available pixel range to calculate the world coordinate.
            Pixel positions will be rounded to a whole number.

            Transformation from world coordinates into pixel coordinates:

                Val_px = Min_px + ((Range_px - 1 px) / ScaleRange_mm) * Val_mm = 0 px + (19 px / 10.0 mm) * Val_mm

                Val_mm |  0.0 |  1.0 |  2.0 |  3.0 |  4.0 |  5.0 |  6.0 |  7.0 |  8.0 |  9.0 | 10.0 |
                -------+------+------+------+------+------+------+------+------+------+------+------+
                Val_px |  0.0 |  1.9 |  3.8 |  5.7 |  7.6 |  9.5 | 11.4 | 13.3 | 15.2 | 17.1 | 19.0 |
                -------+------+------+------+------+------+------+------+------+------+------+------+
                Val_px |   0  |   2  |   4  |   5  |   8  |  10  |  11  |  13  |  15  |  17  |  19  |
                -------+------+------+------+------+------+------+------+------+------+------+------+

            Transformation from pixel coordinates into world coordinates:

                Val_mm = Min_mm + (ScaleRange_mm / (Range_px - 1 px)) * Val_px = 0 mm + (10 mm / 19 px) * Val_px

                Val_px |   0  |   2  |   4  |   5  |   8  |  10  |  11  |  13  |  15  |  17  |  19  |
                -------+------+------+------+------+------+------+------+------+------+------+------+
                Val_mm |  0.0 | 1.05 | 2.11 | 2.63 | 4.21 | 5.26 | 5,79 | 6.84 | 7.89 | 8.95 | 10.0 |
                -------+------+------+------+------+------+------+------+------+------+------+------+

        Example 2:

            Input:

                ScaleMinVal:  0.0 mm (or any other unit like Hz)
                ScaleMaxVal: 10.0 mm
                ScaleRange:  10.0 mm
                Min_px:         0 px
                Max_px:        20 px
                ScaleRange_px: Max/px - Min/px + 1 = 21 px
                DistMinPix:    10 px

            Result:

                  |<-                                   Range: 21 px                              ->|
                  |<-           DistMinPix: 10 px       ->|
                | 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 |
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                | X |   |   |   |   |   |   |   |   |   | X |   |   |   |   |   |   |   |   |   | X |
                +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                 0.0                                     5.0                                     10.0 mm
                  |<-                               DistPix: 10.0 px                            ->|
                  |<-                               DistVal:  5.0 mm                            ->|
                DivLineCount: 3

            Lines can only be drawn on whole number pixel positions but not between two pixels.
            One pixel must be truncated from the available pixel range to calculate the world coordinate.
            Pixel positions will be rounded to a whole number.

            Transformation from world coordinates into pixel coordinates:

                Val_px = Min_px + ((Range_px - 1 px) / ScaleRange_mm) * (Val_mm - ScaleMin_mm) = 0 px + (20 px / 10.0 mm) * (Val_mm - 0.0)

                Val_mm |  0.0 |  1.0 |  2.0 |  3.0 |  4.0 |  5.0 |  6.0 |  7.0 |  8.0 |  9.0 | 10.0 |
                -------+------+------+------+------+------+------+------+------+------+------+------+
                Val_px |  0.0 |  2.0 |  4.0 |  6.0 |  8.0 | 10.0 | 12.0 | 14.0 | 16.0 | 18.0 | 20.0 |
                -------+------+------+------+------+------+------+------+------+------+------+------+
                Val_px |   0  |   2  |   4  |   6  |   8  |  10  |  12  |  14  |  16  |  18  |  20  |
                -------+------+------+------+------+------+------+------+------+------+------+------+

            Transformation from pixel coordinates into world coordinates:

                Val_mm = Min_mm + (ScaleRange_mm / (Range_px - 1 px)) * (Val_px - Min_px) = 0 mm + (10 mm / 19 px) * (Val_px - 0)

                Val_px |   0  |   2  |   4  |   6  |   8  |  10  |  12  |  14  |  16  |  18  |  20  |
                -------+------+------+------+------+------+------+------+------+------+------+------+
                Val_mm |  0.0 |  1.0 |  2.0 |  3.0 |  4.0 |  5.0 |  6.0 |  7.0 |  8.0 |  9.0 | 10.0 |
                -------+------+------+------+------+------+------+------+------+------+------+------+
*/
class ZSSYSDLL_API CScaleDivLines
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::Math"; }
    static QString ClassName() { return "CScaleDivLines"; }
public: // class methods
    static int getPrecision2ShowUniqueNumbers(
        const QVector<double> i_arfVals,
        int i_iExponentDigits,
        int i_iPrecisionMin = 1,
        int i_iPrecisionMax = 10,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
    static int getDivLines4LinSpacing(
        double  i_fScaleMinVal,
        double  i_fScaleMaxVal,
        double  i_fDivLineDistMinVal,
        int     i_iDivLineDistMinPix,
        bool    i_bUseDivLineDistValDecimalFactor25,
        double* o_pfDivLineFirstVal,
        double* o_pfDivLineDistFirstPix,
        double* o_pfDivLineDistVal,
        double* o_pfDivLineDistPix,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
    static int getDivLines4LinSpacing(
        double  i_fScaleMinVal,
        double  i_fScaleMaxVal,
        double  i_fScaleRangePix,
        double  i_fDivLineDistMinVal,
        int     i_iDivLineDistMinPix,
        bool    i_bUseDivLineDistValDecimalFactor25,
        double* o_pfDivLineFirstVal,
        double* o_pfDivLineDistFirstPix,
        double* o_pfDivLineDistVal,
        double* o_pfDivLineDistPix,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
protected: // class methods
    static int getDivLines4LinSpacing(
        bool    i_bUseWorldCoordinateTransformation,
        double  i_fScaleMinVal,
        double  i_fScaleMaxVal,
        double  i_fScaleRangePix,
        double  i_fDivLineDistMinVal,
        int     i_iDivLineDistMinPix,
        bool    i_bUseDivLineDistValDecimalFactor25,
        double* o_pfDivLineFirstVal,
        double* o_pfDivLineDistFirstPix,
        double* o_pfDivLineDistVal,
        double* o_pfDivLineDistPix,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
public: // ctors and dtor
    CScaleDivLines(const QString& i_strObjName, EScaleAxis i_scaleAxis);
    CScaleDivLines(
        const QString& i_strNameSpace, const QString& i_strClassName,
        const QString& i_strObjName, EScaleAxis i_scaleAxis);
    CScaleDivLines(const QString& i_strObjName, const CScaleDivLines& i_other);
    CScaleDivLines(const CScaleDivLines& i_other) = delete;
    ~CScaleDivLines();
public: // operators
    CScaleDivLines& operator = (const CScaleDivLines& i_other);
public: // instance methods
    QString objectName() const;
public: // instance methods (setting properties)
    bool setYScaleAxisOrientation(const CEnumYScaleAxisOrientation& i_eOrientation);
    bool setSpacing(const CEnumSpacing& i_eSpacing);
    bool setUseWorldCoordinateTransformation(bool i_bUseTransformation);
    bool setScale(double i_fMin, double i_fMax, double i_fRes);
    bool setScale(double i_fMin, double i_fMax, double i_fRes, double i_fMin_px, double i_fMax_px);
    bool setScaleInPix(double i_fMin_px, double i_fMax_px);
    bool setScaleRes(double i_fRes);
public: // instance methods (setting properties)
    bool setDivLinesDistMinInPix(const CEnumDivLineLayer& i_eLayer, int i_iDist_px);
public: // instance methods (getting properties)
    bool isValid() const;
    EScaleAxis scaleAxis() const;
    EYScaleAxisOrientation yScaleAxisOrientation() const;
    ESpacing spacing() const;
    bool useWorldCoordinateTransformation() const;
    double scaleMin() const;
    double scaleMax() const;
    double scaleRange() const;
    double scaleRes() const;
    double scaleMinInPix() const;
    double scaleMaxInPix() const;
    double scaleRangeInPix() const;
public: // instance methods (getting properties)
    int divLinesDistMinInPix(const CEnumDivLineLayer& i_eLayer) const;
public: // overridables (to recalculate division lines after changing settings)
    virtual bool divLinesCalculated() const;
    virtual bool update();
public: // instance methods (returning calculated values)
    int getDivLinesCount(const CEnumDivLineLayer& i_eLayer) const;
    double getDivLinesDistMin(const CEnumDivLineLayer& i_eLayer) const;
    double getDivLineVal(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    double getDivLineInPix(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    double getDivLineDistVal(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2) const;
    double getDivLineDistInPix(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2) const;
public: // instance methods (converting values)
    double getValInPix(double i_fVal) const;
    double getVal(double i_fPos_px) const;
    double getDistanceInPix(double i_fDistance) const;
    double getDistance(double i_fDistance_px) const;
protected: // overridable auxiliary instance methods
    virtual void invalidateResults();
protected: // auxiliary instance methods (to recalculate division lines after changing settings)
    void updateLinearSpacing();
    void updateLogarithmicSpacing();
protected: // auxiliary instance methods
    void updateDivLinesValsSorted();
protected:  // class members
    static QVector<double> s_arfScaleRangeFacPixDivValLog;
protected: // instance members (config values)
    /*!< If the class is inherited by another class (like CScaleDivLinesMetrics)
         the name space of the derived class must be passed by the ctor and will
         be stored here. */
    QString m_strNameSpace;
    /*!< If the class is inherited by another class (like CScaleDivLinesMetrics)
         the class name of the derived class must be passed by the ctor and will
         be stored here. */
    QString m_strClassName;
    /*!< Name of the instance. */
    QString m_strObjName;
    /*!< Scale axis (X or Y). Set by the constructor. Not changeable during runtime. */
    EScaleAxis m_scaleAxis;
    /*!< Defines whether pixel coordinates need to be converted to world coordinates
         and vice versa. Defaults to true. */
    bool m_bUseWorldCoordinateTransformation;
    /*!< Y scale axis orientation. Defaults to "BottomUp". */
    EYScaleAxisOrientation m_yScaleAxisOrientation;
    /*!< Spacing of the scale (either linear or logarithmic). */
    ESpacing m_spacing;
    /*!< Minimum scale value. Only used if the scale is setup for world coordinate transformation. */
    double m_fScaleMin;
    /*!< Maximum scale value.  Only used if the scale is setup for world coordinate transformation. */
    double m_fScaleMax;
    /*!< Resolution of minimum and maximum scale values. For pure pixel coordinate system usually 1.0. */
    double m_fScaleRes;
    /*!< Minimum scale value in pixels. */
    double m_fMin_px;
    /*!< Maximum scale value in pixels. */
    double m_fMax_px;
    /*!< Minimum distance between two successive division lines per layer in pixels
         used to calculate the division lines in world coordinates (physical values). */
    QVector<int> m_ariDivLinesDistMin_px;
protected: // instance members (calculated results)
    /*!< Flag to indicate whether the calculated values are up to date. */
    bool m_bDivLinesCalculated;
    /*!< Number of calculated division lines per layer. */
    QVector<int> m_ariDivLinesCount;
    /*!< Calculated minimum distance between two successive division lines per layer
         in world coordinates (physical values). Always a whole number multiple of
         a decimal power. */
    QVector<double> m_arfDivLinesDistMinVal;
    /*!< Calculated values of the division lines per layer in world coordinates (physical values).
         Always a whole number multiple of a decimal power. */
    QVector<QVector<double>> m_ararfDivLinesVals;
    /*!< Merged array of main and sub division lines without duplicated (sub division lines
         overlapping main division lines are not included) in ascending order (lowest values first). */
    QVector<double> m_arfDivLinesValsSorted;
    /*!< Pixel coordinates of the division lines per layer. */
    QVector<QVector<double>> m_ararfDivLines_px;
    /*!< Trace admin object to control method tracing. */
    CTrcAdminObj* m_pTrcAdminObj;

}; // class CScaleDivLines

} // namespace Math

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_MathScaleDivLines_h
