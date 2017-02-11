#ifndef RESTY_MUX_ROUTE_H_
#define RESTY_MUX_ROUTE_H_

#include "fwds.h"

#include <QString>

#include <memory>

namespace resty {
namespace mux {

struct Route {
  Route(QString path, Handler handler): path(path), handler(handler) {}

  QString path;
  Handler handler;

  std::unique_ptr<RouteMatch> match(const Request* request) const;
};

}
}

#endif