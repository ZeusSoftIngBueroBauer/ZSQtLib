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

#include "ZSDiagram/ZSDiagramAux.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysMath.h"

#include <math.h>

#include <QtCore/qrect.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Diagram;


/*******************************************************************************
struct SScale
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
SScale::SScale() :
//------------------------------------------------------------------------------
    m_fMin(0.0),
    m_fMax(0.0),
    m_unit()
{
} // default ctor

//------------------------------------------------------------------------------
SScale::SScale( const SScale& i_scaleOther ) :
//------------------------------------------------------------------------------
    m_fMin(i_scaleOther.m_fMin),
    m_fMax(i_scaleOther.m_fMax),
    m_unit(i_scaleOther.m_unit)
{
} // copy ctor

//------------------------------------------------------------------------------
SScale::SScale(double i_fMin, double i_fMax, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_fMin(i_fMin),
    m_fMax(i_fMax),
    m_unit(i_unit)
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal SScale::physValMin() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fMin, m_unit);
}

//------------------------------------------------------------------------------
CPhysVal SScale::physValMax() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fMax, m_unit);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SScale::operator == ( const SScale& i_scaleOther ) const
//------------------------------------------------------------------------------
{
    //lint -e777
    bool bEqual = false;
    if( m_unit == i_scaleOther.m_unit
     && m_fMin == i_scaleOther.m_fMin
     && m_fMax == i_scaleOther.m_fMax )
    {
        bEqual = true;
    }
    return bEqual;
    //lint +e777
}

//------------------------------------------------------------------------------
bool SScale::operator != ( const SScale& i_scaleOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_scaleOther); //lint !e1702
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SScale::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    str = "Min: " + QString::number(m_fMin);
    str += ", Max: " + QString::number(m_fMax);
    str += ", Unit: " + m_unit.symbol();
    return str;
}


/*******************************************************************************
Exported auxiliary methods
*******************************************************************************/

//------------------------------------------------------------------------------
double ZS::Diagram::getValRes(
    CUnit*   i_pUnitScale,
    double   i_fScaleMinVal,
    double   i_fScaleMaxVal,
    int      i_iScaleRangePix, // zero is allowed
    ESpacing i_spacing,
    CUnit*   i_pUnitVal,       // use scale unit if nullptr
    double   i_fVal,
    CUnit*   i_pUnitRes )      // use unit of value if nullptr
//------------------------------------------------------------------------------
{
    double fRes = 1.0;
    CUnit* pUnitVal = i_pUnitVal;
    CUnit* pUnitRes = i_pUnitRes;
    double fScaleRange;
    double fVal = i_fVal;

    if( pUnitVal == nullptr )
    {
        pUnitVal = i_pUnitScale;
    }
    if( pUnitRes == nullptr )
    {
        pUnitRes = pUnitVal;
    }

    if( i_spacing == ESpacingLinear )
    {
        fScaleRange = fabs(i_fScaleMaxVal-i_fScaleMinVal);

        if( fScaleRange > 0.0 )
        {
            if( i_iScaleRangePix > 1 )
            {
                fRes = fScaleRange / (i_iScaleRangePix-1);
            }
            else
            {
                fRes = fScaleRange / 1000.0;
            }
        }
    } // if( spacing == Linear )

    else // if( i_spacing == ESpacingLogarithmic )
    {
        if( i_fScaleMaxVal > 0.0 && i_fScaleMinVal > 0.0 )
        {
            fScaleRange = log10(i_fScaleMaxVal) - log10(i_fScaleMinVal);

            if( fScaleRange > 0.0 )
            {
                if( i_iScaleRangePix > 1.0 )
                {
                    fRes = fScaleRange / (i_iScaleRangePix-1.0);
                }
                else
                {
                    fRes = fScaleRange / 1000.0;
                }
            }
        } // if( fScaleMaxVal > 0.0 && fScaleMinVal > 0.0 )

        if( pUnitVal != nullptr && i_pUnitScale != nullptr && pUnitVal != i_pUnitScale )
        {
            fVal = pUnitVal->convertValue( fVal, i_pUnitScale );
        }
        fRes = logRes2LinRes(fRes,fVal);

    } // if( spacing == Logarithmic )

    if( pUnitRes != nullptr && i_pUnitScale != nullptr && pUnitRes != i_pUnitScale )
    {
        fRes = i_pUnitScale->convertValue( fRes, pUnitRes );
    }
    fRes = Math::round2LowerDecade(fRes);

    return fRes;

} // getValRes

//------------------------------------------------------------------------------
double ZS::Diagram::logRes2LinRes( double i_fResLog, double i_fValLin )
//------------------------------------------------------------------------------
{
    double fValLog;
    double fValLog1;
    double fValLog2;
    double fValLin1;
    double fValLin2;
    double fValLinDiff;
    double fResLin;

    //fValLin = i_fValLin/11.0;
    //fValLin = round2LowerDecade(fValLin);
    //fResLin = fValLin * pow(10.0,fResLog);
    //fResLin = round2LowerDecade(fResLin);

    fValLog = log10(i_fValLin);
    //fValLog1 = fValLog - fResLog/2.0;
    //fValLog2 = fValLog + fResLog/2.0;
    fValLog1 = fValLog;
    fValLog2 = fValLog + i_fResLog;

    fValLin1 = pow(10.0,fValLog1);
    fValLin2 = pow(10.0,fValLog2);
    fValLinDiff = fValLin2 - fValLin1;
    fResLin = Math::round2LowerDecade(fValLinDiff);
    return fResLin;

} // logRes2LinRes

