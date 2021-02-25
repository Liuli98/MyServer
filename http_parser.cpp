//
// Created by L on 2021/2/24.
//

#include "http_parser.h"

http_parser::http_parser(const std::string request)
{
    assert(request.size()>0);
    this->request=request;
}

http_parser::~http_parser() {}

http_request http_parser::get_parse_result()
{
    parse_line();
    parse_requestline();
    parse_headers();
    return parse_result;
}

void http_parser::parse_line()
{
    std::string::size_type line_begin=0;    //正在解析的行的行首索引
    std::string::size_type check_index=0;   //正在解析的字符索引

    while(check_index<request.size())
    {
        //如果当前字符是 \r 则有可能读到下一行
        if(request[check_index]=='\r')
        {
            //如果当前字符是最后一个字符说明请求没有读取完整
            if((check_index+1)==request.size())
            {
                std::cout<<"Request not to read the complete."<<std::endl;
                return;
            }
            else if(request[check_index+1]=='\n')
            {
                lines.push_back(std::string(request,line_begin,
                check_index-line_begin));
                check_index += 2;
                line_begin =check_index;
            }

            else
            {
                std::cout<<"request error."<<std::endl;
                return;
            }



        }

     else ++check_index;
    }

    return;
}


void http_parser::parse_requestline()
{
    std::string requestline = lines[0];

    //first_ws 指向请求行的第一个空白字符（''或'\t'）
    auto fitst_ws = std::find_if(requestline.cbegin(),requestline.cend(),
            [](char c)->bool { return (c==' ' || c == '\t');});

    //如果请求行中没有空白 则必定错误
    if(fitst_ws == requestline.cend())
    {
        std::cout<<"request error"<<std::endl;
        return;
    }
    //截取请求方法
    parse_result.method=std::string(requestline.cbegin(),fitst_ws);

    //reverse_last_ws指向请求行中的最后一个空白字符（' '或 '\t'）
    auto reverse_last_ws = std::find_if(requestline.crbegin(),requestline.crend(),
            [](char c)->bool{ return (c==' ' || c == '\t');});
    auto last_ws = reverse_last_ws.base();
    parse_result.version = std::string(last_ws,requestline.cend());

    while((*fitst_ws == ' ' || *fitst_ws ==  '\t') && fitst_ws != requestline.cend())
        ++fitst_ws;

    --last_ws;
    while ((*last_ws == ' ' || *last_ws == '\t') && last_ws != requestline.cbegin())
        --last_ws;

    parse_result.uri = std::string(fitst_ws,last_ws+1);
    return;
}


void http_parser::parse_headers()
{
    for(int i=1;i<lines.size(); ++i)
    {
        if(lines[i].empty()) //如果遇到空行说明解析完毕
            return;
        else if(strncasecmp(lines[i].c_str(),"HOST:",5) == 0) //处理host头部字段
        {
            auto iter = lines[i].cbegin()+5;
            while (*iter == ' ' || *iter == '\t')
                ++iter;
            parse_result.host = std::string(iter,lines[i].cend());
        }
        else if(strncasecmp(lines[i].c_str(),"connecting:",11) ==0)     //处理connection头部字段
        {
            auto iter = lines[i].cbegin()+11;
            while(*iter == ' ' || *iter == '\t')
                ++iter;
            parse_result.connecting = std::string(iter,lines[i].cend());
        }
        else
        {
            //暂时忽略其他头部
        }
    }
    return;
}