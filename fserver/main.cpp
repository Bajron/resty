
#include <QCoreApplication>
#include <QTextStream>
#include <QDir>
#include <QFile>

#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include <resty/mux/Router.h>
#include <resty/mux/RouteMatch.h>

#include <iostream>

using namespace resty::mux;

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);
  qhttp::server::QHttpServer server(&app);

  QString rootDir = ".";

  auto router = std::make_shared<Router >();
  router->addResourceHandler("/{file}", [rootDir](Request* req, Response* resp) {
    QString path = rootDir + req->url().path();
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
      resp->end(file.readAll());
    } else {
      resp->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
      resp->end("Not found");
    }
  });
  router->addResourceHandler("/", [rootDir](Request* req, Response* resp) {
    QString path = req->url().path();
    QDir dir(rootDir + path);
    QString body;
    QTextStream stream(&body);
    for (const auto& f : dir.entryInfoList()) {
      stream << "<a href=\"" << path << f.fileName() << (f.isDir() ? "/" : "") << "\">" << f.fileName() << "</a><br>\n";
    }
    resp->end(body.toUtf8());
  });

  // Silly directory eating machine
  auto dirRouter = std::make_shared<Router >();
  dirRouter->setPrefix("/{dir}");
  dirRouter->addSubRouter(dirRouter);
  dirRouter->addSubRouter(router); 

  router->addSubRouter(dirRouter);

  server.listen(QHostAddress::Any, 8080, std::ref(*router));

  if (!server.isListening()) {
    qDebug("failed to listen");
    return -1;
  }

  return app.exec();
}
