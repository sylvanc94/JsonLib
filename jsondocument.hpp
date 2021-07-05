//
//  jsondocument.hpp
//  JsonLib
//
//  Created by Sylvan on 11/1/20.
//  Copyright © 2020 Sylvan Canales. All rights reserved.
//

#ifndef jsondocument_hpp
#define jsondocument_hpp

#include <iostream>
#include <string>
#include "jsonarray.hpp"
#include "jsonobject.hpp"

class JsonDocument
{
public:
    enum Format {
        Compact,
        Indented
    };
    
    JsonDocument(Format format = Compact);
    JsonDocument(const JsonArray&, Format format = Compact);
    JsonDocument(const JsonObject&, Format format = Compact);
    
    bool isValid() const noexcept { return parseOk_; }
    
    void setFormat(Format);
    void setMaxIndent(int);
    void setArray(const JsonArray&);
    void setObject(const JsonObject&);
    
    std::ostream& to_json(std::ostream& os) const;
    void from_json(std::istream& is);
    
    static JsonDocument from_json(const std::string &);
    
private:
    JsonArray array_;
    JsonObject object_;
    JsonValue::Type type_ = JsonValue::Null;
    Format format_;
    bool parseOk_ = true;
    int max_indent_ = 16;
};

std::ostream& operator<<(std::ostream& os, const JsonDocument& doc);
void operator>>(std::istream& is, JsonDocument& doc);

#endif /* jsondocument_hpp */
