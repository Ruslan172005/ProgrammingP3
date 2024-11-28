#include "ini.h"
#include <sstream>
#include <algorithm>

namespace Ini {

    // Реалізація методу AddSection
    Section& Document::AddSection(std::string name) {
        return sections[std::move(name)];
    }

    // Реалізація методу GetSection
    const Section& Document::GetSection(const std::string& name) const {
        auto it = sections.find(name);
        if (it == sections.end()) {
            throw std::out_of_range("Section not found: " + name);
        }
        return it->second;
    }

    // Реалізація методу SectionCount
    size_t Document::SectionCount() const {
        return sections.size();
    }

    // Реалізація функції Load
    Document Load(std::istream& input) {
        Document doc;
        Section* current_section = nullptr;

        std::string line;
        while (std::getline(input, line)) {
            
            if (line.empty()) {
                continue;
            }

            
            if (line[0] == '[' && line.back() == ']') {
                std::string section_name = line.substr(1, line.size() - 2);
                current_section = &doc.AddSection(section_name);
            }
        
            else if (current_section) {
                auto equal_pos = line.find('=');
                if (equal_pos == std::string::npos) {
                    throw std::runtime_error("Invalid INI format: missing '=' in line '" + line + "'");
                }
                std::string key = line.substr(0, equal_pos);
                std::string value = line.substr(equal_pos + 1);
                current_section->emplace(std::move(key), std::move(value));
            }
            else {
                throw std::runtime_error("Key-value pair found outside of a section: '" + line + "'");
            }
        }

        return doc;
    }

} // namespace Ini

