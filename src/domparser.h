#ifndef DOMPARSER_H
#define DOMPARSER_H

class QString;
class QDomDocument;
class QDomNode;

class DomParser {
public:
  DomParser();

  void parse(QString path);
  void organizeItems();
  void save(QString path);
  void print();

  void setVerbose(bool verbose);
  bool isVerbose();

  void setBackup(bool backup);
  bool isBackup();

private:
  QDomDocument *m_document;
  bool m_verbose = false;
  bool m_backup = false;

  void backupOldFile();
  void searchNodes(QDomNode &parent);
  void updateItemOrder(QDomNode &layoutNode);

  static bool compare(const QDomNode &n1, const QDomNode &n2);
};

#endif // DOMPARSER_H
