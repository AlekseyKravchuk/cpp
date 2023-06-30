#include "translator.h"

void Translator::Add(std::string_view source, std::string_view target) {
    std::string_view actual_source = *_sources.insert(std::string(source)).first;
    std::string_view actual_target = *_targets.insert(std::string(target)).first;

    _forward[actual_source].push_back(actual_target);
    _backward[actual_target].push_back(actual_source);
}

std::string_view Translator::TranslateForward(std::string_view source) const {
    if (!_sources.count(std::string(source))) {
        return {};
    } else {
        return _forward.at(source).back();
    }
}

std::string_view Translator::TranslateBackward(std::string_view target) const {
    if (!_targets.count(std::string(target))) {
        return {};
    } else {
        return _backward.at(target).back();
    }
}