#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>
#include <vector>
#include <random>
#include <cassert>

using namespace std;

struct Address {
    string city, street;
    int building;

    bool operator==(const Address& other) const {
        return city == other.city && street == other.street && building == other.building;
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;

    bool operator==(const Person& other) const {
        return name == other.name &&
            height == other.height &&
            weight == other.weight &&
            address == other.address;
    }
};

struct AddressHasher {
    size_t operator()(const Address& address) const {
        const size_t coef = 514'229;
        const hash<int> int_hasher;
        const hash<string> string_hasher;

        return coef * coef * string_hasher(address.city) +
            coef * string_hasher(address.street) +
            int_hasher(address.building);
    }
};

struct PersonHasher {
    size_t operator()(const Person& person) const {
        const size_t coef = 39'916'801;
        const hash<int> int_hasher;
        const hash<double> double_hasher;
        const hash<string> string_hasher;
        const AddressHasher address_hasher;

        return coef * coef * coef * string_hasher(person.name) +
            coef * coef * int_hasher(person.height) +
            coef * double_hasher(person.weight) +
            address_hasher(person.address);
    }
};

// Тестування коректності
void TestSmoke() {
    unordered_set<Person, PersonHasher> people;
    Person person1{ "Alice", 160, 50.5, {"Kyiv", "Main", 1} };
    Person person2{ "Bob", 170, 60.2, {"Lviv", "Central", 2} };
    Person person3{ "Alice", 160, 50.5, {"Kyiv", "Main", 1} };

    people.insert(person1);
    people.insert(person2);

    assert(people.size() == 2);
    assert(people.count(person3) == 1);

    cout << "TestSmoke OK\n";
}

// Тест на чистоту (purity)
void TestPurity() {
    Person person{ "Alice", 160, 50.5, {"Kyiv", "Main", 1} };
    PersonHasher hasher;

    size_t hash1 = hasher(person);
    size_t hash2 = hasher(person);

    assert(hash1 == hash2);
    cout << "TestPurity OK\n";
}

// Тест розподілу хешів
void TestDistribution() {
    mt19937 gen;
    uniform_int_distribution<int> height_dist(150, 200);
    uniform_real_distribution<double> weight_dist(50.0, 100.0);
    vector<string> cities = { "Kyiv", "Lviv", "Odesa", "Kharkiv", "Dnipro" };
    vector<string> streets = { "Main", "Central", "Green", "Blue", "Yellow" };
    vector<string> names = { "Alice", "Bob", "Charlie", "David", "Eve" };

    unordered_set<size_t> hashes;
    PersonHasher hasher;

    for (int i = 0; i < 1'000; ++i) {
        Person person{
            names[gen() % names.size()],
            height_dist(gen),
            weight_dist(gen),
            {
                cities[gen() % cities.size()],
                streets[gen() % streets.size()],
                static_cast<int>(gen() % 100) // Явне приведення до int
            }
        };

        hashes.insert(hasher(person));
    }

    double load_factor = static_cast<double>(hashes.size()) / 1'000;
    assert(load_factor > 0.7); // Переконатися, що колізій мало
    cout << "TestDistribution OK\n";
}

int main() {
    TestSmoke();
    TestPurity();
    TestDistribution();
    return 0;
}

