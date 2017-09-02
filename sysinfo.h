#ifndef SYSINFO_H
#define SYSINFO_H

#include <windows.h>

class SysInfo {
public:
  static SysInfo &instance();
  virtual ~SysInfo();

  virtual void init() = 0;
  virtual double cpuLoadAverage() = 0;
  virtual void getMemoryInfo(double &total, double &percent) = 0;
  virtual QString getMacAddress() = 0;
  virtual SYSTEM_INFO getSystemInfo() = 0;

  virtual QString getTopAppName() = 0;

protected:
  explicit SysInfo();

private:
  SysInfo(const SysInfo &rhs);
  SysInfo &operator=(const SysInfo &rhs);
};

#endif // SYSINFO_H
