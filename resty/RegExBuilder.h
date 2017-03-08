
#ifndef RESTY_REGEXBUILDER_H_
#define RESTY_REGEXBUILDER_H_

#include <QRegularExpression>
#include <QString>

namespace resty {

class RegExBuilder {
public:
  // changes "{name:regex}/{name2}" to regex with named groups
  static QRegularExpression fromPattern(QString pattern) {
    QRegularExpression bracePair(R"(\{([^}:]+)(:([^}]+))?\})");
    
    QHash<QString, QString> replacements;

    auto it = bracePair.globalMatch(pattern);
    while (it.hasNext()) {
      auto match = it.next();
      auto matched = match.captured(0);
      auto name = match.captured(1);
      QString regex = "[^/?]+";
      if (match.lastCapturedIndex() >= 3) {
        regex = match.captured(3);
      }
      replacements[matched] = QStringLiteral("(?<%1>%2)").arg(name).arg(regex);
    }
    
    QString processedPattern = pattern;

    // Terrible complexity, but probably good enough
    for (auto it = replacements.constBegin(); it != replacements.constEnd(); ++it) {
      processedPattern.replace(it.key(), it.value());
    }

    return QRegularExpression(processedPattern);
  }
};

}

#endif

