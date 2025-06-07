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
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string i, std::string d, std::string l)
        : id(i), description(d), location(l) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        if (itemById.find(item->getId()) != itemById.end()) {
            throw DuplicateItemException("Item with ID " + item->getId() + " already exists.");
        }
        itemById[item->getId()] = item;
        itemByDescription[item->getDescription()] = item;
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found.");
        }
        return it->second;
    }

    void removeItem(const std::string& id) {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found.");
        }
        std::string desc = it->second->getDescription();
        itemById.erase(it);
        itemByDescription.erase(desc);
    }

    std::vector<std::shared_ptr<StoredItem>> listItemsByDescription() const {
        std::vector<std::shared_ptr<StoredItem>> items;
        for (const auto& pair : itemByDescription) {
            items.push_back(pair.second);
        }
        return items;
    }
};


#endif // STORAGEMANAGER_H
