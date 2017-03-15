
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

  router.addResourceHandler("/test", [](resty::mux::Request*, resty::mux::Response* resp) {
    resp->end("Hello world!");
  });

  router.addResourceHandler("/about", [](resty::mux::Request* req, resty::mux::Response* resp) {
    QByteArray data;
    QTextStream ss(&data);
    ss << req->httpVersion().toUtf8() << '\n';
    ss << req->methodString() << '\n';
    ss << req->url().url().toUtf8() << '\n';
    ss << req->remoteAddress().toUtf8() << '\n';
    ss << req->remotePort() << endl;
  
    resp->end(data);
  });

  router.addResourceHandler("/hello/{name}", [](resty::mux::Request* req, resty::mux::Response* resp) {
    auto routeMatch = resty::mux::RouteMatch::getFromRequest(req);
    QString message = "Hello " + routeMatch.vars["name"] + "!";
    resp->end(message.toUtf8());
  });

  auto apiRouter = std::make_shared<resty::mux::Router>();
  apiRouter->addResourceHandler("/foo", [](resty::mux::Request* req, resty::mux::Response* resp) {
    auto routeMatch = resty::mux::RouteMatch::getFromRequest(req);
    QString message = "version: " + routeMatch.vars["version"] + " test: " + routeMatch.vars["test"];
    resp->end(message.toUtf8());
  });

  auto v1Router = std::make_shared<resty::mux::Router>();
  v1Router->setPrefix("/{version:v1}");
  v1Router->addSubRouter(apiRouter);

  auto v2Router = std::make_shared<resty::mux::Router>();
  v2Router->setPrefix("/v2");
  v2Router->addSubRouter(apiRouter);
  
  router.addSubRouter(v1Router);
  router.addSubRouter(v2Router);

  server.listen(QHostAddress::Any, 8080, std::ref(router));

  if (!server.isListening()) {
    qDebug("failed to listen");
    return -1;
  }

  return app.exec();
}
