#include "sysinfowindowsimpl.h"
#include <QList>
#include <QNetworkInterface>

#include <windows.h>

SysInfoWindowsImpl::SysInfoWindowsImpl() : SysInfo(), mCpuLoadLastValues() {}

void SysInfoWindowsImpl::init() { mCpuLoadLastValues = cpuRawData(); }

QVector<qulonglong> SysInfoWindowsImpl::cpuRawData() {
  FILETIME idleTime;
  FILETIME kernelTime;
  FILETIME userTime;

  GetSystemTimes(&idleTime, &kernelTime, &userTime);

  QVector<qulonglong> rawData;
  rawData.append(convertFileTime(idleTime));
  rawData.append(convertFileTime(kernelTime));
  rawData.append(convertFileTime(userTime));
  return rawData;
}

qulonglong SysInfoWindowsImpl::convertFileTime(const FILETIME &filetime) const {
  ULARGE_INTEGER largeInteger;
  largeInteger.LowPart = filetime.dwLowDateTime;
  largeInteger.HighPart = filetime.dwHighDateTime;
  return largeInteger.QuadPart;
}

double SysInfoWindowsImpl::cpuLoadAverage() {
  QVector<qulonglong> firstSample = mCpuLoadLastValues;
  QVector<qulonglong> secondSample = cpuRawData();
  mCpuLoadLastValues = secondSample;

  qulonglong currentIdle = secondSample[0] - firstSample[0];
  qulonglong currentKernel = secondSample[1] - firstSample[1];
  qulonglong currentUser = secondSample[2] - firstSample[2];
  qulonglong currentSystem = currentKernel + currentUser;

  double percent = (currentSystem - currentIdle) * 100.0 / currentSystem;
  return qBound(0.0, percent, 100.0);
}

// 传入的参数，其实是返回值；
void SysInfoWindowsImpl::getMemoryInfo(double &total, double &percent) {
  MEMORYSTATUSEX memoryStatus;
  memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memoryStatus);

  const double Gb = 1024 * 1024 * 1024 * 1.0;
  total = memoryStatus.ullTotalPhys / Gb;

  qulonglong memoryPhysicalUsed =
      memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
  percent =
      (double)memoryPhysicalUsed / (double)memoryStatus.ullTotalPhys * 100.0;
}

QString SysInfoWindowsImpl::getMacAddress() {
  // mac 地址不变，所以只需要取得一次
  if (!alreadyHaveMac) {
    QList<QNetworkInterface> nets =
        QNetworkInterface::allInterfaces(); // 获取所有网络接口列表

    int nCnt = nets.count();
    for (int i = 0; i < nCnt; i++) {
      // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
      if (nets[i].flags().testFlag(QNetworkInterface::IsUp) &&
          nets[i].flags().testFlag(QNetworkInterface::IsRunning) &&
          !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack)) {
        macAddr = nets[i].hardwareAddress();
        alreadyHaveMac = true;
        break;
      }
    }
  }

  return macAddr;
}

SYSTEM_INFO SysInfoWindowsImpl::getSystemInfo() {
  if (!alreadyHaveSysInfo) {
    GetSystemInfo(&systemInfo);

    alreadyHaveSysInfo = true;
  }
  return systemInfo;
}

QString SysInfoWindowsImpl::getTopAppName() {
  TCHAR wnd_title[256];
  HWND hwnd = GetForegroundWindow(); // get handle of currently active window
  GetWindowTextA(hwnd, (LPSTR)wnd_title, 256);

  return QString::fromLocal8Bit((const char *)wnd_title, 256);
}
