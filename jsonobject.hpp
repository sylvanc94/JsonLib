//
//  jsonobject.hpp
//  JsonLib
//
//  Created by Sylvan on 10/30/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#ifndef jsonobject_hpp
#define jsonobject_hpp

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

class JsonValue;
class JsonArray;

class JsonObject
{
    using Key = std::string;
    using JsonMap = std::unordered_map<Key, JsonValue>;
    using JsonPair = std::pair<Key, JsonValue>;

public:
    enum Ordering {
        Unordered,
        Ordered
    };
    
    JsonObject() = default;
    ~JsonObject() = default;
    
    JsonObject(std::initializer_list<std::pair<const Key, JsonValue>> ilist);
    
    JsonObject(const JsonObject&) = default;
    JsonObject(JsonObject&&) = default;
    
    JsonObject& operator=(const JsonObject&) = default;
    JsonObject& operator=(JsonObject&&) = default;
    JsonObject& operator=(std::initializer_list<std::pair<const Key, JsonValue>> ilist);

    // Iterators
    JsonMap::iterator       begin()  noexcept       { return data_.begin();  }
    JsonMap::const_iterator begin()  const noexcept { return data_.begin();  }
    JsonMap::const_iterator cbegin() const noexcept { return data_.cbegin(); }
    JsonMap::iterator       end()    noexcept       { return data_.end();    }
    JsonMap::const_iterator end()    const noexcept { return data_.end();    }
    JsonMap::const_iterator cend()   const noexcept { return data_.cend();   }
    
    // Capacity
    bool empty() const noexcept { return data_.empty(); }
    size_t size() const noexcept { return data_.size(); }
    size_t max_size() const noexcept { return data_.max_size(); }

    // Modifiers
    void clear() noexcept { data_.clear(); }
    void swap(JsonObject& other);

    // Inserts
    std::pair<JsonMap::iterator,bool> insert(const JsonPair& value);
    JsonMap::iterator insert(JsonMap::const_iterator hint, const JsonPair& value);
    void insert(std::initializer_list<std::pair<const Key, JsonValue>> ilist);
    template<class InputIt>
    void insert(InputIt first, InputIt last);

    // Erases
    JsonMap::iterator erase(JsonMap::const_iterator pos) { return data_.erase(pos); }
    JsonMap::iterator erase(JsonMap::const_iterator first, JsonMap::const_iterator last) { return data_.erase(first, last); }
    size_t erase(const Key& key) { return data_.erase(key); }

    // Lookup
    JsonValue& at(const Key& key)             { return data_.at(key); }
    const JsonValue& at(const Key& key) const { return data_.at(key); }

    JsonValue& operator[](const Key& key) { return data_[key]; }
    JsonValue& operator[](Key&& key)      { return data_[std::move(key)]; }

    size_t count(const Key& key) const { return data_.count(key); }

    JsonMap::iterator find(const Key& key)             { return data_.find(key); }
    JsonMap::const_iterator find(const Key& key) const { return data_.find(key); }

    bool contains(const Key& key) const { return data_.count(key); }
    
    std::vector<Key> keys(Ordering method) const;
    bool equals(const JsonObject&) const;
    std::ostream& serialize(std::ostream&) const;
    
private:
    JsonMap data_;
};

bool operator==(const JsonObject& lhs, const JsonObject& rhs);
bool operator!=(const JsonObject& lhs, const JsonObject& rhs);

std::ostream& operator<<(std::ostream&, const JsonObject&);

#endif /* jsonobject_hpp */
