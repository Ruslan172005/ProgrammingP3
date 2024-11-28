#pragma once
#include "http_response.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <optional>
#include <map>

struct HttpRequest {
    string method, path, body;
    map<string, string> get_params;
};

class CommentServer {
public:
    HttpResponse ServeRequest(const HttpRequest& req);

private:
    HttpResponse ServeAddUser(const HttpRequest& request);
    HttpResponse ServeAddComment(const HttpRequest& request);
    HttpResponse ServeCheckCaptcha(const HttpRequest& request);
    HttpResponse ServeUserComments(const HttpRequest& request);
    HttpResponse ServeCaptcha(const HttpRequest& request);

    vector<vector<string>> comments_;
    std::optional<LastCommentInfo> last_comment;
    unordered_set<size_t> banned_users;

    pair<size_t, string> ParseIdAndContent(const string& body);
};

struct LastCommentInfo {
    size_t user_id, consecutive_count;
};
