#include "comment_server.h"
#include "http_response.h"
#include <sstream>

pair<size_t, string> CommentServer::ParseIdAndContent(const string& body) {
    auto [id_string, content] = SplitBy(body, " ");
    return {FromString<size_t>(id_string), content};
}

HttpResponse CommentServer::ServeRequest(const HttpRequest& req) {
    if (req.method == "POST") {
        if (req.path == "/add_comment") {
            return ServeAddComment(req);
        } else if (req.path == "/add_user") {
            return ServeAddUser(req);
        } else if (req.path == "/check_captcha") {
            return ServeCheckCaptcha(req);
        }
    } else if (req.method == "GET") {
        if (req.path == "/comments") {
            return ServeUserComments(req);
        } else if (req.path == "/captcha") {
            return ServeCaptcha(req);
        }
    }

    return HttpResponse(HttpCode::NotFound);
}

HttpResponse CommentServer::ServeAddUser(const HttpRequest& request) {
    
    return HttpResponse(HttpCode::Ok);
}

HttpResponse CommentServer::ServeAddComment(const HttpRequest& request) {
    auto [user_id, comment] = ParseIdAndContent(request.body);
    if (!last_comment || last_comment->user_id != user_id) {
        last_comment = LastCommentInfo{user_id, 1};
    } else if (++last_comment->consecutive_count > 3) {
        banned_users.insert(user_id);
    }
    if (banned_users.count(user_id) == 0) {
        comments_[user_id].push_back(comment);
        return HttpResponse(HttpCode::Ok);
    } else {
        return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
    }
}

HttpResponse CommentServer::ServeCheckCaptcha(const HttpRequest& request) {
    auto [id, response] = ParseIdAndContent(request.body);
    if (response == "42") {
        return HttpResponse(HttpCode::Ok);
    }
    return HttpResponse(HttpCode::Found).AddHeader("Location", "/captcha");
}

HttpResponse CommentServer::ServeUserComments(const HttpRequest& request) {
    auto user_id = FromString<size_t>(request.get_params.at("user_id"));
    string response;
    for (const string& c : comments_[user_id]) {
        response += c + '\n';
    }
    return HttpResponse(HttpCode::Ok).SetContent(std::move(response));
}

HttpResponse CommentServer::ServeCaptcha(const HttpRequest&) {
    return HttpResponse(HttpCode::Ok)
        .SetContent("What's the answer for The Ultimate Question of Life, the Universe, and Everything?");
}
