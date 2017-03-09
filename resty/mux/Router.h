
#ifndef RESTY_MUX_ROUTER_H_
#define RESTY_MUX_ROUTER_H_

#include <qhttpfwd.hpp>

#include <QMetaType>
#include <QString>
#include <QRegularExpression>

#include <vector>
#include <memory>
#include <string>

#include "fwds.h"

namespace resty {
namespace mux {


class Router {
public:
  Router();

  typedef std::function<std::unique_ptr<RouteMatch>(const Request* request, const RouteMatch& partial)> MatchFunction;

  Router(const Router&) = delete;
  Router& operator=(const Router&) = delete;

  void addCheck(const MatchFunction& matcher);

  void handle(QString path, Handler handler); 

  void setPrefix(QString prefix);

  void setNotFoundHandler(Handler handler);


  void operator()(Request* request, Response* response);


  Handler handler;

  std::unique_ptr<RouteMatch> match(const Request* request, const RouteMatch& partial) const;

private:
  Handler notFoundHandler;

  std::vector<MatchFunction> matchers;
  std::vector<std::shared_ptr<Route>> routes;
};

}
}

#endif

