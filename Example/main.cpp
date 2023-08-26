#include <QApplication>
#include "CuteComponent.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    CuteComponent::hello();
    MainWindow window;
    window.show();

    return app.exec();
}
