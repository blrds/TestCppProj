#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <hv/json.hpp>

#include "hv/HttpServer.h"
#include "hv/requests.h"
#include "hv/WebSocketServer.h"
#include "hv/WebSocketClient.h"
#include "Circle.h"

/// @brief prints to cli objects from vector
void Say(std::vector<std::unique_ptr<Circle>> *objs){
    for(int i=0;i<(*objs).size();i++)
                if((*objs)[i].get()!=NULL)
                    std::cout<<(*objs)[i].get()->R()<<std::endl;
}
HttpService Router(std::vector<std::unique_ptr<Circle>> *objs){
    
}
/////////////////
void httpServer(std::vector<std::unique_ptr<Circle>> *objs) {
    Say(objs);
    std::cout<<"server on\n";
    hv::HttpServer server;
    HttpService router;
    router.GET("/get", [&](HttpRequest* req, HttpResponse* resp) {
        std::cout<<"new get\n";
        resp->json["circle"] = (*objs)[atoi(req->body.c_str())].get()->R();
        return 200;
    });
    server.registerHttpService(&router);
    server.setPort(11111);
    server.setThreadNum(4);
    server.run();
}

void WebServer(std::vector<std::unique_ptr<Circle>> *objs){
    Say(objs);
    std::cout<<"server on\n";
    WebSocketService ws;
    ws.onopen = [](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        std::cout<<"connection openned\n"; 
    };
    ws.onclose = [](const WebSocketChannelPtr& channel) {
        std::cout<<"connection closed\n";
    };
    hv::WebSocketServer server;
    server.registerWebSocketService(&ws);
    HttpService router;
    router.GET("/get", [&](HttpRequest* req, HttpResponse* resp) {
        std::cout<<"new get\n";
        resp->json["circle"] = (*objs)[atoi(req->body.c_str())].get()->R();
        return 200;
    });
    server.registerHttpService(&router);
    server.setPort(9999);
    server.setThreadNum(4);
    server.run();
} 
//////////////////

int HttpClient(std::string param, std::vector<std::unique_ptr<Circle>> *objs, std::string uri="127.0.0.1:11111/get"){
    std::cout<<"sending request\n";
    auto resp = requests::request(HTTP_GET,uri.c_str(),param);
    if (resp != NULL) {
        std::cout<<"responce came\n";
        std::cout<<resp.get()->Body().c_str();
        std::cout<<std::endl;
        if((*objs).size()<=atoi(param.c_str()))
            (*objs).resize(atoi(param.c_str())+1);
        double val =nlohmann::json::parse(resp.get()->Body()).at("circle");
        (*objs)[atoi(param.c_str())].reset(new Circle(val));
        std::cout<<(*objs)[atoi(param.c_str())].get()->R()<<std::endl;
        return 1;
    }else return 0;
}

int WebClient(std::string param, std::vector<std::unique_ptr<Circle>> *objs){
    hv::WebSocketClient ws;
    int ret=1;
    ws.onopen = []() {
        std::cout<<"connected\n";
    };
    ws.onclose = []() {
        std::cout<<"connection closed\n";
    };

    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    ws.setReconnect(&reconn);

    ws.open("ws://127.0.0.1:9999");
    ret=HttpClient(param,objs,"127.0.0.1:9999/get");
    ws.close();
    return ret;
}

void Client(char connectionMethod, std::vector<std::unique_ptr<Circle>> *objs) {
    while(1){
        std::cout<<"enter the command\n";
        std::string str;
        getline(std::cin, str);
        std::string comm=str.substr(0,str.find(" "));
        std::string param="";
        if(comm!=str)
            param=str.substr(str.find(" ")+1, str.length());
        std::cout<<"command "<<comm<<" entered\n";

        if(comm=="exit")return;

        if(comm=="get"){
            if(connectionMethod=='h')
                HttpClient(param, objs);
            else if(connectionMethod=='w')
                WebClient(param,objs);
        }
        
        if(comm=="say"){
            Say(objs);
        }
        
    }
}

/// @param argv can contains
/// <int>-base elements count(only on s erver side)
/// -s - program starts as server
/// -c - program starts as client
/// -h - program will use http
/// -w - program will use web sockets
int main(int argc, char* argv[]){
    int elementsCount=0, elementsCount_Flag=1, programType_Flag=1, connectionMethod_Flag=1;
    char programType='s';
    char connectionMethod='h';
    for(int i=1;i<argc;i++){//reading programm arguments
        if((argv[i][0]=='-')){
            if((argv[i][1]=='s' || argv[i][1]=='c') && programType_Flag){
                programType=argv[i][1];
                programType_Flag=0;
                continue;
            }
            if((argv[i][1]=='h' || argv[i][1]=='w')){connectionMethod=argv[i][1];continue;}
        }else{
            if(!elementsCount_Flag)continue;
            char* err;
            long c=strtol(argv[i],&err,10);
            if(!*err)elementsCount=c;
        }
    }
    std::vector<std::unique_ptr<Circle>>objs(elementsCount);//initialize vector of elements
    if(programType=='s'){//if server then and new elements
        if(elementsCount!=1)objs.resize(elementsCount);
        for(int i=0;i<elementsCount;i++)
            objs[i].reset(new Circle());
    }
    if(programType=='c')
        Client(connectionMethod,&objs);
    else if(connectionMethod=='h')
        httpServer(&objs);
        else if(connectionMethod=='w')
            WebServer(&objs);
    return 1;
}