#pragma once
#include <istream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <stdexcept>  // Для обробки винятків

using namespace std;

class Node {
public:
    // Конструктори для різних типів даних
    explicit Node(vector<Node> array);
    explicit Node(map<string, Node> map);
    explicit Node(int value);
    explicit Node(string value);

    // Функції для отримання значень
    const vector<Node>& AsArray() const;
    const map<string, Node>& AsMap() const;
    int AsInt() const;
    const string& AsString() const;

private:
    vector<Node> as_array;
    map<string, Node> as_map;
    int as_int;
    string as_string;
};

class Document {
public:
    explicit Document(Node root);
    const Node& GetRoot() const;

private:
    Node root;
};

// Функція для завантаження JSON даних
Document Load(istream& input);

// Оголошення для завантаження даних з JSON
Node LoadNode(istream& input);
Node LoadArray(istream& input);
Node LoadDict(istream& input);
Node LoadString(istream& input);
Node LoadInt(istream& input);
