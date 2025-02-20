#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList filePathsFrom() const;

public slots:
    void addSource();
    void removeSource();
    void browseDestination();
    void generate();
    void addColMapping();
    void removeColMapping();

private:
    Ui::MainWindow *ui;
    void _connectslots();
};
#endif // MAINWINDOW_H
