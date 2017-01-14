#include "controldialog.h"
#include "ui_controldialog.h"
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QWindow>
#include <algorithm>
#include "formatsdialog.h"

ControlDialog::ControlDialog(QWidget *parent) :
	QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::ControlDialog),
	display(new DisplayWindow(this)),
	formats()
{
	ui->setupUi(this);
	this->ui->imageFolderPathEdit->setDefaultDirectory(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
	this->setTabOrder(this->ui->imageFolderPathEdit, this->ui->linearModeButton);

	QSettings settings;
	settings.beginGroup(QStringLiteral("viewSettings"));
	settings.beginGroup(QStringLiteral("mode"));
	this->ui->linearModeButton->setChecked(settings.value(QStringLiteral("normal"), false).toBool());
	this->ui->shuffleModeButton->setChecked(settings.value(QStringLiteral("shuffle"), true).toBool());
	this->ui->randomModeButton->setChecked(settings.value(QStringLiteral("random"), false).toBool());
	settings.endGroup();
	this->ui->loopCheckBox->setChecked(settings.value(QStringLiteral("loop"), true).toBool());
	this->ui->displayTimeDoubleSpinBox->setValue(settings.value(QStringLiteral("delay"), 5.0).toDouble());
	this->ui->movieMinimumLoopCountSpinBox->setValue(settings.value(QStringLiteral("movieLoops"), 1).toInt());
	this->ui->filterFiletypesCheckBox->setChecked(settings.value(QStringLiteral("filters"), true).toBool());
	this->ui->blackOutScreensCheckBox->setChecked(settings.value(QStringLiteral("blackOut"), true).toBool());
	if(settings.childGroups().contains(QStringLiteral("formats"))) {
		int max = settings.beginReadArray(QStringLiteral("formats"));
		for(int i = 0; i < max; ++i) {
			settings.setArrayIndex(i);
			ImageFormat format;
			format.first = settings.value(QStringLiteral("format")).toString();
			format.second = settings.value(QStringLiteral("isMovie"), false).toBool();
			this->formats += format;
		}
		settings.endArray();
	} else
		this->formats = FormatsDialog::defaultFormats();
	settings.endGroup();
}

ControlDialog::~ControlDialog()
{
	delete ui;
}

void ControlDialog::setPath(const QString &path)
{
	QFileInfo fileInfo(path);
	if(fileInfo.isDir())
		this->ui->imageFolderPathEdit->setPath(path);
	else
		this->ui->imageFolderPathEdit->setPath(fileInfo.path());
}

void ControlDialog::accept()
{
	QSettings settings;
	settings.beginGroup(QStringLiteral("viewSettings"));
	settings.beginGroup(QStringLiteral("mode"));
	settings.setValue(QStringLiteral("normal"), this->ui->linearModeButton->isChecked());
	settings.setValue(QStringLiteral("shuffle"), this->ui->shuffleModeButton->isChecked());
	settings.setValue(QStringLiteral("random"), this->ui->randomModeButton->isChecked());
	settings.endGroup();
	settings.setValue(QStringLiteral("loop"), this->ui->loopCheckBox->isChecked());
	settings.setValue(QStringLiteral("delay"), this->ui->displayTimeDoubleSpinBox->value());
	settings.setValue(QStringLiteral("movieLoops"), this->ui->movieMinimumLoopCountSpinBox->value());
	settings.setValue(QStringLiteral("filters"), this->ui->filterFiletypesCheckBox->isChecked());
	settings.setValue(QStringLiteral("blackOut"), this->ui->blackOutScreensCheckBox->isChecked());
	settings.endGroup();

	DisplayWindow::ViewInfo info;

	QStringList filters;
	for(ImageFormat format : this->formats) {
		filters += QStringLiteral("*.") + format.first;
		if(format.second)
			info.movieFormats += format.first;
	}

	QFileInfo fileInfo(this->ui->imageFolderPathEdit->path());
	QDir searchDir;
	if(fileInfo.isDir())
		searchDir = QDir(fileInfo.absoluteFilePath());
	else
		searchDir = fileInfo.dir();
	searchDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	QStringList editFilters = this->ui->liMitToFiletypeLineEdit->text()
							  .split(QRegularExpression(QStringLiteral(R"__(\s+)__")), QString::SkipEmptyParts);
	if(!editFilters.isEmpty()) {
		for(int i = 0, max = editFilters.size(); i < max; ++i)
			editFilters[i] = QStringLiteral("*.") + editFilters[i];
		searchDir.setNameFilters(editFilters);
	} else if(this->ui->filterFiletypesCheckBox->isChecked())
		searchDir.setNameFilters(filters);
	searchDir.setSorting(QDir::Name | QDir::LocaleAware);
	info.files = searchDir.entryInfoList();
	if(this->ui->shuffleModeButton->isChecked())
		std::random_shuffle(info.files.begin(), info.files.end());

	info.random = this->ui->randomModeButton->isChecked();
	info.repeat = this->ui->loopCheckBox->isChecked();
	info.waitDelay = this->ui->displayTimeDoubleSpinBox->value() * 1000;
	info.minMovieLoop = this->ui->movieMinimumLoopCountSpinBox->value();

	this->display->setInfo(info);
	this->display->startShow(this->ui->blackOutScreensCheckBox->isChecked());
	this->QDialog::accept();
}

void ControlDialog::on_formatsButton_clicked()
{
	this->formats = FormatsDialog::editFormats(this->formats, this);
	QSettings settings;
	settings.beginWriteArray(QStringLiteral("viewSettings/formats"), this->formats.size());
	for(int i = 0, max = this->formats.size(); i < max; ++i) {
		settings.setArrayIndex(i);
		settings.setValue(QStringLiteral("format"), this->formats[i].first);
		settings.setValue(QStringLiteral("isMovie"), this->formats[i].second);
	}
	settings.endArray();
}
