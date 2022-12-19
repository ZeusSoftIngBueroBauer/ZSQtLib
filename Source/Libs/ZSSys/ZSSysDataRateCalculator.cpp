/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSSys/ZSSysDataRateCalculator.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CDataRateCalculator
*******************************************************************************/

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Default constructor creating an invalid measure point.
*/
CDataRateCalculator::SMeas::SMeas() :
//------------------------------------------------------------------------------
    m_fRecordTime_s(0.0),
    m_fMeasTime_s(0.0),
    m_iUnits(0)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a measure point with the given units.

    The measure and the time of recording the measurement is set to the
    processor time (time in seconds since program has started).

    @param i_iUnits [in] Data rate units (bytes, symbols, lines, etc.).
*/
CDataRateCalculator::SMeas::SMeas( int i_iUnits ) :
//------------------------------------------------------------------------------
    m_fRecordTime_s(Time::getProcTimeInSec()),
    m_fMeasTime_s(0.0),
    m_iUnits(i_iUnits)
{
    m_fMeasTime_s = m_fRecordTime_s;
}

//------------------------------------------------------------------------------
/*! @brief Creates a measure point with the given measure time and units.

    The record time is set to the measure time.

    @param i_fMeasTime_s [in] Time in seconds the measurement was carried out.
    @param i_iUnits [in] Data rate units (bytes, symbols, lines, etc.).
*/
CDataRateCalculator::SMeas::SMeas( double i_fMeasTime_s, int i_iUnits ) :
//------------------------------------------------------------------------------
    m_fRecordTime_s(i_fMeasTime_s),
    m_fMeasTime_s(i_fMeasTime_s),
    m_iUnits(i_iUnits)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a measure point with the given time of recording, time of
           measurement and units.

    @param i_fRecordTime_s [in] Time in seconds the measurement is recorded.
    @param i_fMeasTime_s [in] Time in seconds the measurement was carried out.
    @param i_iUnits [in] Data rate units (bytes, symbols, lines, etc.).
