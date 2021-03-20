/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
#include <QtGui/qicon.h>
#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSSys/ZSSysErrResult.h"

class QGraphicsScene;
class QGraphicsItem;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CObjPool;
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

//******************************************************************************
class ZSDRAWDLL_API CObjFactory : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class members
    static const QString c_strGroupSeparater;
public: // class methods
    static int GetObjFactoriesCount();
    static CObjFactory* GetObjFactory( int i_idx );
    static CObjFactory* FindObjFactory( const QString& i_strGraphObjPath );
    static CObjFactory* FindObjFactory(
        const QString& i_strGraphObjNameSpace,
        const QString& i_strGraphObjClassName,
        int            i_iGraphObjType );
    static CObjFactory* FindObjFactory(
        const QString& i_strGraphObjNameSpace,
        const QString& i_strGraphObjClassName,
        const QString& i_strGraphObjType );
public: // class methods
    static void ResetAllCtorsDtorsCounters();
public: // class methods
    static ZS::System::CObjPool* ObjPoolAllFactories() { return s_pObjPoolAllFactories; }
    static ZS::System::CObjPool* ObjPoolToolBoxFactories() { return s_pObjPoolToolBoxFactories; }
protected: // class methods
    static void RegisterObjFactory( CObjFactory& i_objFactory, bool i_bAddToToolBoxObjPool = false );
    static void UnregisterObjFactory( CObjFactory& i_objFactory );
protected: // ctor
    CObjFactory(
        const QString& i_strGroupName,          // use "c_strGroupSeparator" ("::") to separate groups
        const QString& i_strGraphObjNameSpace,
        const QString& i_strGraphObjClassName,
        int            i_iGraphObjType,
        const QString& i_strGraphObjType,
        bool           i_bAddToToolBoxObjPool = false,
        const QIcon&   i_toolIcon = QIcon() );
public: // dtor
    virtual ~CObjFactory();
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
    virtual void ResetCtorsDtorsCounters() = 0;
public: // instance methods
    QString getGraphObjNameSpace() const { return m_strGraphObjNameSpace; }
    QString getGraphObjClassName() const { return m_strGraphObjClassName; }
    QString getGroupName() const { return m_strGroupName; }
    int getGraphObjType() const { return m_iGraphObjType; }
    QString getGraphObjTypeAsString() const { return m_strGraphObjType; }
public: // instance methods
    void setToolIcon( const QIcon& i_toolIcon );
    QIcon getToolIcon() const { return m_toolIcon; }
public: // instance methods
    void setObjPoolIdAllFactories( int i_iId ) { m_iObjPoolIdAllFactories = i_iId; }
    int getObjPoolIdAllFactories() const { return m_iObjPoolIdAllFactories; }
    void setObjPoolIdToolBoxFactories( int i_iId ) { m_iObjPoolIdToolBoxFactories = i_iId; }
    int getObjPoolIdToolBoxFactories() const { return m_iObjPoolIdToolBoxFactories; }
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
    static int                      s_iInstCount;
    static ZS::System::CObjPool*    s_pObjPoolAllFactories;
    static ZS::System::CObjPool*    s_pObjPoolToolBoxFactories;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObj;
protected: // instance members
    QString                  m_strGroupName;            // to group the factory in tree views
    QString                  m_strGraphObjNameSpace;
    QString                  m_strGraphObjClassName;
    int                      m_iGraphObjType;
    QString                  m_strGraphObjType;
    QIcon                    m_toolIcon;
    int                      m_iObjPoolIdAllFactories;
    int                      m_iObjPoolIdToolBoxFactories;
    QString                  m_strFileDir;  // The file's path (which may be absolute or relative). This doesn't include the file name.
    QString                  m_strFileName; // Name of the file, excluding the path.
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CObjFactory

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObj_h
