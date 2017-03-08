#ifndef RESTY_MUX_FWDS_H_
#define RESTY_MUX_FWDS_H_

#include <qhttpfwd.hpp>

#include <QHash>
#include <QString>

namespace resty {
namespace mux {

using Handler = qhttp::server::TServerHandler;

using Request = qhttp::server::QHttpRequest;
using Response = qhttp::server::QHttpResponse;

using Vars = QHash<QString, QString>;

class Route;
class RouteMatch;

}
}

#endif

