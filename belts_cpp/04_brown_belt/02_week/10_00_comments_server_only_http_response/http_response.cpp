#include "http_response.h"

using namespace std;

explicit HttpResponse::HttpResponse(HttpCode code)
    : _code(code) {}

HttpResponse& HttpResponse::AddHeader(string name, string value) {
    _headers.emplace_back(name, value);
}

HttpResponse& HttpResponse::SetContent(string a_content) {
    _content_length = a_content.size();
    _content = move(a_content);
    return *this;
}

HttpResponse& HttpResponse::SetCode(HttpCode a_code) {
    _code = a_code;
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

friend ostream& operator<<(ostream& os, const HttpResponse& resp) {
    static const string version = "HTTP/1.1";
    static const string content_length_header = "Content-Length";

    os << version << ' '
       << static_cast<size_t>(resp.code) << ' '
       << resp.code << '\n';

    for (const auto& header : resp.headers) {
        if (header.name != content_length_header) {
            os << header.name << ": " << header.value << '\n';
        }
    }

    if (resp.GetContent().size() > 0) {
        os << content_length_header << ": " << resp.content_length << '\n';
    }

    os << '\n'
       << resp.content;

    return os;
}