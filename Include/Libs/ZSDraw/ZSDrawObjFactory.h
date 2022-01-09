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

#ifndef ZSDraw_ObjFactory_h
#define ZSDraw_ObjFactory_h

#include <QtCore/qobject.h>
#include <QtCore/qsize.h>
#include <QtGui/qpixmap.h>
#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"
#include "ZSSys/ZSSysErrResult.h"

class QGraphicsScene;
class QGraphicsItem;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CIdxTree;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CDrawSettings;
class CGraphObj;
class CGraphObjGroup;
struct SGraphObjLabel;

//******************************************************************************
class ZSDRAWDLL_API CObjFactory : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CObjFactory"; }  // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class members
    /*! Separator used to name group of factories. */
    static const QString c_strGroupSeparater;   // = "::"
    /*! Group name of the factories to create the standard shapes (Line, Point, Rect, etc).
        The group name together with the type as string of the graphical objects must be unique
        and is used to add the object factory to the index tree of all registered factories. */
    static const QString c_strGroupNameStandardShapes; // = "Standard Shapes"
    /*! Group name of the factories to create connections (ConnectionLine, ConnectionPoint).
        The group name together with the type as string of the graphical objects must be unique
        and is used to add the object factory to the index tree of all registered factories. */
    static const QString c_strGroupNameConnections;    // = "Connections"
    /*! Group name for selection points. */
    static const QString c_strGroupNameSelectionPoints; // = "Selection Points"
public: // class methods
    static int GetObjFactoriesCount();
    static CObjFactory* GetObjFactory( int i_idx );
    static CObjFactory* FindObjFactory( const QString& i_strGraphObjPath );
    //static CObjFactory* FindObjFactory( const QString& i_strGroupName, int i_iGraphObjType );
    static CObjFactory* FindObjFactory( const QString& i_strGroupName, const QString& i_strGraphObjType );
public: // class methods
    static ZS::System::CIdxTree* IdxTree() { return s_pIdxTree; }
protected: // ctor
    CObjFactory(
        const QString& i_strGroupName,
        int            i_iGraphObjType,
        const QString& i_strGraphObjType,
        const QPixmap& i_pxmToolIcon = QPixmap() );
public: // dtor
    virtual ~CObjFactory();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // interface methods
    virtual CGraphObj* createGraphObj(
        CDrawingScene*       i_pDrawingScene,
        const QPointF&       i_ptItemPos, // usually QPointF(0.0,0.0)
        const CDrawSettings& i_drawSettings ) = 0;
    virtual ZS::System::SErrResultInfo saveGraphObj(
        CGraphObj*        i_pGraphObj,
        QXmlStreamWriter& i_xmlStreamWriter ) = 0;
    virtual CGraphObj* loadGraphObj(
        CDrawingScene*    i_pDrawingScene,
        CGraphObjGroup*   i_pGraphObjGroup,
        const QString&    i_strObjName,
        const QString&    i_strObjId,
        QXmlStreamReader& i_xmlStreamReader,
        ZS::System::SErrResultInfo&   io_errResultInfo ) = 0;
public: // instance methods
    QString getGroupName() const { return m_strGroupName; }
    int getGraphObjType() const { return m_iGraphObjType; }
    QString getGraphObjTypeAsString() const { return m_strName; }
public: // instance methods
    void setToolIconPixmap( const QPixmap& i_pxm );
    QPixmap getToolIconPixmap() const { return m_pxmToolIcon; }
public: // instance methods
    void setFileDir( const QString& i_strFileDir );   // The file's path (which may be absolute or relative). This doesn't include the file name.
    QString getFileDir() const { return m_strFileDir; }
    void setFileName( const QString& i_strFileName ); // Name of the file, excluding the path. If file name includes a path, the path will be extracted and stored as file path.
    QString getFileName() const { return m_strFileName; }
    void setFilePath( const QString& i_strFilePath ); // File name, including the path (which may be absolute or relative).
    QString getFilePath() const;
public: // overridables
    virtual bool hasInitialSize() const { return false; }
    virtual QSizeF getInitialSize() const { return QSizeF(); }
protected: // overridables
    virtual ZS::System::SErrResultInfo saveGraphObjLabels( const QHash<QString,SGraphObjLabel*>& i_arpLabels, QXmlStreamWriter& i_xmlStreamWriter );
    virtual QHash<QString,SGraphObjLabel*> loadGraphObjLabels( QXmlStreamReader& i_xmlStreamReader, ZS::System::SErrResultInfo& io_errResultInfo );
protected: // class members
    static int                   s_iInstCount;
    static ZS::System::CIdxTree* s_pIdxTree;
protected: // instance members
    QString                  m_strGroupName;            /*!< To group the factory in index tree (e.g. "Draw::Standard Shapes", "Draw::Widgets", "Draw::Electricity") */
    int                      m_iGraphObjType;           /*!< Type as int of the graphic items created by this factory. */
    QPixmap                  m_pxmToolIcon;             /*!< Pixmap for icon to indicate the graphic items created by this factory in the index tree. */
    QString                  m_strFileDir;              /*!< The file's path (which may be absolute or relative). This doesn't include the file name. */
    QString                  m_strFileName;             /*!< Name of the file, excluding the path. */
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;            /*!< Trace admin object. */

}; // class CObjFactory

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObj_h
