#include "imagemodel.h"

ImageModel::ImageModel(QObject *parent) :
	QAbstractListModel(parent)
{}

int ImageModel::rowCount(const QModelIndex &parent) const
{
	// For list models only the root node (an invalid parent) should return the list's size. For all
	// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
	if (parent.isValid())
		return 0;

	// FIXME: Implement me!
	return 0;
}

QVariant ImageModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// FIXME: Implement me!
	return QVariant();
}

void ImageModel::fetchMore(const QModelIndex &parent)
{

}

bool ImageModel::canFetchMore(const QModelIndex &parent) const
{
	return false;
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
	return {
		{ImageUrlRole, "imageUrl"}
	};
}

void ImageModel::setupModel(const QString &path, bool recursive, const QString &filters, int playMode, bool loop)
{
}