*/
CDataRateCalculator::SMeas::SMeas( double i_fRecordTime_s, double i_fMeasTime_s, int i_iUnits ) :
//------------------------------------------------------------------------------
    m_fRecordTime_s(i_fRecordTime_s),
    m_fMeasTime_s(i_fMeasTime_s),
    m_iUnits(i_iUnits)
{
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the data rate calculator.

    @param i_iMaxMeasurePoints [in] Default 100
        Defines the maximum number of measure points which can be added.
        When adding a measurement if the maximum number is already reached
        the first measurement in the vector will be removed.
*/
CDataRateCalculator::CDataRateCalculator( int i_iMaxMeasurePoints ) :
//------------------------------------------------------------------------------
    m_fMeasurementsTimeSpan_s(10.0),
    m_fRecordingTimeSpan_s(10.0),
    m_iMaxMeasurePoints(i_iMaxMeasurePoints),
    m_arMeasurements()
{
}

//------------------------------------------------------------------------------
/*! @brief Destroys the data rate calculator instance.
*/
CDataRateCalculator::~CDataRateCalculator()
//------------------------------------------------------------------------------
{
    m_fMeasurementsTimeSpan_s = 0.0;
    m_fRecordingTimeSpan_s = 0.0;
    m_iMaxMeasurePoints = 0;
    //m_arMeasurements;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the maximum number of measurements which can be added.

    @param i_iMaxMeasurePoints [in]
        Maximum number of measure points which can be added.
        If the size of the vector got to be decreased the first measuremts
        of the vector are removed.
*/
void CDataRateCalculator::setMaxMeasurePoints( int i_iMaxMeasurePoints )
//------------------------------------------------------------------------------
{
    m_iMaxMeasurePoints = i_iMaxMeasurePoints;

    if( m_arMeasurements.size() > m_iMaxMeasurePoints )
    {
        QVector<SMeas> arMeasurements = m_arMeasurements.mid(m_arMeasurements.size()-m_iMaxMeasurePoints);
        m_arMeasurements = arMeasurements;
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum number of measurements which can be added.
*/
int CDataRateCalculator::getMaxMeasurePoints() const
//------------------------------------------------------------------------------
{
    return m_iMaxMeasurePoints;
}

//------------------------------------------------------------------------------
/*! @brief Sets the time span for the time when the measurements are carried.

    If there are older measurements than defined by the time span those measurements
    will be removed.

    This method affects the time the measurements have been carried out.

    @param i_iTimeSpan_s [in]
        Defines the time span the measurements are kept. When calling a method
        of the data rate calculator measurements which are older than the current
        time minus the time span are removed.
*/
void CDataRateCalculator::setMeasurePointsTimeSpanInSec( double i_fTimeSpan_s )
//------------------------------------------------------------------------------
{
    m_fMeasurementsTimeSpan_s = i_fTimeSpan_s;
    updateMeasurements();
}

//------------------------------------------------------------------------------
/*! @brief Returns the time span for the measurements.

    This method affects the time the measurements were carried out.
*/
double CDataRateCalculator::getMeasurePointsTimeSpanInSec() const
//------------------------------------------------------------------------------
{
    return m_fMeasurementsTimeSpan_s;
}

//------------------------------------------------------------------------------
/*! @brief Sets the time span for the time the measurements are recorded.

    This method affects the time the measurements have been recorded.

    @param i_iTimeSpan_s [in]
        Defines the time span the measurements are kept. When calling a method
        of the data rate calculator measurements which are older than the current
        time minus the time span are removed.
*/
void CDataRateCalculator::setRecordTimeSpanInSec( double i_fTimeSpan_s )
//------------------------------------------------------------------------------
{
    m_fRecordingTimeSpan_s = i_fTimeSpan_s;
    updateMeasurements();
}

//------------------------------------------------------------------------------
/*! @brief Returns the time span for the measurements.

    This method affects the time the measurements were recorded.
*/
double CDataRateCalculator::getRecordTimeSpanInSec() const
//------------------------------------------------------------------------------
{
    return m_fRecordingTimeSpan_s;
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of stored measurements.

    @return Number of measurements added to the vector of measurements.
*/
//------------------------------------------------------------------------------
int CDataRateCalculator::getMeasurePointsCount() const
{
    return m_arMeasurements.count();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Adds the number of units to the array of measurements.

    The measure time is taken from the processor time since program start.
    Calling this method updates the array by removing measurements which are
    older than the specified time span.

    @param i_iUnits [in] Number of units to be added to the measurements.
        Units can be anything. Bytes, blocks, symbols, text lines.
*/
void CDataRateCalculator::addMeasurement( int i_iUnits )
//------------------------------------------------------------------------------
{
    double fProcTime_s = ZS::System::Time::getProcTimeInSec();
    addMeasurement(fProcTime_s, i_iUnits);
}

//------------------------------------------------------------------------------
/*! @brief Adds the number of units with the given measurement time to the array
           of measurements.

    Calling this method updates the array by removing measurements which are
    older than the specified time span.

    @param i_fMeasTime_s [in] Time the measurement is carried out.
        Usually the number of seconds since a defined start time.
    @param i_iUnits [in] Number of units to be added to the measurements.
        Units can be anything. Bytes, blocks, symbols, text lines.
*/
void CDataRateCalculator::addMeasurement( double i_fMeasTime_s, int i_iUnits )
//------------------------------------------------------------------------------
{
    double fProcTime_s = ZS::System::Time::getProcTimeInSec();
    addMeasurement(fProcTime_s, i_fMeasTime_s, i_iUnits);
}

//------------------------------------------------------------------------------
/*! @brief Adds the number of units with the given measurement time to the array
           of measurements.

    Calling this method updates the array by removing measurements which are
    older than the specified time span.

    @param i_fRecordTime_s [in] Time the measurement is recorded.
        Usually the number of seconds since a defined start time.
    @param i_fMeasTime_s [in] Time the measurement is carried out.
        Usually the number of seconds since a defined start time.
    @param i_iUnits [in] Number of units to be added to the measurements.
        Units can be anything. Bytes, blocks, symbols, text lines.
*/
void CDataRateCalculator::addMeasurement( double i_fRecordTime_s,  double i_fMeasTime_s, int i_iUnits )
//------------------------------------------------------------------------------
{
    m_arMeasurements << SMeas(i_fRecordTime_s, i_fMeasTime_s, i_iUnits);
    if( m_arMeasurements.size() > m_iMaxMeasurePoints )
    {
        m_arMeasurements.removeFirst();
    }
    updateMeasurements();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the data rate in units per seconds for the measurements
           taken during the last second.

    @note Before calculating the average data rate, measurements which are older
          than the defined maximum time span, are removed from the measurements.
          If no measurements have been added within the time span a data rate
          of 0 is returned.

    @param i_fTimeSpan_s [in] Default: 0.0
        Time span which should be taken into account.
        If 0.0 is passed the configured time span is used.
        If the time span is greater than the configured time span the result
        is less then if the configured time span would have been used.
    @param o_pfTimeSpan_s [out] Default: nullptr
        If not nullptr the time span taken into account for calculating the
        data rate is returned here. This time span may be different from the
        given time span if the time span of the stored measurements is less
        than the given time span.

    @return Calculated average data rate in units per second.
*/
int CDataRateCalculator::getMeasurePointsDataRatePerSec( double i_fTimeSpan_s, double* o_pfTimeSpan_s ) const
//------------------------------------------------------------------------------
{
    int iDataRate_perSec = 0;
    double fTimeSpan_s = 0.0;

    if( !m_arMeasurements.isEmpty() )
    {
        fTimeSpan_s = i_fTimeSpan_s > 0.0 ? i_fTimeSpan_s : m_fMeasurementsTimeSpan_s;

        int    iUnits = 0;
        double fTimeEnd_s = m_arMeasurements.last().m_fMeasTime_s;
        double fTimeBeg_s = fTimeEnd_s;


        for( int idx = m_arMeasurements.size()-1; idx >= 0; --idx )
        {
            SMeas& meas = m_arMeasurements[idx];

            if( (fTimeEnd_s - meas.m_fMeasTime_s) <= fTimeSpan_s )
            {
                iUnits += meas.m_iUnits;
                fTimeBeg_s = meas.m_fMeasTime_s;
            }
            else
            {
                break;
            }
        }
        fTimeSpan_s = fTimeEnd_s - fTimeBeg_s;
        iDataRate_perSec = fTimeSpan_s > 0.0 ?  iUnits / fTimeSpan_s : 0;
    }
    if( o_pfTimeSpan_s != nullptr )
    {
        *o_pfTimeSpan_s = fTimeSpan_s;
    }
    return iDataRate_perSec;

} // getMeasurePointsDataRatePerSec

//------------------------------------------------------------------------------
/*! @brief Returns the data rate in units per seconds for the measurements
           taken during the last second.

    @note Before calculating the average data rate, measurements which are older
          than the defined maximum time span, are removed from the measurements.
          If no measurements have been added within the time span a data rate
          of 0 is returned.

    @param i_fTimeSpan_s [in] Default: 0.0
        Time span which should be taken into account.
        If 0.0 is passed the configured time span is used.
        If the time span is greater than the configured time span the result
        is less then if the configured time span would have been used.
    @param o_pfTimeSpan_s [out] Default: nullptr
        If not nullptr the time span taken into account for calculating the
        data rate is returned here. This time span may be different from the
        given time span if the time span of the stored measurements is less
        than the given time span.

    @return Calculated average data rate in units per second.
*/
int CDataRateCalculator::getRecordingDataRatePerSec( double i_fTimeSpan_s, double* o_pfTimeSpan_s ) const
//------------------------------------------------------------------------------
{
    int iDataRate_perSec = 0;
    double fTimeSpan_s = 0.0;

    if( !m_arMeasurements.isEmpty() )
    {
        fTimeSpan_s = i_fTimeSpan_s > 0.0 ? i_fTimeSpan_s : m_fRecordingTimeSpan_s;

        int    iUnits = 0;
        double fTimeEnd_s = m_arMeasurements.last().m_fRecordTime_s;
        double fTimeBeg_s = fTimeEnd_s;

        for( int idx = m_arMeasurements.size()-1; idx >= 0; --idx )
        {
            SMeas& meas = m_arMeasurements[idx];

            if( (fTimeEnd_s - meas.m_fRecordTime_s) <= fTimeSpan_s )
            {
                iUnits += meas.m_iUnits;
                fTimeBeg_s = meas.m_fRecordTime_s;
            }
            else
            {
                break;
            }
        }
        fTimeSpan_s = fTimeEnd_s - fTimeBeg_s;
        iDataRate_perSec = fTimeSpan_s > 0.0 ?  iUnits / fTimeSpan_s : 0;
    }
    if( o_pfTimeSpan_s != nullptr )
    {
        *o_pfTimeSpan_s = fTimeSpan_s;
    }
    return iDataRate_perSec;

} // getRecordingDataRatePerSec

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Updates the measurements array by removing measurement which are older
           than defined by the configured time span of recording measurements.

    This method may be also called continuosly triggered by a timer to refresh
    the data rate if no data is received.
*/
void CDataRateCalculator::updateMeasurements()
//------------------------------------------------------------------------------
{
    if( m_fRecordingTimeSpan_s <= 0 )
    {
        m_arMeasurements.clear();
    }
    else if( !m_arMeasurements.isEmpty() )
    {
        double fProcTime_s = Time::getProcTimeInSec();
        double fTimeDiff_s = fProcTime_s - m_arMeasurements.first().m_fRecordTime_s;

        while( fTimeDiff_s > static_cast<double>(m_fRecordingTimeSpan_s) )
        {
            m_arMeasurements.removeFirst();
            if( m_arMeasurements.isEmpty() )
            {
                break;
            }
            fTimeDiff_s = fProcTime_s - m_arMeasurements.first().m_fRecordTime_s;
        }
    }
}
