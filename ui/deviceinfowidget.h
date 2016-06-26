#ifndef MANS_UI_DEVICEINFOWIDGET_H
#define MANS_UI_DEVICEINFOWIDGET_H

#include <QList>
#include <QSharedPointer>
#include <QWidget>

class QTreeWidget;

namespace Ui {
  class DeviceInfoWidget;
} // Ui

class DeviceInfo;
class PortInfo;

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
  void loadInfoRecordsForPort(const PortInfo& port);
  void loadInfoRecordsForPort(const PortInfo& port, QTreeWidget* target);

private:
  Ui::DeviceInfoWidget* ui_;
  QList<QSharedPointer<DeviceInfo>> devices_;
  QSharedPointer<DeviceInfo> currentDevice_;

};

#endif // MANS_UI_DEVICEINFOWIDGET_H
