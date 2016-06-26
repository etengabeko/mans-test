#ifndef MANS_DEVICEINFO_PARSER_H
#define MANS_DEVICEINFO_PARSER_H

#include "deviceinfo.h"

template <typename T> class QList;
class QString;

class DeviceInfoParser
{
public:
  static QList<DeviceInfo> loadFromDir(const QString& dirName);

private:
  static DeviceInfo loadDevice(const QString& dirName);
  static DeviceInfo::PortInfo loadPort(const QString& fileName);

};

#endif // MANS_DEVICEINFO_PARSER_H
