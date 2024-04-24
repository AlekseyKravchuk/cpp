 
// trim from beginning of string
string& trim_leading(string& s) {
    s.erase(0, s.find_first_not_of(' '));
    return s;
}

// trim from end of string
string& trim_trailing(string& s) {
    s.erase(s.find_last_not_of(' ') + 1);
    return s;
}

// trim from both ends of string
string& trim(string& s) {
    trim_leading(s);
    trim_trailing(s);
    return s;
}

// ============================
string_view trim_view(string_view s) {
    s.remove_prefix(std::min(s.find_first_not_of(' '), s.size()));

    size_t last_not_space_pos = s.find_last_not_of(' ');
    s.remove_suffix(std::min(s.size() - last_not_space_pos - 1, s.size()));
    return s;
}
