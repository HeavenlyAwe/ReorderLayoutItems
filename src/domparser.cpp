#include "domparser.h"

#include <QDomDocument>
#include <QFile>
#include <QString>

#include <iostream>

DomParser::DomParser() {}

void DomParser::parse(QString path) {
  std::cerr << "Reading file :" << path.toStdString() << std::endl;

  if (!path.isEmpty()) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
      m_document = new QDomDocument;
      m_document->setContent(&file);
      file.close();
    }

    if (m_backup) {
      save(path + ".bkp");
    }
  }
}

void DomParser::save(QString path) {
  std::cerr << "Saving file: " << path.toStdString() << std::endl;

  if (!path.isEmpty()) {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
      file.write(m_document->toByteArray());
      file.close();
    }
  }

  std::cout << m_document->toString().toStdString() << std::endl;
}

void DomParser::organizeItems() {
  QDomNode parentNode = m_document->documentElement();
  searchNodes(parentNode);
}

/**
 * @brief DomParser::searchNodes
 * @param parent
 *
 * Depth-first search of the DOM tree.
 */
void DomParser::searchNodes(QDomNode &parent) {
  QDomNode child = parent.firstChild();
  while (!child.isNull()) {
    if (child.hasChildNodes()) {
      searchNodes(child);
    }
    // Remove the "tabstops" tag, it is not needed after the sort
    if (child.nodeName() == "tabstops") {
      if (m_verbose) {
        std::cerr << "REMOVING \"tabstops\" tag." << std::endl;
      }
      parent.removeChild(child);
    }
    // The "layout" tag has the items we want to organize.
    else if (child.nodeName() == "layout") {
      // TODO: Check if the node class has to be confirmed as well?
      updateItemOrder(child);
    }
    // Go on to the next tag
    child = child.nextSibling();
  }
}

void DomParser::updateItemOrder(QDomNode &layoutNode) {

  // Find all item nodes and put them into a list.
  QList<QDomNode> itemNodes;

  // Start the evaluation from the first child of the layout node.
  QDomNode child = layoutNode.firstChild();
  while (!child.isNull()) {
    if (child.nodeName() == "item") {
      itemNodes.append(child);
    }
    child = child.nextSibling();
  }

  std::cerr
      << "Found: " << itemNodes.size()
      << " item nodes, sorting them based on the ROW and COLUMN attributes."
      << std::endl;

  if (m_verbose) {
    std::cerr << "  Original order:" << std::endl;
    foreach (QDomNode n, itemNodes) {
      int row1 = n.attributes().namedItem("row").nodeValue().toInt();
      int col1 = n.attributes().namedItem("column").nodeValue().toInt();
      std::cerr << "    " << row1 << " " << col1 << " : "
                << n.nodeName().toStdString() << std::endl;
    }
  }

  std::sort(itemNodes.begin(), itemNodes.end(), compare);

  if (m_verbose) {
    std::cerr << "  Sorted order:" << std::endl;
    foreach (QDomNode n, itemNodes) {
      int row1 = n.attributes().namedItem("row").nodeValue().toInt();
      int col1 = n.attributes().namedItem("column").nodeValue().toInt();
      std::cerr << "    " << row1 << " " << col1 << " : "
                << n.nodeName().toStdString() << std::endl;
    }
  }

  for (int i = itemNodes.size() - 1; i >= 0; --i) {
    layoutNode.removeChild(itemNodes[i]);
    layoutNode.insertBefore(itemNodes[i], layoutNode.firstChild());
  }
}

/**
 * @brief DomParser::compare
 * @param n1
 * @param n2
 * @return
 *
 * Compare the ROW and COLUMN attributes of two nodes. This should only be used
 * on nodes with the tag "item".
 */
bool DomParser::compare(const QDomNode &n1, const QDomNode &n2) {

  int row1 = n1.attributes().namedItem("row").nodeValue().toInt();
  int col1 = n1.attributes().namedItem("column").nodeValue().toInt();

  int row2 = n2.attributes().namedItem("row").nodeValue().toInt();
  int col2 = n2.attributes().namedItem("column").nodeValue().toInt();

  if (row1 == row2) {
    return col1 < col2;
  }
  return row1 < row2;
}

void DomParser::setVerbose(bool verbose) { m_verbose = verbose; }

bool DomParser::isVerbose() { return m_verbose; }

void DomParser::setBackup(bool backup) { m_backup = backup; }

bool DomParser::isBackup() { return m_backup; }
