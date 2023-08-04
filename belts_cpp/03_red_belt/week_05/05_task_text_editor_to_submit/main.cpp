#include <algorithm>
#include <iterator>  // std::inserter, std::distance
#include <string>
#include <list>

#include "test_runner.h"

using namespace std::literals;

class Editor {
   public:
    Editor() = default;                      // конструктор по умолчанию
    void Left();                             // сдвинуть курсор влево
    void Right();                            // сдвинуть курсор вправо
    void Insert(char token);                 // вставить символ token
    void Cut(size_t number_of_tokens = 1);   // вырезать не более number_of_tokens символов, начиная с текущей позиции курсора
    void Copy(size_t number_of_tokens = 1);  // cкопировать не более number_of_tokens символов, начиная с текущей позиции курсора
    void Paste();                            // вставить содержимое буфера в текущую позицию курсора
    std::string GetText() const;             // получить текущее содержимое текстового редактора

   private:
    std::list<char> _text{};
    std::string _buf{};
    uint64_t _pos{};
    std::list<char>::iterator _it_pos{std::begin(_text)};
};

void Editor::Left() {
    if (_it_pos != _text.begin()) {
        --_it_pos;
        --_pos;
    }
}

void Editor::Right() {
    if (_it_pos != _text.end()) {
          ++_it_pos;
          ++_pos;
    }
}

void Editor::Insert(char token) {
    _it_pos = _text.insert(_it_pos, token);
    ++_it_pos;
    ++_pos;
}

void Editor::Cut(size_t num_of_tokens) {
    _buf.clear();  // сбрасываем буфер обмена

    std::list<char>::iterator it_to = (num_of_tokens > (_text.size() - _pos)) ? std::end(_text)
                                                                              : std::next(_it_pos, num_of_tokens);
    // _buf = {std::make_move_iterator(_it_pos), std::make_move_iterator(it_to)};
    _buf = std::string{_it_pos, it_to};
    _it_pos = _text.erase(_it_pos, it_to);
}

void Editor::Copy(size_t num_of_tokens) {
    _buf.clear();  // сбрасываем буфер обмена

    std::list<char>::iterator it_to = (num_of_tokens > (_text.size() - _pos)) ? std::end(_text)
                                                                              : std::next(_it_pos, num_of_tokens);
    _buf = {_it_pos, it_to};
}

// вставить содержимое буфера в текущую позицию курсора
void Editor::Paste() {
    if (!_buf.empty()) {  // Вставка пустого фрагмента не имеет эффекта
        _it_pos = _text.insert(_it_pos, _buf.begin(), _buf.end());
        std::advance(_it_pos, _buf.size());
        _pos += _buf.size();
    }
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