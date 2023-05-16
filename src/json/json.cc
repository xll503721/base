//
//  oneten_ad_sdk.cpp
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "json.h"

BEGIN_NAMESPACE_BASE_JSON

Json::Json() {
    json_ = json_null();
}

Json::Json(JsonType type) {
    if (type == JsonType::kArray) {
      json_ = json_array();
    } else if (type == JsonType::kObject) {
      json_ = json_object();
    } else {
      json_ = json_null();
    }
}

Json::Json(int value) {
    json_ = json_integer(value);
}

Json::JsonType Json::type() const {
    return (Json::JsonType)json_typeof(json_);
}

Json::Json(const char* value) {
    json_ = json_string(value);
}

Json::Json(int64_t value) {
    json_ = json_integer(value);
}

Json::Json(bool value) {
    json_ = json_boolean(value);
}

Json::Json(const std::string& value) {
    json_ = json_stringn(value.c_str(), value.length());
}

Json::Json(double value) {
    json_ = json_real(value);
}


const char* Json::iterator::first() const {
    return json_object_iter_key(iter_);
}

Json Json::iterator::second() const {
    json_t* value = json_object_iter_value(iter_);
    Json result;
    result.SetValue(value);
    return result;
}

Json::iterator& Json::iterator::operator++() {
    iter_ = json_object_iter_next(json_object_, iter_);
    return (*this);
}

Json::iterator Json::begin() const {
    return { json_, json_object_iter(json_) };
}

Json::iterator Json::end() const {
    return { json_, nullptr };
}

void Json::Reset() {
    json_decref(json_);
    json_ = json_null();
}

Json::~Json() {
    json_decref(json_);
    json_ = nullptr;
}

Json::Result Json::Parse(const char* json_string, size_t len, size_t flags) {
    Reset();
    std::string str(json_string, len);
    json_ = json_loads(str.c_str(), flags, nullptr);
    if (nullptr == json_) {
        return Result::kError;
    }
    return Result::kSuccess;
}

bool Json::IsObject() const {
    return json_is_object(json_);
}

bool Json::IsArray() const {
    return json_is_array(json_);
}

bool Json::IsString() const {
    return json_is_string(json_);
}

bool Json::IsInteger() const {
    return json_is_integer(json_);
}

bool Json::IsReal() const {
    return json_is_real(json_);
}

bool Json::IsNumber() const {
    return json_is_number(json_);
}

bool Json::IsTrue() const {
    return json_is_true(json_);
}

bool Json::IsFalse() const {
    return json_is_false(json_);
}

bool Json::IsBoolean() const {
    return json_is_boolean(json_);
}

bool Json::IsNull() const {
    return json_is_null(json_);
}

bool Json::Contain(const std::string& key) const {
    json_t* json_node = json_object_get(json_, key.c_str());
    if (nullptr == json_node) {
        return false;
    }
    return true;
}

void Json::SetValue(json_t* json_value) {
    json_incref(json_value);
    json_decref(json_);
    if (json_value) {
        json_ = json_value;
    } else {
        json_ = json_null();
    }
}

Json Json::operator[](const char* key) const {
    json_t* json_node = json_object_get(json_, key);
    Json result;
    result.SetValue(json_node);
    return result;
}

Json Json::operator[](const std::string& key) const {
    return (*this)[key.c_str()];
}


size_t Json::Size() const {
    return json_array_size(json_);
}

Json Json::operator[](size_t index) const {
    json_t* json_node = json_array_get(json_, index);
    Json result;
    result.SetValue(json_node);
    return result;
}


Json::Json(const Json& other) {
    json_ = other.json_;
    json_incref(json_);
}
Json& Json::operator=(const Json& other) {
    if (this == &other) {
      return *this;
    }
    Reset();
    json_ = other.json_;
    json_incref(json_);
    return *this;
}

std::string Json::AsString() const {
    const char* value = json_string_value(json_);
    return value == nullptr ? "" : value;
}

int64_t Json::AsInteger() const {
    return static_cast<int64_t>(json_integer_value(json_));
}

bool Json::AsBoolean() const {
    return json_boolean_value(json_);
}

double Json::AsReal() const {
    return json_real_value(json_);
}

std::string Json::ToStyledString() const {
    size_t flags = JSON_ENCODE_ANY;
    char* styled_string = json_dumps(json_, flags);
    if (nullptr == styled_string) {
        return "";
    }
    std::string result = styled_string;
    free(styled_string);
    return result;
}

bool Json::SetJsonObject(const char* key, const Json& value) {
    if (json_is_null(json_)) {
        json_ = json_object();
    }
    if (!json_is_object(json_)) {
        return false;
    }
    json_object_set(json_, key, value.json_);
    return true;
}

bool Json::AddaJsonObject(const Json& value) {
    if (json_is_null(json_)) {
        json_ = json_array();
    }
    if (!json_is_array(json_)) {
        return false;
    }
    json_array_append(json_, value.json_);
    return true;
}

END_NAMESPACE_BASE_JSON
