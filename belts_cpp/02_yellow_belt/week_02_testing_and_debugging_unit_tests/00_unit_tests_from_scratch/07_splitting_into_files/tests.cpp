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

        AssertEqual(empty, expected, "add synonyms empty");
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

        AssertEqual(synonyms, expected, "add synonyms NOT empty");
    }
}

void TestCount() {
    {
        Synonyms empty;
        AssertEqual(GetSynonymsCount(empty, "a"), 0u, "count for empty");
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        AssertEqual(GetSynonymsCount(synonyms, "a"), 2u, "count for a");
        AssertEqual(GetSynonymsCount(synonyms, "b"), 1u, "count for b");
        AssertEqual(GetSynonymsCount(synonyms, "z"), 0u, "count for z");
    }
}

void TestAreSynonyms() {
    {
        Synonyms emtpy;

        Assert(!AreSynonyms(emtpy, "a", "b"), "are synonyms a, b");
        Assert(!AreSynonyms(emtpy, "b", "a"), "are synonyms b, a");
    }

    {
        Synonyms synonyms = {
            {"a", {"b", "c"}},
            {"b", {"a"}},
            {"c", {"a"}}};

        Assert(AreSynonyms(synonyms, "a", "b"), "are synonyms: a, b");
        Assert(AreSynonyms(synonyms, "b", "a"), "are synonyms: b, a");
        Assert(AreSynonyms(synonyms, "a", "c"), "are synonyms: a, c");
        Assert(AreSynonyms(synonyms, "c", "a"), "are synonyms: c, a");
        Assert(!AreSynonyms(synonyms, "b", "c"), "are synonyms: b, c");
        Assert(!AreSynonyms(synonyms, "c", "b"), "are synonyms: c, b");
    }
}



// ============================ END of TESTS ============================