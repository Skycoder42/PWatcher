#include "imagemodel.h"
#include <QDateTime>
#include <QDirIterator>
#include <QDir>
#include <QRegularExpression>
#include <QImageReader>
#include <QMimeDatabase>
#include <QSet>

ImageModel::ImageModel(QObject *parent) :
	QAbstractListModel(parent),
	_baseData(),
	_baseIndex(0),
	_mode(Linear),
	_loop(false),
	_modelData()
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
	std::srand(QDateTime::currentMSecsSinceEpoch());
}

int ImageModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return _modelData.size();
}

QVariant ImageModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if(role == ImageUrlRole)
		return _modelData[index.row()];
	else
		return QVariant();
}

void ImageModel::fetchMore(const QModelIndex &parent)
{
	if(!parent.isValid()) {
		QUrl element;

		switch (_mode) {
		case Linear:
		case Shuffle:
			element = _baseData[_baseIndex++];
			if(_loop && _baseIndex == _baseData.size())
				_baseIndex = 0;
			break;
		case Random:
		{
			int rndIndex;
			do {
				rndIndex = (qrand()/(double)RAND_MAX)*_baseData.size();
			} while(rndIndex == _baseData.size());
			element = _baseData[rndIndex];
			break;
		}
		}

		beginInsertRows(parent, _modelData.size(), _modelData.size());
		_modelData.append(element);
		endInsertRows();
	}
}

bool ImageModel::canFetchMore(const QModelIndex &parent) const
{
	if (parent.isValid())
		return false;
	else {
		if(_baseData.isEmpty())
			return false;

		switch (_mode) {
		case Linear:
		case Shuffle:
			if(_loop)
				return true;
			else
				return _baseIndex < _baseData.size();
		case Random:
			return true;
		}
	}
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
	return {
		{ImageUrlRole, "imageUrl"}
	};
}

void ImageModel::setupModel(const QUrl &path, bool recursive, const QString &filters, PlayMode playMode, bool loop)
{
	_mode = playMode;
	_loop = loop;

	QDir dir(path.toLocalFile());
	dir.setFilter(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::Name | QDir::LocaleAware | QDir::DirsLast);

	QSet<QString> types;
	if(!filters.isEmpty()) {
		QRegularExpression regex(QStringLiteral(R"__(\s+)__"));
		types = QSet<QString>::fromList(filters.split(regex, QString::SkipEmptyParts));
	} else {
		auto mimes = QImageReader::supportedMimeTypes();
		QMimeDatabase db;
		foreach(auto mime, mimes) {
			auto md = db.mimeTypeForName(mime);
			if(md.isValid())
				types.unite(QSet<QString>::fromList(md.suffixes()));
		}
		foreach(auto format, QImageReader::supportedImageFormats())
			types.insert(QString::fromUtf8(format));
	}
	QStringList nameFilters;
	foreach(auto type, types)
		nameFilters.append(QStringLiteral("*.") + type);
	dir.setNameFilters(nameFilters);

	_baseData.clear();
	_baseIndex = 0;
	QDirIterator iterator(dir, (recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags) | QDirIterator::FollowSymlinks);
	while (iterator.hasNext()){
		iterator.next();
		_baseData.append(QUrl::fromLocalFile(iterator.filePath()));
	}

	if(_mode == Shuffle)
		std::random_shuffle(_baseData.begin(), _baseData.end());

	beginResetModel();
	_modelData.clear();
	endResetModel();
}
