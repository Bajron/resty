
#include "Router.h"

#include <QUrl>
#include <QVariant>
#include <QByteArray>

#include <qhttpfwd.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

namespace resty {
namespace mux {

bool Route::match(const Request* request) const {
  const QUrl& url = request->url();

  return path == url.path(); 
}


Router::Router() {
  notFoundHandler = [](Request* request, Response* response) {
    request->onEnd([request, response](){
        QByteArray info;
        switch (request->method()) {
          case qhttp::THttpMethod::EHTTP_GET:
            info = "Requested resource is not found.";
          case qhttp::THttpMethod::EHTTP_HEAD:
            response->setStatusCode(qhttp::ESTATUS_NOT_FOUND);
            break;
          default:
            info = "Not allowed.";
            response->setStatusCode(qhttp::ESTATUS_METHOD_NOT_ALLOWED);
        }
        response->end(info);
    });
  };
}

void Router::operator()(Request* request, Response* response) {
  for (const auto& route : routes) {
    if (route->match(request)) {
      RouteMatch match{route.get(), route->handler};
      request->setProperty("routeMatch", QVariant::fromValue(match));
      route->handler(request, response);
      return;
    }
  }
  notFoundHandler(request, response);
}

void Router::handle(const std::string& path, Handler handler) {
  auto route = std::make_shared<Route>(QString::fromStdString(path), handler);
  routes.insert(route); 
}

}
}
