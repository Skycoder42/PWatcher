#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractListModel>

class ImageModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit ImageModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	void fetchMore(const QModelIndex &parent) override;
	bool canFetchMore(const QModelIndex &parent) const override;
	QHash<int, QByteArray> roleNames() const override;
};

#endif // IMAGEMODEL_H
