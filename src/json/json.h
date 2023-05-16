#ifndef LB_BASE_JSON_PCH_H
#define LB_BASE_JSON_PCH_H

#include "../pch.h"
#include <jansson/src/jansson.h>

BEGIN_NAMESPACE_BASE_JSON

typedef void (*key_sorter)(const char **keys, size_t size);

class Json {
    
public:
    enum class Result {
      kSuccess = 0,
      kError,
    };
    
    enum class JsonType : int32_t {
      kObject,
      kArray,
      kString,
      kInteger,
      kReal,
      kTrue,
      kFalse,
      kNull
    };
    
    enum JsonParseFlag {
      // JSON_DISABLE_EOF_CHECK
      kFlagJsonDisableEofCheck = 0x2,
    };
    
    class iterator {
    public:
      iterator(json_t* ta, void* iter) : json_object_(ta), iter_(iter) {}

      bool operator!=(const iterator& other) const {
        return iter_ != other.iter_;
      }

      const char* first() const;

      Json second() const;

      iterator& operator++();

    private:
      json_t* json_object_ = nullptr;
      void* iter_ = nullptr;
    };
    
    ~Json();
    Json();
    Json(const Json& other);
    Json(JsonType type);

    Json(const char* value);
    Json(int value);
    Json(int64_t value);
    Json(double value);
    Json(bool value);
    Json(const std::string& value);

    Result Parse(const char* json_string, size_t len, size_t flags = JsonParseFlag::kFlagJsonDisableEofCheck);

    bool IsObject() const;

    bool IsArray() const;

    bool IsString() const;

    bool IsInteger() const;

    bool IsReal() const;

    bool IsNumber() const;

    bool IsTrue() const;

    bool IsFalse() const;

    bool IsBoolean() const;

    bool IsNull() const;

    bool Contain(const std::string& key) const;

    JsonType type() const;
    Json operator[](const char* key) const;

    Json operator[](const std::string& key) const;

    size_t Size() const;

    Json operator[](size_t index) const;
    Json& operator=(const Json& other);

    std::string AsString() const;

    int64_t AsInteger() const;

    bool AsBoolean() const;

    double AsReal() const;

    void Reset();

    operator int64_t() const {
      return AsInteger();
    }

    operator bool() const {
      return AsBoolean();
    }

    operator double() const {
      return AsReal();
    }

    bool SetJsonObject(const char* key, const Json& value);
    bool AddaJsonObject(const Json& value);

    std::string ToStyledString() const;

    iterator begin() const;

    iterator end() const;
    
private:
    void SetValue(json_t* json_value);
    json_t *json_;
};

END_NAMESPACE_BASE_JSON

#endif /* LB_BASE_JSON_PCH_H */
