#include "json.h"
#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <numeric> // Для std::accumulate

using namespace std;

struct Spending {
    string category;
    int amount;
};

// Оператор порівняння для Spending
bool operator==(const Spending& lhs, const Spending& rhs) {
    return lhs.category == rhs.category && lhs.amount == rhs.amount;
}

// Оператор виводу для Spending
ostream& operator<<(ostream& os, const Spending& s) {
    return os << "Spending(category: " << s.category << ", amount: " << s.amount << ")";
}

// Функція для обчислення загальних витрат
int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(spendings.begin(), spendings.end(), 0, [](int sum, const Spending& s) {
        return sum + s.amount;
        });
}

// Функція для визначення найбільш витратної категорії
string MostExpensiveCategory(const vector<Spending>& spendings) {
    if (spendings.empty()) {
        return "";
    }
    const Spending* max_spending = &*max_element(spendings.begin(), spendings.end(), [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
        });
    return max_spending->category;
}

// Функція завантаження витрат із JSON
vector<Spending> LoadFromJson(istream& input) {
    Document doc = Load(input); // Завантажити документ за допомогою функції Load із json.h
    const vector<Node>& root = doc.GetRoot().AsArray();

    vector<Spending> result;
    for (const Node& item : root) {
        const map<string, Node>& spending_map = item.AsMap();
        result.push_back({
            spending_map.at("category").AsString(),
            spending_map.at("amount").AsInt()
            });
    }
    return result;
}

// Тест функції LoadFromJson
void TestLoadFromJson() {
    istringstream json_input(R"([
        {"amount": 2500, "category": "food"},
        {"amount": 1150, "category": "transport"},
        {"amount": 5780, "category": "restaurants"},
        {"amount": 7500, "category": "clothes"},
        {"amount": 23740, "category": "travel"},
        {"amount": 12000, "category": "sport"}
    ])");
    const vector<Spending> spendings = LoadFromJson(json_input);
    const vector<Spending> expected = {
        {"food", 2500},
        {"transport", 1150},
        {"restaurants", 5780},
        {"clothes", 7500},
        {"travel", 23740},
        {"sport", 12000}
    };
    ASSERT_EQUAL(spendings, expected);
}

// Тест бібліотеки JSON
void TestJsonLibrary() {
    istringstream json_input(R"([
        {"amount": 2500, "category": "food"},
        {"amount": 1150, "category": "transport"},
        {"amount": 12000, "category": "sport"}
    ])");
    Document doc = Load(json_input);
    const vector<Node>& root = doc.GetRoot().AsArray();
    ASSERT_EQUAL(root.size(), 3u);
    const map<string, Node>& food = root.front().AsMap();
    ASSERT_EQUAL(food.at("category").AsString(), "food");
    ASSERT_EQUAL(food.at("amount").AsInt(), 2500);
    const map<string, Node>& sport = root.back().AsMap();
    ASSERT_EQUAL(sport.at("category").AsString(), "sport");
    ASSERT_EQUAL(sport.at("amount").AsInt(), 12000);
    Node transport(map<string, Node>{{"category", Node("transport")}, { "amount", Node(1150) }});
    Node array_node(vector<Node>{transport});
    ASSERT_EQUAL(array_node.AsArray().size(), 1u);
}

// Точка входу
int main() {
    TestRunner tr;
    RUN_TEST(tr, TestJsonLibrary);
    RUN_TEST(tr, TestLoadFromJson);
}

