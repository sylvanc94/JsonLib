//
//  jsondocument.cpp
//  JsonLib
//
//  Created by Sylvan on 11/1/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#include "jsondocument.hpp"

std::ostream& buildJson(std::ostream& os, const JsonValue& val, int tabLevel);
std::ostream& buildJson(std::ostream& os, const JsonArray& val, int tabLevel);
std::ostream& buildJson(std::ostream& os, const JsonObject& val, int tabLevel);

JsonValue parseValue(std::istream&, bool& ok);
JsonArray parseArray(std::istream&, bool& ok);
JsonObject parseObject(std::istream&, bool& ok);
JsonValue parseLiteral(std::istream&, bool& ok);
std::string parseString(std::istream&, bool& ok);
std::string parseEscape(std::istream&, bool& ok);
JsonValue parseNumeric(std::istream&, bool& ok);

JsonDocument::JsonDocument(Format format) :
    format_(format)
{}

JsonDocument::JsonDocument(const JsonArray& a, Format format) :
    array_(a),
    type_(JsonValue::Array),
    format_(format)
{}

JsonDocument::JsonDocument(const JsonObject& o, Format format) :
    object_(o),
    type_(JsonValue::Object),
    format_(format)
{}

void JsonDocument::setFormat(Format format)
{
    format_ = format;
}

void JsonDocument::setMaxIndent(int max)
{
    max_indent_ = max;
}

void JsonDocument::setArray(const JsonArray& a)
{
    type_ = JsonValue::Array;
    array_ = a;
    object_.clear();
}

void JsonDocument::setObject(const JsonObject& o)
{
    type_ = JsonValue::Object;
    object_ = o;
    array_.clear();
}

std::ostream& JsonDocument::to_json(std::ostream &os) const
{
    if (format_ == Compact) {
        if (type_ == JsonValue::Array) {
            os << array_;
        }
        else if (type_ == JsonValue::Object) {
            os << object_;
        }
        else {
            os << "{}";
        }
    }
    else {
        if (type_ == JsonValue::Array) {
            return buildJson(os, array_, 0);
        }
        else if (type_ == JsonValue::Object) {
            return buildJson(os, object_, 0);
        }
        else {
            os << "{\n}";
        }
    }
    return os;
}

//std::istream& JsonDocument::from_json(std::istream& is)
void JsonDocument::from_json(std::istream &is)
{
    // Start with either a JsonObject or a JsonArray
    char c = is.peek();
    if (is.good()) {
        switch (c) {
            case '[':
                type_ = JsonValue::Array;
                array_ = parseArray(is, parseOk_);
                break;
            case '{':
                type_ = JsonValue::Object;
                object_ = parseObject(is, parseOk_);
                break;
            default:
                type_ = JsonValue::Null;
                array_.clear();
                object_.clear();
                parseOk_ = false;
                break;
        }
    }
}


std::ostream& operator<<(std::ostream& os, const JsonDocument& doc)
{
    return doc.to_json(os);
}

void operator>>(std::istream& is, JsonDocument& doc)
{
    doc.from_json(is);
}


void insertTab(std::ostream& os, int n)
{
    while (n--) {
        os << '\t';
    }
}

std::ostream& buildJson(std::ostream& os, const JsonValue& val, int tabLevel)
{
    switch (val.type()) {
        case JsonValue::Array:
            buildJson(os, val.to_array(), tabLevel);
            break;
        case JsonValue::Object:
            buildJson(os, val.to_object(), tabLevel);
            break;
        default:
            os << val;
    }
    return os;
}

std::ostream& buildJson(std::ostream& os, const JsonArray& array, int tabLevel)
{
    os << "[\n";
    size_t i = 0;
    size_t sz = array.size();
    for (const JsonValue& value : array) {
        insertTab(os, tabLevel + 1);
        buildJson(os, value, tabLevel + 1);
        if (i++ < sz - 1) {
            os << ",";
        }
        os << "\n";
    }
    insertTab(os, tabLevel);
    return os << "]";
}

std::ostream& buildJson(std::ostream& os, const JsonObject& object, int tabLevel)
{
    os << "{\n";
    size_t i = 0;
    size_t sz = object.size();
    for (const auto& pr : object) {
        insertTab(os, tabLevel + 1);
        os << "\"" << pr.first << "\": ";
        buildJson(os, pr.second, tabLevel + 1);
        if (i++ < sz - 1) {
            os << ",";
        }
        os << "\n";
    }
    insertTab(os, tabLevel);
    return os << "}";
}


// Consume whitespace
void eatWs(std::istream& is)
{
    for (;;) {
        char c = is.peek();
        if (is.good() && std::isspace(c)) {
            c = is.get();
        }
        else {
            break;
        }
    }
}

