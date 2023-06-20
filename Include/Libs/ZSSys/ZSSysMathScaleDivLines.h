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
/*! @brief Class which calculates division lines for a given scale.

    The division lines are rounded to a whole number of a decimal power.

    @Example:

    Input:

                  DistMinVal
        |         |<--   -->|                                                                               |
        |         |<- 10.0->|                                                                               |
    ScaleMinVal                                                                                        ScaleMaxVal
       0.0                                                                                                100.0
        +---------------------------------------------------------------------------------------------------+
        |<-------------------------------------------------- 1000 ----------------------------------------->|
        |<----------------------------------------------- ScaleRangePix ----------------------------------->|
        |       |<50>|                                                                                      |
        |       |<-->|                                                                                      |
        |       DistMinPix                                                                                  |

    Output:

        |<DistVal>|
        |<- 10.0->|
     FirstVal
       0.0                                                                                                  |
        +---------|---------|---------|---------|---------|---------|---------|---------|---------|---------+
       0.0       10.0      20.0      30.0      40.0      50.0      60.0      70.0      80.0      90.0     100.0
     FirstPix                                                                                               |
        |<- 100 ->|                                                                                         |
        |<DistPix>|                                                                                         |
*/
class ZSSYSDLL_API CScaleDivLines
{
public: // class methods
    static QString NameSpace() { return "ZS::System::Math"; }
    static QString ClassName() { return "CScaleDivLines"; }
public: // class methods
    static int getDivLines4LinSpacing(
        double  i_fScaleMinVal,
        double  i_fScaleMaxVal,
        int     i_iScaleRangePix,
        double  i_fDivLineDistMinVal,
        int     i_iDivLineDistMinPix,
        bool    i_bUseDivLineDistValDecimalFactor25,
        double* o_pfDivLineFirstVal,
        double* o_pfDivLineDistFirstPix,
        double* o_pfDivLineDistVal,
        double* o_pfDivLineDistPix,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
    static int getPrecision2ShowUniqueNumbers(
        const QVector<double> i_arfVals,
        int i_iExponentDigits,
        int i_iPrecisionMin = 1,
        int i_iPrecisionMax = 10,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
public: // ctors and dtor
    CScaleDivLines(const QString& i_strObjName, EScaleDir i_scaleDir);
    ~CScaleDivLines();
public: // instance methods
    QString objectName() const;
public: // instance methods (setting properties)
    bool setSpacing(const CEnumSpacing& i_eSpacing);
    bool setScale(double i_fMin, double i_fMax, double i_fRes, int i_iMin_px, int i_iMax_px);
    bool setScaleMin(double i_fMin);
    bool setScaleMax(double i_fMax);
    bool setScaleRes(double i_fRes);
    bool setScaleMinInPix(int i_iMin_px);
    bool setScaleMaxInPix(int i_iMax_px);
    bool setScaleRangeInPix(int i_iRange_px);
public: // instance methods (setting properties)
    bool setDivLineDistMinInPix(const CEnumDivLineLayer& i_eLayer, int i_iDist_px);
public: // instance methods (getting properties)
    bool isValid() const;
    EScaleDir scaleDir() const;
    ESpacing spacing() const;
    double scaleMin() const;
    double scaleMax() const;
    double scaleRange() const;
    double scaleRes() const;
    int scaleMinInPix() const;
    int scaleMaxInPix() const;
    int scaleRangeInPix() const;
public: // instance methods (getting properties)
    int divLineDistMinInPix(const CEnumDivLineLayer& i_eLayer) const;
public: // overridables (to recalculate divsion lines after changing settings)
    bool divLinesCalculated() const;
    bool update();
public: // instance methods (returning calculated values)
    int getDivLineCount(const CEnumDivLineLayer& i_eLayer) const;
    double getDivLineDistMin(const CEnumDivLineLayer& i_eLayer) const;
    double getDivLineVal(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    double getDivLineInPix(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine) const;
    double getDivLineDistVal(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2) const;
    double getDivLineDistInPix(const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2) const;
public: // instance methods (converting values)
    int getValInPix(double i_fVal) const;
    double getVal(double i_fPix) const;
public: // instance methods (returning calculated format info)
    //std::tuple<int, int, int> getFormatInfo() const;
protected: // auxiliary instance methods (to recalculate divsion lines after changing settings)
    void updateLinearSpacing();
    void updateLogarithmicSpacing();
protected:  // class members
    static QVector<double> s_arfScaleRangeFacPixDivValLog;
protected: // instance members (config values)
    /*!< Name of the instance. */
    QString m_strObjName;
    /*!< Scale direction (X or Y). Set by the constructor. Not changeable during runtime. */
    EScaleDir m_scaleDir;
    /*!< Spacing of the scale (either linear or logarithmic). */
    ESpacing m_spacing;
    /*!< Minimum scale value. */
    double m_fScaleMin;
    /*!< Maximum scale value. */
    double m_fScaleMax;
    /*!< Resolution of minimum and maximum scale values. */
    double m_fScaleRes;
    /*!< Minimum scale value in pixels. */
    int m_iMin_px;
    /*!< Maximum scale value in pixels. */
    int m_iMax_px;
    /*!< Minimum distance between two successive division lines per layer in pixels
         used to calculate the division lines in world coordinates (physical values). */
    QVector<int> m_ariDivLineDistMin_px;
protected: // instance members (calculated)
    /*!< Flag to indicate whether the calculated values are up to date. */
    bool m_bDivLinesCalculated;
    /*!< Number of calculated division lines per layer. */
    QVector<int> m_ariDivLineCount;
    /*!< Calculated minimum distance between two successive division lines per layer
         in world coordinates (physical values). Always a whole number multiple of
         a decimal power. */
    QVector<double> m_arfDivDistMinVal;
    /*!< Calculated values of the division lines per layer in world coordinates (physical values).
         Always a whole number multiple of a decimal power. */
    QVector<QVector<double>> m_ararfDivLineVal;
    /*!< Pixel coordinates of the division lines per layer. */
    QVector<QVector<double>> m_ararfDivLine_px;
    /*!< Trace admin object to control method tracing. */
    CTrcAdminObj* m_pTrcAdminObj;

}; // class CScaleDivLines

} // namespace Math

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_MathScaleDivLines_h
