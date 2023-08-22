//
// Created by Елизавета Кравченкова on 27.04.2023.
//

#pragma once
#include "ParserSQL.h"
#include "working_with_string.h"
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <set>
#include <variant>
#include <algorithm>
#include <fstream>
#include <iomanip>


class varchar {
public:
    varchar() noexcept = default;

    varchar(const std::string& v, int64_t s) : value(v.substr(0, s)), size(s) {}

    void Set(const std::string& v) {
        value = v.substr(0, size);
    }

    std::string Get() const {
        return value;
    }

    bool operator==(const varchar& other) const {
        return value == other.value && size == other.size;
    }

    bool operator<(const varchar& other) const {
        return value < other.Get();
    }

    bool operator>(const varchar& other) const {
        return value > other.Get();
    }

    bool operator<=(const varchar& other) const {
        return value <= other.Get();
    }

    bool operator>=(const varchar& other) const {
        return value >= other.Get();
    }

private:
    std::string value;
    int64_t size = 1;
};

namespace std {

    template<>
    struct hash<varchar> {
        std::size_t operator()(const varchar& k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return hash<string>()(k.Get());
        }
    };

}

struct SQLtype {
    int T_enum;
    int size_for_varchar;
};

typedef std::variant<bool, int, float, double, varchar, std::string> object;

class Table;

class MyCoolDB;

class PrintTableClass;

class PrintTableClassJoin;

class MyRow {
    friend Table;
public:
    std::vector<object> data;
    std::vector<bool> not_null;

    MyRow() = default;
};

class Table {
    friend MyCoolDB;
    friend PrintTableClass;
    friend PrintTableClassJoin;
public:
    Table(std::vector<column>& types, std::vector<Table*>& foreign_tables, std::string table_name);

    Table() = default;

    bool IhaveThisColumn(const std::string& col) {
        return index_of_col.contains(col);
    }

    void add(const MyRow& row);

    void Print();

    void PrintRow(const MyRow& it);

    void PrintRow(const MyRow& it, std::vector<int64_t>& col_number);

    void Insert_with_col(const InsertSQL& value);

    void Insert(const InsertSQL& value);

    void Select(const SelectSQL& value);

    void Update(const UpdateSQL& value);

    void Delete(const DeleteSQL& value);


private:
    //       название столбца               значения которые принимает    номер
    std::unordered_map<std::string, std::unordered_map<object, std::vector<int64_t>>> idx;
    std::vector<std::string> name_of_col; //названия столбоцов
    std::unordered_map<std::string, int64_t> index_of_col; //для получения номера столбца по названию
    std::unordered_map<int64_t, MyRow> table; //получение строки по уникальному номеру
    std::vector<SQLtype> type; // типы хранимых значений по номеру стобца
    int64_t cout_of_col;
    std::vector<bool> primary_keys;
    std::vector<bool> not_null_keys;
    std::vector<Table*> foreign_keys;
    std::vector<std::string> name_of_foreign_keys;
    std::vector<Table*> children_keys;
    std::vector<std::string> name_of_children_keys;
    std::string table_name_;
    size_t unique_num = 0;

    object string_to_object(const std::string& value, int64_t i);

    std::set<int64_t>
    find_indexsORstr(const std::vector<std::pair<std::string, std::pair<int, std::string>>>& trebovania);

    std::set<int64_t>
    find_indexsANDstr(const std::vector<std::pair<std::string, std::pair<int, std::string>>>& trebovania);

    std::set<int64_t> find_indexs(const std::vector<std::pair<std::string, std::pair<int, std::string>>>& trebovania,
                                  const std::vector<bool>& OR_conditionnal, const bool not_in_the_begin);

    void SetNull(const std::string& key, const object& value);

    void SetNullall(const std::string& key);

    std::set<object> GetValuesInThisKey(const std::string& key);
};

class PrintTableClass {
    friend Table;
public:
    PrintTableClass(Table* table, const std::string& name);

    PrintTableClass() = default;

    void init(Table* table, const std::string& name);

    void Print(std::ofstream& fout);

    void PrintRow(const MyRow& it, std::ofstream& fout);

    void Print();

    void PrintRow(const MyRow& it);

    void PrintRowSomeCols(const MyRow& it, std::vector<int64_t>& col_number);

    void Print_Hello();

    void PrintHelloCols(std::vector<int64_t>& col_number);

private:
    std::string AddToMaxsize(std::string name) const;

    Table* table_;
    int max_widht = 5;
    std::string table_name;
};

class PrintTableClassJoin {
    friend Table;
public:
    PrintTableClassJoin(Table* table1, Table* table2, const std::string& name1, const std::string& name2) : table_1(
            table1), table_2(table2), table_name1(name1), table_name2(name2) {
        for (uint64_t i = 0; i < table_1->cout_of_col; ++i) {
            if (table_1->type[i].T_enum == STRING) {
                max_widht = std::max(max_widht, table_1->type[i].size_for_varchar);
            }
        }
        for (uint64_t i = 0; i < table_2->cout_of_col; ++i) {
            if (table_2->type[i].T_enum == STRING) {
                max_widht = std::max(max_widht, table_2->type[i].size_for_varchar);
            }
        }
    };

    PrintTableClassJoin() = default;

    void PrintHelloCols(const std::vector<std::pair<bool, std::string>>& col_number);

    std::string AddToMaxsize(std::string name) const;

private:
    Table* table_1;
    Table* table_2;
    int max_widht = 5;
    std::string table_name1;
    std::string table_name2;
};

class MyCoolDB {
public:
    void LoadToFile(const std::string& path);

    void PrintTable(const std::string& name);

    bool Parse(std::string& for_parsing);

private:
    //            имя таблицы
    std::unordered_map<std::string, Table> DB;

    void CreateTable(CreateTableSQL t);

    void GiveChildrens(CreateTableSQL& t);

    void Insert(const InsertSQL& value);

    void Select(const SelectSQL& value);

    void Delete(const DeleteSQL& value);

    void Drop(const DropSQL& value);

    void Update(const UpdateSQL& value);

    void Join(JoinSQL value);

    void InnerJoin(const JoinSQL& value, Table& Table1, Table& Table2, int type);

    void
    LeftJoin(const std::string& key_from_table1, const std::string& key_from_table2, const std::string& name_of_table1,
             const std::string& name_of_table2, Table& Table1, Table& Table2,
             std::vector<std::pair<bool, std::string>>& choose);

    void
    RightJoin(const std::string& key_from_table1, const std::string& key_from_table2, const std::string& name_of_table1,
              const std::string& name_of_table2, Table& Table1, Table& Table2,
              std::vector<std::pair<bool, std::string>>& choose);

    Table& GetTable(const std::string& name) {
        return DB[name];
    }
};

