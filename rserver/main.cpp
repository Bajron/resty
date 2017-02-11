
#include <QCoreApplication>

#include <qhttpserver.hpp>

#include <resty/mux/Router.h>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  qhttp::server::QHttpServer server(&app);
  resty::mux::Router router;

  server.listen(QHostAddress::Any, 8080, router);

  if (!server.isListening()) {
    qDebug("failed to listen");
    return -1;
  }

  return app.exec();
}
