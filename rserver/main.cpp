
#include <QCoreApplication>

#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include <resty/mux/Router.h>
#include <resty/mux/RouteMatch.h>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  qhttp::server::QHttpServer server(&app);
  resty::mux::Router router;

  router.handle("/test", [](resty::mux::Request*, resty::mux::Response* resp) {
    resp->end("Hello world!");
  });

  router.handle("/hello/{name}", [](resty::mux::Request* req, resty::mux::Response* resp) {
    auto routeMatch = resty::mux::RouteMatch::getFromRequest(req);
    QString message = "Hello " + routeMatch.vars["name"] + "!";
    resp->end(message.toUtf8());
  });
  
  server.listen(QHostAddress::Any, 8080, router);

  if (!server.isListening()) {
    qDebug("failed to listen");
    return -1;
  }

  return app.exec();
}
