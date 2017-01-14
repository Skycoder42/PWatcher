#ifndef FORMATSDIALOG_H
#define FORMATSDIALOG_H

#include <QDialog>
#include "controldialog.h"
class QAbstractButton;

namespace Ui {
class FormatsDialog;
}

class FormatsDialog : public QDialog
{
	Q_OBJECT

public:
	static QList<ControlDialog::ImageFormat> defaultFormats();
	static QList<ControlDialog::ImageFormat> editFormats(const QList<ControlDialog::ImageFormat> &formats = FormatsDialog::defaultFormats(),
														 QWidget *parent = Q_NULLPTR);

private slots:
	void on_actionAdd_Format_triggered();
	void on_actionDelete_Format_triggered();
	void on_buttonBox_clicked(QAbstractButton *button);

private:
	Ui::FormatsDialog *ui;

	explicit FormatsDialog(QWidget *parent = Q_NULLPTR);
	~FormatsDialog();
};

#endif // FORMATSDIALOG_H
