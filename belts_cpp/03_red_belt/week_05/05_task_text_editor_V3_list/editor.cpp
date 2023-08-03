#include "editor.h"

void Editor::Left() {
    if (_pos != _text.begin()) {
        --_pos;
        --_pos;
    }
}

void Editor::Right() {
    if (_pos != _text.end()) {
        ++_pos;
        ++_pos;
    }
}

void Editor::Insert(char token) {
    _pos = _text.insert(_pos, token);
    ++_pos;
    ++_pos;
}

void Editor::Cut(size_t tokens) {
    _cliboard.clear();  // сбрасываем буфер обмена

    if (tokens > (_text.size() - _pos)) {
        _cliboard.splice(std::begin(_cliboard), _text, _pos, std::end(_text));
    } else {
        _cliboard.splice(std::begin(_cliboard), _text, _pos, std::next(_pos, tokens));
    }
}

void Editor::Copy(size_t number_of_tokens) {
    _cliboard.clear();  // сбрасываем буфер обмена

    if (number_of_tokens > (_text.size() - _pos)) {
        std::copy(_pos, _text.end(), std::begin(_cliboard));
    } else {
        std::copy(_pos, std::next(_pos, number_of_tokens), std::begin(_cliboard));
    }
}

// вставить содержимое буфера в текущую позицию курсора
void Editor::Paste() {
    // Вставка пустого фрагмента не имеет эффекта
    if (_cliboard.empty()) {
        return;
    }

    size_t len = _cliboard.size();
    _text.splice(_pos, _cliboard, std::begin(_cliboard), std::end(_cliboard));
    std::advance(_pos, len);
    _pos+= len;
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