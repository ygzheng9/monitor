#ifndef SYSINFOWINDOWSIMPL_H
#define SYSINFOWINDOWSIMPL_H

#include <QVector>
#include <QtGlobal>

#include "SysInfo.h"

typedef struct _FILETIME FILETIME;

class SysInfoWindowsImpl : public SysInfo {
public:
  SysInfoWindowsImpl();

  void init() override;
  double cpuLoadAverage() override;

  void getMemoryInfo(double &total, double &rate) override;
  QString getMacAddress() override;

  SYSTEM_INFO getSystemInfo() override;

  QString getTopAppName() override;

private:
  QVector<qulonglong> cpuRawData();
  qulonglong convertFileTime(const FILETIME &filetime) const;

private:
  bool alreadyHaveMac = false;
  QString macAddr;

  bool alreadyHaveSysInfo = false;
  SYSTEM_INFO systemInfo;

private:
  QVector<qulonglong> mCpuLoadLastValues;
};

#endif // SYSINFOWINDOWSIMPL_H
