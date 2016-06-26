#include "deviceinfowidget.h"
#include "ui_deviceinfowidget.h"

#include <deviceinfo/parser.h>

#include <QDateTime>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace {

QString datetimeFormat() { return "yyyy-MM-dd hh:mm:ss.zzz"; }

}

DeviceInfoWidget::DeviceInfoWidget(QWidget* parent) :
  QWidget(parent),
  ui_(new Ui::DeviceInfoWidget()),
  currentDevice_(nullptr)
{
  ui_->setupUi(this);

  connect(ui_->closeButton, &QPushButton::clicked, this, &QWidget::close);
  connect(ui_->devListWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DeviceInfoWidget::slotDeviceSelect);
  connect(ui_->portListWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DeviceInfoWidget::slotPortSelect);
}

DeviceInfoWidget::~DeviceInfoWidget()
{
  delete ui_;
  ui_ = nullptr;
}

void DeviceInfoWidget::init(const QString& workDirName)
{
  if (workDirName.isEmpty() == true) {
    return;
  }

  devices_ = DeviceInfoParser::loadFromDir(workDirName);

  for (auto it = devices_.constBegin(), end = devices_.constEnd(); it != end; ++it) {
    ui_->devListWidget->addItem(it->name());
  }

  if (ui_->devListWidget->count() > 0) {
    ui_->devListWidget->item(0)->setSelected(true);
  }
}

void DeviceInfoWidget::slotDeviceSelect()
{
  ui_->portListWidget->clear();
  currentDevice_ = nullptr;

  if (ui_->devListWidget->selectionModel()->hasSelection() == true) {
    foreach (auto item, ui_->devListWidget->selectedItems()) {
      int row = ui_->devListWidget->row(item);
      if (row < devices_.count()) {
        currentDevice_ = &(devices_.at(row));
        for (int i = 0, sz = currentDevice_->portsCount(); i < sz; ++i) {
          ui_->portListWidget->addItem(QString::number(currentDevice_->portAt(i).number()));
        }
      }
    }
  }

  if (ui_->portListWidget->count() > 0) {
    ui_->portListWidget->item(0)->setSelected(true);
  }
}

void DeviceInfoWidget::slotPortSelect()
{
  ui_->receiveTreeWidget->clear();
  ui_->transmitTreeWidget->clear();

  if (   currentDevice_ != nullptr
      && ui_->portListWidget->selectionModel()->hasSelection() == true) {
    foreach (auto item, ui_->portListWidget->selectedItems()) {
      int row = ui_->portListWidget->row(item);
      if (row < currentDevice_->portsCount()) {
        loadInfoRecordsForPort(currentDevice_->portAt(row));
      }
    }
  }
}

void DeviceInfoWidget::loadInfoRecordsForPort(const DeviceInfo::PortInfo& port)
{
  loadInfoRecordsForPort(port, ui_->receiveTreeWidget, DeviceInfo::PortInfo::DirectionType::RECEIVE);
  loadInfoRecordsForPort(port, ui_->transmitTreeWidget, DeviceInfo::PortInfo::DirectionType::TRANSMIT);
}

void DeviceInfoWidget::loadInfoRecordsForPort(const DeviceInfo::PortInfo& port, QTreeWidget* target, DeviceInfo::PortInfo::DirectionType type)
{
  Q_CHECK_PTR(target);

  for (int i = 1, sz = port.recordsCount(type); i < sz; ++i) {
    qint64 curDt = port.recordAt(type, i).time_;
    qint64 prevDt = port.recordAt(type, i-1).time_;
    if (prevDt > curDt) {
      continue;
    }
    double duration = (curDt - prevDt)/1000.0;
    double speed = (qFuzzyCompare(1.0 + duration, 1.0 + 0.0) == false) ? (port.recordAt(type, i).bytes_)/duration
                                                                       : 0.0;
    QStringList fields;
    fields << QDateTime::fromMSecsSinceEpoch(prevDt).toString(::datetimeFormat())
           << QDateTime::fromMSecsSinceEpoch(curDt).toString(::datetimeFormat())
           << QString::number(duration, 'f', 3)
           << QString::number(speed, 'f', 3);
    QTreeWidgetItem* item = new QTreeWidgetItem(target, fields);
    target->addTopLevelItem(item);
  }

  for (int i = 0, sz = target->columnCount(); i < sz; ++i) {
    target->resizeColumnToContents(i);
  }
}
