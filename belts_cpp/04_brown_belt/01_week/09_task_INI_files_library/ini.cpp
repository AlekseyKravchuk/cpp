#include "ini.h"

namespace Ini {

std::pair<std::string_view, std::string_view> Split(std::string_view sview, char delim) {
    size_t pos = sview.find(delim);
    return {sview.substr(0, pos), sview.substr(pos + 1)};
}

Section& Document::AddSection(std::string name) {
    return _sections[name];
}

const Section& Document::GetSection(const std::string& name) const {
    return _sections.at(name);
}

size_t Document::SectionCount() const {
    return _sections.size();
}

// void FillSection(std::istream& input, Ini::Section* section_ptr) {
//     std::string line;

//     while (input && input.peek() != '[') {
//         if (std::getline(input, line) && !line.empty()) {
//             auto [var, value] = Split(line, '=');
//             section_ptr->insert({std::string(var), std::string(value)});
//         }
//     }
// }

// Document Load(std::istream& input) {
//     Document doc;

//     for (std::string line; std::getline(input, line);) {
//         if (!line.empty()) {
//             if (line[0] == '[') { // start of new section has been identified
//                 Ini::Section* section = &doc.AddSection(std::move(line.substr(1, line.size() - 2)));
//                 FillSection(input, section);
//             }
//         }
//     }

//     return doc; // return local var => NRVO (Named Return Value Optimization) should work
// }

Document Load(std::istream& input) {
    Document doc;
    Ini::Section* section = nullptr;

    for (std::string line; std::getline(input, line);) {
        if (!line.empty()) {
            if (line[0] == '[') { // start of new section has been identified
                section = &doc.AddSection(std::move(line.substr(1, line.size() - 2)));
            } else {
                size_t pos = line.find('=');
                section->insert({line.substr(0, pos), line.substr(pos + 1)});
            }
        }
    }

    return doc; // return local var => NRVO (Named Return Value Optimization) should work
}
} // namespace Ini