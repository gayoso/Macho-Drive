#include "Server.h"
#include <iostream>

Server::Server()
{
    server_mgr = new struct mg_mgr;
    handlerManager = new HandlerManager();
    mg_mgr_init(server_mgr, this);
    active = false;
}

Server::~Server()
{
    if(active){
        stop();
    }
}

void Server::poll(){
    mg_mgr_poll(server_mgr, 1000);
}

void Server::stop(){
    active = false;
    mg_mgr_free(server_mgr);
}

bool Server::createListeningConnection(const char * addr){
    //tal vez guardar la mg_connection como var de clase? todavia no se si hace falta
    struct mg_connection* nc = mg_bind(server_mgr, addr, Server::staticHandler);
    if(nc == NULL){ //error
        return false;
    }
    mg_set_protocol_http_websocket(nc);
    //mg_enable_multithreading(nc);
    return true;
}

void Server::staticHandler(struct mg_connection *nc, int ev, void* ev_data){
    Server* server_m = (Server*) nc->listener->mgr->user_data;
    //std::cout << "llego2" << std::endl;
    server_m->handler(nc, ev, ev_data);
}

void Server::handler(struct mg_connection* nc, int ev, void* ev_data){
    int connect_status;
    struct http_message* hmsg = (struct http_message*) ev_data;
    //struct mg_serve_http_opts s;

    std::string content = "";
    //int content_length = snprintf(content, sizeof(content), "Hello world");
    //printf("%.*s\n", hmsg->message.len, hmsg->message.p);

    switch(ev){
        case NS_HTTP_REQUEST:
            handlerManager->handle(hmsg);
            content.append(hmsg->method.p, hmsg->method.len);
            content.append(" de ");
            content.append(hmsg->uri.p, hmsg->uri.len);
            //std::cout << "llego" << std::endl;
            //mg_printf(nc, "GET de elemento: %s", hmsg->uri);
            //if(mg_vcmp(&hmsg->uri, "/datos") == 0){
            
            mg_printf(nc, "HTTP/1.1 200 OK\r\n"
                            "Transfer-Encoding: chunked\r\n"
                            "\r\n");
            mg_printf_http_chunk(nc, "%s", content.c_str());
            mg_send_http_chunk(nc, "", 0);
            
            //std::cout << "llego" << std::endl;

            /*if(mg_vcmp(&hmsg->uri, "/datos") == 0){
                mg_printf(nc, "HTTP/1.1 200 OK\r\n"
                                "Transfer-Encoding: chunked\r\n"
                                "\r\n");
                mg_printf_http_chunk(nc, "Hello world");
                mg_send_http_chunk(nc, "", 0);

            } else {
                struct mg_serve_http_opts opts = { .document_root = "/var/www" };
                mg_serve_http(nc, hmsg, opts);
            }*/

            //usar mg_vcmp y los campos de http_message para ver que hacer
            //guardar mg_serve_https_opts y usar mg_serve_http para requests que no nos importan?
            break;
        default:
            break;
    }
}