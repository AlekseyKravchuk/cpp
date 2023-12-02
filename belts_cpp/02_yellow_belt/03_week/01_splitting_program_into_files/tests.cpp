#include "tests.h"

void TestAddSynonyms() {
    {
        Synonyms empty;
        AddSynonyms(empty, "a", "b");
        const Synonyms expected = {
            {"a", {"b"}},
            {"b", {"a"}}};
        AssertEqual(empty, expected, "empty == expected"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b"}},
            {"b", {"a", "c"}},
            {"c", {"b"}}};
        AddSynonyms(synonyms, "a", "c");
        Synonyms expected = {
            {"a", {"b", "c"}},
            {"b", {"a", "c"}},
            {"c", {"b", "a"}}};
        AssertEqual(expected, synonyms, "expected == synonyms"s);
    }
}

void TestGetSynonymsCount() {
    {
        Synonyms empty;
        AssertEqual(GetSynonymsCount(empty, "a"), 0u, "count for empty"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        AssertEqual(GetSynonymsCount(synonyms, "a"), 2u, "count for a"s);
        AssertEqual(GetSynonymsCount(synonyms, "b"), 1u, "count for b"s);
        AssertEqual(GetSynonymsCount(synonyms, "z"), 0u, "count for z"s);
    }
}

void TestAreSynonyms() {
    {
        Synonyms empty;
        Assert(!AreSynonyms(empty, "a", "b"), "empty a b"s);
        Assert(!AreSynonyms(empty, "b", "a"), "empty b a"s);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        Assert(AreSynonyms(synonyms, "a", "b"), "synonyms: a b"s);
        Assert(AreSynonyms(synonyms, "b", "a"), "synonyms: b a"s);
        Assert(AreSynonyms(synonyms, "a", "c"), "synonyms: a c"s);
        Assert(AreSynonyms(synonyms, "c", "a"), "synonyms: c a"s);
        Assert(!AreSynonyms(synonyms, "b", "c"), "synonyms: b c"s);
        Assert(!AreSynonyms(synonyms, "c", "b"), "synonyms: c b"s);
    }
}

// Хочется добиться следующего поведения от unit-тестов:
// 1) Если все тесты отработали успешно, то выполняется основная часть программы.
// 2) Если хотя бы один тест упал, все тесты отрабатывают, но основная часть уже не выполняется.
void TestAll() {
    // Unit-тесты выводят результаты в СТАНДАРТНЫЙ ПОТОК ОШИБОК (std::cerr).
    // Это позволяет нам не комментировать запуск тестов при отправке в тестирующую систему.
    TestRunner tr;
    tr.RunTest(TestAddSynonyms, "TestAddSynonyms"s);
    tr.RunTest(TestAreSynonyms, "TestAreSynonyms"s);
    tr.RunTest(TestGetSynonymsCount, "TestGetSynonymsCount"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner" - для этого мы и используем функцию TestAll