//
//  jsonarray.hpp
//  JsonLib
//
//  Created by Sylvan on 10/29/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#ifndef jsonarray_hpp
#define jsonarray_hpp

#include <iostream>
#include <vector>
#include "jsonvalue.hpp"

class JsonObject;

class JsonArray
{
    using JsonVector = std::vector<JsonValue>;

public:
    JsonArray() = default;
    ~JsonArray() = default;
    
    explicit JsonArray(size_t count);
    JsonArray(size_t count, const JsonValue& value);
    template <class InputIt>
    JsonArray(InputIt first, InputIt last);
    JsonArray(std::initializer_list<JsonValue> ilist);
    
    JsonArray(const JsonArray&) = default;
    JsonArray(JsonArray&&) = default;
    
    JsonArray& operator=(const JsonArray&) = default;
    JsonArray& operator=(JsonArray&&) = default;
    JsonArray& operator=(std::initializer_list<JsonValue>);
    
    // assign
    template<class InputIt>
    void assign(InputIt first, InputIt last)            { data_.assign(first, last); }
    void assign(size_t count, const JsonValue& value)   { data_.assign(count, value); }
    void assign(std::initializer_list<JsonValue> ilist) { data_.assign(ilist); }
    
    // Lookup
    JsonValue& at(size_t pos)             { return data_.at(pos); }
    const JsonValue& at(size_t pos) const { return data_.at(pos); }

    JsonValue& operator[](size_t pos)             { return data_[pos]; }
    const JsonValue& operator[](size_t pos) const { return data_[pos]; }

    JsonValue& front()             { return data_.front(); }
    const JsonValue& front() const { return data_.front(); }

    JsonValue& back()             { return data_.back(); }
    const JsonValue& back() const { return data_.back(); }
    
    // Iterators
    JsonVector::iterator       begin()  noexcept       { return data_.begin();  }
    JsonVector::const_iterator begin()  const noexcept { return data_.begin();  }
    JsonVector::const_iterator cbegin() const noexcept { return data_.cbegin(); }
    JsonVector::iterator       end()    noexcept       { return data_.end();    }
    JsonVector::const_iterator end()    const noexcept { return data_.end();    }
    JsonVector::const_iterator cend()   const noexcept { return data_.cend();   }

    // Reverse iterators
    JsonVector::reverse_iterator       rbegin()  noexcept       { return data_.rbegin();  }
    JsonVector::const_reverse_iterator rbegin()  const noexcept { return data_.rbegin();  }
    JsonVector::const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }
    JsonVector::reverse_iterator       rend()    noexcept       { return data_.rend();    }
    JsonVector::const_reverse_iterator rend()    const noexcept { return data_.rend();    }
    JsonVector::const_reverse_iterator crend()   const noexcept { return data_.crend();   }
    
    // Capacity
    bool empty() const noexcept  { return data_.empty(); }
    size_t size() const noexcept { return data_.size(); }
    size_t max_size() const noexcept { return data_.max_size(); }
    size_t capacity() const noexcept { return data_.capacity(); }
    void reserve(size_t new_cap) { data_.reserve(new_cap); }
    void shrink_to_fit() { data_.shrink_to_fit(); }
    
    // inserts
    JsonVector::iterator insert(JsonVector::const_iterator pos, const JsonValue& value) { return data_.insert(pos, value); }
    JsonVector::iterator insert(JsonVector::const_iterator pos, JsonValue&& value) { return data_.insert(pos, std::move(value)); }
    JsonVector::iterator insert(JsonVector::const_iterator pos, size_t count, const JsonValue& value) { return data_.insert(pos, count, value); }
    template<class InputIt>
    JsonVector::iterator insert(JsonVector::const_iterator pos, InputIt first, InputIt last) { return data_.insert(pos, first, last); }
    JsonVector::iterator insert(JsonVector::const_iterator pos, std::initializer_list<JsonValue> ilist) { return data_.insert(pos, ilist); }

    // erases
    JsonVector::iterator erase(JsonVector::const_iterator pos) { return data_.erase(pos); }
    JsonVector::iterator erase(JsonVector::const_iterator first, JsonVector::const_iterator last) { return data_.erase(first, last); }
    
    void clear() noexcept { data_.clear(); }
    
    void push_back(const JsonValue& val) { data_.push_back(val); }
    void push_back(JsonValue&& val)      { data_.push_back(std::move(val)); }
    
    void pop_back() { data_.pop_back(); }
    
    bool equals(const JsonArray&) const;
    std::ostream& serialize(std::ostream&) const;
    
private:
    JsonVector data_;
};

bool operator==(const JsonArray& lhs, const JsonArray& rhs);
bool operator!=(const JsonArray& lhs, const JsonArray& rhs);

std::ostream& operator<<(std::ostream&, const JsonArray&);

#endif /* jsonarray_hpp */
