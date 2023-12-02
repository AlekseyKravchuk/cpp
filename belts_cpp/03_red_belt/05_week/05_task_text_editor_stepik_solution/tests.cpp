#include "editor.h"
#include "test_runner.h"

void TestFunctionality() {
    Editor editor;
    const std::string text = "hello, world"s;

    for (char c : text) {
        editor.Insert(c);
    }

    // Текущее состояние редактора: `hello, world|`
    // буфер пуст
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

        // Текущее состояние редактора: `hello, world|`
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }

        // Текущее состояние редактора: `|hello, world`
        editor.Cut(first_part_len);

        // Текущее состояние редактора: `|world`
        // в буфере обмена находится текст `hello, `
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const std::string text = "esreveR";
    for (char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
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

    ASSERT_EQUAL(editor.GetText(), "example");
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
}