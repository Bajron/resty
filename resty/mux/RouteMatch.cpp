#include "RouteMatch.h"
#include <qhttpserverrequest.hpp>
#include <QVariant>

namespace resty {
namespace mux {

RouteMatch RouteMatch::getFromRequest(Request* request) {
  QVariant variant = request->property(PROPERTY_NAME);
  if (variant.isValid() && variant.canConvert<RouteMatch>())
    return variant.value<RouteMatch>();
  else
    return RouteMatch();
}

}
}

