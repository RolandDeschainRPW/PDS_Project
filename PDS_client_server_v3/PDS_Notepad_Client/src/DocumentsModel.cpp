//
// Created by raffa on 30/08/2019.
//

#include "../include/DocumentsModel.h"

DocumentsModel::DocumentsModel(QObject *parent) : QAbstractTableModel(parent) { /* empty */ }

// Create a method to populate the model with data:
void DocumentsModel::populateData(const QList<QString> &contactName,const QList<QString> &contactPhone)
{
    documents_filenames.clear();
    documents_filenames = contactName;
    documents_owners.clear();
    documents_owners = contactPhone;
    return;
}

int DocumentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return documents_filenames.length();
}

int DocumentsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant DocumentsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return documents_filenames[index.row()];
    } else if (index.column() == 1) {
        return documents_owners[index.row()];
    }
    return QVariant();
}

QVariant DocumentsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) {
            return QString("Document");
        } else if (section == 1) {
            return QString("Owner");
        }
    }
    return QVariant();
}