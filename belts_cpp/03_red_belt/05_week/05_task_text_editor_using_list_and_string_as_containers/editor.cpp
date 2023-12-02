#include "editor.h"

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