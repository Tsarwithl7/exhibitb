#include "Cbct_Project.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QIcon icon("images/icon.png");
    app.setWindowIcon(icon);
    Cbct_Project w;
    w.show();
    return app.exec();
}
