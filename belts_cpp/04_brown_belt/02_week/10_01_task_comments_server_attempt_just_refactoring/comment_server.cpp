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
    std::string _content{};
};

class CommentServer {
  public:
    inline static const string GET_CAPTCHA_BODY =
        "What's the answer for The Ultimate Question of Life, the Universe, and Everything?";

    HttpResponse ServeRequest(const HttpRequest& req);

  private:
    const size_t SPAM_LIMIT = 3; // A user who posts three comments in a row is considered a spammer.

    vector<vector<string>> _comments; // index in external vector is served as "user id"
    std::optional<LastCommentInfo> _last_comment;
    unordered_set<size_t> _banned_users;
};

HttpResponse CommentServer::ServeRequest(const HttpRequest& req) {
    if (req.method == "POST") {
        if (req.path == "/add_user") {
            _comments.emplace_back();
            return HttpResponse(HttpCode::Ok).SetContent(to_string(_comments.size() - 1));
        } else if (req.path == "/add_comment") {
            auto [user_id, comment] = ParseIdAndContent(req.body);

            if (!_last_comment.has_value() || _last_comment->user_id != user_id) {
                _last_comment = LastCommentInfo{user_id, 1}; // reset comments counter for a given user_id
            } else if (++_last_comment->consecutive_count > SPAM_LIMIT) {
                _banned_users.insert(user_id);
            }

            if (!_banned_users.count(user_id)) {
                _comments[user_id].push_back(string(comment));
                return HttpResponse(HttpCode::Ok);
            } else {
                return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
            }
        } else if (req.path == "/checkcaptcha") {
            if (auto [id, response] = ParseIdAndContent(req.body); response == "42") {
                _banned_users.erase(id);

                if (_last_comment && _last_comment->user_id == id) {
                    _last_comment.reset();
                }
                return HttpResponse(HttpCode::Ok);
            } else {
                return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
            }
        } else {
            return HttpResponse(HttpCode::NotFound);
        }
    } else if (req.method == "GET") {
        if (req.path == "/user_comments") {
            size_t user_id = FromString<size_t>(req.get_params.at("user_id"));

            string content;
            for (const string& comment : _comments[user_id]) {
                content += comment + '\n';
            }

            return HttpResponse(HttpCode::Ok).SetContent(content);
        } else if (req.path == "/captcha") {
            return HttpResponse(HttpCode::Ok).SetContent(GET_CAPTCHA_BODY);
        } else {
            return HttpResponse(HttpCode::NotFound);
        }
    }
}

// ======================= Implementation of HttpResponse class =======================
HttpResponse::HttpResponse(HttpCode code)
    : _code(code) {}

HttpResponse& HttpResponse::AddHeader(string header_name, string value) {
    _headers.push_back(HttpHeader{header_name, value});
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
// ==================== End of HttpResponse class Implementation ======================

// ============================== Supplementary Functions =============================
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

ostream& operator<<(ostream& os, const HttpCode& code) {
    switch (code) {
    case HttpCode::Ok:
        os << "OK";
        break;
    case HttpCode::NotFound:
        os << "Not found";
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
// =========================== End of Supplementary Functions =========================

void Test(CommentServer& srv,
          const HttpRequest& request,
          const ParsedResponse& expected) {
    HttpResponse resp = srv.ServeRequest(request);

    ASSERT_EQUAL(static_cast<size_t>(resp.GetCode()), expected.code);
    ASSERT_EQUAL(resp.GetHeaders(), expected.headers);
    ASSERT_EQUAL(resp.GetContent(), expected.content);
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
    Test(cs, {"POST", "/checkcaptcha", "1 24", {}}, redirect_to_captcha); // <== fails here
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
