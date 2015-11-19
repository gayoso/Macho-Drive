#include "UserDeleteHandler.h"
#include "User.h"

UserDeleteHandler::UserDeleteHandler(Database *db, TokenAuthenticator *a) : EventHandlerRemovesAuthentication(db, a)
{
    //ctor
}

UserDeleteHandler::~UserDeleteHandler()
{
    //dtor
}

bool UserDeleteHandler::isMyRequest(HttpRequest &hmsg){
    // DELETE /users/'username' quiere decir borrar el usuario
    if(hmsg.getUriParsedByIndex(0) == HttpRequest::USERS &&
        hmsg.getUriCantCampos() == 2 &&
        hmsg.getUriType() ==  HttpRequest::ELEMENT_URI &&
        hmsg.getMethod() == HttpRequest::DELETE){
        return true;
    }
    return false;
}

void UserDeleteHandler::_handle(HttpRequest &hmsg){
    Status s;

    std::string username = hmsg.getUsername();
    // ver si falta el campo

    User user(db);
    user.setUsername(username);
    s = user.DBerase();
    // devolver mensaje de error
    /*bool existed = auth->removeToken(username);
    if(!existed){
        //informar o loggear que no existia el usuario
    }*/
    hmsg.setResponse(s);
}
