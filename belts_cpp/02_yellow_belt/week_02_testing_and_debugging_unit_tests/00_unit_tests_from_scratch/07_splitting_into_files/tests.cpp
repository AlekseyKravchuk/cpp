#include "tests.h"
#include "test_runner.h"
#include "synonyms.h"

// =============================== TESTS ===============================
void TestAddSynonyms() {
    {
        Synonyms empty;
        AddSynonyms(empty, "a", "b");

        const Synonyms expected = {
            {"a", {"b"}},
            {"b", {"a"}}};

        ASSERT_EQUAL(empty, expected);
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
            {"c", {"a", "b"}}};

        ASSERT_EQUAL(synonyms, expected);
    }
}

void TestCount() {
    {
        Synonyms empty;
        ASSERT_EQUAL(GetSynonymsCount(empty, "a"), 0u);
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        ASSERT_EQUAL(GetSynonymsCount(synonyms, "a"), 2u);
        ASSERT_EQUAL(GetSynonymsCount(synonyms, "b"), 1u);
        ASSERT_EQUAL(GetSynonymsCount(synonyms, "z"), 0u);
    }
}

void TestAreSynonyms() {
    {
        Synonyms emtpy;

        ASSERT(!AreSynonyms(emtpy, "a", "b"));
        ASSERT(!AreSynonyms(emtpy, "b", "a"));
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        ASSERT(AreSynonyms(synonyms, "a", "b"));
        ASSERT(AreSynonyms(synonyms, "b", "a"));
        ASSERT(AreSynonyms(synonyms, "a", "c"));
        ASSERT(AreSynonyms(synonyms, "c", "a"));
        ASSERT(!AreSynonyms(synonyms, "b", "c"));
        ASSERT(!AreSynonyms(synonyms, "c", "b"));
    }
}
// ============================ END of TESTS ============================