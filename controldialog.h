#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QDialog>
#include "displaywindow.h"

namespace Ui {
class ControlDialog;
}

class ControlDialog : public QDialog
{
	Q_OBJECT

public:
	typedef QPair<QString, bool> ImageFormat;

	explicit ControlDialog(QWidget *parent = Q_NULLPTR);
	~ControlDialog();

	void setPath(const QString &path);

	void accept() Q_DECL_OVERRIDE;

private slots:
	void on_formatsButton_clicked();

private:
	Ui::ControlDialog *ui;
	DisplayWindow *display;
	QList<ImageFormat> formats;
};

#endif // CONTROLDIALOG_H
