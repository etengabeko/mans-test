#include "deviceinfo.h"

DeviceInfo::DeviceInfo(const QString& devName) :
  name_(devName)
{
}

const QString& DeviceInfo::name() const
{
  return name_;
}

int DeviceInfo::portsCount() const
{
  return ports_.count();
}

const DeviceInfo::PortInfo& DeviceInfo::portAt(int index) const
{
  return ports_.at(index);
}

void DeviceInfo::addPort(const PortInfo& port)
{
  ports_.append(port);
}

DeviceInfo::PortInfo::PortInfo(int num) :
  number_(num)
{
}

int DeviceInfo::PortInfo::number() const
{
  return number_;
}

bool DeviceInfo::PortInfo::isValid() const
{
  return number_ > 0;
}

int DeviceInfo::PortInfo::receiveRecordsCount() const
{
  return recordsCount(DirectionType::RECEIVE);
}

int DeviceInfo::PortInfo::transmitRecordsCount() const
{
  return recordsCount(DirectionType::TRANSMIT);
}


int DeviceInfo::PortInfo::recordsCount(DirectionType type) const
{
  return records_[type].count();
}

const DeviceInfo::PortInfo::InfoRecord& DeviceInfo::PortInfo::receiveRecordAt(int index) const
{
  return recordAt(DirectionType::RECEIVE, index);
}

const DeviceInfo::PortInfo::InfoRecord& DeviceInfo::PortInfo::transmitRecordAt(int index) const
{
  return recordAt(DirectionType::TRANSMIT, index);
}

const DeviceInfo::PortInfo::InfoRecord& DeviceInfo::PortInfo::recordAt(DirectionType type, int index) const
{
  return records_[type].at(index);
}

void DeviceInfo::PortInfo::addReceiveRecord(const InfoRecord& record)
{
  addRecord(DirectionType::RECEIVE, record);
}

void DeviceInfo::PortInfo::addTransmitRecord(const InfoRecord& record)
{
  addRecord(DirectionType::TRANSMIT, record);
}

void DeviceInfo::PortInfo::addRecord(DirectionType type, const InfoRecord& record)
{
  records_[type].append(record);
}

DeviceInfo::PortInfo DeviceInfo::PortInfo::merge(const PortInfo& one, const PortInfo& two)
{
  if (   one.isValid() == true
      && two.isValid() == true
      && (one.number() == two.number())) {
    PortInfo result(one);

    for (auto it = two.records_.constBegin(), end = two.records_.constEnd(); it != end; ++it) {
      if (result.records_.contains(it.key()) == false) {
        result.records_.insert(it.key(), it.value());
      }
      else {
        for (auto recIt = it.value().constBegin(), recEnd = it.value().constEnd(); recIt != recEnd; ++recIt) {
          if (result.records_[it.key()].contains(*recIt) == false) {
            result.records_[it.key()].append(*recIt);
          }
        }
      }
    }
    return result;
  }

  return PortInfo();
}

const QMap<QString, DeviceInfo::PortInfo::DirectionType>& DeviceInfo::PortInfo::portSuffixes()
{
  static QMap<QString, DirectionType> suffixes;
  if (suffixes.isEmpty() == true) {
    suffixes.insert("tx", DirectionType::TRANSMIT);
    suffixes.insert("rx", DirectionType::RECEIVE);
  }

  return suffixes;
}

DeviceInfo::PortInfo::InfoRecord::InfoRecord(qint64 time, quint64 bytes) :
  time_(time),
  bytes_(bytes)
{
}

bool operator == (const DeviceInfo::PortInfo::InfoRecord& lhs, const DeviceInfo::PortInfo::InfoRecord& rhs)
{
  return    lhs.time_ == rhs.time_
         && lhs.bytes_ == rhs.bytes_;
}
