#ifndef MYSERVICE_H
#define MYSERVICE_H

#include <QScopedPointer>
#include <QTimer>

#include "qtservice/qtservice.h"
#include "ticktask.h"

class MyService : public QtService<QCoreApplication> {
public:
  MyService(int argc, char *argv[]);

public:
  void start();
  void pause();
  void resume();
  void stop();

private:
  void installMessageHandler();
  QScopedPointer<TickTask> tickTask;
};

#endif // MYSERVICE_H
