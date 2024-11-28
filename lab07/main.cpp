#include "comment_server.h"
#include <iostream>

int main() {
    CommentServer server;

    
    HttpRequest req1 = {"POST", "/add_comment", "1 Hello!", {{"user_id", "1"}}};
    HttpResponse res1 = server.ServeRequest(req1);
    cout << res1;

    HttpRequest req2 = {"GET", "/comments", {}, {{"user_id", "1"}}};
    HttpResponse res2 = server.ServeRequest(req2);
    cout << res2;

    return 0;
}
