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

#ifndef ZSDiagObjProgressBar_h
#define ZSDiagObjProgressBar_h

#include "ZSDiagram/ZSDiagObj.h"

#include <QtGui/qcolor.h>

namespace ZS
{
namespace Diagram
{
struct SFrameStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjProgressBar : public CDiagObj
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString ClassName() { return "CDiagObjProgressBar"; }
public: // ctors and dtor
    CDiagObjProgressBar(
        const QString& i_strObjName,
        ELayoutPos     i_layoutPos );
    virtual ~CDiagObjProgressBar();
public: // must overridables of base class CDiagObj
    virtual QString className() { return ClassName(); }
public: // instance methods
    void setValMin( double i_fVal );
    double getValMin() const;
    void setValMax( double i_fVal );
    double getValMax() const;
    void setVal( double i_fVal );
    double getVal() const;
    void setColFg( const QColor& i_col );
    QColor getColFg() const;
    void setColBg( const QColor& i_col );
    QColor getColBg() const;
    void setIndentTop( int i_iIndent );
    int getIndentTop() const;
    void setIndentBottom( int i_iIndent );
    int getIndentBottom() const;
    void setIndentLeft( int i_iIndent );
    int getIndentLeft() const;
    void setIndentRight( int i_iIndent );
    int getIndentRight() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    void setWidth( int i_iWidth );  // Inside the frame: corresponds to width if LayoutLeft or Right, height if LayoutTop or Bottom
    int getWidth() const;
public: // overridables of base class CDiagObj
    virtual QSize sizeHint();
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected: // overridables of base class QObject
    virtual bool event( QEvent* i_pEv );
private: // copy ctor not allowed
    CDiagObjProgressBar( const CDiagObjProgressBar& );
private: // assignment operator not allowed
    void operator=( const CDiagObjProgressBar& );
protected:  // instance members
    double       m_fValMin;
    double       m_fValMax;
    double       m_fVal;
    QColor       m_colFg;
    QColor       m_colBg;
    int          m_iIndentTop;            // Indents are the space between
    int          m_iIndentBottom;         // the content rect and the bars frame.
    int          m_iIndentLeft;
    int          m_iIndentRight;
    SFrameStyle* m_pFrameStyle;           // Frame around the bar.
    int          m_iWidth;                // Inside the frame: corresponds to width if LayoutLeft or Right, height if LayoutTop or Bottom
    QRect        m_rectBarContent;        // May differ from m_rectContent if limited to the diagrams center part
    QRect        m_rectBarFrame;          // Inside indents, including frame
    QRect        m_rectBarInFrameContent; // Inside frame
    QRect        m_rectBarInFrameValue;   // Inside frame indicating the value
    bool         m_bUpdWidget;

}; // class CDiagObjProgressBar

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjProgressBar_h
