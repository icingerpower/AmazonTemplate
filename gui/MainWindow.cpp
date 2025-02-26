#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>

#include "model/ColMapping.h"
#include "model/TemplateMerger.h"

#include "gui/DialogAddMapping.h"

#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewMapping->setModel(ColMapping::instance());
    _connectslots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::filePathsFrom() const
{
    QStringList filePaths;
    int nRows = ui->listWidgetFrom->count();
    for (int i=0; i<nRows; ++i)
    {
        filePaths << ui->listWidgetFrom->item(i)->text();
    }
    return filePaths;
}

void MainWindow::_connectslots()
{
    connect(ui->buttonAddSourceFrom,
            &QPushButton::clicked,
            this,
            &MainWindow::addSource);
    connect(ui->buttonRemoveFrom,
            &QPushButton::clicked,
            this,
            &MainWindow::removeSource);
    connect(ui->buttonBrowseTo,
            &QPushButton::clicked,
            this,
            &MainWindow::browseDestination);
    connect(ui->buttonGenerate,
            &QPushButton::clicked,
            this,
            &MainWindow::generate);
    connect(ui->buttonAddMapping,
            &QPushButton::clicked,
            this,
            &MainWindow::addColMapping);
    connect(ui->buttonRemoveMapping,
            &QPushButton::clicked,
            this,
            &MainWindow::removeColMapping);
}

void MainWindow::addSource()
{
    QSettings settings;
    const QString key{"addSource"};
    QDir lastDir{settings.value(key, QDir{}.path()).toString()};
    const QString &filePath = QFileDialog::getOpenFileName(
        this,
        tr("Template file"),
        lastDir.path(),
        QString{"Xlsx (*.xlsx *.XLSX *.xlsm *.XLSM)"},
        nullptr,
        QFileDialog::DontUseNativeDialog);
    if (!filePath.isEmpty())
    {
        settings.setValue(key, QFileInfo{filePath}.dir().path());
        ui->listWidgetFrom->addItem(filePath);
    }
}

void MainWindow::removeSource()
{
    auto selItems = ui->listWidgetFrom->selectedItems();
    while(selItems.size() > 0)
    {
        auto lastItem = selItems.takeLast();
        int row = ui->listWidgetFrom->row(lastItem);
        delete ui->listWidgetFrom->takeItem(row);
    }
}

void MainWindow::browseDestination()
{
    QSettings settings;
    const QString key{"addDestination"};
    QDir lastDir{settings.value(key, QDir{}.path()).toString()};
    const QString &filePath = QFileDialog::getOpenFileName(
        this,
        tr("Template file"),
        lastDir.path(),
        QString{"Xlsx (*.xlsm *.XLSM *.xlsx *.XLSX)"},
        nullptr,
        QFileDialog::DontUseNativeDialog);
    if (!filePath.isEmpty())
    {
        settings.setValue(key, QFileInfo{filePath}.dir().path());
        ui->lineEditTo->setText(filePath);
    }
}

void MainWindow::generate()
{
    const auto &fromFilePaths = filePathsFrom();
    //TOTO create a class that will help
    TemplateMerger templateMerger{
        fromFilePaths,
        ui->lineEditTo->text()
    };
    QFileInfo fileInfoTo(ui->lineEditTo->text());
    const auto &fileNameTo = fileInfoTo.baseName() + "-FILLED.xlsx";
    const auto &filePathTo = QDir{fileInfoTo.path()}.absoluteFilePath(fileNameTo);
    templateMerger.exportTo(filePathTo);
}

void MainWindow::addColMapping()
{
    DialogAddMapping dialog;
    dialog.exec();
    if (dialog.result() == QDialog::Accepted)
    {
        ColMapping::instance()->addMapping(
            dialog.getFrom(),
            dialog.getTo());
    }
}

void MainWindow::removeColMapping()
{
    const auto &selIndexes = ui->tableViewMapping->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        ColMapping::instance()->removeMapping(selIndexes.first());
    }
    else
    {
        QMessageBox::information(
            this,
            tr("No selection"),
            tr("You need to select a mapping to remove"));
    }
}

