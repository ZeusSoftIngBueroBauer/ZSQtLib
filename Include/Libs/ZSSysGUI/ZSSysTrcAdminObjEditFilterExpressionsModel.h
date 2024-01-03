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

#ifndef ZSSysGUI_TrcAdminObjEditFilterExpressionsModel_h
#define ZSSysGUI_TrcAdminObjEditFilterExpressionsModel_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

#include <QtCore/qabstractitemmodel.h>

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelTrcAdminObjEditFilterExpressions : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System::GUI"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelTrcAdminObjEditFilterExpressions"; }
public: // type definitions and constants
    enum EColumn {
        EColumnSelected             = 0,
        EColumnFilterExpressionType = 1,
        EColumnFilterExpression     = 2,
        EColumnError                = 3,
        EColumnCount
    };
    static QString column2Str(int i_clm);
public: // ctors and dtor
    CModelTrcAdminObjEditFilterExpressions(QObject* i_pObjParent = nullptr);
    virtual ~CModelTrcAdminObjEditFilterExpressions();
signals:
    /*! This signal is emitted if the indicated content has been changed. */
    void contentChanged();
public: // instance methods
    void setTraceAdminObj(CTrcAdminObj* i_pTrcAdminObj);
    void setFilterToEdit(EMethodTraceFilterProperty i_filter);
public: // instance methods
    bool hasErrors() const;
    bool hasChanges() const;
    void acceptChanges();
    void rejectChanges();
public: // instance methods
    void addFilterExpression();
    QList<int> selectedRows() const;
    void removeSelectedRows();
public: // overridables of base class QAbstractItemModel
    int rowCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole) override;
    QVariant headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& i_modelIdx) const override;
protected slots:
    void onTraceAdminObjFilterChanged(const QString& i_strFilter);
protected: // instance methods
    void clearModel();
    void fillModel();
    void disconnectFromTraceAdminObjFilterChangedSignal();
    void connectWithTraceAdminObjFilterChangedSignal();
protected: // type definitions and constants
    struct SFilterExpression {
    public: // ctors
        SFilterExpression();
        SFilterExpression(EMethodTraceFilterExpressionType i_filterType, const QString& i_strFilter);
    public: // operators
        bool operator == (const SFilterExpression& i_other) const;
        bool operator != (const SFilterExpression& i_other) const;
    public: // struct members
        bool m_bSelected;
        EMethodTraceFilterExpressionType m_filterType;
        QString m_strFilter;
        ZS::System::SErrResultInfo m_errResultInfo;
    };
protected: // auxiliary instance methods
    QList<SFilterExpression> getFilterExpressions(CTrcAdminObj* i_pTrcAdminObj) const;
    QString toFilterExpression(QList<SFilterExpression>) const;
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    CEnumMethodTraceFilterProperty m_eFilter;
    QList<SFilterExpression> m_arFilterExpressions;
    /*!< Flag to indicate that the content of a data row has been changed while the "contentChanged"
         signal was blocked by the "contentChanged" counter. */
    bool m_bContentChanged;
    /*!< Counts how ofter the "contentChanged" signal has been blocked. A value greater than 0
         for the counter means that the signal "contentChanged" should not be emitted. Instead
         the flag m_bContentChanged should be set to true.
         If the counter is decremented and reaches 0 the flag "m_bContentChanged" is checked and
         the signal "contentChanged" is emitted if the flag is set. */
    int m_iContentChangedSignalBlockedCounter;

}; // class CModelTrcAdminObjEditFilterExpressions

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjEditFilterExpressionsModel_h
