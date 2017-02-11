
#ifndef RESTY_MUX_ROUTER_H_
#define RESTY_MUX_ROUTER_H_

#include <qhttpfwd.hpp>

#include <QMetaType>

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <string>

namespace resty {
namespace mux {

using Handler = qhttp::server::TServerHandler;

using Request = qhttp::server::QHttpRequest;
using Response = qhttp::server::QHttpResponse;

using Vars = std::unordered_map<std::string, std::string>;

struct Route {
  Route(QString path, Handler handler): path(path), handler(handler) {}

  QString path;
  Handler handler;

  bool match(const Request* request) const;
};

struct RouteMatch {
  Route* route;
  Handler handler;
  Vars vars;
};


class Router {
public:
  Router();

  void handle(const std::string& path, Handler handler); 

  void setNotFoundHandler(Handler handler);

  void operator()(Request* request, Response* response);

private:
  Handler notFoundHandler;

  std::unordered_set<std::shared_ptr<Route>> routes;
};

}
}

Q_DECLARE_METATYPE(resty::mux::RouteMatch)

#endif

