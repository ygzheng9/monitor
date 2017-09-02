#include <QCoreApplication>

#include "myservice.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  MyService service(argc, argv);

  return service.exec();
}
