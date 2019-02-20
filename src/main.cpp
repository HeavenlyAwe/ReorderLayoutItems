#include "domparser.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>

#include <iostream>

int main(int argc, char *argv[]) {
  DomParser parser;

  QString path = "";
  for (int i = 1; i < argc; ++i) {
    if (QString(argv[i]) == "--verbose") {
      parser.setVerbose(true);

    } else if (QString(argv[i]) == "--backup") {
      parser.setBackup(true);

    } else if (QString(argv[i]) == "--help" || QString(argv[i]) == "-h") {
      std::cerr << "Please give the UI file to reorder." << std::endl;
      std::cerr << argv[0] << "<ui file> [--verbose]" << std::endl;
      return 1;

    } else {
      path = QString(argv[i]);
    }
  }

  parser.parse(path);
  parser.organizeItems();
  parser.print();

  return 0;
}
