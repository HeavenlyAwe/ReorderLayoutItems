#include "mainwindow.h"

#include "domitem.h"
#include "dommodel.h"

#include "qalgorithms.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomNodeList>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QTreeView>

MainWindow::MainWindow() : QMainWindow(), model(0) {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(tr("&Open..."), this, &MainWindow::openFile,
                      QKeySequence::Open);
  fileMenu->addAction(tr("E&xit"), this, &QWidget::close, QKeySequence::Quit);

  toolMenu = menuBar()->addMenu(tr("Tools"));
  toolMenu->addAction(tr("Organize"), this, &MainWindow::organizeLayout);

  model = new DomModel(QDomDocument(), this);
  model_output = new DomModel(QDomDocument(), this);

  view = new QTreeView(this);
  view->setModel(model);

  view_output = new QTreeView(this);
  view_output->setModel(model_output);

  QWidget *container = new QWidget;
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(view);
  layout->addWidget(view_output);
  container->setLayout(layout);

  setCentralWidget(container);
  setWindowTitle(tr("Simple DOM Model"));
}

void MainWindow::openFile() {
  //  QString filePath = QFileDialog::getOpenFileName(
  //      this, tr("Open File"), xmlPath,
  //      tr("XML files (*.xml);;HTML files (*.html);;"
  //         "SVG files (*.svg);;User Interface files (*.ui)"));
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), xmlPath, tr("User Interface files (*.ui)"));

  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {

      document = new QDomDocument;
      if (document->setContent(&file)) {
        DomModel *newModel = new DomModel(*document, this);
        view->setModel(newModel);
        delete model;
        model = newModel;
        xmlPath = filePath;
      }

      file.close();
    }
  }

  view->expandAll();
  for (int i = 0; i < model->columnCount(); ++i) {
    view->resizeColumnToContents(i);
  }

  view_output->expandAll();
  for (int i = 0; i < model_output->columnCount(); ++i) {
    view_output->resizeColumnToContents(i);
  }
}

void MainWindow::organizeLayout() {
  qDebug() << "Organizing the Layout";

  QDomNode parentNode = document->documentElement();
  searchNodes(parentNode);

  // Save the edited document to a new file
  QString outputFilePath = xmlPath + "_new.ui";
  QFile outputFile(outputFilePath);
  if (outputFile.open(QIODevice::WriteOnly)) {
    outputFile.write(document->toByteArray());
    outputFile.close();
  }
}

void MainWindow::searchNodes(QDomNode &parent) {
  QDomNode child = parent.firstChild();
  while (!child.isNull()) {
    if (child.hasChildNodes()) {
      searchNodes(child);
    }
    if (child.nodeName() == "tabstops") {
      parent.removeChild(child);
    } else if (child.nodeName() == "layout") {
      updateItemOrder(child);
    }
    child = child.nextSibling();
  }
}

void MainWindow::updateItemOrder(QDomNode &layoutNode) {

  //    QDomNode child = layoutNode.firstChild();
  //    QDomNodeList itemNodes = child.toElement().elementsByTagName("item");
  //    qDebug() << "Node has " << itemNodes.size() << " items";

  QDomNode child = layoutNode.firstChild();
  QList<QDomNode> itemNodes;
  while (!child.isNull()) {
    if (child.nodeName() == "item") {
      itemNodes.append(child);
    }
    child = child.nextSibling();
  }

  qDebug() << itemNodes.size();

  foreach (QDomNode n, itemNodes) {
    int row1 = n.attributes().namedItem("row").nodeValue().toInt();
    int col1 = n.attributes().namedItem("column").nodeValue().toInt();
    qDebug() << "    " << row1 << col1 << ":" << n.nodeName();
  }

  std::sort(itemNodes.begin(), itemNodes.end(), compare);

  layoutNode.removeChild(itemNodes[0]);
  layoutNode.insertBefore(itemNodes[0], layoutNode.firstChild());
  QDomNode prevNode = itemNodes[0];
  for (int i = 1; i < itemNodes.size(); i++) {
    layoutNode.removeChild(itemNodes[i]);
    layoutNode.insertAfter(itemNodes[i], prevNode);
    prevNode = itemNodes[i];
  }
}

bool MainWindow::compare(const QDomNode &n1, const QDomNode &n2) {

  int row1 = n1.attributes().namedItem("row").nodeValue().toInt();
  int col1 = n1.attributes().namedItem("column").nodeValue().toInt();

  int row2 = n2.attributes().namedItem("row").nodeValue().toInt();
  int col2 = n2.attributes().namedItem("column").nodeValue().toInt();

  if (row1 == row2) {
    return col1 < col2;
  }
  return row1 < row2;
}
