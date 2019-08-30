//
// Created by raffa on 30/08/2019.
//

#ifndef NOTEPAD_DOCUMENTSMODEL_H
#define NOTEPAD_DOCUMENTSMODEL_H

#include <QAbstractTableModel>

class DocumentsModel : public QAbstractTableModel {
    Q_OBJECT

public:
    DocumentsModel(QObject* parent = 0);

    void populateData(const QList<QString> &documentFilename, const QList<QString> &documentOwner);
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<QString> documents_filenames;
    QList<QString> documents_owners;
};


#endif //NOTEPAD_DOCUMENTSMODEL_H
