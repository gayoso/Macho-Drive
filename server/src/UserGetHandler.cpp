#include "UserGetHandler.h"
#include "UserMetadata.h"

UserGetHandler::UserGetHandler(Database *db) : EventHandler(db)
{
    //ctor
}

UserGetHandler::~UserGetHandler()
{
    //dtor
}

void UserGetHandler::handle(HttpRequest &hmsg){
    UserMetadata user_metadata(db);
    user_metadata.setUsername(hmsg.getCampo("username"));

    Status s = this->db->get(user_metadata);
    hmsg.setResponse(user_metadata.getValueToString());
}
