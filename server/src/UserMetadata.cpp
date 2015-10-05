#include "UserMetadata.h"
#include "JsonSerializer.h"
#include <string>
#include "json/json.h"

using namespace Json;

UserMetadata::UserMetadata()
{
    //ctor
}

UserMetadata::~UserMetadata()
{
    //dtor
}

void UserMetadata::_setKey(){
    this->key = this->user_token;
    this->key.append(".usertoken");
}

void UserMetadata::_setValueVars(){
    Reader reader;
    Value json_value;
    reader.parse(this->value, json_value);

    setJoinDate(json_value["join_date"].toStyledString());

    for(ValueIterator it = json_value["my_file_tokens"].begin(); it != json_value["my_file_tokens"].end(); ++it){
        addMyFileToken((*it).asString());
    }

    for(ValueIterator it = json_value["shared_file_tokens"].begin(); it != json_value["shared_file_tokens"].end(); ++it){
        addMyFileToken((*it).asString());
    }
}

void UserMetadata::_setValue(){
    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_file_tokens, "my_file_tokens", array_my_file_tokens);
    std::string array_shared_file_tokens = "";
    serializer.turnVectorToArray(shared_file_tokens, "shared_file_tokens", array_shared_file_tokens);
    std::string val_date_joined = "";
    serializer.addValueToObjectList(val_date_joined, "join_date", join_date);

    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.joinValueIntoList(val_json, val_date_joined);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
}