#include <drogon/drogon_test.h>
#include "../../src/HttpResponseParser.h"
#include <trantor/utils/MsgBuffer.h>
#include <string_view>

using namespace drogon;

DROGON_TEST(HttpResponseParserRejectsMalformedStatusLine)
{
    for (std::string_view line : {"",
                                  " ",
                                  " HTTP/1.1 200 OK",
                                  "HTTP/1.1",
                                  "HTTP/1.1 ",
                                  "HTTP/1.1 20 OK",
                                  "HTTP/1.1 200",
                                  "HTTP/1.1 abc OK",
                                  "X1 200 OK"})
    {
        HttpResponseParser parser(trantor::TcpConnectionPtr{});
        trantor::MsgBuffer buffer;
        buffer.append(line.data(), line.size());
        buffer.append("\r\n", 2);
        CHECK(!parser.parseResponse(&buffer));
    }
}

DROGON_TEST(HttpResponseParserAcceptsValidStatusLine)
{
    for (std::string_view line : {"HTTP/1.0 200 OK", "HTTP/1.1 204 "})
    {
        HttpResponseParser parser(trantor::TcpConnectionPtr{});
        trantor::MsgBuffer buffer;
        buffer.append(line.data(), line.size());
        constexpr std::string_view suffix = "\r\nContent-Length: 0\r\n\r\n";
        buffer.append(suffix.data(), suffix.size());
        CHECK(parser.parseResponse(&buffer));
        CHECK(parser.gotAll());
    }
}
