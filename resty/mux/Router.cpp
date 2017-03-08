
#include "Router.h"
#include "Route.h"
#include "RouteMatch.h"

#include <QUrl>
#include <QVariant>
#include <QByteArray>

#include <qhttpfwd.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

namespace resty {
namespace mux {

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
    if (auto routeMatch = route->match(request)) {
      request->setProperty(RouteMatch::PROPERTY_NAME, QVariant::fromValue(*(routeMatch.release())));
      route->handler(request, response);
      return;
    }
  }
  notFoundHandler(request, response);
}

void Router::handle(QString path, Handler handler) {
  auto route = std::make_shared<Route>(path, handler);
  routes.insert(route); 
}

}
}
