#include "formatsdialog.h"
#include "ui_formatsdialog.h"
#include <QImageReader>
#include <QMovie>
#include <QHash>

FormatsDialog::FormatsDialog(QWidget *parent) :
	QDialog(parent, Qt::WindowCloseButtonHint),
	ui(new Ui::FormatsDialog)
{
	ui->setupUi(this);

	this->ui->addButton->setDefaultAction(this->ui->actionAdd_Format);
	this->ui->removeButton->setDefaultAction(this->ui->actionDelete_Format);
	this->ui->listWidget->addActions({
										 this->ui->actionAdd_Format,
										 this->ui->actionDelete_Format
									 });
}

FormatsDialog::~FormatsDialog()
{
	delete ui;
}

QList<ControlDialog::ImageFormat> FormatsDialog::editFormats(const QList<ControlDialog::ImageFormat> &formats, QWidget *parent)
{
	FormatsDialog dialog(parent);
	for(ControlDialog::ImageFormat format : formats) {
		QListWidgetItem *item = new QListWidgetItem(dialog.ui->listWidget);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
		item->setText(format.first);
		item->setCheckState(format.second ? Qt::Checked : Qt::Unchecked);
	}
	int res = dialog.exec();
	if(res == QDialog::Accepted) {
		QList<ControlDialog::ImageFormat> result;
		for(int i = 0, max = dialog.ui->listWidget->count(); i < max; ++i) {
			QListWidgetItem *item = dialog.ui->listWidget->item(i);
			result.append({item->text(), item->checkState() == Qt::Checked});
		}
		return result;
	} else if(res == QDialog::Rejected)
		return formats;
	else
		return FormatsDialog::defaultFormats();
}

QList<ControlDialog::ImageFormat> FormatsDialog::defaultFormats()
{
	QHash<QString, bool> formats;
	for(QByteArray format : QImageReader::supportedImageFormats())
		formats.insert(QString::fromLocal8Bit(format), false);
	for(QByteArray format : QMovie::supportedFormats())
		formats.insert(QString::fromLocal8Bit(format), true);
	QList<ControlDialog::ImageFormat> resList;
	for(QHash<QString, bool>::const_iterator it = formats.begin(), end = formats.end(); it != end; ++it)
		resList += {it.key(), it.value()};
	return resList;
}

void FormatsDialog::on_actionAdd_Format_triggered()
{
	QListWidgetItem *item = new QListWidgetItem(this->ui->listWidget);
	item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
	item->setCheckState(Qt::Unchecked);
	this->ui->listWidget->setCurrentItem(item);
	this->ui->listWidget->editItem(item);
}

void FormatsDialog::on_actionDelete_Format_triggered()
{
	delete this->ui->listWidget->currentItem();
}

void FormatsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
	switch(this->ui->buttonBox->standardButton(button)) {
	case QDialogButtonBox::Save:
		this->accept();
		break;
	case QDialogButtonBox::Discard:
		this->reject();
		break;
	case QDialogButtonBox::RestoreDefaults:
		this->done(QDialog::Accepted + 1);
		break;
	default:
		break;
	}
}
