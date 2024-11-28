#include "http_response.h"
#include <stdexcept>

HttpResponse::HttpResponse(HttpCode code) : code(code) {}

HttpResponse& HttpResponse::AddHeader(string name, string value) {
    headers.push_back({name, value});
    return *this;
}

HttpResponse& HttpResponse::SetContent(string a_content) {
    content = std::move(a_content);
    return *this;
}

HttpResponse& HttpResponse::SetCode(HttpCode a_code) {
    code = a_code;
    return *this;
}

ostream& operator<<(ostream& output, HttpCode code) {
    switch (code) {
        case HttpCode::Ok:
            output << "200 OK";
            break;
        case HttpCode::Found:
            output << "302 Found";
            break;
        case HttpCode::NotFound:
            output << "404 Not Found";
            break;
        default:
            throw invalid_argument("Unknown HTTP code");
    }
    return output;
}

ostream& operator<<(ostream& output, const HttpHeader& h) {
    return output << h.name << ": " << h.value;
}

ostream& operator<<(ostream& output, const HttpResponse& resp) {
    output << "HTTP/1.1 " << resp.code << '\n';
    for (const auto& header : resp.headers) {
        output << header << '\n';
    }
    if (!resp.content.empty()) {
        output << "Content-Length: " << resp.content.size() << '\n';
    }
    return output << '\n' << resp.content;
}
