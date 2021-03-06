#include "Server.h"
#include <iostream>
#include "HttpRequestConcrete.h"

Server* Server::open_server = NULL;

Server::Server(std::string db_path, bool create_if_missing, bool clean_on_start)
{
    // levantar exepcion si ya fue seteada 'open_server'
    server_mgr = new struct mg_mgr;
    handlerManager = new HandlerManager(db_path, create_if_missing, clean_on_start);
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
    mg_enable_multithreading(nc);
    Server::open_server = this;

    return true;
}

//int count = 0;
void Server::staticHandler(struct mg_connection *nc, int ev, void* ev_data){
    /// estos comentarios deberian estar en un log de debug para ver si anda el multithreading
    /// si no anda, nunca va a imprimir (porque no se entra aca dos veces al mismo tiempo)
    /// si no, count > 1 --> imprime
    //++count;
    //if(count > 1)
    //    std::cout << count << std::endl;
    Server* server_m = Server::open_server;
    server_m->handler(nc, ev, ev_data);
    //--count;
}

void Server::handler(struct mg_connection* nc, int ev, void* ev_data){
    int connect_status;
    struct http_message* hmsg = (struct http_message*) ev_data;
    HttpRequestConcrete req;

    std::string content = "";

    switch(ev){
        case MG_EV_HTTP_REQUEST:
            req.init(nc, hmsg);
            handlerManager->handle(req);

            //content.append(hmsg->message.p, hmsg->message.len);
            content.append(req.getResponse());

            mg_printf(nc, "HTTP/1.1 %d\r\n"
                            "Transfer-Encoding: chunked\r\n"
                            "\r\n", req.getStatusCode());
            mg_printf_http_chunk(nc, "%s", content.c_str());
            mg_send_http_chunk(nc, "", 0);

            //usar mg_vcmp y los campos de http_message para ver que hacer
            //guardar mg_serve_https_opts y usar mg_serve_http para requests que no nos importan?
            break;
        default:
            break;
    }
}
