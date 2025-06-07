#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <vector>
#include <stdexcept>

class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class StoredItem {
private:
    std::string uid;
    std::string desc;
    std::string loc;

public:
    StoredItem(std::string u, std::string d, std::string l)
        : uid(u), desc(d), loc(l) {}

    std::string getId() const { return uid; }
    std::string getDescription() const { return desc; }
    std::string getLocation() const { return loc; }
};

class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> idMap;
    std::map<std::string, std::shared_ptr<StoredItem>> descMap;

public:
    void addItem(const std::shared_ptr<StoredItem>& obj) {
        if (idMap.find(obj->getId()) != idMap.end()) {
            throw DuplicateItemException("Item with ID " + obj->getId() + " already exists.");
        }
        idMap[obj->getId()] = obj;
        descMap[obj->getDescription()] = obj;
    }

    std::shared_ptr<StoredItem> findById(const std::string& uid) const {
        auto itr = idMap.find(uid);
        if (itr == idMap.end()) {
            throw ItemNotFoundException("Item with ID " + uid + " not found.");
        }
        return itr->second;
    }

    void removeItem(const std::string& uid) {
        auto itr = idMap.find(uid);
        if (itr == idMap.end()) {
            throw ItemNotFoundException("Item with ID " + uid + " not found.");
        }
        std::string descKey = itr->second->getDescription();
        idMap.erase(itr);
        descMap.erase(descKey);
    }

    std::vector<std::shared_ptr<StoredItem>> listItemsByDescription() const {
        std::vector<std::shared_ptr<StoredItem>> result;
        for (const auto& entry : descMap) {
            result.push_back(entry.second);
        }
        return result;
    }
};

#endif // STORAGEMANAGER_H
