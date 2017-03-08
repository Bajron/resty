#ifndef RESTY_MUX_ROUTEMATCH_H_
#define RESTY_MUX_ROUTEMATCH_H_

#include "fwds.h"

#include <QMetaType>
#include <QString>

namespace resty {
namespace mux {

struct RouteMatch {
  const Route* route = nullptr;
  Handler handler = nullptr;
  Vars vars;
  
  static constexpr const char* PROPERTY_NAME = "routeMatch";

  static RouteMatch getFromRequest(Request* request);
};

}
}

Q_DECLARE_METATYPE(resty::mux::RouteMatch)

#endif
