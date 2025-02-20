#include "../common/workingdirectory/WorkingDirectoryManager.h"
#include "../common/workingdirectory/DialogOpenConfig.h"
#include "../common/types/types.h"

#include "gui/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QHash<QString, QString>>();
    QApplication a(argc, argv);
    WorkingDirectoryManager::instance()->installDarkOrangePalette();
    DialogOpenConfig dialog;
    dialog.exec();
    if (dialog.wasRejected())
    {
        return 0;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
