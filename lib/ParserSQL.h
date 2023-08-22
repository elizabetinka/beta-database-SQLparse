//
// Created by Елизавета Кравченкова on 16.05.2023.
//

#pragma once

#include "working_with_string.h"

enum FUNC {
    CREATETABLE = 0, SELECT = 1, INSERT = 2, DELETE = 3, UPDATE = 4, DROP = 5, SelectJoin = 6
};
enum OPERATIONS {
    less, more, lessE, moreE, equal, no_equal
};

enum TYPES {
    BOOL, INT, FLOAT, DOUBLE, STRING
};

enum JoinType {
    INNER, LEFT, RIGHT
};

struct column {
    int type;
    int size_for_varchar;
    std::string name;
    std::string name_foreign_table;
    std::string name_foreign_key;
    bool IamPrimary = false;
    bool IamForeign = false;
    bool InotNull = false;
};

struct InsertSQL {
    std::string name_of_table;
    std::vector<std::string> values;
    std::vector<std::string> cols;
};

struct JoinSQL {
    std::string name_of_table1;
    std::string name_of_table2;
    int join_type;
    bool all_choose = false;
    std::string key_from_table1;
    std::string key_from_table2;
    std::vector<std::pair<bool, std::string>> choose; //true для первой таблицы
};

struct CreateTableSQL {
    std::string name_of_table;
    std::vector<column> cols;
};

struct DropSQL {
    std::string name_of_table;
};

struct SelectSQL {
    std::string from;
    std::vector<std::string> choose;
    bool all_choose = false;
    std::vector<std::pair<std::string, std::pair<int, std::string>>> trebovania;
    std::vector<bool> OR_conditionnal; //между i i+1
    bool not_in_the_begin = false;
};

struct UpdateSQL {
    std::string from;
    std::vector<std::pair<std::string, std::string>> for_set;
    std::vector<std::pair<std::string, std::pair<int, std::string>>> trebovania;
    std::vector<bool> OR_conditionnal; //между i i+1
    bool not_in_the_begin = false;
};

struct DeleteSQL {
    std::string name_of_table;
    std::vector<std::pair<std::string, std::pair<int, std::string>>> trebovania;
    std::vector<bool> OR_conditionnal; //между i i+1
    bool not_in_the_begin = false;
};

struct Result {
    int operations;
    std::variant<CreateTableSQL, InsertSQL, SelectSQL, UpdateSQL, DeleteSQL, DropSQL, JoinSQL> t;
};

class ParserSQL {
public:
    Result res;

    bool Parse(std::string& for_parse);

    bool ParseDelete(std::vector<std::string>& words);

    bool ParseJoin(std::vector<std::string>& words);

    bool ParseInsert(std::vector<std::string>& words);

    bool ParseSelect(std::vector<std::string>& words);

    bool ParseUpdate(std::vector<std::string>& words);

    bool ParseCreateTable(std::string& for_parse, std::string::size_type found);

    bool ParseNameCol(std::string_view& cols2, std::vector<column>& col_names_types);
};
