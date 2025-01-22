#include "option.h"

void OptionsDescription::add_option(const std::string& name, const std::string& description) {
    _options.emplace_back(name, description);
}

OptionsDescription::Proxy OptionsDescription::add_options() {
    auto tmp = Proxy(*this);
    return tmp;
}

void OptionsDescription::print() const {
    for (const auto& opt : _options) {
        std::cout << opt._name << ": " << opt._description << std::endl;
    }
}

OptionsDescription::Proxy& OptionsDescription::Proxy::operator()(const std::string& name,
                                                                 const std::string& description) {
    _opt_description.add_option(name, description);
    return *this; // Возвращаем тот же proxy object
}