JsonValue parseValue(std::istream& is, bool& ok)
{
    char c = is.peek();
    if (is.good()) {
        switch (c) {
            case '[':
                // start new array
                return parseArray(is, ok);
                break;
            case '{':
                // start new object
                return parseObject(is, ok);
                break;
            case '"':
                // start a string
                return parseString(is, ok);
                break;
            case ']':
                // end of this value (empty array)
                break;
            case '}':
                // not expected
                ok = false;
                break;
            case 't':   // true (fallthrough)
            case 'f':   // false (fallthrough)
            case 'n':   // null (fallthrough)
                return parseLiteral(is, ok);
                break;
            default:
                return parseNumeric(is, ok);
                // Attempt to process numeric value
                break;
        }
    }
    
    return JsonValue();
}

// Make JsonArrays, JsonObjects from the stream
JsonArray parseArray(std::istream& is, bool& ok)
{
    JsonArray array;
    
    // [ true , +2.5, "a string" ]
    
    is.get();   // opening [
    
    char c;
    
    for (;;) {
        eatWs(is);
        
        array.push_back(parseValue(is, ok));
        
        eatWs(is);
        
        // If next char is ',' keep going in the loop.
        // If next char is ']' then we're done with this array
        c = is.get();
        if (c == ']') {
            break;
        }
    }
    
    return array;
}

JsonObject parseObject(std::istream& is, bool& ok)
{
    JsonObject object;
    
    // { "key" : value, "key" : value, }
    
    is.get();   // opening {
    
    char c;
    
    for (;;) {
        eatWs(is);
        
        // Get key
        std::string key = parseString(is, ok);
        
        eatWs(is);
        
        // Get :
        c = is.get();
        if (!is.good() || c != ':') {
            // Not a proper object. What should we do?
            // Consume the stream until the next closing '}' brace, if available, then return what we've parsed so far
            while (is.good()) {
                c = is.get();
                if (c == '}') {
                    break;
                }
            }
            ok = false;
            return object;
        }
        
        eatWs(is);
        
        // Get value and add it to the JsonObject
        object[key] = parseValue(is, ok);
        
        eatWs(is);
        
        // If next char is ',' keep going in the loop.
        // If next char is '}' then we're done with this object
        c = is.get();
        if (c == '}') {
            break;
        }
        if (c != ',') {
            // Not a proper object. What should we do?
            // Consume the stream until the next closing '}' brace, if available, then return what we've parsed so far
            while (is.good()) {
                c = is.get();
                if (c == '}') {
                    break;
                }
            }
            ok = false;
            return object;
        }
    }
    
    return object;
}

std::string parseString(std::istream& is, bool& ok)
{
    std::string s;
    char c = is.get();  // opening "
    for (;;) {
        c = is.peek();
        if (is.good()) {
            if (c == '\\') {
                s += parseEscape(is, ok);
            }
            else if (c == '"') {
                is.get();   // closing "
                break;
            }
            else {
                s.push_back(is.get());
            }
        }
        else {
            break;
        }
    }
    return s;
}

std::string parseEscape(std::istream& is, bool& ok)
{
    std::string s;
    is.get();   // initial '\' to start the escape
    char c = is.get();
    if (is.good()) {
        switch (c) {
            case '"':
                s.push_back('"');
                break;
            case '\\':
                s.push_back('\\');
                break;
            case '/':
                s.push_back('/');
                break;
            case 'b':
                s.push_back('\b');
                break;
            case 'f':
                s.push_back('\f');
                break;
            case 'n':
                s.push_back('\n');
                break;
            case 'r':
                s.push_back('\r');
                break;
            case 't':
                s.push_back('\t');
                break;
            default:
                ok = false;
                break;
        }
    }
    return s;
}

JsonValue parseLiteral(std::istream& is, bool& ok)
{
    // read up to: whitespace, comma, ], }, or EOF (or other failure).
    std::string word;
    char c;
    for (;;) {
        c = is.peek();
        if (is.good()) {
            if (std::isspace(c) || c == ',' || c == ']' || c == '}') {
                break;
            }
            word.push_back(is.get());
        }
        else {
            break;
        }
    }
    
    if (word == "true") {
        return true;
    }
    else if (word == "false") {
        return false;
    }
    else if (word == "null") {
        return JsonValue();
    }
    ok = false;
    return JsonValue();
}

JsonValue parseNumeric(std::istream& is, bool& ok)
{
    std::string word;
    char c;
    for (;;) {
        c = is.peek();
        if (is.good()) {
            if (c == ',' || c == ']' || c == '}') {
                break;
            }
            word.push_back(is.get());
        }
        else {
            break;
        }
    }
    
    double v = 0.0;
    try {
        v = std::stod(word);
    }
    catch (std::exception &) {
        ok = false;
        return JsonValue(); // Return a null-type JsonValue if the numeric value couldn't be parsed
    }
    
    return JsonValue(v);
}


JsonDocument JsonDocument::from_json(const std::string& s)
{
    JsonDocument doc;
    return doc;
}
