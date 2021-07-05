//
//  jsonarray.cpp
//  JsonLib
//
//  Created by Sylvan on 10/29/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#include "jsonarray.hpp"
#include "jsonobject.hpp"

JsonArray::JsonArray(size_t count) :
    data_(count)
{}

JsonArray::JsonArray(size_t count, const JsonValue& value) :
    data_(count, value)
{}

template <class InputIt>
JsonArray::JsonArray(InputIt first, InputIt last) :
    data_(first, last)
{}

JsonArray::JsonArray(std::initializer_list<JsonValue> ilist) :
    data_(ilist)
{}

JsonArray& JsonArray::operator=(std::initializer_list<JsonValue> ilist)
{
    data_ = ilist;
    return *this;
}

bool JsonArray::equals(const JsonArray& other) const
{
    return (data_ == other.data_);
}

std::ostream& JsonArray::serialize(std::ostream& os) const
{
    os << "[";
    if (!data_.empty()) {
        os << data_.front();
    }
    for (auto it = data_.cbegin() + 1; it != data_.cend(); ++it) {
        os << "," << *it;
    }
    return os << "]";
}



// -------------------------------
// Related non-member functions
// -------------------------------

bool operator==(const JsonArray& lhs, const JsonArray& rhs)
{
    return lhs.equals(rhs);
}

bool operator!=(const JsonArray& lhs, const JsonArray& rhs)
{
    return !lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const JsonArray& a)
{
    return a.serialize(os);
}
