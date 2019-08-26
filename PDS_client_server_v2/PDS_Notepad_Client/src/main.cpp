#include "../include/Notepad.h"
#include "../include/StartDialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication EditorApp(argc, argv);
    StartDialog startDialog;
    startDialog.show();

    return EditorApp.exec();
}
