#ifndef ESCHER_TEXT_AREA_H
#define ESCHER_TEXT_AREA_H

#include <assert.h>
#include <string.h>
#include <escher/scrollable_view.h>
#include <escher/text_cursor_view.h>
#include <escher/text_area_delegate.h>

class TextArea : public ScrollableView, public ScrollViewDataSource {
public:
  TextArea(Responder * parentResponder, char * textBuffer, size_t textBufferSize,
    TextAreaDelegate * delegate = nullptr, KDText::FontSize fontSize = KDText::FontSize::Large,
    KDColor textColor = KDColorBlack, KDColor backgroundColor = KDColorWhite);

  void setDelegate(TextAreaDelegate * delegate);
  bool handleEvent(Ion::Events::Event event) override;

private:
  class Text {
  public:
    Text(char * buffer, size_t bufferSize);

    class Line {
    public:
      Line(const char * text);
      const char * text() const { return m_text; }
      size_t length() const { return m_length; }
      bool contains(const char * c) const;
    private:
      const char * m_text;
      size_t m_length;
    };

    class LineIterator {
    public:
      LineIterator(const char * text) : m_line(text) {}
      Line operator*() { return m_line; }
      LineIterator& operator++();
      bool operator!=(const LineIterator& it) const { return m_line.text() != it.m_line.text(); }
    private:
      Line m_line;
    };

    class Position {
    public:
      Position(int column, int line) : m_column(column), m_line(line) {}
      int column() const { return m_column; }
      int line() const { return m_line; }
    private:
      int m_column;
      int m_line;
    };

    LineIterator begin() const { return LineIterator(m_buffer); };
    LineIterator end() const { return LineIterator(nullptr); };

    Position span() const;

    Position positionAtIndex(size_t index);
    size_t indexAtPosition(Position p);

    void insertChar(char c, size_t index);
    char removeChar(size_t index);
    char operator[](size_t index) {
      assert(index >= 0 && index < m_bufferSize);
      return m_buffer[index];
    }
    size_t bufferSize() {
      return m_bufferSize;
    }
    size_t textLength() {
      return strlen(m_buffer);
    }
  private:
    char * m_buffer;
    size_t m_bufferSize;
  };

  class ContentView : public View {
  public:
    ContentView(char * textBuffer, size_t textBufferSize, KDText::FontSize size,
      KDColor textColor, KDColor backgroundColor);
    void drawRect(KDContext * ctx, KDRect rect) const override;
    KDSize minimalSizeForOptimalDisplay() const override;
    void insertText(const char * text);
    void moveCursorIndex(int deltaX);
    void moveCursorGeo(int deltaX, int deltaY);
    void removeChar();
    KDRect cursorRect();
  private:
    int numberOfSubviews() const override;
    View * subviewAtIndex(int index) override;
    void layoutSubviews() override;
    KDRect characterFrameAtIndex(size_t index);
    KDRect dirtyRectFromCursorPosition(size_t index, bool lineBreak);
    TextCursorView m_cursorView;
    size_t m_cursorIndex;
    Text m_text;
    KDText::FontSize m_fontSize;
    KDColor m_textColor;
    KDColor m_backgroundColor;
  };

  ContentView m_contentView;
  TextAreaDelegate * m_delegate;
};

#endif
