#ifndef FILEDATA_H
#define FILEDATA_H

#include "DBElement.h"
#include <string>
#include <vector>
#include <algorithm>
#include "rocksdb/status.h"

class FileData : public DBElement
{
    public:
        FileData(Database* db);
        virtual ~FileData();

        std::string getContent() { if(content.size() < 1) return ""; return content[content.size()-1]; }
        std::string getFilename() { return filename; }
        std::string getExtension() { return extension; }
        std::string getOwnerUsername() { return owner_username; }
        std::string getOwnerKey() { return owner_key; }
        std::vector<std::string>* getUsersWithReadPermission() { return &users_with_read_permission; }
        std::vector<std::string>* getUsersWithWritePermission() { return &users_with_write_permission; }
        std::string getDateLastModified() { return date_last_modified; }
        std::string getUserWhoLastModified() { return user_who_modified; }
        std::vector<std::string>* getTags() { return &tags; }

        void setContent(std::string n_content);
        void setFilename(std::string n_filename) { filename = n_filename; }
        void setExtension(std::string n_extension) { extension = n_extension; }
        void setOwnerUsername(std::string n_owner_username) { owner_username = n_owner_username; }
        void setOwnerKey(std::string n_owner_key) { owner_key = n_owner_key; }
        void addUserWithReadPermission(std::string user_key);
        void addUserWithWritePermission(std::string user_key);
        void removeUserWithReadPermission(std::string user_key);
        void removeUserWithWritePermission(std::string user_key);
        void setDateLastModified(std::string date) { date_last_modified = date; }
        void setUserWhoLastModified(std::string user_key) { user_who_modified = user_key; }
        void addTag(std::string tag_key);
        void removeTag(std::string tag_key) { tags.erase(std::remove(tags.begin(), tags.end(), tag_key), tags.end()); }
        bool check_read_permission(std::string username);
        bool check_write_permission(std::string username);

        bool check_if_tag_belongs(std::string t) { return (std::find(tags.begin(),tags.end(),t) != tags.end());}
        bool check_extension(std::string e) {return extension == e;}

        Status DBaddUserWithReadPermission(std::string user_key);
        Status DBaddUserWithWritePermission(std::string user_key);
        Status DBremoveUserWithReadPermission(std::string user_key);
        Status DBremoveUserWithWritePermission(std::string user_key);
        Status DBerase();
        Status DBcreate(std::string content, std::string ubicacion);
        Status DBget_for_read(std::string username);
        Status DBget_for_modify(std::string username);
        Status DBsetFilename(std::string name);
        Status DBsetContent(std::string content);
        Status DBaddTag(std::string tag);
        Status DBremoveTag(std::string tag);
        Status DBeraseVersion(int v);
        Status DBmodify(std::string username, std::string n_filename, std::string ubicacion, std::string n_content, std::vector<std::string> &users_read_add,
                        std::vector<std::string> &users_read_remove, std::vector<std::string> &users_write_add, std::vector<std::string> &users_write_remove,
                        std::vector<std::string> &tags_add, std::vector<std::string> &tags_remove, std::vector<int> delete_versions_except_last_n);

    protected:
        virtual void _setKey();
        virtual void _setValue();
        virtual void _setValueVars();

    private:
        std::vector<std::string> content;
        std::string filename;
        std::string extension;
        std::string owner_username;
        std::string owner_key; // si es el MD5 del username no hace falta tenerlo

        std::vector<std::string> users_with_read_permission; // tal vez deberian ser sets para evitar duplicados
        std::vector<std::string> users_with_write_permission;

        std::string date_last_modified;
        std::string user_who_modified;

        std::vector<std::string> tags; // tal vez deberia ser set

        Status DBchangeModified(std::string username);
        Status DBsetExtension(std::string new_extension);
        int contentSize();
};

#endif // FILEDATA_H
