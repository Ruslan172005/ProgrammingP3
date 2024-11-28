#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302,
};

ostream& operator<<(ostream& output, HttpCode code);

struct HttpHeader {
    string name, value;
};

ostream& operator<<(ostream& output, const HttpHeader& h);

class HttpResponse {
public:
    explicit HttpResponse(HttpCode code);

    HttpResponse& AddHeader(string name, string value);
    HttpResponse& SetContent(string a_content);
    HttpResponse& SetCode(HttpCode a_code);

    friend ostream& operator<<(ostream& output, const HttpResponse& resp);

private:
    HttpCode code;
    vector<HttpHeader> headers;
    string content;
};