//------------------------------------------------------------------------------
QRect ZS::Diagram::calcRect( const QPoint& i_ptStart, const QPoint& i_ptEnd )
//------------------------------------------------------------------------------
{
    QRect rct;

    // Previously zoomed area:
    if( i_ptStart.x() < i_ptEnd.x() )
    {
        rct.setX(i_ptStart.x());
        rct.setWidth(i_ptEnd.x()-i_ptStart.x()+1); //lint !e834
    }
    else
    {
        rct.setX(i_ptEnd.x());
        rct.setWidth(i_ptStart.x()-i_ptEnd.x()+1); //lint !e834
    }
    if( i_ptStart.y() < i_ptEnd.y() )
    {
        rct.setY(i_ptStart.y());
        rct.setHeight(i_ptEnd.y()-i_ptStart.y()+1); //lint !e834
    }
    else
    {
        rct.setY(i_ptEnd.y());
        rct.setHeight(i_ptStart.y()-i_ptEnd.y()+1); //lint !e834
    }
    return rct;

} // calcRect

//------------------------------------------------------------------------------
QString ZS::Diagram::qtKey2Str( int i_iKey )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    switch( i_iKey )
    {
        case Qt::Key_Escape:
        {
            strTrcMsg += "Qt::Key_Escape";
            break;
        }
        case Qt::Key_Tab:
        {
            strTrcMsg += "Qt::Key_Tab";
            break;
        }
        case Qt::Key_Backtab:
        {
            strTrcMsg += "Qt::Key_Backtab";
            break;
        }
        case Qt::Key_Backspace:
        {
            strTrcMsg += "Qt::Key_Backspace";
            break;
        }
        case Qt::Key_Return:
        {
            strTrcMsg += "Qt::Key_Return";
            break;
        }
        case Qt::Key_Enter:
        {
            strTrcMsg += "Qt::Key_Enter";
            break;
        }
        case Qt::Key_Insert:
        {
            strTrcMsg += "Qt::Key_Insert";
            break;
        }
        case Qt::Key_Delete:
        {
            strTrcMsg += "Qt::Key_Delete";
            break;
        }
        case Qt::Key_Pause:
        {
            strTrcMsg += "Qt::Key_Pause";
            break;
        }
        case Qt::Key_Print:
        {
            strTrcMsg += "Qt::Key_Print";
            break;
        }
        case Qt::Key_SysReq:
        {
            strTrcMsg += "Qt::Key_SysReq";
            break;
        }
        case Qt::Key_Clear:
        {
            strTrcMsg += "Qt::Key_Clear";
            break;
        }
        case Qt::Key_Home:
        {
            strTrcMsg += "Qt::Key_Home";
            break;
        }
        case Qt::Key_End:
        {
            strTrcMsg += "Qt::Key_End";
            break;
        }
        case Qt::Key_Left:
        {
            strTrcMsg += "Qt::Key_Left";
            break;
        }
        case Qt::Key_Up:
        {
            strTrcMsg += "Qt::Key_Up";
            break;
        }
        case Qt::Key_Right:
        {
            strTrcMsg += "Qt::Key_Right";
            break;
        }
        case Qt::Key_Down:
        {
            strTrcMsg += "Qt::Key_Down";
            break;
        }
        case Qt::Key_Shift:
        {
            strTrcMsg += "Qt::Key_Shift";
            break;
        }
        case Qt::Key_Control:
        {
            strTrcMsg += "Qt::Key_Control";
            break;
        }
        case Qt::Key_Meta:
        {
            strTrcMsg += "Qt::Key_Meta";
            break;
        }
        case Qt::Key_Alt:
        {
            strTrcMsg += "Qt::Key_Alt";
            break;
        }
        case Qt::Key_CapsLock:
        {
            strTrcMsg += "Qt::Key_CapsLock";
            break;
        }
        case Qt::Key_NumLock:
        {
            strTrcMsg += "Qt::Key_NumLock";
            break;
        }
        case Qt::Key_ScrollLock:
        {
            strTrcMsg += "Qt::Key_ScrollLock";
            break;
        }
        case Qt::Key_F1:
        {
            strTrcMsg += "Qt::Key_F1";
            break;
        }
        case Qt::Key_F2:
        {
            strTrcMsg += "Qt::Key_F2";
            break;
        }
        case Qt::Key_F3:
        {
            strTrcMsg += "Qt::Key_F3";
            break;
        }
        case Qt::Key_F4:
        {
            strTrcMsg += "Qt::Key_F4";
            break;
        }
        case Qt::Key_F5:
        {
            strTrcMsg += "Qt::Key_F5";
            break;
        }
        case Qt::Key_F6:
        {
            strTrcMsg += "Qt::Key_F6";
            break;
        }
        case Qt::Key_F7:
        {
            strTrcMsg += "Qt::Key_F7";
            break;
        }
        case Qt::Key_F8:
        {
            strTrcMsg += "Qt::Key_F8";
            break;
        }
        case Qt::Key_F9:
        {
            strTrcMsg += "Qt::Key_F9";
            break;
        }
        case Qt::Key_F10:
        {
            strTrcMsg += "Qt::Key_F10";
            break;
        }
        case Qt::Key_F11:
        {
            strTrcMsg += "Qt::Key_F11";
            break;
        }
        case Qt::Key_F12:
        {
            strTrcMsg += "Qt::Key_F12";
            break;
        }
        case Qt::Key_F13:
        {
            strTrcMsg += "Qt::Key_F13";
            break;
        }
        case Qt::Key_F14:
        {
            strTrcMsg += "Qt::Key_F14";
            break;
        }
        case Qt::Key_F15:
        {
            strTrcMsg += "Qt::Key_F15";
            break;
        }
        case Qt::Key_F16:
        {
            strTrcMsg += "Qt::Key_F16";
            break;
        }
        case Qt::Key_F17:
        {
            strTrcMsg += "Qt::Key_F17";
            break;
        }
        case Qt::Key_F18:
        {
            strTrcMsg += "Qt::Key_F18";
            break;
        }
        case Qt::Key_F19:
        {
            strTrcMsg += "Qt::Key_F19";
            break;
        }
        case Qt::Key_F20:
        {
            strTrcMsg += "Qt::Key_F20";
            break;
        }
        case Qt::Key_F21:
        {
            strTrcMsg += "Qt::Key_F21";
            break;
        }
        case Qt::Key_F22:
        {
            strTrcMsg += "Qt::Key_F22";
            break;
        }
        case Qt::Key_F23:
        {
            strTrcMsg += "Qt::Key_F23";
            break;
        }
        case Qt::Key_F24:
        {
            strTrcMsg += "Qt::Key_F24";
            break;
        }
        case Qt::Key_F25:
        {
            strTrcMsg += "Qt::Key_F25";
            break;
        }
        case Qt::Key_F26:
        {
            strTrcMsg += "Qt::Key_F26";
            break;
        }
        case Qt::Key_F27:
        {
            strTrcMsg += "Qt::Key_F27";
            break;
        }
        case Qt::Key_F28:
        {
            strTrcMsg += "Qt::Key_F28";
            break;
        }
        case Qt::Key_F29:
        {
            strTrcMsg += "Qt::Key_F29";
            break;
        }
        case Qt::Key_F30:
        {
            strTrcMsg += "Qt::Key_F30";
            break;
        }
        case Qt::Key_F31:
        {
            strTrcMsg += "Qt::Key_F31";
            break;
        }
        case Qt::Key_F32:
        {
            strTrcMsg += "Qt::Key_F32";
            break;
        }
        case Qt::Key_F33:
        {
            strTrcMsg += "Qt::Key_F33";
            break;
        }
        case Qt::Key_F34:
        {
            strTrcMsg += "Qt::Key_F34";
            break;
        }
        case Qt::Key_F35:
        {
            strTrcMsg += "Qt::Key_F35";
            break;
        }
        case Qt::Key_Super_L:
        {
            strTrcMsg += "Qt::Key_Super_L";
            break;
        }
        case Qt::Key_Super_R:
        {
            strTrcMsg += "Qt::Key_Super_R";
            break;
        }
        case Qt::Key_Menu:
        {
            strTrcMsg += "Qt::Key_Menu";
            break;
        }
        case Qt::Key_Hyper_L:
        {
            strTrcMsg += "Qt::Key_Hyper_L";
            break;
        }
        case Qt::Key_Hyper_R:
        {
            strTrcMsg += "Qt::Key_Hyper_R";
            break;
        }
        case Qt::Key_Help:
        {
            strTrcMsg += "Qt::Key_Help";
            break;
        }
        case Qt::Key_Direction_L:
        {
            strTrcMsg += "Qt::Key_Direction_L";
            break;
        }
        case Qt::Key_Direction_R:
        {
            strTrcMsg += "Qt::Key_Direction_R";
            break;
        }
        case Qt::Key_Space:
        {
            strTrcMsg += "Qt::Key_Space";
            break;
        }
        case Qt::Key_Exclam:
        {
            strTrcMsg += "Qt::Key_Exclam";
            break;
        }
        case Qt::Key_QuoteDbl:
        {
            strTrcMsg += "Qt::Key_QuoteDbl";
            break;
        }
        case Qt::Key_NumberSign:
        {
            strTrcMsg += "Qt::Key_NumberSign";
            break;
        }
        case Qt::Key_Dollar:
        {
            strTrcMsg += "Qt::Key_Dollar";
            break;
        }
        case Qt::Key_Percent:
        {
            strTrcMsg += "Qt::Key_Percent";
            break;
        }
        case Qt::Key_Ampersand:
        {
            strTrcMsg += "Qt::Key_Ampersand";
            break;
        }
        case Qt::Key_Apostrophe:
        {
            strTrcMsg += "Qt::Key_Apostrophe";
            break;
        }
        case Qt::Key_ParenLeft:
        {
            strTrcMsg += "Qt::Key_ParenLeft";
            break;
        }
        case Qt::Key_ParenRight:
        {
            strTrcMsg += "Qt::Key_ParenRight";
            break;
        }
        case Qt::Key_Asterisk:
        {
            strTrcMsg += "Qt::Key_Asterisk";
            break;
        }
        case Qt::Key_Plus:
        {
            strTrcMsg += "Qt::Key_Plus";
            break;
        }
        case Qt::Key_Comma:
        {
            strTrcMsg += "Qt::Key_Comma";
            break;
        }
        case Qt::Key_Minus:
        {
            strTrcMsg += "Qt::Key_Minus";
            break;
        }
        case Qt::Key_Period:
        {
            strTrcMsg += "Qt::Key_Period";
            break;
        }
        case Qt::Key_Slash:
        {
            strTrcMsg += "Qt::Key_Slash";
            break;
        }
        case Qt::Key_0:
        {
            strTrcMsg += "Qt::Key_0";
            break;
        }
        case Qt::Key_1:
        {
            strTrcMsg += "Qt::Key_1";
            break;
        }
        case Qt::Key_2:
        {
            strTrcMsg += "Qt::Key_2";
            break;
        }
        case Qt::Key_3:
        {
            strTrcMsg += "Qt::Key_3";
            break;
        }
        case Qt::Key_4:
        {
            strTrcMsg += "Qt::Key_4";
            break;
        }
        case Qt::Key_5:
        {
            strTrcMsg += "Qt::Key_5";
            break;
        }
        case Qt::Key_6:
        {
            strTrcMsg += "Qt::Key_6";
            break;
        }
        case Qt::Key_7:
        {
            strTrcMsg += "Qt::Key_7";
            break;
        }
        case Qt::Key_8:
        {
            strTrcMsg += "Qt::Key_8";
            break;
        }
        case Qt::Key_9:
        {
            strTrcMsg += "Qt::Key_9";
            break;
        }
        case Qt::Key_Colon:
        {
            strTrcMsg += "Qt::Key_Colon";
            break;
        }
        case Qt::Key_Semicolon:
        {
            strTrcMsg += "Qt::Key_Semicolon";
            break;
        }
        case Qt::Key_Less:
        {
            strTrcMsg += "Qt::Key_Less";
            break;
        }
        case Qt::Key_Equal:
        {
            strTrcMsg += "Qt::Key_Equal";
            break;
        }
        case Qt::Key_Greater:
        {
            strTrcMsg += "Qt::Key_Greater";
            break;
        }
        case Qt::Key_Question:
        {
            strTrcMsg += "Qt::Key_Question";
            break;
        }
        case Qt::Key_At:
        {
            strTrcMsg += "Qt::Key_At";
            break;
        }
        case Qt::Key_A:
        {
            strTrcMsg += "Qt::Key_A";
            break;
        }
        case Qt::Key_B:
        {
            strTrcMsg += "Qt::Key_B";
            break;
        }
        case Qt::Key_C:
        {
            strTrcMsg += "Qt::Key_C";
            break;
        }
        case Qt::Key_D:
        {
            strTrcMsg += "Qt::Key_D";
            break;
        }
        case Qt::Key_E:
        {
            strTrcMsg += "Qt::Key_E";
            break;
        }
        case Qt::Key_F:
        {
            strTrcMsg += "Qt::Key_F";
            break;
        }
        case Qt::Key_G:
        {
            strTrcMsg += "Qt::Key_G";
            break;
        }
        case Qt::Key_H:
        {
            strTrcMsg += "Qt::Key_H";
            break;
        }
        case Qt::Key_I:
        {
            strTrcMsg += "Qt::Key_I";
            break;
        }
        case Qt::Key_J:
        {
            strTrcMsg += "Qt::Key_J";
            break;
        }
        case Qt::Key_K:
        {
            strTrcMsg += "Qt::Key_K";
            break;
        }
        case Qt::Key_L:
        {
            strTrcMsg += "Qt::Key_L";
            break;
        }
        case Qt::Key_M:
        {
            strTrcMsg += "Qt::Key_M";
            break;
        }
        case Qt::Key_N:
        {
            strTrcMsg += "Qt::Key_N";
            break;
        }
        case Qt::Key_O:
        {
            strTrcMsg += "Qt::Key_O";
            break;
        }
        case Qt::Key_P:
        {
            strTrcMsg += "Qt::Key_P";
            break;
        }
        case Qt::Key_Q:
        {
            strTrcMsg += "Qt::Key_Q";
            break;
        }
        case Qt::Key_R:
        {
            strTrcMsg += "Qt::Key_R";
            break;
        }
        case Qt::Key_S:
        {
            strTrcMsg += "Qt::Key_S";
            break;
        }
        case Qt::Key_T:
        {
            strTrcMsg += "Qt::Key_T";
            break;
        }
        case Qt::Key_U:
        {
            strTrcMsg += "Qt::Key_U";
            break;
        }
        case Qt::Key_V:
        {
            strTrcMsg += "Qt::Key_V";
            break;
        }
        case Qt::Key_W:
        {
            strTrcMsg += "Qt::Key_W";
            break;
        }
        case Qt::Key_X:
        {
            strTrcMsg += "Qt::Key_X";
            break;
        }
        case Qt::Key_Y:
        {
            strTrcMsg += "Qt::Key_Y";
            break;
        }
        case Qt::Key_Z:
        {
            strTrcMsg += "Qt::Key_Z";
            break;
        }
        case Qt::Key_BracketLeft:
        {
            strTrcMsg += "Qt::Key_BracketLeft";
            break;
        }
        case Qt::Key_Backslash:
        {
            strTrcMsg += "Qt::Key_Backslash";
            break;
        }
        case Qt::Key_BracketRight:
        {
            strTrcMsg += "Qt::Key_BracketRight";
            break;
        }
        case Qt::Key_AsciiCircum:
        {
            strTrcMsg += "Qt::Key_AsciiCircum";
            break;
        }
        case Qt::Key_Underscore:
        {
            strTrcMsg += "Qt::Key_Underscore";
            break;
        }
        case Qt::Key_QuoteLeft:
        {
            strTrcMsg += "Qt::Key_QuoteLeft";
            break;
        }
        case Qt::Key_BraceLeft:
        {
            strTrcMsg += "Qt::Key_BraceLeft";
            break;
        }
        case Qt::Key_Bar:
        {
            strTrcMsg += "Qt::Key_Bar";
            break;
        }
        case Qt::Key_BraceRight:
        {
            strTrcMsg += "Qt::Key_BraceRight";
            break;
        }
        case Qt::Key_AsciiTilde:
        {
            strTrcMsg += "Qt::Key_AsciiTilde";
            break;
        }
        case Qt::Key_nobreakspace:
        {
            strTrcMsg += "Qt::Key_nobreakspace";
            break;
        }
        case Qt::Key_exclamdown:
        {
            strTrcMsg += "Qt::Key_exclamdown";
            break;
        }
        case Qt::Key_cent:
        {
            strTrcMsg += "Qt::Key_cent";
            break;
        }
        case Qt::Key_sterling:
        {
            strTrcMsg += "Qt::Key_sterling";
            break;
        }
        case Qt::Key_currency:
        {
            strTrcMsg += "Qt::Key_currency";
            break;
        }
        case Qt::Key_yen:
        {
            strTrcMsg += "Qt::Key_yen";
            break;
        }
        case Qt::Key_brokenbar:
        {
            strTrcMsg += "Qt::Key_brokenbar";
            break;
        }
        case Qt::Key_section:
        {
            strTrcMsg += "Qt::Key_section";
            break;
        }
        case Qt::Key_diaeresis:
        {
            strTrcMsg += "Qt::Key_diaeresis";
            break;
        }
        case Qt::Key_copyright:
        {
            strTrcMsg += "Qt::Key_copyright";
            break;
        }
        case Qt::Key_ordfeminine:
        {
            strTrcMsg += "Qt::Key_ordfeminine";
            break;
        }
        case Qt::Key_guillemotleft:
        {
            strTrcMsg += "Qt::Key_guillemotleft";
            break;
        }
        case Qt::Key_notsign:
        {
            strTrcMsg += "Qt::Key_notsign";
            break;
        }
        case Qt::Key_hyphen:
        {
            strTrcMsg += "Qt::Key_hyphen";
            break;
        }
        case Qt::Key_registered:
        {
            strTrcMsg += "Qt::Key_registered";
            break;
        }
        case Qt::Key_macron:
        {
            strTrcMsg += "Qt::Key_macron";
            break;
        }
        case Qt::Key_degree:
        {
            strTrcMsg += "Qt::Key_degree";
            break;
        }
        case Qt::Key_plusminus:
        {
            strTrcMsg += "Qt::Key_plusminus";
            break;
        }
        case Qt::Key_twosuperior:
        {
            strTrcMsg += "Qt::Key_twosuperior";
            break;
        }
        case Qt::Key_threesuperior:
        {
            strTrcMsg += "Qt::Key_threesuperior";
            break;
        }
        case Qt::Key_acute:
        {
            strTrcMsg += "Qt::Key_acute";
            break;
        }
        case Qt::Key_mu:
        {
            strTrcMsg += "Qt::Key_mu";
            break;
        }
        case Qt::Key_paragraph:
        {
            strTrcMsg += "Qt::Key_paragraph";
            break;
        }
        case Qt::Key_periodcentered:
        {
            strTrcMsg += "Qt::Key_periodcentered";
            break;
        }
        case Qt::Key_cedilla:
        {
            strTrcMsg += "Qt::Key_cedilla";
            break;
        }
        case Qt::Key_onesuperior:
        {
            strTrcMsg += "Qt::Key_onesuperior";
            break;
        }
        case Qt::Key_masculine:
        {
            strTrcMsg += "Qt::Key_masculine";
            break;
        }
        case Qt::Key_guillemotright:
        {
            strTrcMsg += "Qt::Key_guillemotright";
            break;
        }
        case Qt::Key_onequarter:
        {
            strTrcMsg += "Qt::Key_onequarter";
            break;
        }
        case Qt::Key_onehalf:
        {
            strTrcMsg += "Qt::Key_onehalf";
            break;
        }
        case Qt::Key_threequarters:
        {
            strTrcMsg += "Qt::Key_threequarters";
            break;
        }
        case Qt::Key_questiondown:
        {
            strTrcMsg += "Qt::Key_questiondown";
            break;
        }
        case Qt::Key_Agrave:
        {
            strTrcMsg += "Qt::Key_Agrave";
            break;
        }
        case Qt::Key_Aacute:
        {
            strTrcMsg += "Qt::Key_Aacute";
            break;
        }
        case Qt::Key_Acircumflex:
        {
            strTrcMsg += "Qt::Key_Acircumflex";
            break;
        }
        case Qt::Key_Atilde:
        {
            strTrcMsg += "Qt::Key_Atilde";
            break;
        }
        case Qt::Key_Adiaeresis:
        {
            strTrcMsg += "Qt::Key_Adiaeresis";
            break;
        }
        case Qt::Key_Aring:
        {
            strTrcMsg += "Qt::Key_Aring";
            break;
        }
        case Qt::Key_AE:
        {
            strTrcMsg += "Qt::Key_AE";
            break;
        }
        case Qt::Key_Ccedilla:
        {
            strTrcMsg += "Qt::Key_Ccedilla";
            break;
        }
        case Qt::Key_Egrave:
        {
            strTrcMsg += "Qt::Key_Egrave";
            break;
        }
        case Qt::Key_Eacute:
        {
            strTrcMsg += "Qt::Key_Eacute";
            break;
        }
        case Qt::Key_Ecircumflex:
        {
            strTrcMsg += "Qt::Key_Ecircumflex";
            break;
        }
        case Qt::Key_Ediaeresis:
        {
            strTrcMsg += "Qt::Key_Ediaeresis";
            break;
        }
        case Qt::Key_Igrave:
        {
            strTrcMsg += "Qt::Key_Igrave";
            break;
        }
        case Qt::Key_Iacute:
        {
            strTrcMsg += "Qt::Key_Iacute";
            break;
        }
        case Qt::Key_Icircumflex:
        {
            strTrcMsg += "Qt::Key_Icircumflex";
            break;
        }
        case Qt::Key_Idiaeresis:
        {
            strTrcMsg += "Qt::Key_Idiaeresis";
            break;
        }
        case Qt::Key_ETH:
        {
            strTrcMsg += "Qt::Key_ETH";
            break;
        }
        case Qt::Key_Ntilde:
        {
            strTrcMsg += "Qt::Key_Ntilde";
            break;
        }
        case Qt::Key_Ograve:
        {
            strTrcMsg += "Qt::Key_Ograve";
            break;
        }
        case Qt::Key_Oacute:
        {
            strTrcMsg += "Qt::Key_Oacute";
            break;
        }
        case Qt::Key_Ocircumflex:
        {
            strTrcMsg += "Qt::Key_Ocircumflex";
            break;
        }
        case Qt::Key_Otilde:
        {
            strTrcMsg += "Qt::Key_Otilde";
            break;
        }
        case Qt::Key_Odiaeresis:
        {
            strTrcMsg += "Qt::Key_Odiaeresis";
            break;
        }
        case Qt::Key_multiply:
        {
            strTrcMsg += "Qt::Key_multiply";
            break;
        }
        case Qt::Key_Ooblique:
        {
            strTrcMsg += "Qt::Key_Ooblique";
            break;
        }
        case Qt::Key_Ugrave:
        {
            strTrcMsg += "Qt::Key_Ugrave";
            break;
        }
        case Qt::Key_Uacute:
        {
            strTrcMsg += "Qt::Key_Uacute";
            break;
        }
        case Qt::Key_Ucircumflex:
        {
            strTrcMsg += "Qt::Key_Ucircumflex";
            break;
        }
        case Qt::Key_Udiaeresis:
        {
            strTrcMsg += "Qt::Key_Udiaeresis";
            break;
        }
        case Qt::Key_Yacute:
        {
            strTrcMsg += "Qt::Key_Yacute";
            break;
        }
        case Qt::Key_THORN:
        {
            strTrcMsg += "Qt::Key_THORN";
            break;
        }
        case Qt::Key_ssharp:
        {
            strTrcMsg += "Qt::Key_ssharp";
            break;
        }
        case Qt::Key_division:
        {
            strTrcMsg += "Qt::Key_division";
            break;
        }
        case Qt::Key_ydiaeresis:
        {
            strTrcMsg += "Qt::Key_ydiaeresis";
            break;
        }
        case Qt::Key_Back :
        {
            strTrcMsg += "Qt::Key_Back ";
            break;
        }
        case Qt::Key_Forward :
        {
            strTrcMsg += "Qt::Key_Forward ";
            break;
        }
        case Qt::Key_Stop :
        {
            strTrcMsg += "Qt::Key_Stop ";
            break;
        }
        case Qt::Key_Refresh :
        {
            strTrcMsg += "Qt::Key_Refresh ";
            break;
        }
        case Qt::Key_VolumeDown:
        {
            strTrcMsg += "Qt::Key_VolumeDown";
            break;
        }
        case Qt::Key_VolumeMute :
        {
            strTrcMsg += "Qt::Key_VolumeMute ";
            break;
        }
        case Qt::Key_VolumeUp:
        {
            strTrcMsg += "Qt::Key_VolumeUp";
            break;
        }
        case Qt::Key_BassBoost:
        {
            strTrcMsg += "Qt::Key_BassBoost";
            break;
        }
        case Qt::Key_BassUp:
        {
            strTrcMsg += "Qt::Key_BassUp";
            break;
        }
        case Qt::Key_BassDown:
        {
            strTrcMsg += "Qt::Key_BassDown";
            break;
        }
        case Qt::Key_TrebleUp:
        {
            strTrcMsg += "Qt::Key_TrebleUp";
            break;
        }
        case Qt::Key_TrebleDown:
        {
            strTrcMsg += "Qt::Key_TrebleDown";
            break;
        }
        case Qt::Key_MediaPlay :
        {
            strTrcMsg += "Qt::Key_MediaPlay ";
            break;
        }
        case Qt::Key_MediaStop :
        {
            strTrcMsg += "Qt::Key_MediaStop ";
            break;
        }
        case Qt::Key_MediaNext :
        {
            strTrcMsg += "Qt::Key_MediaNext ";
            break;
        }
        case Qt::Key_MediaRecord:
        {
            strTrcMsg += "Qt::Key_MediaRecord";
            break;
        }
        case Qt::Key_HomePage :
        {
            strTrcMsg += "Qt::Key_HomePage ";
            break;
        }
        case Qt::Key_Favorites :
        {
            strTrcMsg += "Qt::Key_Favorites ";
            break;
        }
        case Qt::Key_Search :
        {
            strTrcMsg += "Qt::Key_Search ";
            break;
        }
        case Qt::Key_Standby:
        {
            strTrcMsg += "Qt::Key_Standby";
            break;
        }
        case Qt::Key_OpenUrl:
        {
            strTrcMsg += "Qt::Key_OpenUrl";
            break;
        }
        case Qt::Key_LaunchMail :
        {
            strTrcMsg += "Qt::Key_LaunchMail ";
            break;
        }
        case Qt::Key_LaunchMedia:
        {
            strTrcMsg += "Qt::Key_LaunchMedia";
            break;
        }
        case Qt::Key_Launch0 :
        {
            strTrcMsg += "Qt::Key_Launch0 ";
            break;
        }
        case Qt::Key_Launch1 :
        {
            strTrcMsg += "Qt::Key_Launch1 ";
            break;
        }
        case Qt::Key_Launch2 :
        {
            strTrcMsg += "Qt::Key_Launch2 ";
            break;
        }
        case Qt::Key_Launch3 :
        {
            strTrcMsg += "Qt::Key_Launch3 ";
            break;
        }
        case Qt::Key_Launch4 :
        {
            strTrcMsg += "Qt::Key_Launch4 ";
            break;
        }
        case Qt::Key_Launch5 :
        {
            strTrcMsg += "Qt::Key_Launch5 ";
            break;
        }
        case Qt::Key_Launch6 :
        {
            strTrcMsg += "Qt::Key_Launch6 ";
            break;
        }
        case Qt::Key_Launch7 :
        {
            strTrcMsg += "Qt::Key_Launch7 ";
            break;
        }
        case Qt::Key_Launch8 :
        {
            strTrcMsg += "Qt::Key_Launch8 ";
            break;
        }
        case Qt::Key_Launch9 :
        {
            strTrcMsg += "Qt::Key_Launch9 ";
            break;
        }
        case Qt::Key_LaunchA :
        {
            strTrcMsg += "Qt::Key_LaunchA ";
            break;
        }
        case Qt::Key_LaunchB :
        {
            strTrcMsg += "Qt::Key_LaunchB ";
            break;
        }
        case Qt::Key_LaunchC :
        {
            strTrcMsg += "Qt::Key_LaunchC ";
            break;
        }
        case Qt::Key_LaunchD :
        {
            strTrcMsg += "Qt::Key_LaunchD ";
            break;
        }
        case Qt::Key_LaunchE :
        {
            strTrcMsg += "Qt::Key_LaunchE ";
            break;
        }
        case Qt::Key_LaunchF:
        {
            strTrcMsg += "Qt::Key_LaunchF";
            break;
        }
        default:
        {
            strTrcMsg += "Unknown Key";
            break;
        }
    }
    return strTrcMsg;

} // qtKey2AsciiStr

