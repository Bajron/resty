
#include "Router.h"
#include "Route.h"
#include "RouteMatch.h"
#include "RegExBuilder.h"

#include <QUrl>
#include <QVariant>
#include <QByteArray>
#include <QString>

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

void Router::setNotFoundHandler(Handler handler) {
  notFoundHandler = handler;
}

Router::~Router() {

}

void Router::addResourceHandler(QString path, Handler handler) {
  auto route = std::make_shared<Route>(path, handler);
  routes.emplace_back(std::move(route));
}

void Router::addCheck(const MatchFunction& matcher){
  matchers.push_back(matcher);
}

void Router::addSubRouter(std::shared_ptr<Router> router) {
  subRouters.push_back(router);
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
  std::unique_ptr<RouteMatch> context(new RouteMatch(partial));

  for (const auto& check : matchers) {
    context = std::move(check(request, *context));
    if (!context)
      return nullptr;
  }

  for (const auto& subRouter : subRouters) {
    if (auto routeMatch = subRouter->match(request, *context)) {
      return routeMatch;
    }
  }

  for (const auto& route : routes) {
    if (auto routeMatch = route->match(request, *context)) {
      return routeMatch;
    }
  }
  return nullptr;
}

struct PrefixChecker {
  PrefixChecker(QString prefix):
    re(RegExBuilder::fromPattern("^" + prefix)) {
  }

  std::unique_ptr<RouteMatch> operator()(const Request* request, const RouteMatch& partial) const {
    const QUrl& url = request->url();

    int offset = partial.pathPrefix;
    auto hit = re.match(url.path(), offset);
    if (hit.hasMatch()) {
      std::unique_ptr<RouteMatch> routeMatch(new RouteMatch{nullptr, nullptr, partial.pathPrefix + hit.capturedEnd(), partial.vars});
      for (const auto& groupName : re.namedCaptureGroups()) {
        if (groupName.isEmpty())
          continue;
        routeMatch->vars[groupName] = hit.captured(groupName);
      }
      return std::move(routeMatch);
    } else {
      return nullptr;
    }
  }

  QRegularExpression re;
};

// FIXME: call twice for crash
void Router::setPrefix(QString prefix) {
  prefixChecker.reset(new PrefixChecker(prefix));
  addCheck(std::ref(*prefixChecker));
}

}
}
