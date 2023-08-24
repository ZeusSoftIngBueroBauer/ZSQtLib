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

#ifndef ZSSys_DataRateCalculator_h
#define ZSSys_DataRateCalculator_h

#include "ZSSys/ZSSysDllMain.h"

#include <QtCore/qvector.h>

namespace ZS
{
namespace System
{
//******************************************************************************
/*! @brief Data rate calculator.

    The data rate calculator can be used to calculate the average data rate.

    Measure points are kept in memory for a maximum configured time span and
    a maximum configured count of measure points.

    For a measure point two different times are stored:

    - The time the measure point has been recorded.
    - The time the measure point has been originally carried out.

    Those two times may be the same if the measure point is immediately added
    to the data rate calculator when the measurement is carried out.

    It might be necessary to limit the recording time not to the time span
    as given by the measure points but to a maximum recording time.

    This might be the case when a client receives data from a server.
    The server may add a time stamp to each block of data when sending the data block.
    The client may want to add a time stamp when the data block has been received.
    The time span on servers side for sending the data blocks might be less than
    the time span on clients side for receiving and processing the data.
    If the server sends the data faster than the client can process the data not all
    data blocks may be received during the defined recording time span.

    Measure points may be thrown away for both if the measure points time span is
    exceeded or if the recording time span is exceeded.

    The data rate calculator provides therefore two different data rates:

    - A data rate for the recording time of the measure points (e.g. when a client received the data).
    - A data rate for the time the measurements have been carried out (e.g. when the server sent the data).

    The two data rates are equal if the measure points are stored at the same time
    the measurement is carried out. E.g. if the server sending the data will add the
    measure point to the data rate calculator when he sends the data.

    Before retrieving the data rate you need to call updateMeasuremts to remove
    measurements which are older the configured recording time span.

    @note As data rate you can use any unit not just bytes. You may also use
          symbols or lines.

    **Usage**

    @code

    class CMyClient {
    public: // ctor
        CMyClient();
    private: // instance members
        !< Data rate calculator for calculating the data rate in bytes per seconds.
        CDataRateCalculator m_dataRateCalculatorBytes;
        !< Data rate calculator for calculating the data rate in lines per seconds.
        CDataRateCalculator m_dataRateCalculatorLines;
        !< Here we store for the last 10 seconds the difference between the data rates
           the server was sending the data and the client received and processed the data.
        QVector<int> m_ariDataRateDiffs_linesPerSec;
    };

    CMyClient::CMyClient() :
        // We want to have the average data rate for the bytes received during the last 10 seconds.
        m_dataRateCalculatorBytes(),
        // We want to have the average data rate for the lines received during the last 10 seconds.
        m_dataRateCalculatorLines()
    {
        m_dataRateCalculatorBytes.setRecordTimeSpanInSec(10.0);
        m_dataRateCalculatorBytes.setMeasurePointsTimeSpanInSec(10.0);

        m_dataRateCalculatorLines.setRecordTimeSpanInSec(10.0);
        m_dataRateCalculatorLines.setMeasurePointsTimeSpanInSec(10.0);
    }

    void CMyClient::onDataReceived( const QString& i_strData )
    {
        // We received a text line as a string. In the text line
        // the time the data was sent is encoded.
        double fTimeSent_s = retrieveTimeStamp(i_strData);
        double fRecordTime_s = Time::getProcTimeInSec();

        // As we want to know the data rate in bytes per seconds
        // we save the number received bytes.
        dataRateCalculatorBytes.addMeasurement(fRecordTime_s, fTimeSent_s, 2 * strLine.size());

        // As we want to know the data rate in text lines per seconds
        // we add the number of lines received (which is one line).
        dataRateCalculatorLines.addMeasurement(fRecordTime_s, fTimeSent_s, 1);
    }

    void CMyClient::onTmrDataRateRefreshTimeout()
    {
        m_dataRateCalculatorBytes.updateMeasurements();
        m_dataRateCalculatorLines.updateMeasurements();

        // A timer has been started which should update the average data rates every second.
        // On clients site we are interested in both the data rate the server sends data
        // and the data rate the client can process the data.
        int iServerSendDataRate_bytesPerSec = m_dataRateCalculatorBytes.getMeasurePointsDataRatePerSec();
        int iClientProcDataRate_bytesPerSec = m_dataRateCalculatorBytes.getRecordingDataRatePerSec();
        int iServerSendDataRate_linesPerSec = m_dataRateCalculatorLines.getMeasurePointsDataRatePerSec();
        int iClientProcDataRate_linesPerSec = m_dataRateCalculatorLines.getRecordingDataRatePerSec();

        // Here we store the difference between the data rates on server and client side
        // for the last 10 seconds.
        m_ariDataRateDiffs_linesPerSec.insert(0,
            m_dataRateCalculatorLines.getMeasurePointsDataRatePerSec()
          - m_dataRateCalculatorLines.getRecordingDataRatePerSec());
        if( m_ariDataRateDiffs_linesPerSec.size() > 10 )
        {
            m_ariDataRateDiffs_linesPerSec.removeLast();
        }

        bool bClientCanProcessData = false;

        // Here we check whether the client can process the data.
        for( int iDataRateDiff : m_ariDataRateDiffs_linesPerSec )
        {
            // If for at least one second the client was able to process the data ..
            if( iDataRateDiff <= 0 )
            {
                bClientCanProcessData = true;
                break;
            }
        }

        // If the client could not process the data for a period longer than 10 seconds
        // we need to do something as in that case the input buffer may overrun and the
        // client may be become unresponsive.
    }
    @endcode
*/
class ZSSYSDLL_API CDataRateCalculator
//******************************************************************************
{
public: // ctors and dtor
    CDataRateCalculator( int i_iMaxMeasurePoints = 100 );
    ~CDataRateCalculator();
public: // instance methods
    void setMaxMeasurePoints( int i_iMaxMeasurePoints );
    int getMaxMeasurePoints() const;
    void setMeasurePointsTimeSpanInSec( double i_fTimeSpan_s );
    double getMeasurePointsTimeSpanInSec() const;
    void setRecordTimeSpanInSec( double i_fTimeSpan_s );
    double getRecordTimeSpanInSec() const;
    int getMeasurePointsCount() const;
public: // instance methods
    void addMeasurement( int i_iUnits );
    void addMeasurement( double i_fMeasTime_s, int i_iUnits );
    void addMeasurement( double i_fRecordTime_s, double i_fMeasTime_s, int i_iUnits );
public: // instance methods
    int getMeasurePointsDataRatePerSec( double i_fTimeSpan_s = 0.0, double* o_pfTimeSpan_s = nullptr ) const;
    int getRecordingDataRatePerSec( double i_fTimeSpan_s = 0.0, double* o_pfTimeSpan_s = nullptr ) const;
protected: // type definitions and constants
    struct SMeas {
        SMeas();
        SMeas( int i_iUnits );
        SMeas( double i_fMeasTime_s, int i_iUnits );
        SMeas( double i_fRecordTime_s, double i_fMeasTime_s, int i_iUnits );
        double m_fRecordTime_s; /*!< Time at which the measurement has been recorded. */
        double m_fMeasTime_s;   /*!< Time at which the measurement was carried out. */
        int m_iUnits;           /*!< Measured units. */
    };
public: // instance methods
    void updateMeasurements();
protected:
    /*!< Time span in seconds for the time the measurements were carried out.
         Default: 10.0 seconds */
    double m_fMeasurementsTimeSpan_s;
    /*!< Time span in seconds for the time the measurements are recorded.
         Default: 10.0 seconds */
    double m_fRecordingTimeSpan_s;
    /*!< Maximum number of measurements which can be added. Default: 100 */
    int m_iMaxMeasurePoints;
    /*!< Vector with measurements (time and number of bytes received).
         Only the measurements within the specified time span are kept. */
    mutable QVector<SMeas> m_arMeasurements;

}; // class CDataRateCalculator

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_DataRateCalculator_h
