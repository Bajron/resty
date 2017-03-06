
#ifndef RESTY_QT_TEST_H_
#define RESTY_QT_TEST_H_

#include <iostream>
#include <QString>

void PrintTo(const QString& str, ::std::ostream *out) {
  *out << str.toStdString();
}

#endif

