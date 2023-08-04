#include "editor.h"

Editor::Editor()
    : _it_pos(_text.end()) {}

void Editor::Left() {
    _it_pos = Advance(_it_pos, -1);
}

void Editor::Right() {
    _it_pos = Advance(_it_pos, 1);
}

void Editor::Insert(char token) {
    _text.insert(_it_pos, token);
}

void Editor::Cut(size_t num_of_tokens) {
    auto it_pos2 = Advance(_it_pos, num_of_tokens);
    _buffer.assign(_it_pos, it_pos2);
    _it_pos = _text.erase(_it_pos, it_pos2);
}

void Editor::Copy(size_t num_of_tokens) {
    auto it_pos2 = Advance(_it_pos, num_of_tokens);
    _buffer.assign(_it_pos, it_pos2);
}

// вставить содержимое буфера в текущую позицию курсора
void Editor::Paste() {
    _text.insert(_it_pos, _buffer.begin(), _buffer.end());
}

Editor::Iterator Editor::Advance(Iterator it, int steps) const {
    while (steps > 0 && it != _text.end()) {
        ++it;
        --steps;
    }

    while (steps < 0 && it != _text.begin()) {
        --it;
        ++steps;
    }
    
    return it;
}

// получить текущее содержимое текстового редактора
std::string Editor::GetText() const {
    return {_text.begin(), _text.end()};
}

void TypeText(Editor& editor, const std::string& text) {
    for (char c : text) {
        editor.Insert(c);
    }
}