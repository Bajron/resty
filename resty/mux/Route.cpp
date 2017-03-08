#include "Route.h"
#include "RouteMatch.h"

#include <QUrl>

#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

namespace resty {
namespace mux {

std::unique_ptr<RouteMatch> Route::match(const Request* request) const {
  const QUrl& url = request->url();

  auto hit = regularExpression.match(url.path());
  if (hit.hasMatch()) {
    std::unique_ptr<RouteMatch> routeMatch(new RouteMatch{this, handler});
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

