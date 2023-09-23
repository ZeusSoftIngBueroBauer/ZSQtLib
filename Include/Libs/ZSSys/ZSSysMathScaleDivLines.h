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
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::Math"; }
    static QString ClassName() { return "CScaleDivLines"; }
public: // class methods
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
    static int getPrecision2ShowUniqueNumbers(
        const QVector<double> i_arfVals,
        int i_iExponentDigits,
        int i_iPrecisionMin = 1,
        int i_iPrecisionMax = 10,
        CTrcAdminObj* i_pTrcAdminObj = nullptr);
public: // ctors and dtor
    CScaleDivLines(const QString& i_strObjName, EScaleDir i_scaleDir);
    CScaleDivLines(
        const QString& i_strNameSpace, const QString& i_strClassName,
        const QString& i_strObjName, EScaleDir i_scaleDir);
    CScaleDivLines(const QString& i_strObjName, const CScaleDivLines& i_other);
    CScaleDivLines(const CScaleDivLines& i_other) = delete;
    ~CScaleDivLines();
public: // operators
    CScaleDivLines& operator = (const CScaleDivLines& i_other);
public: // instance methods
    QString objectName() const;
public: // instance methods (setting properties)
    bool setSpacing(const CEnumSpacing& i_eSpacing);
    bool setScale(double i_fMin, double i_fMax, double i_fRes, double i_fMin_px, double i_fMax_px);
    bool setScaleMin(double i_fMin);
    bool setScaleMax(double i_fMax);
    bool setScaleRes(double i_fRes);
    bool setScaleMinInPix(double i_fMin_px);
    bool setScaleMaxInPix(double i_fMax_px);
    bool setScaleRangeInPix(double i_fRange_px);
public: // instance methods (setting properties)
    bool setDivLinesDistMinInPix(const CEnumDivLineLayer& i_eLayer, int i_iDist_px);
public: // instance methods (getting properties)
    bool isValid() const;
    EScaleDir scaleDir() const;
    ESpacing spacing() const;
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
    double getVal(double i_fPix) const;
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
