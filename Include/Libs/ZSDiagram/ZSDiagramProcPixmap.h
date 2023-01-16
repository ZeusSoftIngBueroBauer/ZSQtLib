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

#ifndef ZSDiagram_ProcPixmap_h
#define ZSDiagram_ProcPixmap_h

#include "ZSDiagram/ZSDiagramProcData.h"

#include <QtCore/qrect.h>
#include <QtGui/qcolor.h>

class QPrinter;
class QIODevice;

namespace ZS
{
namespace Diagram
{
struct SFrameStyle;

//******************************************************************************
class ZSDIAGRAMDLL_API CPixmapDiagram : public CDataDiagram
//******************************************************************************
{
public: // class methods
    static QString ClassName() { return "CPixmapDiagram"; }
public: // class members
    static const char* g_arszImageFormatFileFilter[EImageFormatCount+1]; // Last element set to nullptr
    static const char* g_arszImageFormatFileExt[EImageFormatCount+1];    // Last element set to nullptr
    static const int   g_ariImageFormat[EImageFormatCount+1];            // Last element set to -1
    static const char* g_arszQtImageFormat[EImageFormatCount+1];         // Last element set to nullptr
public: // class methods
    static EImageFormat GetImageFormat( const QString& i_strImageFormat );
public: // ctors and dtor
    CPixmapDiagram(
        const QString&     i_strObjName,
        EDiagramUpdateType i_updateType  = EDiagramUpdateTypePixmap );
    virtual ~CPixmapDiagram();
public: // copy ctor not allowed but diagrams may be cloned
    virtual CDataDiagram* clone( EDiagramUpdateType i_diagramUpdateType ) const;
public: // instance methods
    bool save( const QString& i_strFileName, const QString& i_strQtImageFormat );
    bool save( const QString& i_strFileName, EImageFormat i_imageFormat );
    bool save( QIODevice* i_pDevice, EImageFormat i_imageFormat );
    bool save( QIODevice* i_pDevice, const QString& i_strQtImageFormat );
    void print( const QPrinter* i_pPrinter ) const;
public: // instance methods to set and get the attributes of the diagram
    void setWidth( int i_cxWidth );
    int getWidth() const; // kann nicht "width" verwenden wegen CustomXXXXyhdfViewer vom CompassGuiFramework
    void setHeight( int i_cyHeight );
    int getHeight() const; // kann nicht "height" verwenden wegen CustomXXXXyhdfViewer vom CompassGuiFramework
    void setSize( const QSize& i_size );
    QSize getSize() const; // kann nicht "size" verwenden wegen CustomXXXXyhdfViewer vom CompassGuiFramework
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    QRect getRectPartTop() const;
    QRect getRectPartBottom() const;
    QRect getRectPartLeft() const;
    QRect getRectPartRight() const;
    QRect getRectPartCenter() const;
    void setMinimumHeightPartTop( int i_cyHeight );
    int getMinimumHeightPartTop() const;
    void setMinimumHeightPartBottom( int i_cyHeight );
    int getMinimumHeightPartBottom() const;
    void setMinimumWidthPartLeft( int i_cxWidth );
    int getMinimumWidthPartLeft() const;
    void setMinimumWidthPartRight( int i_cxWidth );
    int getMinimumWidthPartRight() const;
    void setColErase( const QColor& i_col );
    QColor getColErase() const;
    void setColBgPartCenter( const QColor& i_col );
    QColor getColBgPartCenter() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    void setFrameStylePartCenter( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStylePartCenter();
    QPixmap* getPixmap();
public: // overridables of base class CDataDiagram
    virtual void update( CDiagObj* i_pDiagObj, const QRect& i_rect );
    virtual void invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags ) override;
protected: // overridable auxiliary instance methods of base class CDataDiagram
    virtual void updateLayout() override;
protected: // overridable auxiliary instance methods
    virtual void updatePixmap();
private: // copy ctor not allowed
    CPixmapDiagram( const CPixmapDiagram& );
private: // assignment operator not allowed
    void operator=( const CPixmapDiagram& );
protected:  // instance members
    QColor       m_colErase;
    QColor       m_colBgPartCenter;
    QPixmap*     m_pPixmap;
    QPrinter*    m_pPrinter;
    QSize        m_size;
    int          m_cyMinimumHeightPartTop;
    int          m_cyMinimumHeightPartBottom;
    int          m_cxMinimumWidthPartLeft;
    int          m_cxMinimumWidthPartRight;
    int          m_iMarginTop;
    int          m_iMarginBottom;
    int          m_iMarginLeft;
    int          m_iMarginRight;
    QRect        m_rectPartTop;
    QRect        m_rectPartBottom;
    QRect        m_rectPartLeft;
    QRect        m_rectPartRight;
    QRect        m_rectPartCenter;
    SFrameStyle* m_pFrameStyle;
    SFrameStyle* m_pFrameStylePartCenter;
    QRect        m_rectFramePartCenterTopLineInv;
    QRect        m_rectFramePartCenterBottomLineInv;
    QRect        m_rectFramePartCenterLeftLineInv;
    QRect        m_rectFramePartCenterRightLineInv;
private:  // instance members
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CPixmapDiagram

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ProcPixmap_h
