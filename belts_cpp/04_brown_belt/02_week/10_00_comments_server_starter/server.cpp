#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "test_runner.h"

using namespace std;

struct HttpRequest {
    string method;
    string path;
    string body;
    map<string, string> get_params;
};

pair<string, string> SplitBy(const string& what, const string& by) {
    size_t pos = what.find(by);
    if (by.size() < what.size() && pos < what.size() - by.size()) {
        return {what.substr(0, pos), what.substr(pos + by.size())};
    } else {
        return {what, {}};
    }
}

template <typename T>
T FromString(const string& s) {
    T x;
    istringstream is(s);
    is >> x;
    return x;
}

pair<size_t, string> ParseIdAndContent(const string& body) {
    auto [id_string, content] = SplitBy(body, " ");
    return {FromString<size_t>(id_string), content};
}

struct LastCommentInfo {
    size_t user_id;
    size_t consecutive_count;
};

class CommentServer {
   private:
    vector<vector<string>> _comments;
    std::optional<LastCommentInfo> _last_comment;
    unordered_set<size_t> _banned_users;

   public:
    inline static const string GET_CAPTCHA_BODY =
        "What's the answer for The Ultimate Question of Life, the Universe, and Everything?";

    // метод ServeRequest принимает HTTP-запрос, обрабатывает его и записывает HTTP-ответ в выходной поток
    void ServeRequest(const HttpRequest& req, ostream& os) {
        if (req.method == "POST") {
            if (req.path == "/add_user") {
                _comments.emplace_back();
                string response = to_string(_comments.size() - 1);
                os << "HTTP/1.1 200 OK\n"
                   << "Content-Length: " << response.size() << "\n"
                   << "\n"
                   << response;  // тело ответа
            } else if (req.path == "/add_comment") {
                auto [user_id, comment] = ParseIdAndContent(req.body);

                if (!_last_comment.has_value() || _last_comment->user_id != user_id) {
                    _last_comment = LastCommentInfo{user_id, 1};
                } else if (++_last_comment->consecutive_count > 3) {
                    _banned_users.insert(user_id);
                }

                if (_banned_users.count(user_id) == 0) {
                    _comments[user_id].push_back(string(comment));
                    os << "HTTP/1.1 200 OK\n\n";
                } else {
                    os << "HTTP/1.1 302 Found\n"
                       << "Location: /captcha\n"
                       << "\n";
                }
            } else if (req.path == "/checkcaptcha") {
                if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
                    _banned_users.erase(id);
                    if (_last_comment && _last_comment->user_id == id) {
                        _last_comment.reset();
                    }
                    os << "HTTP/1.1 200 OK\n\n";
                } else {
                    os << "HTTP/1.1 302 Found\n"
                       << "Location: /captcha\n"
                       << "\n";
                }
            } else {
                os << "HTTP/1.1 404 Not found\n\n";
            }
        } else if (req.method == "GET") {
            if (req.path == "/user_comments") {
                size_t user_id = FromString<size_t>(req.get_params.at("user_id"));

                string response;
                for (const string& comment : _comments[user_id]) {
                    response += comment + '\n';
                }

                os << "HTTP/1.1 200 OK\n"
                   << "Content-Length: " << response.size() << "\n"
                   << "\n"
                   << response;
            } else if (req.path == "/captcha") {
                os << "HTTP/1.1 200 OK\n"
                   << "Content-Length: " << GET_CAPTCHA_BODY.size() << "\n"
                   << "\n"
                   << GET_CAPTCHA_BODY;
            } else {
                os << "HTTP/1.1 404 Not found\n\n";
            }
        }
    }
};

struct HttpHeader {
    string name;
    string value;
};

