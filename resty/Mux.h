
#include <qhttpfwd.hpp>

namespace resty {

class Mux {
public:
  void operator()(qhttp::server::QHttpRequest* request, qhttp::server::QHttpResponse* response);
};

}
