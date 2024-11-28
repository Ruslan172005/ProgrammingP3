#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <unordered_map>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = size_t;

    // Добавити об'єкт з нульовим пріоритетом
    Id Add(T object) {
        Id id = next_id++;
        items.emplace_back(move(object), 0, id);
        id_to_index[id] = items.size() - 1;
        return id;
    }

    // Добавити всі елементи діапазону [range_begin, range_end)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (auto it = range_begin; it != range_end; ++it) {
            *ids_begin++ = Add(move(*it));
        }
    }

    // Визначити, чи належить ідентифікатор якому-небудь об'єкту
    bool IsValid(Id id) const {
        return id_to_index.count(id) > 0;
    }

    // Отримати об'єкт по ідентифікатору
    const T& Get(Id id) const {
        return items[id_to_index.at(id)].object;
    }

    // Збільшити пріоритет об'єкта на 1
    void Promote(Id id) {
        auto& item = items[id_to_index.at(id)];
        item.priority++;
    }

    // Отримати об'єкт з максимальним пріоритетом
    pair<const T&, int> GetMax() const {
        return *max_element(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return tie(a.priority, a.id) < tie(b.priority, b.id);
            });
    }

    // Аналогічно GetMax, однок вилучає елемент з контейнера
    pair<T, int> PopMax() {
        auto it = max_element(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return tie(a.priority, a.id) < tie(b.priority, b.id);
            });
        pair<T, int> result = { move(it->object), it->priority };
        id_to_index.erase(it->id);
        items.erase(it);
        return result;
    }

private:
    struct Item {
        T object;
        int priority;
        Id id;

        Item(T obj, int prio, Id identifier) : object(move(obj)), priority(prio), id(identifier) {}
    };

    vector<Item> items;
    unordered_map<Id, size_t> id_to_index;
    Id next_id = 0;
};

class StringNonCopyable : public string {
public:
    using string::string;
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");
    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