ostream& operator<<(ostream& output, const HttpHeader& h) {
    return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs) {
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

struct ParsedResponse {
    int code;
    vector<HttpHeader> headers;
    string content;
};

istream& operator>>(istream& input, ParsedResponse& r) {
    string line;
    getline(input, line);

    {
        istringstream code_input(line);
        string dummy;
        code_input >> dummy >> r.code;
    }

    size_t content_length = 0;

    r.headers.clear();  // ??? непонятно, зачем здесь очищать заголовки
    while (getline(input, line) && !line.empty()) {
        if (auto [name, value] = SplitBy(line, ": "); name == "Content-Length") {
            istringstream length_input(value);
            length_input >> content_length;
        } else {
            r.headers.push_back({std::move(name), std::move(value)});
        }
    }

    r.content.resize(content_length);

    // извлечь из потока input "r.content.size()" символов и сохранить их в массиве, на который указывает ptr "r.content.data()"
    input.read(r.content.data(), r.content.size());
    return input;
}

void Test(CommentServer& srv,
          const HttpRequest& request,
          const ParsedResponse& expected) {
    stringstream ss;
    srv.ServeRequest(request, ss);

    // ====== Debugging ======
    // As std::stringstream does not provide a copy constructor, we have to build it from the std::string ss outputs.
    //
    stringstream ss_copy(ss.str());

    string debug_line;
    while (getline(ss_copy, debug_line) /* && !debug_line.empty() */) {
        std::cerr << debug_line << std::endl;
    }
    std::cerr << "===================================================" << std::endl;
    // =======================

    ParsedResponse resp;
    ss >> resp;
    ASSERT_EQUAL(resp.code, expected.code);
    ASSERT_EQUAL(resp.headers, expected.headers);
    ASSERT_EQUAL(resp.content, expected.content);
}

template <typename CommentServer>
void TestServer() {
    CommentServer cs;

    const ParsedResponse ok{200, {}, {}};
    const ParsedResponse redirect_to_captcha{302, {{"Location", "/captcha"}}, {}};
    const ParsedResponse not_found{404, {}, {}};

    Test(cs, {"POST", "/add_user", {}, {}}, {200, {}, "0"});                        
    Test(cs, {"POST", "/add_user", {}, {}}, {200, {}, "1"});                        
    Test(cs, {"POST", "/add_comment", "0 Hello", {}}, ok);                        
    Test(cs, {"POST", "/add_comment", "1 Hi", {}}, ok);                            
    Test(cs, {"POST", "/add_comment", "1 Buy my goods", {}}, ok);                   
    Test(cs, {"POST", "/add_comment", "1 Enlarge", {}}, ok);                        
    Test(cs, {"POST", "/add_comment", "1 Buy my goods", {}}, redirect_to_captcha); 
    Test(cs, {"POST", "/add_comment", "0 What are you selling?", {}}, ok);         
    Test(cs, {"POST", "/add_comment", "1 Buy my goods", {}}, redirect_to_captcha);  
    Test(                                                                          
        cs,
        {"GET", "/user_comments", "", {{"user_id", "0"}}},
        {200, {}, "Hello\nWhat are you selling?\n"});
    Test(                                                                          
        cs,
        {"GET", "/user_comments", "", {{"user_id", "1"}}},
        {200, {}, "Hi\nBuy my goods\nEnlarge\n"});
    Test(                                                                          
        cs,
        {"GET", "/captcha", {}, {}},
        {200, {}, {"What's the answer for The Ultimate Question of Life, the Universe, and Everything?"}});
    Test(cs, {"POST", "/checkcaptcha", "1 24", {}}, redirect_to_captcha);           
    Test(cs, {"POST", "/checkcaptcha", "1 42", {}}, ok);
    Test(cs, {"POST", "/add_comment", "1 Sorry! No spam any more", {}}, ok);
    Test(
        cs,
        {"GET", "/user_comments", "", {{"user_id", "1"}}},
        {200, {}, "Hi\nBuy my goods\nEnlarge\nSorry! No spam any more\n"});

    Test(cs, {"GET", "/user_commntes", {}, {}}, not_found);
    Test(cs, {"POST", "/add_uesr", {}, {}}, not_found);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestServer<CommentServer>);
}
