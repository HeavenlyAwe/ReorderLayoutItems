#include "domparser.h"
#include <QString>

#include <iostream>

void help(QString message, QString arg) {
  std::cerr << "  " << message.toStdString() << std::endl;
  std::cerr << "  " << arg.toStdString() << " "
            << "<ui file> [--verbose] [--backup] [--help]" << std::endl;
}

int main(int argc, char *argv[]) {
  DomParser parser;

  QString path = "";
  if (argc < 2) {
    help("Please give the UI file to reorder.", argv[0]);
    return 1;
  }

  // Reading the first argument to the path variable. This can still be the help
  // command.
  path = QString(argv[1]);

  // If the help command was
  if (path == "--help" || path == "-h") {
    help("Help", argv[0]);
    return 0;
  }

  for (int i = 2; i < argc; ++i) {
    if (QString(argv[i]) == "--verbose") {
      parser.setVerbose(true);

    } else if (QString(argv[i]) == "--backup") {
      parser.setBackup(true);

    } else if (path == "--help" || path == "-h") {
      help("Help", argv[0]);
      return 0;

    } else {
      help("Unknown command", argv[0]);
      return 1;
    }
  }

  if (parser.parse(path)) {
    parser.organizeItems();
    parser.print();
  } else {
      help("The given file doesn't exist", argv[0]);
  }

  return 0;
}
