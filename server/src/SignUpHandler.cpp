#include "SignUpHandler.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "User.h"
#include "UserMetadata.h"

SignUpHandler::SignUpHandler(Database *db, TokenAuthenticator *a) : EventHandlerIgnoresAuthentication(db, a)
{

}

SignUpHandler::~SignUpHandler(){

}

void SignUpHandler::_handle(HttpRequest &hmsg){
    Status s = Status::OK();

    std::string usuario = hmsg.getCampo("username");
    if(usuario == "") return;
    std::string password = hmsg.getCampo("password");
    if(password == "") return;

    User user(db);
    user.setUsername(usuario);
    user.setPassword(password);
    s = user.DBcreate();
    hmsg.setStatusCode(s);
    // ver status
//    hmsg.setResponse(s.ToString());
}
