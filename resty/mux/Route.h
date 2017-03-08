#ifndef RESTY_MUX_ROUTE_H_
#define RESTY_MUX_ROUTE_H_

#include "fwds.h"
#include "RegExBuilder.h"

#include <QString>
#include <QRegularExpression>

#include <memory>

namespace resty {
namespace mux {

struct Route {
  Route(QString path, Handler handler) :
    regularExpression(RegExBuilder::fromPattern("^" + path + "$")),
    handler(handler) {
      
    }

  QRegularExpression regularExpression;
  Handler handler;

  std::unique_ptr<RouteMatch> match(const Request* request) const;
};

}
}

#endif
