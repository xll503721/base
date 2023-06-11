//
//  storage_memory.cpp
//  SDKCommon
//
//  Created by li zhixuan on 2023/3/16.
//

#include "database.h"
#include <string/string.h>

BEGIN_NAMESPACE_BASE_STORAGE_DATABASE

DataBase::DataBase(const std::string& path):path_(path) {
    int ret = sqlite3_open_v2(path, &db_, 0, nullptr);
    if (ret != SQLITE_OK) {
        printf("open sqlite3 failed");
        sqlite3_close(db_);
    }
}

bool DataBase::Insert(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var) {
    if (!Verify(row_var)) {
        return false;
    }
    
    std::string sql = "insert" + table_name + " ";
    std::vector<BASE_PLATFORM::Platform::Var> values;
    if (!JointSQL(row_var, sql, values)) {
        return false;
    }
    
    sqlite3_stmt* stmt;
    if (PrepareStmt(&stmt, sql, values)) {
        return false;
    }
    
    int ret_step = -1;
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
    
    if (ret_step != SQLITE_OK || ret_step != SQLITE_DONE) {
        printf("exe failed, check sql");
        return false;
    }
    
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

bool DataBase::Query(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var) {
    
}

bool DataBase::Verify(BASE_PLATFORM::Platform::Var& row_var) {
    if (!db_) {
        return false;
    }
    if (row_var.GetType() != BASE_PLATFORM::Platform::Var::Type::kTypeMap) {
        return false;
    }
}

bool DataBase::JointSQL(BASE_PLATFORM::Platform::Var& row_var, std::string& sql, std::vector<BASE_PLATFORM::Platform::Var>& values) {
    std::string prepare_sql;
    std::vector<std::string> fields;
    std::string placeholder;
    auto map = row_var.GetMap();
    for (auto i = map->begin(); i != map->end(); i++) {
        auto field = i->first;
        auto value = i->second;
        
        if (typeid(field).name() || typeid(value).name()) {
            return false;
        }
        
        fields.push_back(field);
        values.push_back(value);
        
        placeholder += "? ";
    }
    
    auto fields_string = BASE_STRING::Join(fields, " ,");
    prepare_sql += "(" + fields_string + ") ";
    prepare_sql += "(" + placeholder + ") ";
    return true;
}

bool DataBase::PrepareStmt(sqlite3_stmt** stmt, const std::string& sql, std::vector<BASE_PLATFORM::Platform::Var>& values) {
    int ret = sqlite3_prepare_v2(db_, sql, sql.size(), &stmt, nullptr);
    if (ret != SQLITE_OK) {
        return false;
    }
    
    for (int i = 0; i < values.size(); i++) {
        auto value = values[i];
        switch (value.GetType()) {
            case BASE_PLATFORM::Platform::Var::Type::kTypeInt: {
                sqlite3_bind_int(stmt, i, value.GetInt32());
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeDouble: {
                sqlite3_bind_double(stmt, i, value.GetDouble());
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeString: {
                auto text = value.GetString();
//                sqlite3_bind_text(stmt, i, text, static_cast<int>(text.size()), SQLITE_STATIC);
            }
                break;
            case BASE_PLATFORM::Platform::Var::Type::kTypeInt64: {
                sqlite3_bind_int64(stmt, i, value.GetInt64());
            }
                break;
                
            default:
                break;
        }
    }
}

END_NAMESPACE_BASE_STORAGE_DATABASE
