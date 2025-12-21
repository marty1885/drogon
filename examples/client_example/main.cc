#include <drogon/drogon.h>
#include <iostream>
#include <deque>
#include <functional>

using namespace drogon;

struct TestTarget {
    std::string url;
    std::string path;
};

std::deque<TestTarget> targets;

void test_next_site() {
    if (targets.empty()) {
        app().quit();
        return;
    }

    TestTarget target = targets.front();
    targets.pop_front();

    std::string url = target.url;
    std::string path = target.path;

    std::cout << "Testing: " << url << path << std::endl;

    // force HTTP/2
    auto client = HttpClient::newHttpClient(url, nullptr, false, true, Version::kHttp2);
    auto req = HttpRequest::newHttpRequest();
    req->setPath(path);
    req->setMethod(drogon::Get);
    req->addHeader("User-Agent", "DrogonClient/1.0");

    client->sendRequest(
        req, [url, path](ReqResult result, const HttpResponsePtr &response) {
            std::cout << "--------------------------------------------------" << std::endl;
            if (result != ReqResult::Ok)
            {
                std::cout
                    << "FAILED: " << url << path << " error: "
                    << result << std::endl;
            }
            else
            {
                std::cout << "SUCCESS: " << url << path << std::endl;
                std::cout << "Status: " << response->getStatusCode() << std::endl;
                std::cout << "Version: " << response->getVersionString() << std::endl;
                std::cout << "Body Size: " << response->getBody().size() << std::endl;
            }
            std::cout << "--------------------------------------------------" << std::endl;

            // Trigger next test after this one completes
            // We use getLoop()->queueInLoop to ensure we don't recurse deeply in stack if callback is immediate (though likely async)
            drogon::app().getLoop()->queueInLoop([](){
                test_next_site();
            });
        });
}

int main()
{
    // Set log level to Trace to see detailed protocol logs as requested
    trantor::Logger::setLogLevel(trantor::Logger::kTrace);

    // List of sites to test
    targets = {
        {"https://www.google.com", "/"},
        {"https://nghttp2.org", "/"},
        {"https://www.cloudflare.com", "/"},
        {"https://httpbin.org", "/get"},
        {"https://www.facebook.com", "/"},
        {"https://www.wikipedia.org", "/"},
        {"https://github.com", "/"}
    };

    // Note: Some sites like amazon.com or twitter.com might return 503 or 404/403
    // due to bot protection, but the HTTP/2 connection itself is successful.

    // Start the first test
    test_next_site();

    app().run();
}
