#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <vector>

class SQLServerConnection {
private:
    SQLHANDLE handle;
    SQLHANDLE connectionHandle;
    SQLHANDLE statementHandle;

public:
    SQLServerConnection() {
        handle = nullptr;
        connectionHandle = nullptr;
        statementHandle = nullptr;
    }

    ~SQLServerConnection() {
        SQLFreeHandle(SQL_HANDLE_STMT, statementHandle);
        SQLDisconnect(connectionHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, connectionHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, handle);
    }

    bool connect(const std::wstring& connectionString) {
        SQLRETURN result;

        result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &handle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配环境句柄" << std::endl;
            return false;
        }

        result = SQLSetEnvAttr(handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法设置环境属性" << std::endl;
            return false;
        }

        result = SQLAllocHandle(SQL_HANDLE_DBC, handle, &connectionHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配连接句柄" << std::endl;
            return false;
        }

        result = SQLDriverConnectW(connectionHandle, NULL, (SQLWCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法连接数据库" << std::endl;
            return false;
        }

        std::cout << "连接成功！" << std::endl;

        return true;
    }

    bool insertData(const std::string& tableName, const std::string& columnNames, const std::string& values) {
        SQLRETURN result;

        // 将 std::string 转换为 std::wstring
        std::wstring wTableName(tableName.begin(), tableName.end());
        std::wstring wColumnNames(columnNames.begin(), columnNames.end());
        std::wstring wValues(values.begin(), values.end());

        std::wstring query = L"INSERT INTO " + wTableName + L" (" + wColumnNames + L") VALUES (" + wValues + L")";

        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return false;
        }

        result = SQLExecDirectW(statementHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "插入数据失败" << std::endl;
            return false;
        }

        std::cout << "数据插入成功！" << std::endl;

        return true;
    }

    bool deleteData(const std::string& tableName, const std::string& condition) {
        SQLRETURN result;

        // 将 std::string 转换为 std::wstring
        std::wstring wTableName(tableName.begin(), tableName.end());
        std::wstring wCondition(condition.begin(), condition.end());

        std::wstring query = L"DELETE FROM " + wTableName + L" WHERE " + wCondition;

        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return false;
        }

        result = SQLExecDirectW(statementHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "删除数据失败" << std::endl;
            return false;
        }

        std::cout << "数据删除成功！" << std::endl;

        return true;
    }

    //修改数据库中某项数据
    bool updateData(const std::string& tableName, const std::string& columnNamesValues, const std::string& condition) {
        SQLRETURN result;

        // 将 std::string 转换为 std::wstring
        std::wstring wTableName(tableName.begin(), tableName.end());
        std::wstring wColumnNamesValues(columnNamesValues.begin(), columnNamesValues.end());
        std::wstring wCondition(condition.begin(), condition.end());

        std::wstring query = L"UPDATE " + wTableName + L" SET " + wColumnNamesValues + L" WHERE " + wCondition;

        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return false;
        }

        result = SQLExecDirectW(statementHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "更新数据失败" << std::endl;
            return false;
        }

        std::cout << "数据更新成功！" << std::endl;

        return true;
    }

    //读取数据库中的数据
    std::vector<std::vector<std::string>> readTableColumnsData(const std::string& tableName, const std::vector<std::string>& columnNames) {
        std::vector<std::vector<std::string>> data;

        SQLRETURN result;

        // 构建查询语句
        std::string columns;
        for (const auto& columnName : columnNames) {
            if (!columns.empty()) {
                columns += ", ";
            }
            columns += columnName;
        }
        std::string query = "SELECT " + columns + " FROM " + tableName;

        // 分配语句句柄
        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return data;
        }

        // 执行查询
        result = SQLExecDirectA(statementHandle, (SQLCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "查询数据失败" << std::endl;
            return data;
        }

        // 获取结果集元数据
        SQLSMALLINT columnCount;
        result = SQLNumResultCols(statementHandle, &columnCount);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "获取结果列数失败" << std::endl;
            return data;
        }

        // 逐行读取结果数据
        while (SQLFetch(statementHandle) == SQL_SUCCESS) {
            std::vector<std::string> row;
            for (int i = 1; i <= columnCount; ++i) {
                SQLCHAR columnValue[256];
                SQLLEN columnLength;
                result = SQLGetData(statementHandle, i, SQL_C_CHAR, columnValue, sizeof(columnValue), &columnLength);
                if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
                    std::cout << "获取结果数据失败" << std::endl;
                    return data;
                }
                row.emplace_back(reinterpret_cast<char*>(columnValue));
            }
            data.emplace_back(row);
        }

        return data;
    }

    //查找数据并直接返回
    std::vector<std::vector<std::string>> selectData(const std::string& tableName, const std::string& columnNames, const std::string& condition) {
        std::vector<std::vector<std::string>> data;

        SQLRETURN result;

        // 将 std::string 转换为 std::wstring
        std::wstring wTableName(tableName.begin(), tableName.end());
        std::wstring wColumnNames(columnNames.begin(), columnNames.end());
        std::wstring wCondition(condition.begin(), condition.end());

        std::wstring query = L"SELECT " + wColumnNames + L" FROM " + wTableName + L" WHERE " + wCondition;

        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return data;
        }

        result = SQLExecDirectW(statementHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "查询数据失败" << std::endl;
            return data;
        }

        SQLSMALLINT columnCount;
        result = SQLNumResultCols(statementHandle, &columnCount);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "获取结果列数失败" << std::endl;
            return data;
        }

        while (SQLFetch(statementHandle) == SQL_SUCCESS) {
            std::vector<std::string> row;
            for (int i = 1; i <= columnCount; ++i) {
                SQLCHAR columnValue[256];
                SQLLEN columnLength;
                result = SQLGetData(statementHandle, i, SQL_C_CHAR, columnValue, sizeof(columnValue), &columnLength);
                if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
                    std::cout << "获取结果数据失败" << std::endl;
                    return data;
                }
                row.emplace_back(reinterpret_cast<char*>(columnValue));
            }
            data.emplace_back(row);
        }

        return data;
    }
};


/* bool executeQuery(const std::wstring& query) {
        SQLRETURN result;

        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return false;
        }

        result = SQLExecDirectW(statementHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "查询失败" << std::endl;
            return false;
        }

        SQLWCHAR data[256];
        SQLLEN indicator;
        while (SQLFetch(statementHandle) == SQL_SUCCESS) {
            SQLGetData(statementHandle, 1, SQL_WCHAR, data, sizeof(data), &indicator);
            std::wcout << data << std::endl;
        }

        return true;
    }
    
     bool readData(const std::string& tableName, const std::string& columnNames) {
        SQLRETURN result;

        // 将 std::string 转换为 std::wstring
        std::wstring wTableName(tableName.begin(), tableName.end());
        std::wstring wColumnNames(columnNames.begin(), columnNames.end());

        std::wstring query = L"SELECT " + wColumnNames + L" FROM " + wTableName;

        result = SQLAllocHandle(SQL_HANDLE_STMT, connectionHandle, &statementHandle);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "无法分配语句句柄" << std::endl;
            return false;
        }

        result = SQLExecDirectW(statementHandle, (SQLWCHAR*)query.c_str(), SQL_NTS);
        if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO) {
            std::cout << "查询失败" << std::endl;
            return false;
        }

        SQLWCHAR data[256];
        SQLLEN indicator;
        while (SQLFetch(statementHandle) == SQL_SUCCESS) {
            SQLGetData(statementHandle, 1, SQL_WCHAR, data, sizeof(data), &indicator);
            std::wcout << data << std::endl;
        }

        return true;
    }*/