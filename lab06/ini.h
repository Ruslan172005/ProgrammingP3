#pragma once
#include <string>
#include <unordered_map>
#include <istream>
#include <stdexcept>

namespace Ini {

    // Тип для зберігання секцій і ключів
    using Section = std::unordered_map<std::string, std::string>;

    class Document {
    public:
        // Додає нову секцію або повертає існуючу
        Section& AddSection(std::string name);

        // Повертає секцію за її ім'ям (константний метод)
        const Section& GetSection(const std::string& name) const;

        // Повертає кількість секцій у документі
        size_t SectionCount() const;

    private:
        std::unordered_map<std::string, Section> sections;
    };

    // Завантажує INI-документ із вхідного потоку
    Document Load(std::istream& input);

} // namespace Ini

