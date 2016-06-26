#ifndef MANS_DEVICEINFO_PARSER_H
#define MANS_DEVICEINFO_PARSER_H

#include <QSharedPointer>

class DeviceInfo;
class PortInfo;

template <typename T> class QList;
class QString;

class DeviceInfoParser
{
public:
  static QList<QSharedPointer<DeviceInfo>> loadFromDir(const QString& dirName);

private:
  static QSharedPointer<DeviceInfo> loadDevice(const QString& dirName);
  static PortInfo loadPort(const QString& fileName);

};

#endif // MANS_DEVICEINFO_PARSER_H
