//
//  jsonvalue.hpp
//  JsonLib
//
//  Created by Sylvan on 10/29/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#ifndef jsonvalue_hpp
#define jsonvalue_hpp

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

class JsonArray;
class JsonObject;

class JsonValue
{
public:
    enum Type {
        Null,
        Bool,
        Int,
        Double,
        String,
        Array,
        Object
    };
    
    JsonValue() = default;
    ~JsonValue() = default;
    
    JsonValue(bool);
    JsonValue(int);
    JsonValue(double);
    JsonValue(const char*);
    JsonValue(const std::string&);
    JsonValue(std::string&&);
    JsonValue(const JsonArray&);
    JsonValue(JsonArray&&);
    JsonValue(const JsonObject&);
    JsonValue(JsonObject&&);
    
    JsonValue(const JsonValue&);
    JsonValue(JsonValue&&) = default;
    
    JsonValue& operator=(const JsonValue&);
    JsonValue& operator=(JsonValue&&) = default;
    
    const JsonArray& to_array() const;
    const JsonObject& to_object() const;
    
    Type type() const noexcept { return type_; }
    bool equals(const JsonValue&) const;
    std::ostream& serialize(std::ostream&) const;
    
private:
    std::unique_ptr<JsonArray> array_ptr_;
    std::unique_ptr<JsonObject> object_ptr_;
    double double_val_ = 0.0;
    std::string string_val_;
    bool bool_val_ = false;
    Type type_ = Null;
};

bool operator==(const JsonValue& lhs, const JsonValue& rhs);
bool operator!=(const JsonValue& lhs, const JsonValue& rhs);

std::ostream& operator<<(std::ostream&, const JsonValue&);

#endif /* jsonvalue_hpp */
