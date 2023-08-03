#include "editor.h"

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