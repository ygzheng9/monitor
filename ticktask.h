#ifndef TICKTASK_H
#define TICKTASK_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class TickTask : public QObject {
public:
  TickTask();

private:
  QTimer *timer;
  QNetworkAccessManager accessManager;

private:
  // 每 1 分钟
  const int PERIOD = 1000 * 60 * 1;
  const QString serviceURL = "http://localhost:8088/api/tick";

private slots:
  void onTimerOut();
};

#endif // TICKTASK_H
