#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QStringList>

#include <ui/deviceinfowidget.h>

namespace {

QString getWorkingDirName(const QStringList& arguments)
{
  if (arguments.size() >= 2) {
    return arguments.at(1);
  }
  return QString::null;
}

bool checkExistsWorkingDir(const QString& dirName)
{
  if (dirName.isEmpty() == false) {
    QDir dir(dirName);
    return dir.exists();
  }
  return false;
}

}

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QString errorString;
  QString workDirName = ::getWorkingDirName(app.arguments());

  if (workDirName.isEmpty() == true) {
    errorString = QObject::trUtf8("Не указан рабочий каталог");
  }
  else {
    if (::checkExistsWorkingDir(workDirName) == false) {
      errorString = QObject::trUtf8("Рабочий каталог не существует");
    }
  }

  if (errorString.isEmpty() == false) {
    QMessageBox::critical(nullptr,
                          QObject::trUtf8("Ошибка"),
                          errorString,
                          QMessageBox::Close);
    return EXIT_FAILURE;
  }

  DeviceInfoWidget wgt;
  wgt.show();
  app.processEvents();

  wgt.init(workDirName);

  return app.exec();
}
