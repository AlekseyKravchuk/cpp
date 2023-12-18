#include "ini.h"

namespace Ini {

std::pair<std::string_view, std::string_view> Split(std::string_view sview, char delim) {
    size_t pos = sview.find(delim);
    return {sview.substr(0, pos), sview.substr(pos + 1)};
}

Section& Document::AddSection(std::string name) {
   _sections[name] = Section{};
   return _sections[name];
}

const Section& Document::GetSection(const std::string& name) const {
    return _sections.at(name);
}

size_t Document::SectionCount() const {
    return _sections.size();
}

Document Load(std::istream& input) {
    Document doc;

    for (std::string line; std::getline(input, line);) {
        if (line == "\n") {
            continue;
        }

        if (line[0] == '[') {
            Ini::Section* section = &doc.AddSection(std::move(line.substr(1, line.size() - 2)));

            for (std::string str; std::getline(input, str); ) {
                if (str == "") {
                    break;
                }

                auto [var, value] = Split(str, '=');
                section->insert({std::string(var), std::string(value)});
            }
        }
    }

    return doc;  // return local var => NRVO (Named Return Value Optimization) should work

}
} // namespace Ini