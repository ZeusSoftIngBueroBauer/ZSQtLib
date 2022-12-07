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

#include "ZSQuickControls/ZSQuickControlsThemeWindowsStyle.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysException.h"

#include <QtQml/qqmlapplicationengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQuickControls2/qquickstyle.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::QuickControls;


/*******************************************************************************
class CThemeWindowsStyle : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CThemeWindowsStyle* CThemeWindowsStyle::s_pTheInst = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the address of the theme.

    This method does neither create an instance of the class nor increments
    the reference counter.
    If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure
    that the instance is already existing this method should be preferred to the
    createInstance call as this method does not affect the reference counter and
    there is no need to call releaseInstance later on.

    @note After a getInstance call a releaseInstance MUST NOT be called.

    @return Pointer to trace server or nullptr, if the instance has not been created yet.
*/
CThemeWindowsStyle* CThemeWindowsStyle::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Creates the theme if not already existing.

    If the instance is already existing the reference to the existing
    instance is returned and a reference counter is incremented.

    @return Pointer to theme instance.
*/
CThemeWindowsStyle* CThemeWindowsStyle::CreateInstance(QQmlApplicationEngine* i_pQmlAppEngine)
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr )
    {
        // The ctor sets s_pTheInst to this.
        // If the ctor itself calls methods of other classes which again recursively
        // call "GetInstance" this way "GetInstance" does not return null but the
        // pointer to the server instance currently beeing created.
        // But of course this requires special caution as within the ctor it must
        // be assured that recursively accessed instance members are already valid.
        new CThemeWindowsStyle(i_pQmlAppEngine);
    }
    s_pTheInst->incrementRefCount();
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Releases the theme instance.

    Before invoking this method a reference to the instance must have been retrieved
    with a createInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @note A reference returned by getInstance MUST NOT be freed.
*/
void CThemeWindowsStyle::ReleaseInstance()
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, "ZS::QuickControls::CThemeWindowsStyle");
    }

    int iRefCount = s_pTheInst->decrementRefCount();

    if( iRefCount == 0 )
    {
        try
        {
            delete s_pTheInst;
        }
        catch(...)
        {
        }
        s_pTheInst = nullptr;
    }
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the theme.

    The constructor is protected. The singleton class must be created via the
    static method createInstance.

*/
CThemeWindowsStyle::CThemeWindowsStyle(QQmlApplicationEngine* i_pQmlAppEngine) :
//------------------------------------------------------------------------------
    QObject(),
    m_pQmlAppEngine(i_pQmlAppEngine),
    m_clrMain("#17a81a"),
    m_iRefCount(0),
    m_pIdxTree(nullptr),
    m_pModelIdxTree(nullptr)
{
    setObjectName("theInst");

    // See comment in "CreateInstance" above.
    s_pTheInst = this;

    m_pIdxTree = new CIdxTree(StyleName());
    m_pModelIdxTree = new CModelIdxTree(m_pIdxTree);

    QQuickStyle::setStyle(StyleName());

    QQmlContext* pQmlCtx = m_pQmlAppEngine->rootContext();

    pQmlCtx->setContextProperty(ContextPropertyName(), this);
    pQmlCtx->setContextProperty(ContextPropertyNameControlsModel(), m_pModelIdxTree);

    setMainColor(m_clrMain);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the theme.

    The destructor is protected. The singleton class must be destroyed via the
    static method releaseInstance.
*/
CThemeWindowsStyle::~CThemeWindowsStyle()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pModelIdxTree;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pIdxTree;
    }
    catch(...)
    {
    }

    m_pQmlAppEngine = nullptr;
    //m_clrMain;
    m_iRefCount = 0;
    m_pIdxTree = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QColor CThemeWindowsStyle::mainColor() const
//------------------------------------------------------------------------------
{
    return m_clrMain;
}

//------------------------------------------------------------------------------
void CThemeWindowsStyle::setMainColor( const QColor& i_color )
//------------------------------------------------------------------------------
{
    if( m_clrMain != i_color )
    {
        m_clrMain = i_color;
        emit mainColorChanged(m_clrMain);
    }
}

/*==============================================================================
protected: // instance methods (reference counter)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of active references to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references.
*/
//------------------------------------------------------------------------------
int CThemeWindowsStyle::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CThemeWindowsStyle::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references after decrement.
*/
//------------------------------------------------------------------------------
int CThemeWindowsStyle::decrementRefCount()
{
    if( m_iRefCount <= 0)
    {
        throw CException(__FILE__, __LINE__, EResultObjRefCounterIsZero, "CThemeWindowsStyle::" + objectName());
    }
    return --m_iRefCount;
}
