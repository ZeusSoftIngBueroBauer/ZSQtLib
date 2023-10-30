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

#ifndef ZSDraw_GraphObjLabelsModel_h
#define ZSDraw_GraphObjLabelsModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSDraw/Common/ZSDrawCommon.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CModelGraphObjLabels : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelGraphObjLabels"; }
public: // type definitions and constants
    enum EColumn {
        EColumnName              = 0,
        EColumnText              = 1,
        EColumnVisible           = 2,
        EColumnAnchor            = 3,
        EColumnAnchorLineVisible = 4,
        EColumnCount
    };
    static QString column2Str(int i_clm);
public: // ctors and dtor
    CModelGraphObjLabels(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QObject* i_pObjParent = nullptr);
    virtual ~CModelGraphObjLabels();
signals:
    /*! This signal is emitted if the indicated content has been changed. */
    void contentChanged();
public: // instance methods
    void setGraphObj(CGraphObj* i_pGraphObj);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const;
    bool hasChanges() const;
    void acceptChanges();
    void rejectChanges();
    void applySettings();
public: // overridables of base class QAbstractItemModel
    int rowCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole) override;
    QVariant headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& i_modelIdx) const override;
protected: // type definitions and constants
    struct SLabelSettings {
    public: // ctors
        SLabelSettings() :
            m_strName(), m_iRowIdx(-1), m_bIsPredefinedLabelName(false),
            m_strText(), m_selPt(ESelectionPoint::None), m_bVisible(false), m_bAnchorLineVisible(false)
        {}
        SLabelSettings(const QString& i_strName, int i_iRowIdx, bool i_bIsPredefinedLabelName) :
            m_strName(i_strName), m_iRowIdx(i_iRowIdx), m_bIsPredefinedLabelName(i_bIsPredefinedLabelName),
            m_strText(), m_selPt(ESelectionPoint::None), m_bVisible(false), m_bAnchorLineVisible(false)
        {}
        SLabelSettings(
            const QString& i_strName, int i_iRowIdx, bool i_bIsPredefinedLabelName,
            const QString& i_strText, ESelectionPoint i_selPt, bool i_bVisible, bool i_bAnchorLineVisible) :
            m_strName(i_strName), m_iRowIdx(i_iRowIdx), m_bIsPredefinedLabelName(i_bIsPredefinedLabelName),
            m_strText(i_strText), m_selPt(i_selPt), m_bVisible(i_bVisible), m_bAnchorLineVisible(i_bAnchorLineVisible)
        {}
    public: // operators
        bool operator == (const SLabelSettings& i_other) const {
            bool bEqual = true;
            if (m_strName != i_other.m_strName) {
                bEqual = false;
            }
            else if (m_iRowIdx != i_other.m_iRowIdx) {
                bEqual = false;
            }
            else if (m_bIsPredefinedLabelName != i_other.m_bIsPredefinedLabelName) {
                bEqual = false;
            }
            else if (m_strText != i_other.m_strText) {
                bEqual = false;
            }
            else if (m_selPt != i_other.m_selPt) {
                bEqual = false;
            }
            else if (m_bVisible != i_other.m_bVisible) {
                bEqual = false;
            }
            else if (m_bAnchorLineVisible != i_other.m_bAnchorLineVisible) {
                bEqual = false;
            }
            return bEqual;
        }
        bool operator != (const SLabelSettings& i_other) const {
            return !(*this == i_other);
        }
    public: // struct members
        QString m_strName;
        int m_iRowIdx;
        bool m_bIsPredefinedLabelName;
        QString m_strText;
        ESelectionPoint m_selPt;
        bool m_bVisible;
        bool m_bAnchorLineVisible;
    };
protected: // instance methods
    void clearModel();
    void fillModel();
    QList<SLabelSettings> getLabelSettings(CGraphObj* i_pGraphObj) const;
protected slots:
    void onGraphObjLabelAdded(const QString& i_strName);
    void onGraphObjLabelRemoved(const QString& i_strName);
    void onGraphObjLabelChanged(const QString& i_strName);
    void onGraphObjAboutToDestroyed();
protected: // instance methods (tracing emitting signals)
    void emit_contentChanged();
    void _beginInsertRows(const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast);
    void _endInsertRows();
    void _beginRemoveRows(const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast);
    void _endRemoveRows();
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    CGraphObj* m_pGraphObj;
    /*!< Cached draw settings of the graphical object. */
    QList<SLabelSettings> m_arLabelSettings;
    QHash<QString, int> m_hshName2RowIdx;
    /*!< Flag to indicate that the content of a data row has been changed while the "contentChanged"
         signal was blocked by the "contentChanged" counter. */
    bool m_bContentChanged;
    /*!< Counts how ofter the "contentChanged" signal has been blocked. A value greater than 0
         for the counter means that the signal "contentChanged" should not be emitted. Instead
         the flag m_bContentChanged should be set to true.
         If the counter is decremented and reaches 0 the flag "m_bContentChanged" is checked and
         the signal "contentChanged" is emitted if the flag is set. */
    int m_iContentChangedSignalBlockedCounter;
    /*!< Method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;

}; // class CModelGraphObjLabels

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabelsModel_h
