#ifndef RESTY_MUX_FWDS_H_
#define RESTY_MUX_FWDS_H_

#include <qhttpfwd.hpp>

#include <unordered_map>

namespace resty {
namespace mux {

using Handler = qhttp::server::TServerHandler;

using Request = qhttp::server::QHttpRequest;
using Response = qhttp::server::QHttpResponse;

using Vars = std::unordered_map<std::string, std::string>;

class Route;
class RouteMatch;

}
}

#endif

