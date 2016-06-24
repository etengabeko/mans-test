#include "parser.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>

QList<DeviceInfo> DeviceInfoParser::loadFromDir(const QString& dirName)
{
  QList<DeviceInfo> result;

  if (dirName.isEmpty() == false) {
    QDir dir(dirName);
    foreach (const QFileInfo& finfo, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
      DeviceInfo devinfo = loadDevice(finfo.absoluteFilePath());
      if (devinfo.portsCount() > 0) {
        result.append(devinfo);
      }
    }
  }

  return result;
}

DeviceInfo DeviceInfoParser::loadDevice(const QString& dirName)
{
  QDir dir(dirName);
  DeviceInfo result(dir.dirName());

  QStringList nameFilters;
  foreach (const QString& sfx, DeviceInfo::PortInfo::portSuffixes().keys()) {
    nameFilters.append(QString("*.%1").arg(sfx));
  }

  QMap<int, QList<DeviceInfo::PortInfo>> allports;
  foreach (const QFileInfo& finfo, dir.entryInfoList(nameFilters, QDir::Files)) {
    DeviceInfo::PortInfo pi = loadPort(finfo.absoluteFilePath());
    if (pi.isValid() == true) {
      allports[pi.number()].append(pi);
    }
  }

  for (auto it = allports.constBegin(), end = allports.constEnd(); it != end; ++it) {
    const QList<DeviceInfo::PortInfo>& each = it.value();
    if (each.count() == 1) {
      result.addPort(each.first());
    }
    else if (each.count() > 1) {
      DeviceInfo::PortInfo merged = each.first();
      for (auto pIt = each.constBegin()+1, pEnd = each.constEnd(); pIt != pEnd; ++pIt) {
        merged = DeviceInfo::PortInfo::merge(merged, *pIt);
      }
      result.addPort(merged);
    }
  }

  return result;
}

DeviceInfo::PortInfo DeviceInfoParser::loadPort(const QString& fileName)
{
  QFileInfo fi(fileName);

  DeviceInfo::PortInfo result(fi.baseName().toInt());
  DeviceInfo::PortInfo::DirectionType type = DeviceInfo::PortInfo::portSuffixes()[fi.completeSuffix()];

  QFile f(fileName);
  if (f.open(QFile::ReadOnly) == true) {
    QString first,
            second;
    while (f.atEnd() == false) {
      QTextStream input(f.readLine());
      input >> first >> second;
      bool ok = false;
      qint64 time = first.toLongLong(&ok);
      if (ok == true) {
        quint64 bytes = second.toULongLong(&ok);
        if (ok == true) {
          result.addRecord(type, DeviceInfo::PortInfo::InfoRecord(time, bytes));
        }
      }
    }
  }
  else {
    qDebug() << QString("%1: %2").arg(fileName).arg(f.errorString());
  }

  return result;
}
