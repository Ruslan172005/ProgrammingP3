#include "ini.h"
#include "test_runner.h"
#include <sstream>

using namespace Ini;

void TestLoadIni() {
    std::istringstream input(R"(
[section1]
key1=value1
key2=value2

[section2]
key3=value3
key4=value4
)");

    Document doc = Load(input);

    ASSERT_EQUAL(doc.SectionCount(), 2u);

    const Section& section1 = doc.GetSection("section1");
    ASSERT_EQUAL(section1.at("key1"), "value1");
    ASSERT_EQUAL(section1.at("key2"), "value2");

    const Section& section2 = doc.GetSection("section2");
    ASSERT_EQUAL(section2.at("key3"), "value3");
    ASSERT_EQUAL(section2.at("key4"), "value4");
}

void TestDocument() {
    Document doc;
    Section& section = doc.AddSection("section");
    section["key"] = "value";

    ASSERT_EQUAL(doc.SectionCount(), 1u);
    ASSERT_EQUAL(doc.GetSection("section").at("key"), "value");
}

void TestUnknownSection() {
    Document doc;
    doc.AddSection("known_section");

    try {
        doc.GetSection("unknown_section");
        Assert(false, "Expected out_of_range exception");
    }
    catch (const std::out_of_range&) {
        // Очікувана помилка
    }
    catch (...) {
        Assert(false, "Unexpected exception type");
    }
}

void TestDuplicateSections() {
    std::istringstream input(R"(
[section]
key1=value1

[section]
key2=value2
)");

    Document doc = Load(input);

    ASSERT_EQUAL(doc.SectionCount(), 1u);
    const Section& section = doc.GetSection("section");
    ASSERT_EQUAL(section.at("key1"), "value1");
    ASSERT_EQUAL(section.at("key2"), "value2");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLoadIni);
    RUN_TEST(tr, TestDocument);
    RUN_TEST(tr, TestUnknownSection);
    RUN_TEST(tr, TestDuplicateSections);
    return 0;
}

