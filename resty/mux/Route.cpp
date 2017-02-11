#include "Route.h"
#include "RouteMatch.h"

#include <QUrl>

#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

namespace resty {
namespace mux {

std::unique_ptr<RouteMatch> Route::match(const Request* request) const {
  const QUrl& url = request->url();

  if (path == url.path()) {
    std::unique_ptr<RouteMatch> m(new RouteMatch{this, handler});
    return std::move(m);
  } else {
    return nullptr;
  }
}

}
}

