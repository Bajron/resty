#ifndef RESTY_MUX_ROUTEMATCH_H_
#define RESTY_MUX_ROUTEMATCH_H_

#include "fwds.h"

#include <QMetaType>

namespace resty {
namespace mux {

struct RouteMatch {
  Route* route;
  Handler handler;
  Vars vars;
};

}
}

Q_DECLARE_METATYPE(resty::mux::RouteMatch)

#endif
