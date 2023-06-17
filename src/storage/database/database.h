#ifndef LB_BASE_STORAGE_DATABASE_PCH_H
#define LB_BASE_STORAGE_DATABASE_PCH_H

#include "../../pch.h"
#include <sqlite3.h>
#include <platform/platform.h>

BEGIN_NAMESPACE_BASE_STORAGE_DATABASE

class DataBase {
    
public:
    DataBase(const std::string& path);
    ~DataBase();
    
    bool CreateTable(const std::string& schema);
    
    bool Insert(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var);
    bool Update(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var);
    bool Delete(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var);
    bool Select(const std::string& table_name, BASE_PLATFORM::Platform::Var& row_var);
    
private:
    bool Verify(BASE_PLATFORM::Platform::Var& row_var);
    bool JointSQL(BASE_PLATFORM::Platform::Var& row_var, std::string& sql, std::vector<BASE_PLATFORM::Platform::Var>& values);
    bool PrepareStmt(sqlite3_stmt** stmt, const std::string& sql, std::vector<BASE_PLATFORM::Platform::Var>& values);
    
private:
    std::string path_;
    sqlite3* db_;
};

END_NAMESPACE_BASE_STORAGE_DATABASE

#endif /* LB_BASE_STORAGE_DATABASE_PCH_H */
