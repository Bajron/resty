#ifndef RESTY_MUX_ROUTEMATCH_H_
#define RESTY_MUX_ROUTEMATCH_H_

#include "fwds.h"

#include <QMetaType>
#include <QString>

#include <memory>

namespace resty {
namespace mux {

struct RouteMatch {
  // choose one, route causes one level of indirection on handle
  const Route* route = nullptr;
  Handler handler = nullptr;

  int pathPrefix = 0;
  Vars vars;

  static constexpr const char* PROPERTY_NAME = "routeMatch";

  static RouteMatch getFromRequest(Request* request);
};

}
}

Q_DECLARE_METATYPE(resty::mux::RouteMatch)

#endif
