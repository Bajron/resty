
#ifndef RESTY_MUX_ROUTER_H_
#define RESTY_MUX_ROUTER_H_

#include <qhttpfwd.hpp>

#include <QMetaType>
#include <QString>

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <string>

#include "fwds.h"

namespace resty {
namespace mux {

class Router {
public:
  Router();

  void handle(QString path, Handler handler); 

  void setNotFoundHandler(Handler handler);

  void operator()(Request* request, Response* response);

private:
  Handler notFoundHandler;

  std::unordered_set<std::shared_ptr<Route>> routes;
};

}
}

#endif

