#include <QApplication>
#include "displaymanager.h"
#include "orientationsensor.h"
#include "trayicon.h"
#include "rotateinput.h"
#include <QtDBus/QDBusConnection>
#include <QDebug>
#include <signal.h>

void signalhandler(int sig){
  if(sig==SIGINT){
    DisplayManager displayManager;
    RotateInput rotateInput;
    const Orientation defaultOrient = TopUp;
    qDebug() << "Resetting orientation...";
    displayManager.setOrientation(defaultOrient);
    rotateInput.rotate(defaultOrient);
    qApp->quit();
  }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if(!connection.registerService("net.gulinux.ScreenRotator")) {
      qCritical() << "Unable to register DBUS interface \"net.gulinux.ScreenRotator\". More than one instances running?";
      return 1;
    }

    DisplayManager displayManager;
    TrayIcon tray;
    OrientationSensor sensor;
    RotateInput rotateInput;
    QObject::connect(&sensor, &OrientationSensor::reading, &tray, &TrayIcon::orientationUpdated);
    QObject::connect(&tray, &TrayIcon::emitRotation, &displayManager, &DisplayManager::setOrientation);
    QObject::connect(&tray, &TrayIcon::emitRotation, &rotateInput, &RotateInput::rotate);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        qDebug() <<  "aboutToQuit()";
        signalhandler(SIGINT);
    });
    signal(SIGINT, signalhandler);

    return app.exec();
}
