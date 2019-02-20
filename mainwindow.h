#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemModel>
#include <QMainWindow>
#include <QString>

class QDomDocument;
class QDomNode;
class QDomElement;

class DomModel;
class QMenu;
class QTreeView;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

public slots:
  void openFile();
  void organizeLayout();

  void searchNodes(QDomNode &parent);
  void updateItemOrder(QDomNode &layoutNode);

  static bool compare(const QDomNode &n1, const QDomNode &n2);

private:
  DomModel *model;
  DomModel *model_output;

  QMenu *fileMenu;
  QMenu *toolMenu;
  QString xmlPath;

  QTreeView *view;
  QTreeView *view_output;

  QDomDocument *document;
};

#endif // MAINWINDOW_H
