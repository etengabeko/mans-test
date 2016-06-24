#ifndef MANS_UI_DEVICEINFOWIDGET_H
#define MANS_UI_DEVICEINFOWIDGET_H

#include <deviceinfo_parser/deviceinfo.h>

#include <QList>
#include <QWidget>

class QTreeWidget;

namespace Ui {
  class DeviceInfoWidget;
} // Ui

class DeviceInfoWidget : public QWidget
{
  Q_OBJECT

public:
  DeviceInfoWidget(QWidget* parent = nullptr);
  ~DeviceInfoWidget();

  void init(const QString& workDirName);

private slots:
  void slotDeviceSelect();
  void slotPortSelect();

private:
  void loadInfoRecordsForPort(const DeviceInfo::PortInfo& port);
  void loadInfoRecordsForPort(const DeviceInfo::PortInfo& port, QTreeWidget* target, DeviceInfo::PortInfo::DirectionType type);

private:
  Ui::DeviceInfoWidget* ui_;
  QList<DeviceInfo> devices_;
  const DeviceInfo* currentDevice_;

};

#endif // MANS_UI_DEVICEINFOWIDGET_H
