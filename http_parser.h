//
// Created by L on 2021/2/24.
//

#ifndef SERVER_HTTP_PARSER_H
#define SERVER_HTTP_PARSER_H

#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <strings.h>

enum PARSE_STATE{ PARSE_REQUESTLINE=0, PARSE_HEADER};

//表示正在解析的状态 一个完整行，行出错，接收数据不完整
enum LINE_STATUS{   LINE_OK=0,LINE_ERROR,LINE_MORE };

enum  HTTP_CODE{ MORE_DATA=0,GET_REQUEST,REQUEST_ERROR,FORBIDDEN_REQUEST,
        INTERNAL_ERROR,CLOSE_CONNECTING};

//解析后的数据存储在结构体中
typedef  struct
{
    std::string method;
    std::string uri;
    std::string version;
    std::string host;
    std::string connecting;
}http_request;

class http_parser
{
public:
    http_parser(const std::string request);
    ~http_parser();
    http_request get_parse_result();

private:
    void parse_line();
    void parse_requestline();
    void parse_headers();

    std::string request;
    std::vector<std::string> lines;
    http_request parse_result;
};




#endif //SERVER_HTTP_PARSER_H
