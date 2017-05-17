#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractListModel>
#include <QUrl>

class ImageModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum Roles {
		ImageUrlRole = Qt::UserRole + 1
	};
	Q_ENUM(Roles)

	enum PlayMode {
		Linear,
		Shuffle,
		Random
	};
	Q_ENUM(PlayMode)

	explicit ImageModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	void fetchMore(const QModelIndex &parent) override;
	bool canFetchMore(const QModelIndex &parent) const override;
	QHash<int, QByteArray> roleNames() const override;

public slots:
	void setupModel(const QUrl &path,
					bool recursive,
					const QString &filters,
					PlayMode playMode,
					bool loop);

private:
	QList<QUrl> _baseData;
	int _baseIndex;
	PlayMode _mode;
	bool _loop;

	QList<QUrl> _modelData;
};

#endif // IMAGEMODEL_H
