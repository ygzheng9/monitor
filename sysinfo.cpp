#include "sysinfo.h"

#include <QtGlobal>

#include "SysInfoWindowsImpl.h"

SysInfo &SysInfo::instance() {
  static SysInfoWindowsImpl singleton;
  return singleton;
}
SysInfo::SysInfo() {}

SysInfo::~SysInfo() {}
