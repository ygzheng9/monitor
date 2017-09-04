#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QTime>

#include "sysinfo.h"
#include "ticktask.h"

TickTask::TickTask() {
  // 创建定时器，设定时间间隔，及回调函数
  timer = new QTimer();
  timer->setInterval(PERIOD);

  connect(timer, &QTimer::timeout, this, &TickTask::onTimerOut);

  // 启动定时器
  timer->start();
  //  qInfo() << "start timer.....";
}

void TickTask::onTimerOut() {
  //  QString cpu = QTime::currentTime().toString("YYYY-MM-DD HH:mm:ss");
  //  qInfo() << "tick..." << cpu;

  // 获取 当前用户，mac，cpu 及 memory 使用情况
  double cpuLoadAverage = SysInfo::instance().cpuLoadAverage();

  double memoryTotal;
  double memoryPercent;
  SysInfo::instance().getMemoryInfo(memoryTotal, memoryPercent);

  QString mac = SysInfo::instance().getMacAddress();

  SYSTEM_INFO sysInfo = SysInfo::instance().getSystemInfo();
  QString cpu_number = QString("%1").arg(sysInfo.dwNumberOfProcessors);
  QString cpu_type = QString("%1").arg(sysInfo.dwProcessorType);
  QString cpu_level = QString("%1").arg(sysInfo.wProcessorLevel);
  QString cpu_revision = QString("%1").arg(sysInfo.wProcessorRevision);

  QString app_title = SysInfo::instance().getTopAppName();

  // 拼接 josn
  QJsonObject json;
  json.insert("mac", mac);

  json.insert("cpu_load", QString::number(cpuLoadAverage, 10, 2));

  json.insert("cpu_number", cpu_number);
  json.insert("cpu_type", cpu_type);
  json.insert("cpu_level", cpu_level);
  json.insert("cpu_revision", cpu_revision);

  json.insert("memory_total", QString::number(memoryTotal, 10, 2));
  json.insert("memory_used", QString::number(memoryPercent, 10, 2));

  json.insert("app_title", app_title);

  QJsonDocument doc;
  doc.setObject(json);
  QByteArray request_body = doc.toJson();

  //  qDebug() << "data: "
  //           << QString::fromUtf8(request_body.data(), request_body.size());

  // 发起 post 请求
  QNetworkRequest request(serviceURL);
  accessManager.post(request, request_body);

  // 后台服务器程序，接收数据，处理数据；
}
