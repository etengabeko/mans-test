#include "parser.h"
#include "deviceinfo.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTextStream>

QList<QSharedPointer<DeviceInfo>> DeviceInfoParser::loadFromDir(const QString& dirName)
{
  QList<QSharedPointer<DeviceInfo>> result;

  if (dirName.isEmpty() == false) {
    QDir dir(dirName);
    foreach (const QFileInfo& finfo, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
      QSharedPointer<DeviceInfo> devinfo = loadDevice(finfo.absoluteFilePath());
      if (   devinfo.isNull() == false
          && devinfo->portsCount() > 0) {
        result.append(devinfo);
      }
    }
  }
  return result;
}

QSharedPointer<DeviceInfo> DeviceInfoParser::loadDevice(const QString& dirName)
{
  QDir dir(dirName);
  QSharedPointer<DeviceInfo> result(new DeviceInfo(dir.dirName()));

  QStringList nameFilters;
  foreach (const QString& sfx, PortInfo::portSuffixes().keys()) {
    nameFilters.append(QString("*.%1").arg(sfx));
  }

  QMap<int, QList<PortInfo>> allports;
  foreach (const QFileInfo& finfo, dir.entryInfoList(nameFilters, QDir::Files)) {
    PortInfo pi = loadPort(finfo.absoluteFilePath());
    if (pi.isValid() == true) {
      allports[pi.number()].append(pi);
    }
  }

  for (auto it = allports.constBegin(), end = allports.constEnd(); it != end; ++it) {
    const QList<PortInfo>& each = it.value();
    if (each.count() == 1) {
      result->addPort(each.first());
    }
    else if (each.count() > 1) {
      PortInfo merged = each.first();
      for (auto pIt = each.constBegin()+1, pEnd = each.constEnd(); pIt != pEnd; ++pIt) {
        merged = PortInfo::merge(merged, *pIt);
      }
      result->addPort(merged);
    }
  }
  return result;
}

PortInfo DeviceInfoParser::loadPort(const QString& fileName)
{
  QFileInfo fi(fileName);

  PortInfo result(fi.baseName().toInt());
  PortInfo::DirectionType type = PortInfo::portSuffixes()[fi.completeSuffix()];

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
          result.addRecord(type, PortInfo::InfoRecord(time, bytes));
        }
      }
    }
  }
  else {
    qDebug() << QString("%1: %2").arg(fileName).arg(f.errorString());
  }
  return result;
}
