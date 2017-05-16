#include "controldialog.h"
#include "ui_controldialog.h"
#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QSettings>
#include <QStandardPaths>
#include <QWindow>
#include <algorithm>

ControlDialog::ControlDialog(QWidget *parent) :
	QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::MSWindowsFixedSizeDialogHint),
	ui(new Ui::ControlDialog),
	display(new DisplayWindow(this))
{
	ui->setupUi(this);
	ui->imageFolderPathEdit->setDefaultDirectory(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
	setTabOrder(ui->imageFolderPathEdit, ui->linearModeButton);

	QSettings settings;
	settings.beginGroup(QStringLiteral("viewSettings"));
	settings.beginGroup(QStringLiteral("mode"));
	ui->linearModeButton->setChecked(settings.value(QStringLiteral("normal"), false).toBool());
	ui->shuffleModeButton->setChecked(settings.value(QStringLiteral("shuffle"), true).toBool());
	ui->randomModeButton->setChecked(settings.value(QStringLiteral("random"), false).toBool());
	settings.endGroup();
	ui->loopCheckBox->setChecked(settings.value(QStringLiteral("loop"), true).toBool());
	ui->displayTimeDoubleSpinBox->setValue(settings.value(QStringLiteral("delay"), 5.0).toDouble());
	ui->movieMinimumLoopCountSpinBox->setValue(settings.value(QStringLiteral("movieLoops"), 1).toInt());
	ui->filterFiletypesCheckBox->setChecked(settings.value(QStringLiteral("filters"), true).toBool());
	ui->blackOutScreensCheckBox->setChecked(settings.value(QStringLiteral("blackOut"), true).toBool());
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
		ui->imageFolderPathEdit->setPath(path);
	else
		ui->imageFolderPathEdit->setPath(fileInfo.path());
}

void ControlDialog::accept()
{
	QSettings settings;
	settings.beginGroup(QStringLiteral("viewSettings"));
	settings.beginGroup(QStringLiteral("mode"));
	settings.setValue(QStringLiteral("normal"), ui->linearModeButton->isChecked());
	settings.setValue(QStringLiteral("shuffle"), ui->shuffleModeButton->isChecked());
	settings.setValue(QStringLiteral("random"), ui->randomModeButton->isChecked());
	settings.endGroup();
	settings.setValue(QStringLiteral("loop"), ui->loopCheckBox->isChecked());
	settings.setValue(QStringLiteral("delay"), ui->displayTimeDoubleSpinBox->value());
	settings.setValue(QStringLiteral("movieLoops"), ui->movieMinimumLoopCountSpinBox->value());
	settings.setValue(QStringLiteral("filters"), ui->filterFiletypesCheckBox->isChecked());
	settings.setValue(QStringLiteral("blackOut"), ui->blackOutScreensCheckBox->isChecked());
	settings.endGroup();

	DisplayWindow::ViewInfo info;

	QFileInfo fileInfo(ui->imageFolderPathEdit->path());
	QDir searchDir;
	if(fileInfo.isDir())
		searchDir = QDir(fileInfo.absoluteFilePath());
	else
		searchDir = fileInfo.dir();
	searchDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	QStringList editFilters = ui->liMitToFiletypeLineEdit->text()
							  .split(QRegularExpression(QStringLiteral(R"__(\s+)__")), QString::SkipEmptyParts);
	if(!editFilters.isEmpty()) {
		for(int i = 0, max = editFilters.size(); i < max; ++i)
			editFilters[i] = QStringLiteral("*.") + editFilters[i];
		searchDir.setNameFilters(editFilters);
	} else if(ui->filterFiletypesCheckBox->isChecked()) {
		QStringList filters;
		foreach(auto filter, QImageReader::supportedImageFormats())
			filters.append(QStringLiteral("*.") + filter);
		searchDir.setNameFilters(filters);
	}
	searchDir.setSorting(QDir::Name | QDir::LocaleAware);
	info.files = searchDir.entryInfoList();
	if(ui->shuffleModeButton->isChecked())
		std::random_shuffle(info.files.begin(), info.files.end());

	info.random = ui->randomModeButton->isChecked();
	info.repeat = ui->loopCheckBox->isChecked();
	info.waitDelay = ui->displayTimeDoubleSpinBox->value() * 1000;
	info.minMovieLoop = ui->movieMinimumLoopCountSpinBox->value();

	display->setInfo(info);
	display->startShow(ui->blackOutScreensCheckBox->isChecked());
	QDialog::accept();
}
