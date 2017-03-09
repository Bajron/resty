
#include "Router.h"
#include "Route.h"
#include "RouteMatch.h"
#include "RegExBuilder.h"

#include <QUrl>
#include <QVariant>
#include <QByteArray>

#include <qhttpfwd.hpp>
#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

#include <functional>

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

  handler = std::ref(*this);
}

void Router::handle(QString path, Handler handler) {
  auto route = std::make_shared<Route>(path, handler);
  routes.emplace_back(std::move(route));
}

void Router::operator()(Request* request, Response* response) {
  if (auto routeMatch = match(request, RouteMatch())) {
    const auto& matchedHandler = routeMatch->handler;
    request->setProperty(RouteMatch::PROPERTY_NAME, QVariant::fromValue(*(routeMatch.release())));
    return matchedHandler(request, response);
  }
  notFoundHandler(request, response);
}

std::unique_ptr<RouteMatch> Router::match(const Request* request, const RouteMatch& partial) const {
  for (const auto& check : matchers) {
    auto routeMatch = check(request, partial);
    if (!routeMatch)
      return nullptr;
  }

  for (const auto& route : routes) {
    if (auto routeMatch = route->match(request, partial)) {
      return routeMatch;
    }
  }
  return nullptr;
}

void Router::addCheck(const MatchFunction& matcher){
  matchers.push_back(matcher);
}

}
}
