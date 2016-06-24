#ifndef MANS_DEVICEINFO_PARSER_DEVICEINFO_H
#define MANS_DEVICEINFO_PARSER_DEVICEINFO_H

#include <QList>
#include <QMap>
#include <QString>

class DeviceInfo
{
public:
  class PortInfo;

  DeviceInfo(const QString& devName);

  const QString& name() const;

  int portsCount() const;
  const PortInfo& portAt(int index) const;

  void addPort(const PortInfo& port);

private:
  QString name_;
  QList<PortInfo> ports_;

};

class DeviceInfo::PortInfo
{
public:
  enum class DirectionType {
    RECEIVE,
    TRANSMIT
  };

  struct InfoRecord
  {
    qint64  time_  = 0;
    quint64 bytes_ = 0;

    InfoRecord() = default;
    InfoRecord(qint64 time, quint64 bytes);
  };

  PortInfo() = default;
  PortInfo(int num);

  int number() const;

  bool isValid() const;

  int receiveRecordsCount() const;
  int transmitRecordsCount() const;
  int recordsCount(DirectionType type) const;

  const InfoRecord& receiveRecordAt(int index) const;
  const InfoRecord& transmitRecordAt(int index) const;
  const InfoRecord& recordAt(DirectionType type, int index) const;

  void addReceiveRecord(const InfoRecord& record);
  void addTransmitRecord(const InfoRecord& record);
  void addRecord(DirectionType type, const InfoRecord& record);

  static PortInfo merge(const PortInfo& one, const PortInfo& two);

  static const QMap<QString, DirectionType>& portSuffixes();

private:
  int number_ = 0;
  QMap<DirectionType, QList<InfoRecord>> records_;

};

bool operator == (const DeviceInfo::PortInfo::InfoRecord& lhs, const DeviceInfo::PortInfo::InfoRecord& rhs);

#endif // MANS_DEVICEINFO_PARSER_DEVICEINFO_H
