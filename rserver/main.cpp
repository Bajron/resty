
#include <QCoreApplication>

#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include <resty/mux/Router.h>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  qhttp::server::QHttpServer server(&app);
  resty::mux::Router router;

  router.handle("/test", [](resty::mux::Request*, resty::mux::Response* resp) {
    resp->end("Hello world!");
  });

  server.listen(QHostAddress::Any, 8080, router);

  if (!server.isListening()) {
    qDebug("failed to listen");
    return -1;
  }

  return app.exec();
}
