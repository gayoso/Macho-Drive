#include "UserMetadata.h"
#include "JsonSerializer.h"
#include <string>
#include "json/json.h"
#include "Database.h"
#include "FileData.h"
#include "Util.h"

using namespace Json;

UserMetadata::UserMetadata(Database* db) : DBElement(db)
{
    //ctor
}

UserMetadata::~UserMetadata()
{
    //dtor
}

void UserMetadata::addMyFile(std::string name){
    this->my_files.push_back(name);
}

void UserMetadata::removeMyFile(std::string name){
    my_files.erase(std::remove(my_files.begin(), my_files.end(), name), my_files.end());
}

void UserMetadata::addSharedFile(std::string name, std::string user){
    this->shared_files.push_back(std::make_pair(user, name));
}

void UserMetadata::removeSharedFile(std::string name, std::string user){
    shared_files.erase(std::remove(shared_files.begin(), shared_files.end(), std::make_pair(user, name)), shared_files.end());
}

void UserMetadata::_setKey(){
    this->key = this->username;
    this->key.append(".usertoken");
}

void UserMetadata::_setValueVars(){
    Reader reader;
    Value json_value;
    reader.parse(this->value, json_value);

    setJoinDate(JsonSerializer::removeBegAndEndQuotes(json_value["join_date"].toStyledString()));

    for(ValueIterator it = json_value["my_file_tokens"].begin(); it != json_value["my_file_tokens"].end(); ++it){
        addMyFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()));
    }

    for(ValueIterator it = json_value["shared_file_tokens"].begin(); it != json_value["shared_file_tokens"].end(); ++it){
        addSharedFile(JsonSerializer::removeBegAndEndQuotes((*it).asString()), JsonSerializer::removeBegAndEndQuotes((it.key()).asString()));
    }
}

void UserMetadata::_setValue(){
    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_files, "my_file_tokens", array_my_file_tokens);
    std::string array_shared_file_tokens = "";
    serializer.turnPairsVectorToObject(shared_files, "shared_file_tokens", array_shared_file_tokens);
    std::string val_date_joined = "";
    serializer.addValueToObjectList(val_date_joined, "join_date", join_date);

    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.joinValueIntoList(val_json, val_date_joined);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
}

std::string UserMetadata::getFileTreeJson(){
    JsonSerializer serializer;

    std::string array_my_file_tokens = "";
    serializer.turnVectorToArray(my_files, "my_file_tokens", array_my_file_tokens);
    std::string array_shared_file_tokens = "";
    serializer.turnPairsVectorToObject(shared_files, "shared_file_tokens", array_shared_file_tokens);
    std::string val_json = "";
    serializer.joinValueIntoList(val_json, array_my_file_tokens);
    serializer.joinValueIntoList(val_json, array_shared_file_tokens);
    serializer.turnObjectListToObject(val_json);
    this->value = val_json;
}

// asumo que me dieron el username correcto
Status UserMetadata::DBerase(){
    Status s;

    s = this->db->get(*this);
    // ver status

    for(std::vector<std::string>::iterator it = my_files.begin(); it != my_files.end(); ++it){
        FileData file_data(db);
        file_data.setOwnerUsername(this->getUsername());
        file_data.setFilename(*it);
        s = file_data.DBerase();
        // ver status
    }

    for(std::vector< std::pair<std::string, std::string> >::iterator it = shared_files.begin(); it != shared_files.end(); ++it){
        FileData file_data(db);
        file_data.setOwnerUsername(it->first);
        file_data.setFilename(it->second);
        s = file_data.DBremoveUserWithReadPermission(this->getUsername());
        // ver status
    }

    s = this->db->erase(*this);
    // ver status
    return s;
}

Status UserMetadata::DBcreate(){
    Status s;

    s = this->db->get(*this);
    // ver status, si ya existe borrar y devolver error (pq si llegue aca ya me fije y no existe el 'User', asi que no deberia haber metadata)
    this->setJoinDate(get_date_and_time());
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBremove_my_file(std::string filename){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->removeMyFile(filename);
    s = this->db->put(*this);
    // ver status

    return s;
}

Status UserMetadata::DBremove_shared_file(std::string user, std::string filename){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->removeSharedFile(filename, user);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBadd_my_file(std::string filename){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->addMyFile(filename);
    s = this->db->put(*this);
    // ver status
    return s;
}

Status UserMetadata::DBadd_shared_file(std::string user, std::string filename){
    Status s;

    s = this->db->get(*this);
    // ver status
    this->addSharedFile(filename, user);
    s = this->db->put(*this);
    // ver status
    return s;
}
