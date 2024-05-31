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

#ifndef ZSSys_Math_h
#define ZSSys_Math_h

#include <math.h>
#include <limits.h>
#include <float.h>

#include <QtCore/qstring.h>
#include <QtCore/qrect.h>
#include <QtCore/qvector.h>

#include "ZSSys/ZSSysDllMain.h"

#ifndef _I8_MIN
#define _I8_MIN     (-127 - 1)    // minimum signed 8 bit value
#endif
#ifndef _I8_MAX
#define _I8_MAX       127         // maximum signed 8 bit value
#endif
#ifndef _UI8_MAX
#define _UI8_MAX      0xff       // maximum unsigned 8 bit value
#endif

#ifndef _I16_MIN
#define _I16_MIN    (-32767 - 1) // minimum signed 16 bit value
#endif
#ifndef _I16_MAX
#define _I16_MAX      32767      // maximum signed 16 bit value
#endif
#ifndef _UI16_MAX
#define _UI16_MAX     0xffff     // maximum unsigned 16 bit value
#endif

#ifndef _I32_MIN
#define _I32_MIN    (-2147483647 - 1) // minimum signed 32 bit value
#endif
#ifndef _I32_MAX
#define _I32_MAX      2147483647 // maximum signed 32 bit value
#endif
#ifndef _UI32_MAX
#define _UI32_MAX     0xffffffff // maximum unsigned 32 bit value
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace Math
{
const int c_iCalculationAccuracyTrailingDigits = 14; // maximum digits according computation accuracy of floating point unit
const double c_fPI  = acos(-1.0);
const double c_f2PI = 2.0*c_fPI;
const double c_f3PI = 3.0*c_fPI;
const double c_f4PI = 4.0*c_fPI;
const double c_f5PI = 5.0*c_fPI;
const double c_f6PI = 6.0*c_fPI;
const double c_f7PI = 7.0*c_fPI;
const double c_f8PI = 8.0*c_fPI;
const double c_f45Degrees_rad = acos(-1.0) / 4.0;
const double c_f90Degrees_rad = acos(-1.0) / 2.0;
const double c_f135Degrees_rad = 3.0 * acos(-1.0) / 4.0;
const double c_f180Degrees_rad = acos(-1.0);
const double c_f225Degrees_rad = 5.0 * acos(-1.0) / 4.0;
const double c_f270Degrees_rad = 3.0 * acos(-1.0) / 2.0;
const double c_f315Degrees_rad = 7.0 * acos(-1.0) / 4.0;
const double c_f360Degrees_rad = 2.0 * acos(-1.0);

const double c_fe = 2.718281828459;

const QChar   c_chSymbolSquare(0x00B2);
const QString c_strSymbolSquare(c_chSymbolSquare);

const QChar   c_chSymbolSquareRoot(0x221A);
const QString c_strSymbolSquareRoot(c_chSymbolSquareRoot);

const QChar   c_chSymbolDegree(0x00B0);
const QString c_strSymbolDegree(c_chSymbolDegree);
const QChar   c_chSymbolPhi(0x03C6);
const QString c_strSymbolPhi(c_chSymbolPhi);

const QChar   c_chSymbolMicro(0x00B5);
const QString c_strSymbolMicro(c_chSymbolMicro);

ZSSYSDLL_API double sqr( double i_fVal );
ZSSYSDLL_API double sqrt( double i_fVal );
ZSSYSDLL_API double abs( double i_fVal );
ZSSYSDLL_API double normalizeAngleInRad( double i_fAngle_rad );
ZSSYSDLL_API double normalizeAngleInDegree( double i_fAngle_degree );
ZSSYSDLL_API double rad2Degree( double i_fAngle_rad, bool i_bNormalize = true );
ZSSYSDLL_API double degree2Rad( double i_fAngle_degree, bool i_bNormalize = true );
ZSSYSDLL_API double toClockWiseAngleRad( double i_fAngle_rad, bool i_bNormalize = true );
ZSSYSDLL_API double toCounterClockWiseAngleRad( double i_fAngle_rad, bool i_bNormalize = true );
ZSSYSDLL_API double toClockWiseAngleDegree( double i_fAngle_degree, bool i_bNormalize = true );
ZSSYSDLL_API double toCounterClockWiseAngleDegree( double i_fAngle_degree, bool i_bNormalize = true );
ZSSYSDLL_API double round2Nearest( double i_fVal, int i_iTrailingDigits = -1 ); // -1 means c_iCalculationAccuracyTrailingDigits
ZSSYSDLL_API double round2Lower( double i_fVal, int i_iTrailingDigits = -1 );   // -1 means c_iCalculationAccuracyTrailingDigits
ZSSYSDLL_API double round2Upper( double i_fVal, int i_iTrailingDigits = -1 );   // -1 means c_iCalculationAccuracyTrailingDigits
ZSSYSDLL_API double round2Resolution( double i_fVal, double i_fRes );
ZSSYSDLL_API double round2LowerDecade( double i_fVal );
ZSSYSDLL_API double round2UpperDecade( double i_fVal );
ZSSYSDLL_API double round2LowerIntMultOfTen( double i_fVal );
ZSSYSDLL_API double round2UpperIntMultOfTen( double i_fVal );
ZSSYSDLL_API int    minVal( int i_iValCount, int    i_iVal1 ... );
ZSSYSDLL_API double minVal( int i_iValCount, double i_fVal1 ... );
ZSSYSDLL_API int    maxVal( int i_iValCount, int    i_iVal1 ... );
ZSSYSDLL_API double maxVal( int i_iValCount, double i_fVal1 ... );
ZSSYSDLL_API std::tuple<double, double> getAbsMinMax( const QVector<double>& i_arVals );
ZSSYSDLL_API int getFirstSignificantDigit( double i_fVal );
ZSSYSDLL_API void normalize( double i_fVal, int* o_piSign, int* o_piMant, int* o_piExp );

typedef double (*TFctFFTWindow)( double i_fVal, int i_iFFTSegementLen );

ZSSYSDLL_API double fctFFTWindowRect( double i_fVal, int i_iFFTSegmentLen );
ZSSYSDLL_API double fctFFTWindowHanning( double i_fVal, int i_iFFTSegmentLen );
ZSSYSDLL_API double fctFFTWindowFlatTop( double i_fVal, int i_iFFTSegmentLen );
ZSSYSDLL_API double fctFFTWindowBlackman( double i_fVal, int i_iFFTSegmentLen );
ZSSYSDLL_API double fctFFTWindowWelch( double i_fVal, int i_iFFTSegmentLen );

ZSSYSDLL_API double logRes2LinRes( double i_fResLog, double i_fValLin );

ZSSYSDLL_API QRect calcRect( const QPoint& i_ptStart, const QPoint& i_ptEnd );

ZSSYSDLL_API QVector<double> merge(const QVector<double>& i_arfVals1, const QVector<double>& i_arfVals2);

} // namespace Math

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Math_h
