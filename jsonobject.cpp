//
//  jsonobject.cpp
//  JsonLib
//
//  Created by Sylvan on 10/30/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#include "jsonobject.hpp"
#include "jsonvalue.hpp"
#include "jsonarray.hpp"
#include <algorithm>

JsonObject::JsonObject(std::initializer_list<std::pair<const Key, JsonValue>> ilist) :
    data_(ilist)
{}

JsonObject& JsonObject::operator=(std::initializer_list<std::pair<const Key, JsonValue>> ilist)
{
    data_ = ilist;
    return *this;
}

void JsonObject::swap(JsonObject& other)
{
    data_.swap(other.data_);
}

std::pair<JsonObject::JsonMap::iterator,bool> JsonObject::insert(const JsonPair& value)
{
    return data_.insert(value);
}

JsonObject::JsonMap::iterator JsonObject::insert(JsonMap::const_iterator hint, const JsonPair& value)
{
    return data_.insert(hint, value);
}

template<class InputIt>
void JsonObject::insert(InputIt first, InputIt last)
{
    data_.insert(first, last);
}

std::vector<JsonObject::Key> JsonObject::keys(Ordering method) const
{
    std::vector<Key> keys;
    keys.reserve(data_.size());
    for (auto const& pr : data_) {
        keys.push_back(pr.first);
    }
    if (method == Ordered) {
        std::sort(keys.begin(), keys.end());
    }
    return keys;
}

bool JsonObject::equals(const JsonObject& other) const
{
    return (data_ == other.data_);
}

std::ostream& JsonObject::serialize(std::ostream& os) const
{
    os << "{";
    if (!data_.empty()) {
        auto it = data_.cbegin();
        os << "\"" << it->first << "\":" << it->second;
        for (auto it = ++data_.cbegin(); it != data_.cend(); ++it) {
            os << ",\"" << it->first << "\":" << it->second;
        }
    }
    return os << "}";
}



// -------------------------------
// Related non-member functions
// -------------------------------

bool operator==(const JsonObject& lhs, const JsonObject& rhs)
{
    return lhs.equals(rhs);
}

bool operator!=(const JsonObject& lhs, const JsonObject& rhs)
{
    return !lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const JsonObject& o)
{
    return o.serialize(os);
}
