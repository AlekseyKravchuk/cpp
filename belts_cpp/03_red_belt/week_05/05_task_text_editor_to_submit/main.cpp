#include <algorithm>
#include <deque>
#include <iterator>  // std::inserter, std::distance
#include <string>
#include <vector>

#include "test_runner.h"

using namespace std::literals;

class Editor {
   public:
    Editor() = default;            // конструктор по умолчанию
    void Left();                   // сдвинуть курсор влево
    void Right();                  // сдвинуть курсор вправо
    void Insert(char token);       // вставить символ token
    void Cut(size_t tokens = 1);   // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1);  // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Paste();                  // вставить содержимое буфера в текущую позицию курсора
    std::string GetText() const;   // получить текущее содержимое текстового редактора

   private:
    uint64_t _cursor_pos{};
    std::vector<char> _text;
    std::vector<char> _cliboard;
};

void Editor::Left() {
    if (_cursor_pos) {
        --_cursor_pos;
    }
}

void Editor::Right() {
    if (_cursor_pos != _text.size()) {
        ++_cursor_pos;
    }
}

void Editor::Insert(char token) {
    auto it = _text.begin();
    _text.insert(it + _cursor_pos, token);
    ++_cursor_pos;
}

void Editor::Cut(size_t tokens) {
    _cliboard.clear();                           // сбрасываем буфер обмена
    auto it_from = _text.begin() + _cursor_pos;  // получаем итератор на начало копируемого диапазона

    if (tokens > (_text.size() - _cursor_pos)) {
        std::copy(it_from, _text.end(), std::inserter(_cliboard, _cliboard.begin()));
        _text.erase(it_from, _text.end());
    } else {
        std::copy(it_from, it_from + tokens, std::inserter(_cliboard, _cliboard.begin()));
        _text.erase(it_from, it_from + tokens);
    }
}

void Editor::Copy(size_t tokens) {
    _cliboard.clear();                           // сбрасываем буфер обмена
    auto it_from = _text.begin() + _cursor_pos;  // получаем итератор на начало копируемого диапазона

    if (tokens > (_text.size() - _cursor_pos)) {
        std::copy(it_from, _text.end(), std::inserter(_cliboard, _cliboard.begin()));
    } else {
        std::copy(it_from, it_from + tokens, std::inserter(_cliboard, _cliboard.begin()));
    }
}

// вставить содержимое буфера в текущую позицию курсора
void Editor::Paste() {
    // Вставка пустого фрагмента не имеет эффекта
    if (_cliboard.empty()) {
        return;
    }

    _text.insert(_text.begin() + _cursor_pos, _cliboard.begin(), _cliboard.end());
    _cursor_pos += std::distance(_cliboard.begin(), _cliboard.end());
}

// получить текущее содержимое текстового редактора
std::string Editor::GetText() const {
    return std::string(_text.begin(), _text.end());
}

void TypeText(Editor& editor, const std::string& text) {
    for (char c : text) {
        editor.Insert(c);
    }
}

void TestFunctionality() {
    Editor editor;
    const std::string text = "hello, world"s;

    for (char c : text) {
        editor.Insert(c);
    }

    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }

    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);

    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }

    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');

    // Текущее состояние редактора: `world, |`
    editor.Paste();

    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();

    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа

    // Текущее состояние редактора: `world, hello|`
    std::cout << editor.GetText();
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world"s);
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello"s);
    }
    {
        Editor editor;

        TypeText(editor, "misprnit"s);
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint"s);
    }
}

void TestReverse() {
    Editor editor;

    const std::string text = "esreveR"s;
    for (char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse"s);
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), ""s);

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), ""s);
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example"s);
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example"s);
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
}

int main() {
    // TestFunctionality();
    TestAll();

    return 0;
}