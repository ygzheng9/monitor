#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#include "messagehandler.h"
#include "myservice.h"

MyService::MyService(int argc, char *argv[])
    : QtService<QCoreApplication>(argc, argv, "Dajun") {
  setServiceDescription("This is my Windows Service");
  setServiceFlags(QtServiceBase::CanBeSuspended);
}

void MyService::start() {
  // Only for "-e" option, as message handler has not been installed
  qDebug() << "Get into start().......";

  installMessageHandler();

  tickTask.reset(new TickTask);

  qInfo() << "Start MyWindowsService...";
}

void MyService::stop() { qInfo() << "Stop MyWindowsService..."; }

void MyService::pause() { qInfo() << "Pause MyWindowsService..."; }

void MyService::resume() { qInfo() << "Resume MyWindowsService..."; }

void MyService::installMessageHandler() {
  // Install Message Handler
  QString logDirName = "C:/localWork/QTProjs/ReadyGo";
  QDir logDir(logDirName);

  if (!logDir.exists()) {
    qDebug() << "Creating folder: " << logDirName;
    QDir().mkdir(logDirName);
  }

  QString logFile = logDirName + "/mws.log";

  // Remove the Macro defined in winbase.h
  //#ifdef FormatMessage
  //#undef FormatMessage
  MessageHandler::setLogFile(logFile);
  qInstallMessageHandler(MessageHandler::FormatMessage);
  //#endif
}

// XXX.exe -i
// Windows键+R键，再运行services.msc
// 继续运行：XXX.exe -s  这是表示启动了这个Service.
// 此后，可以用-p、-r、-t、-u选项分别来暂停、继续、停止和卸载这个服务。
// 而-e选项是如普通程序般运行此exe程序，即不以Service来运行，这是为了Debug方便。
