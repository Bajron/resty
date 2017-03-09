
#include <QCoreApplication>
#include <QTextStream>

#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include <resty/mux/Router.h>
#include <resty/mux/RouteMatch.h>

#include <iostream>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  std::clog << "RouteMatch: " << sizeof(resty::mux::RouteMatch) << std::endl;
  std::clog << "Handler: " << sizeof(resty::mux::Handler) << std::endl;
  std::clog << "Vars: " << sizeof(resty::mux::Vars) << std::endl;

  qhttp::server::QHttpServer server(&app);
  resty::mux::Router router;

  //router.setPrefix("/prefix");

  router.handle("/test", [](resty::mux::Request*, resty::mux::Response* resp) {
    resp->end("Hello world!");
  });

  router.handle("/about", [](resty::mux::Request* req, resty::mux::Response* resp) {
    QByteArray data;
    QTextStream ss(&data);
    ss << req->httpVersion().toUtf8() << '\n';
    ss << req->methodString() << '\n';
    ss << req->url().url().toUtf8() << '\n';
    ss << req->remoteAddress().toUtf8() << '\n';
    ss << req->remotePort() << endl;
  
    resp->end(data);
  });

  router.handle("/hello/{name}", [](resty::mux::Request* req, resty::mux::Response* resp) {
    auto routeMatch = resty::mux::RouteMatch::getFromRequest(req);
    QString message = "Hello " + routeMatch.vars["name"] + "!";
    resp->end(message.toUtf8());
  });
  
  server.listen(QHostAddress::Any, 8080, std::ref(router));

  if (!server.isListening()) {
    qDebug("failed to listen");
    return -1;
  }

  return app.exec();
}
