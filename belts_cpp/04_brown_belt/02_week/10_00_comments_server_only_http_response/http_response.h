#pragma once

#include <cstddef> // size_t
#include <string>
#include <vector>

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302
};

struct HttpHeader {
    std::string name;
    std::string value;
};

// Класс HttpResponse представляет собой HTTP-ответ
class HttpResponse {
  public:
    explicit HttpResponse(HttpCode code);

    HttpResponse& AddHeader(std::string name, std::string value);
    HttpResponse& SetContent(std::string a_content);
    HttpResponse& SetCode(HttpCode a_code);

    HttpCode GetCode() const;
    const std::vector<HttpHeader>& GetHeaders() const;
    const std::string& GetContent() const;

    // инкапсуляция формата вывода HTTP-ответа в поток через operator<<
    friend ostream& operator<<(ostream& output, const HttpResponse& resp);

  private:
    HttpCode _code;
    std::vector<HttpHeader> _headers;
    size_t _content_length;
    std::string _content;
};