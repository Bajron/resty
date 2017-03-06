
#ifndef RESTY_REGEXBUILDER_H_
#define RESTY_REGEXBUILDER_H_

#include <QRegularExpression>

namespace resty {

class RegExBuilder {
public:
  // changes "{name:regex}/{name2}" to regex with named groups
  QRegularExpression fromPattern(QString pat);
};

}

#endif