//------------------------------------------------------------------------------
QString ZS::Diagram::onOffState2Str( int i_iState )
//------------------------------------------------------------------------------
{
    static const char* s_arszOnOffState[EOnOffStateCount] = {
        "Off",
        "On"
    };
    if( i_iState >= 0 && i_iState < EOnOffStateCount )
    {
        return s_arszOnOffState[i_iState];
    }
    if( i_iState == EOnOffStateCount )
    {
        return "Count";
    }
    if( i_iState == EOnOffStateUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

//------------------------------------------------------------------------------
EOnOffState ZS::Diagram::str2OnOffState( const QString& i_str )
//------------------------------------------------------------------------------
{
    static const char* s_arszOnOffState[EOnOffStateCount] = {
        "Off",
        "On"
    };

    EOnOffState onOffState = EOnOffStateUndefined;
    int         idxState;

    for( idxState = 0; idxState < EOnOffStateCount; idxState++ )
    {
        if( s_arszOnOffState[idxState] == i_str )
        {
            onOffState = static_cast<EOnOffState>(idxState);
            break;
        }
    }
    if( onOffState == EOnOffStateUndefined )
    {
        if( i_str == "0" )
        {
            onOffState = EOnOffStateOff;
        }
        else if( i_str == "1" )
        {
            onOffState = EOnOffStateOn;
        }
    }
    return onOffState;
}

//------------------------------------------------------------------------------
QString ZS::Diagram::measState2Str( int i_iMeasState )
//------------------------------------------------------------------------------
{
    static const char* s_arszMeasState[EMeasStateCount] = {
        "Off",
        "On",
        "Halted"
    };
    if( i_iMeasState >= 0 && i_iMeasState < EMeasStateCount )
    {
        return s_arszMeasState[i_iMeasState];
    }
    if( i_iMeasState == EMeasStateCount )
    {
        return "Count";
    }
    if( i_iMeasState == EMeasStateUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

//------------------------------------------------------------------------------
QString ZS::Diagram::measMode2Str( int i_iMeasMode )
//------------------------------------------------------------------------------
{
    static const char* s_arszMeasMode[EMeasModeCount] = {
        "Single",
        "Cont."
    };
    if( i_iMeasMode >= 0 && i_iMeasMode < EMeasModeCount )
    {
        return s_arszMeasMode[i_iMeasMode];
    }
    if( i_iMeasMode == EMeasModeCount )
    {
        return "Count";
    }
    if( i_iMeasMode == EMeasModeUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

//------------------------------------------------------------------------------
QString ZS::Diagram::diagramUpdateType2Str( int i_iUpdateType )
//------------------------------------------------------------------------------
{
    static const char* s_arszUpdateType[EDiagramUpdateTypeCount] = {
        "Data",
        "Pixmap",
        "Widget"
    };
    if( i_iUpdateType >= 0 && i_iUpdateType < EDiagramUpdateTypeCount )
    {
        return s_arszUpdateType[i_iUpdateType];
    }
    if( i_iUpdateType == EDiagramUpdateTypeCount )
    {
        return "Count";
    }
    if( i_iUpdateType == EDiagramUpdateTypeUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

static const char* s_arszUpdateFlags[EUpdateAll+1] = {
    "None",                      // = 0x00, 0000 0000
    "Layout",                    // = 0x01, 0000 0001
    "Data",                      // = 0x02, 0000 0010
    "Layout|Data",               // = 0x03, 0000 0011
    "Pixmap",                    // = 0x04, 0000 0100
    "Layout|Pixmap",             // = 0x05, 0000 0101
    "Data|Pixmap",               // = 0x06, 0000 0110
    "Layout|Data|Pixmap",        // = 0x07, 0000 0111
    "Widget",                    // = 0x08, 0000 1000
    "Layout|Widget",             // = 0x09, 0000 1001
    "Data|Widget",               // = 0x0A, 0000 1010
    "Layout|Data|Widget",        // = 0x0B, 0000 1011
    "Pixmap|Widget",             // = 0x0C, 0000 1100
    "Layout|Pixmap|Widget",      // = 0x0D, 0000 1101
    "Data|Pixmap|Widget",        // = 0x0E, 0000 1110
    "Layout|Data|Pixmap|Widget"  // = 0x0F  0000 1111
};

//------------------------------------------------------------------------------
QString ZS::Diagram::updateFlags2Str( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    unsigned int uUpdateFlags = EUpdateAll & i_uUpdateFlags;
    if( uUpdateFlags <= EUpdateAll )
    {
        return s_arszUpdateFlags[uUpdateFlags];
    }
    return "?";

} // updateFlags2Str

//------------------------------------------------------------------------------
QString ZS::Diagram::scaleDir2Str( int i_iScaleDir )
//------------------------------------------------------------------------------
{
    static const char* s_arszScaleDir[EScaleDirCount] = {
        "X",
        "Y"
    };
    if( i_iScaleDir >= 0 && i_iScaleDir < EScaleDirCount )
    {
        return s_arszScaleDir[i_iScaleDir];
    }
    if( i_iScaleDir == EScaleDirCount )
    {
        return "Count";
    }
    if( i_iScaleDir == EScaleDirUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";

} // scaleDir2Str

//------------------------------------------------------------------------------
QString ZS::Diagram::orientation2Str( int i_iOrientation )
//------------------------------------------------------------------------------
{
    static const char* s_arszOrientation[EOrientationCount] = {
        "Hor",
        "Ver"
    };
    if( i_iOrientation >= 0 && i_iOrientation < EOrientationCount )
    {
        return s_arszOrientation[i_iOrientation];
    }
    if( i_iOrientation == EOrientationCount )
    {
        return "Count";
    }
    if( i_iOrientation == EOrientationUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";

} // orientation2Str

//------------------------------------------------------------------------------
QString ZS::Diagram::spacing2Str( int i_iSpacing )
//------------------------------------------------------------------------------
{
    static const char* s_arszSpacing[ESpacingCount] = {
        "Linear",
        "Logarithmic"
    };
    if( i_iSpacing >= 0 && i_iSpacing < ESpacingCount )
    {
        return s_arszSpacing[i_iSpacing];
    }
    if( i_iSpacing == ESpacingCount )
    {
        return "Count";
    }
    if( i_iSpacing == ESpacingUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";

} // spacing2Str

static const char* s_arszLayoutPos[ELayoutPosCount] = {
    "Top",
    "Bottom",
    "Left",
    "Right",
    "Center",
    "Undefined"
};

//------------------------------------------------------------------------------
QString ZS::Diagram::layoutPos2Str( int i_iLayoutPos )
//------------------------------------------------------------------------------
{
    if( i_iLayoutPos >= 0 && i_iLayoutPos < ELayoutPosCount )
    {
        return s_arszLayoutPos[i_iLayoutPos];
    }
    if( i_iLayoutPos == ELayoutPosCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // layoutPos2Str

//------------------------------------------------------------------------------
QString ZS::Diagram::divLineLayer2Str( int i_iLayer )
//------------------------------------------------------------------------------
{
    static const char* s_arszLayer[EDivLineLayerCount] = {
        "Main",
        "Sub"
    };
    if( i_iLayer >= 0 && i_iLayer < EDivLineLayerCount )
    {
        return s_arszLayer[i_iLayer];
    }
    if( i_iLayer == EDivLineLayerCount )
    {
        return "Count";
    }
    if( i_iLayer == EDivLineLayerUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";

} // divLineLayer2Str

static const char* s_arszTextDirection[ETextDirectionCount] = {
    "Left2Right",
    "Right2Left",
    "Bottom2Top",
    "Top2Bottom"
};

//------------------------------------------------------------------------------
QString ZS::Diagram::textDirection2Str( int i_iTextDirection )
//------------------------------------------------------------------------------
{
    if( i_iTextDirection >= 0 && i_iTextDirection < ETextDirectionCount )
    {
        return s_arszTextDirection[i_iTextDirection];
    }
    if( i_iTextDirection == ETextDirectionCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // textDirection2Str

static const char* s_arszMoveMode[EDiagObjMoveModeCount] = {
    "ToTop",
    "ToBottom",
    "OneStepUp",
    "OneStepDown",
    "Relative",
    "Absolute"
};

//------------------------------------------------------------------------------
QString ZS::Diagram::moveMode2Str( int i_iMoveMode )
//------------------------------------------------------------------------------
{
    if( i_iMoveMode >= 0 && i_iMoveMode < EDiagObjMoveModeCount )
    {
        return s_arszMoveMode[i_iMoveMode];
    }
    if( i_iMoveMode == EDiagObjMoveModeCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // moveMode2Str

static const char* s_arszDiagObjState[] = {
    "Normal",
    "Focused",
    "Editing"
};

//------------------------------------------------------------------------------
QString ZS::Diagram::diagObjState2Str( int i_iDiagObjState )
//------------------------------------------------------------------------------
{
    if( i_iDiagObjState >= 0 && i_iDiagObjState < _ZSArrLen(s_arszDiagObjState) )
    {
        return s_arszDiagObjState[i_iDiagObjState];
    }
    if( i_iDiagObjState == EDiagObjStateCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // diagObjState2Str
