
#include <qhttpfwd.hpp>

namespace resty {
namespace mux {

class Router {
public:
  

  void operator()(qhttp::server::QHttpRequest* request, qhttp::server::QHttpResponse* response);
};

}
}

