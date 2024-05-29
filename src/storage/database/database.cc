//
//  storage_memory.cpp
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "database.h"
#include <string/string.h>
#include <device/device.h>

BEGIN_NAMESPACE_BASE_STORAGE_DATABASE

DataBase& DataBase::DefaultInstance() {
    static const std::string database_name = "sdk_db";
    static DataBase data_base = BASE_STORAGE_DATABASE::DataBase(BASE_DEVICE::Device::DefaultInstance().GetFile()->GetCachesPath() + "/" + database_name);
    return data_base;
}

DataBase::~DataBase() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

DataBase::DataBase(const std::string& path):
path_(path) {
    int32_t config_ret = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    if (config_ret != SQLITE_OK) {
        printf("sqlite config failed");
    }
    
    int ret = sqlite3_open_v2(path.c_str(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (ret != SQLITE_OK) {
        printf("open sqlite3 failed");
        sqlite3_close(db_);
    }
}

bool DataBase::CreateTable(const std::string& schema) {
    char* err_msg = nullptr;
    int32_t ret = sqlite3_exec(db_, schema.c_str(), nullptr, nullptr, &err_msg);
    if (ret != SQLITE_OK) {
       std::cerr << "Error creating table: " << err_msg << std::endl;
       sqlite3_free(err_msg);
       sqlite3_close(db_);
       return -1;
    }
    return true;
}

bool DataBase::Insert(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var) {
    if (!Verify(row_var)) {
        return false;
    }
    
    std::string sql = "INSERT INTO " + table_name + " ";
    std::vector<BASE_PLATFORM::Platform::Var> values;
    if (!JointSQL(row_var, sql, values)) {
        return false;
    }
    
    sqlite3_stmt* stmt;
    if (!PrepareStmt(&stmt, sql, values)) {
        return false;
    }
    
    int ret_step = SQLITE_OK;
    do {
        ret_step = sqlite3_step(stmt);
        
        if (ret_step == SQLITE_BUSY) {
            printf("exe busy, check sql");
            break;
        }
        
        if (ret_step == SQLITE_ERROR) {
            sqlite3_errmsg(db_);
            printf("exe error, check sql");
            break;
        }
        
        if (ret_step == SQLITE_MISUSE) {
            printf("exe operate error, check sql");
            break;
        }
        
    } while (ret_step == SQLITE_ROW);
    
    if (ret_step != SQLITE_OK && ret_step != SQLITE_DONE) {
        printf("exe failed, check sql");
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool DataBase::Update(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var) {
    if (!Verify(row_var)) {
        return false;
    }
    
    std::string sql = "update" + table_name + " ";
    std::vector<BASE_PLATFORM::Platform::Var> values;
    if (!JointSQL(row_var, sql, values)) {
        return false;
    }
    
    sqlite3_stmt* stmt;
    if (PrepareStmt(&stmt, sql, values)) {
        return false;
    }
}

bool DataBase::Delete(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var) {
    if (!Verify(row_var)) {
        return false;
    }
    
    std::string sql = "update" + table_name + " ";
    std::vector<BASE_PLATFORM::Platform::Var> values;
    if (!JointSQL(row_var, sql, values)) {
        return false;
    }
    
    sqlite3_stmt* stmt;
    if (PrepareStmt(&stmt, sql, values)) {
        return false;
    }
}

bool DataBase::Select(const std::string& table_name, BASE_PLATFORM::Platform::Var& condition_var, BASE_PLATFORM::Platform::Var& row_var) {
    if (!Verify(condition_var)) {
        return false;
    }
    
    std::ostringstream sql;
    sql << "SELECT ";
    
    auto map = condition_var.GetMap();
    std::vector<std::string> fields;
    for (auto i = map->begin(); i != map->end(); i++) {
        auto field = i->first;
        auto value = i->second;
        fields.push_back(field);
    }
    
    auto fields_string = BASE_STRING::Join(fields, ", ");
    if (fields_string == "") {
        fields_string = "*";
    }
    sql << fields_string << " FROM " << table_name;
    
    sqlite3_stmt* stmt;
    std::vector<BASE_PLATFORM::Platform::Var> values;
    if (!PrepareStmt(&stmt, sql.str().c_str(), values)) {
        return false;
    }
    
    int ret_step = SQLITE_OK;
    int32_t index = 0;
    do {
        ret_step = sqlite3_step(stmt);
        
        if (ret_step == SQLITE_BUSY) {
            printf("exe busy, check sql");
            break;
        }
        
        if (ret_step == SQLITE_ERROR) {
            sqlite3_errmsg(db_);
            printf("exe error, check sql");
            break;
        }
        
        if (ret_step == SQLITE_MISUSE) {
            printf("exe operate error, check sql");
            break;
        }
        
        if (fields.size() > index) {
            auto key = fields[index];
            if (map->find(key) == map->end()) {
                continue;
            }
            auto value = map->operator[](key);
            int32_t ret = SQLITE_OK;
            switch (value.GetType()) {
                case BASE_PLATFORM::Platform::Var::Type::kTypeInt: {
                    int32_t int_value = sqlite3_column_int(stmt, index);
                    value.SetInt32(int_value);
                }
                    break;
                case BASE_PLATFORM::Platform::Var::Type::kTypeDouble: {
                    double double_value = sqlite3_column_double(stmt, index);
                    value.SetDouble(double_value);
                }
                    break;
                case BASE_PLATFORM::Platform::Var::Type::kTypeString: {
                    const unsigned char *text = sqlite3_column_text(stmt, index);
                    std::string string(reinterpret_cast<const char *>(text));
                    value.SetString(string);
                }
                    break;
                case BASE_PLATFORM::Platform::Var::Type::kTypeInt64: {
                    int64_t int64_value = sqlite3_column_int64(stmt, index);
                    value.SetInt64(int64_value);
                }
                    break;
                    
                default:
                    break;
            }
            map->operator[](key) = value;
        }
        index++;
    } while (ret_step == SQLITE_ROW);
    
    if (ret_step != SQLITE_OK && ret_step != SQLITE_DONE) {
        printf("exe failed, check sql");
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool DataBase::Verify(BASE_PLATFORM::Platform::Var& row_var) {
    if (!db_) {
        return false;
    }
    if (row_var.GetType() != BASE_PLATFORM::Platform::Var::Type::kTypeMap) {
        return false;
    }
    return true;
}

bool DataBase::JointSQL(BASE_PLATFORM::Platform::Var& row_var, std::string& sql, std::vector<BASE_PLATFORM::Platform::Var>& values) {
    std::string prepare_sql;
    std::vector<std::string> fields;
    std::vector<std::string> placeholder;
    auto map = row_var.GetMap();
    for (auto i = map->begin(); i != map->end(); i++) {
        auto field = i->first;
        auto value = i->second;
        
        fields.push_back(field);
        values.push_back(value);
        placeholder.push_back("?");
    }
    
    auto fields_string = BASE_STRING::Join(fields, " ,");
    auto placeholder_string = BASE_STRING::Join(placeholder, " ,");
    prepare_sql += "(" + fields_string + ") ";
    prepare_sql += " VALUES ";
    prepare_sql += "( " + placeholder_string + ") ";
    
    sql += prepare_sql;
    return true;
}

bool DataBase::PrepareStmt(sqlite3_stmt** stmt, const std::string& sql, std::vector<BASE_PLATFORM::Platform::Var>& values) {
    int ret = sqlite3_prepare_v2(db_, sql.c_str(), -1, stmt, nullptr);
    if (ret != SQLITE_OK) {
        return false;
    }
    
    for (int i = 0; i < values.size(); i++) {
        auto value = values[i];
        
        int32_t sqlite3_index = i + 1;
        int32_t ret = SQLITE_OK;
        switch (value.GetType()) {
            case BASE_PLATFORM::Platform::Var::Type::kTypeInt: {
                ret = sqlite3_bind_int(*stmt, sqlite3_index, value.GetInt32());
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeDouble: {
                ret = sqlite3_bind_double(*stmt, sqlite3_index, value.GetDouble());
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeString: {
                auto text = value.GetString();
                ret = sqlite3_bind_text(*stmt, sqlite3_index, text.c_str(), -1, SQLITE_STATIC);
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeInt64: {
                ret = sqlite3_bind_int64(*stmt, sqlite3_index, value.GetInt64());
            }
                break;
                
            default:
                break;
        }
        
        if (ret != SQLITE_OK) {
            return false;
        }
    }
    return true;
}

END_NAMESPACE_BASE_STORAGE_DATABASE
