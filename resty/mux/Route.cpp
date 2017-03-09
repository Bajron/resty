#include "Route.h"
#include "RouteMatch.h"

#include <QUrl>

#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

namespace resty {
namespace mux {

std::unique_ptr<RouteMatch> Route::match(const Request* request, const RouteMatch& partial) const {
  const QUrl& url = request->url();

  int offset = partial.pathPrefix;
  auto hit = regularExpression.match(url.path(), offset);
  if (hit.hasMatch()) {
    std::unique_ptr<RouteMatch> routeMatch(new RouteMatch{this, handler, partial.pathPrefix, partial.vars});
    for (const auto& groupName : regularExpression.namedCaptureGroups()) {
      if (groupName.isEmpty())
        continue;
      routeMatch->vars[groupName] = hit.captured(groupName);
    }
    return std::move(routeMatch);
  } else {
    return nullptr;
  }
}

}
}

