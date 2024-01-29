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

// ======================= Used Structures =======================
struct HttpRequest {
    string method;
    string path;
    string body;
    map<string, string> get_params;
};

struct LastCommentInfo {
    size_t user_id;
    size_t consecutive_count;
};

struct HttpHeader {
    string name;
    string value;
};

struct ParsedResponse {
    int code;
    vector<HttpHeader> headers;
    string content;
};

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302
};
// ================================================================

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

class HttpResponse {
  public:
    explicit HttpResponse(HttpCode code);

    HttpResponse& AddHeader(std::string header_name, std::string value);
    HttpResponse& SetContent(std::string content);
    HttpResponse& SetCode(HttpCode code);

    HttpCode GetCode() const;
    const std::vector<HttpHeader>& GetHeaders() const;
    const std::string& GetContent() const;

    friend std::ostream& operator<<(std::ostream& output, const HttpResponse& resp);

  private:
    HttpCode _code;
    std::vector<HttpHeader> _headers;
    size_t _content_length;
    std::string _content;
};

class CommentServer {
  public:
    inline static const string GET_CAPTCHA_BODY =
        "What's the answer for The Ultimate Question of Life, the Universe, and Everything?";

    // метод ServeRequest принимает HTTP-запрос "req", обрабатывает его и записывает HTTP-ответ в выходной поток "os"
    // void ServeRequest(const HttpRequest& req, ostream& os);

    HttpResponse ServeRequest(const HttpRequest& req);

  private:
    vector<vector<string>> _comments; // index of external vector is used as "user_id"
    std::optional<LastCommentInfo> _last_comment;
    unordered_set<size_t> _banned_users;
};

// void CommentServer::ServeRequest(const HttpRequest& req, ostream& os) {
//     if (req.method == "POST") {
//         if (req.path == "/add_user") {
//             _comments.emplace_back();
//             string response = to_string(_comments.size() - 1);
//             os << "HTTP/1.1 200 OK\n"
//                << "Content-Length: " << response.size() << "\n"
//                << "\n"
//                << response; // тело ответа
//         } else if (req.path == "/add_comment") {
//             auto [user_id, comment] = ParseIdAndContent(req.body);

//             if (!_last_comment.has_value() || _last_comment->user_id != user_id) {
//                 _last_comment = LastCommentInfo{user_id, 1};
//             } else if (++_last_comment->consecutive_count > 3) {
//                 _banned_users.insert(user_id);
//             }

//             if (_banned_users.count(user_id) == 0) {
//                 _comments[user_id].push_back(string(comment));
//                 os << "HTTP/1.1 200 OK\n\n";
//             } else {
//                 os << "HTTP/1.1 302 Found\n"
//                    << "Location: /captcha\n"
//                    << "\n";
//             }
//         } else if (req.path == "/checkcaptcha") {
//             if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
//                 _banned_users.erase(id);
//                 if (_last_comment && _last_comment->user_id == id) {
//                     _last_comment.reset();
//                 }
//                 os << "HTTP/1.1 200 OK\n\n";
//             } else {
//                 os << "HTTP/1.1 302 Found\n"
//                    << "Location: /captcha\n"
//                    << "\n";
//             }
//         } else {
//             os << "HTTP/1.1 404 Not found\n\n";
//         }
//     } else if (req.method == "GET") {
//         if (req.path == "/user_comments") {
//             size_t user_id = FromString<size_t>(req.get_params.at("user_id"));

//             string response;
//             for (const string& comment : _comments[user_id]) {
//                 response += comment + '\n';
//             }

//             os << "HTTP/1.1 200 OK\n"
//                << "Content-Length: " << response.size() << "\n"
//                << "\n"
//                << response;
//         } else if (req.path == "/captcha") {
//             os << "HTTP/1.1 200 OK\n"
//                << "Content-Length: " << GET_CAPTCHA_BODY.size() << "\n"
//                << "\n"
//                << GET_CAPTCHA_BODY;
//         } else {
//             os << "HTTP/1.1 404 Not found\n\n";
//         }
//     }
// }

// ==> TODO:
HttpResponse CommentServer::ServeRequest(const HttpRequest& req) {
    if (req.method == "POST") {
        if (req.path == "/add_user") {
            _comments.emplace_back();
            string response = to_string(_comments.size() - 1);
            os << "HTTP/1.1 200 OK\n"
               << "Content-Length: " << response.size() << "\n"
               << "\n"
               << response; // тело ответа
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

ostream& operator<<(ostream& output, const HttpHeader& h) {
    return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs) {
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

istream& operator>>(istream& input, ParsedResponse& r) {
    string line;
    getline(input, line);

    {
        istringstream code_input(line);
        string dummy;
        code_input >> dummy >> r.code;
    }

    size_t content_length = 0;

    r.headers.clear(); // ??? непонятно, зачем здесь очищать заголовки
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

// ======================= Implementation of HttpResponse class =======================
HttpResponse::HttpResponse(HttpCode code)
    : _code(code) {}

HttpResponse& HttpResponse::AddHeader(string header_name, string value) {
    _headers.emplace_back(header_name, value);
    return *this;
}

HttpResponse& HttpResponse::SetContent(string content) {
    _content_length = content.size();
    _content = move(content);
    return *this;
}

HttpResponse& HttpResponse::SetCode(HttpCode code) {
    _code = code;
    return *this;
}

HttpCode HttpResponse::GetCode() const {
    return _code;
}

const vector<HttpHeader>& HttpResponse::GetHeaders() const {
    return _headers;
}

const string& HttpResponse::GetContent() const {
    return _content;
}

ostream& operator<<(ostream& os, const HttpCode& code) {
    switch (code) {
    case HttpCode::Ok:
        os << "Ok";
        break;
    case HttpCode::NotFound:
        os << "NotFound";
        break;
    case HttpCode::Found:
        os << "Found";
        break;
    default:
        os.setstate(ios_base::failbit);
    }

    return os;
}

ostream& operator<<(ostream& os, const HttpResponse& resp) {
    static const string http_version = "HTTP/1.1";
    static const string content_length_header = "Content-Length";

    os << http_version << ' ' << static_cast<size_t>(resp._code) << ' '
       << resp._code << '\n';

    for (const auto& header : resp._headers) {
        if (header.name != content_length_header) {
            os << header.name << ": " << header.value << '\n';
        }
    }

    if (resp.GetContent().size() > 0) {
        os << content_length_header << ": " << resp._content_length << '\n';
    }

    os << "\n"
       << resp._content;

    return os;
}
// ==================== End of HttpResponse class Implementation ======================

void Test(CommentServer& srv,
          const HttpRequest& request,
          const ParsedResponse& expected) {
    stringstream ss;
    srv.ServeRequest(request, ss);

    // ============================ Debugging ============================
    // As std::stringstream does not provide a copy constructor, we have to build it from the std::string ss outputs.
    stringstream ss_copy(ss.str());

    string debug_line;
    while (getline(ss_copy, debug_line) /* && !debug_line.empty() */) {
        std::cerr << debug_line << std::endl;
    }
    std::cerr << "===================================================" << std::endl;
    // ====================================================================

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
