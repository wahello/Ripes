#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QList>
#include <QPlainTextEdit>

// Extended version of Qt's CodeEditor example
// http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html

class LineNumberArea;
class BreakpointArea;

class CodeEditor : public QPlainTextEdit {
  Q_OBJECT
public:
  CodeEditor(QWidget *parent = 0);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  void breakpointAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void updateSidebarWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateSidebar(const QRect &, int);

private:
  LineNumberArea *m_lineNumberArea;
  BreakpointArea *m_breakpointArea;

  QList<int> m_breakpoints;
};

// base class for side area widgets that are attached to the code editor
class SideArea : public QWidget {
  Q_OBJECT
public:
  SideArea(QWidget *parent = nullptr);

private:
};

class LineNumberArea : public QWidget {
public:
  LineNumberArea(CodeEditor *editor) : QWidget(editor) { codeEditor = editor; }

  QSize sizeHint() const override {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) override {
    codeEditor->lineNumberAreaPaintEvent(event);
  }

private:
  CodeEditor *codeEditor;
};

class BreakpointArea : public QWidget {
public:
  BreakpointArea(CodeEditor *editor) : QWidget(editor) { codeEditor = editor; }

  QSize sizeHint() const override { return QSize(width(), 0); }
  int width() const { return imageWidth + padding * 2; }
  QSize breakpointSize() { return QSize(imageWidth, imageHeight); }

  int imageWidth = 16;
  int imageHeight = 16;
  int padding = 3; // padding on each side of the breakpoint
  QPixmap m_breakpoint =
      QPixmap(":/logos/breakpoint_enabled.png").scaled(imageWidth, imageHeight);
  QPixmap m_breakpoint_disabled =
      QPixmap(":/logos/breakpoint_disabled.png").scaled(imageWidth, imageWidth);

protected:
  void paintEvent(QPaintEvent *event) override {
    codeEditor->breakpointAreaPaintEvent(event);
  }

private:
  CodeEditor *codeEditor;

  void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CODEEDITOR_H