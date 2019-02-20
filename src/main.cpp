#include "domparser.h"
#include <QString>

#include <iostream>

void help(QString message, QString arg) {
  std::cerr << "  " << message.toStdString() << std::endl;
  std::cerr << "  " << arg.toStdString() << " " << "<ui file> [--verbose] [--backup]" << std::endl;
}

int main(int argc, char *argv[]) {
  DomParser parser;

  QString path = "";
  if (argc < 2) {
    help("Please give the UI file to reorder.", argv[0]);
    return 1;
  }
  path = QString(argv[1]);

  for (int i = 2; i < argc; ++i) {
    if (QString(argv[i]) == "--verbose") {
      parser.setVerbose(true);

    } else if (QString(argv[i]) == "--backup") {
      parser.setBackup(true);

    } else if (QString(argv[i]) == "--help" || QString(argv[i]) == "-h") {
      help("Help", argv[0]);
      return 0;

    } else {
      help("Unknown command", argv[0]);
      return 1;
    }
  }

  parser.parse(path);
  parser.organizeItems();
  parser.print();

  return 0;
}
