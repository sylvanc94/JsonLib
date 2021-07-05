//
//  jsonvalue.cpp
//  JsonLib
//
//  Created by Sylvan on 10/29/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#include "jsonarray.hpp"
#include "jsonobject.hpp"
#include "jsonvalue.hpp"
#include <iomanip>
#include <limits>

JsonValue::JsonValue(bool b) :
    bool_val_(b),
    type_(Bool)
{}

JsonValue::JsonValue(int v) :
    double_val_(v),
    type_(Int)
{}

JsonValue::JsonValue(double v) :
    double_val_(v),
    type_(Double)
{}

JsonValue::JsonValue(const char* s) :
    string_val_(s),
    type_(String)
{}

JsonValue::JsonValue(const std::string& s) :
    string_val_(s),
    type_(String)
{}

JsonValue::JsonValue(std::string&& s) :
    string_val_(s),
    type_(String)
{}

JsonValue::JsonValue(const JsonArray& a) :
    array_ptr_(std::make_unique<JsonArray>(a)),
    type_(Array)
{}

JsonValue::JsonValue(JsonArray&& a) :
    array_ptr_(std::make_unique<JsonArray>(std::move(a))),
    type_(Array)
{}

JsonValue::JsonValue(const JsonObject& o) :
    object_ptr_(std::make_unique<JsonObject>(o)),
    type_(Object)
{}

JsonValue::JsonValue(JsonObject&& o) :
    object_ptr_(std::make_unique<JsonObject>(std::move(o))),
    type_(Object)
{}

JsonValue::JsonValue(const JsonValue& other) :
    double_val_(other.double_val_),
    string_val_(other.string_val_),
    bool_val_(other.bool_val_),
    type_(other.type_)
{
    if (other.array_ptr_) {
        array_ptr_ = std::make_unique<JsonArray>(*other.array_ptr_);
    }
    if (other.object_ptr_) {
        object_ptr_ = std::make_unique<JsonObject>(*other.object_ptr_);
    }
}

JsonValue& JsonValue::operator=(const JsonValue& other)
{
    if (this != &other) {
        array_ptr_.reset();
        object_ptr_.reset();
        if (other.array_ptr_) {
            array_ptr_ = std::make_unique<JsonArray>(*other.array_ptr_);
        }
        if (other.object_ptr_) {
            object_ptr_ = std::make_unique<JsonObject>(*other.object_ptr_);
        }
        double_val_ = other.double_val_;
        string_val_ = other.string_val_;
        bool_val_ = other.bool_val_;
        type_ = other.type_;
    }
    return *this;
}

const JsonArray& JsonValue::to_array() const
{
    return *array_ptr_;
}

const JsonObject& JsonValue::to_object() const
{
    return *object_ptr_;
}

bool JsonValue::equals(const JsonValue& other) const
{
    if (!(type_ == other.type_ &&
          bool_val_ == other.bool_val_ &&
          double_val_ == other.double_val_ &&
          string_val_ == other.string_val_)) {
        return false;
    }
    
    if ((array_ptr_ && !other.array_ptr_) || (other.array_ptr_ && !array_ptr_)) {
        return false;
    }
    else if (array_ptr_ && other.array_ptr_) {
        if (!array_ptr_->equals(*other.array_ptr_)) {
            return false;
        }
    }
    
    if ((object_ptr_ && !other.object_ptr_) || (other.object_ptr_ && !object_ptr_)) {
        return false;
    }
    else if (object_ptr_ && other.object_ptr_) {
        if (!object_ptr_->equals(*other.object_ptr_)) {
            return false;
        }
    }
    
    return true;
}

std::ostream& JsonValue::serialize(std::ostream& os) const
{
    switch (type_) {
        case Bool:
            os << std::boolalpha << bool_val_;
            break;
        case Int:
            os << (int)double_val_;
            break;
        case Double:
            os << std::setprecision(std::numeric_limits<double>::digits10 + 1) << double_val_;
            break;
        case String:
            os << std::quoted(string_val_);
            break;
        case Array:
            os << *array_ptr_;
            break;
        case Object:
            os << *object_ptr_;
            break;
        default:
            os << "null";
            break;
    }
    return os;
}



// -------------------------------
// Related non-member functions
// -------------------------------

bool operator==(const JsonValue& lhs, const JsonValue& rhs)
{
    return lhs.equals(rhs);
}

bool operator!=(const JsonValue& lhs, const JsonValue& rhs)
{
    return !lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const JsonValue& v)
{
    return v.serialize(os);
}
