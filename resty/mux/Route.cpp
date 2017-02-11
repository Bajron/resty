#include "Route.h"

#include <QUrl>

#include <qhttpserverrequest.hpp>
#include <qhttpserverresponse.hpp>

namespace resty {
namespace mux {

bool Route::match(const Request* request) const {
  const QUrl& url = request->url();

  return path == url.path(); 
}

}
}

