//
// Created by Елизавета Кравченкова on 16.05.2023.
//

#include "ParserSQL.h"

bool ParserSQL::Parse(std::string& for_parse) {
    std::vector<std::string> words = split_string_all_sep(std::string(for_parse));
    if (words[0] == "CREATE") {
        if (words.size() == 1) {
            return false;
        }
        if (words[1] == "TABLE") {
            std::string::size_type found = for_parse.find("TABLE");
            //"CREATE TABLE"
            if (found != std::string::npos) {
                res.operations = CREATETABLE;
                return ParseCreateTable(for_parse, found);
            }
            return false;

        }
        return false;
    } else if (words[0] == "INSERT") {
        if (words.size() == 1) {
            return false;
        }
        if (words[1] == "INTO") {
            res.operations = INSERT;
            return ParseInsert(words);
        }
        return false;
    } else if (words[0] == "SELECT") {
        if (for_parse.find("JOIN") != std::string::npos) {
            res.operations = SelectJoin;
            return ParseJoin(words);
        }
        res.operations = SELECT;
        return ParseSelect(words);
    } else if (words[0] == "UPDATE") {
        res.operations = UPDATE;
        return ParseUpdate(words);
    } else if (words[0] == "DELETE") {
        res.operations = DELETE;
        return ParseDelete(words);
    } else if (words[0] == "DROP") {
        if (words.size() != 3) {
            return false;
        }
        if (words[1] == "TABLE") {
            res.operations = DROP;
            DropSQL ans;
            ans.name_of_table = words[2];
            res.t = ans;
            return true;
        }
        return false;
    } else {
        return false;
    }
    return true;
}

bool ParserSQL::ParseJoin(std::vector<std::string>& words) {
    const int kMustSize = 11;
    if (words.size() < kMustSize) {
        return false;
    }
    JoinSQL ans;
    std::vector<std::string> name_colums;
    int64_t iter = 1;

    if (words[iter] == "*") {
        ans.all_choose = true;
        ++iter;
        if (words[iter] != "FROM") {
            return false;
        }
    } else {
        while (words[iter] != "FROM") {
            name_colums.push_back(words[iter]);
            ++iter;
            if (iter >= words.size()) {
                return false;
            }
        }
    }

    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    ans.name_of_table1 = words[iter];
    ++iter;

    if (words[iter] == "INNER") {
        ans.join_type = INNER;
    } else if (words[iter] == "LEFT") {
        ans.join_type = LEFT;
    } else if (words[iter] == "RIGHT") {
        ans.join_type = RIGHT;
    } else {
        return false;
    }

    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    if (words[iter] != "JOIN") {
        return false;
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    ans.name_of_table2 = words[iter];
    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    if (words[iter] != "ON") {
        return false;
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    std::string key_from_table1 = words[iter];
    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    if (words[iter] != "=") {
        return false;
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    std::string key_from_table2 = words[iter];
    ++iter;
    if (iter < words.size()) {
        return false;
    }
    bool need_swap = false;

    std::vector<std::string_view> for_split = split_string('.', key_from_table1);
    if (for_split.size() > 2 || for_split.size() < 2) {
        return false;
    } else {
        ans.key_from_table1 = for_split[1];
        if (for_split[0] == ans.name_of_table2) {
            need_swap = true;
        } else {
            if (for_split[0] != ans.name_of_table1) {
                return false;
            }
        }
    }

    for_split = split_string('.', key_from_table2);
    if (for_split.size() > 2 || for_split.size() < 2) {
        return false;
    } else {
        ans.key_from_table2 = for_split[1];
        if (for_split[0] == ans.name_of_table1) {
            if (!need_swap) {
                return false;
            }
        } else {
            if (for_split[0] != ans.name_of_table2) {
                return false;
            }
        }
    }
    if (need_swap) {
        std::swap(ans.key_from_table2, ans.key_from_table1);
    }
    if (!ans.all_choose){
        for (uint64_t i = 0; i < name_colums.size(); ++i) {
            for_split = split_string('.', name_colums[i]);
            if (for_split.size() > 2 || for_split.size() < 2) {
                return false;
            }
            if (for_split[0] == ans.name_of_table1) {
                ans.choose.emplace_back(true, for_split[1]);
            } else if (for_split[0] == ans.name_of_table2) {
                ans.choose.emplace_back(false, for_split[1]);
            } else {
                return false;
            }
        }
    }
    res.t = ans;
    return true;

}

bool ParserSQL::ParseDelete(std::vector<std::string>& words) {
    const int kMustSize = 3;
    if (words.size() < kMustSize) {
        return false;
    }
    DeleteSQL ans;
    if (words[1] != "FROM") {
        return false;
    }
    ans.name_of_table = words[2];
    if (words.size() == 3) {
        res.t = ans;
        return true;
    }
    int64_t iter = 3;
    if (words[iter] != "WHERE") {
        res.t = ans;
        return true;
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }

    if (words[iter] == "NOT") {
        ans.not_in_the_begin = true;
        ++iter;
    }
    int i = 0;
    while (iter < words.size()) {
        ans.trebovania.emplace_back();
        ans.trebovania[i].first = words[iter];
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
        if (words[iter] == "IS" || words[iter] == "=") {
            ans.trebovania[i].second.first = equal;
        } else if (words[iter] == "<") {
            ans.trebovania[i].second.first = less;
        } else if (words[iter] == "<=") {
            ans.trebovania[i].second.first = lessE;
        } else if (words[iter] == ">") {
            ans.trebovania[i].second.first = more;
        } else if (words[iter] == ">=") {
            ans.trebovania[i].second.first = moreE;
        } else if (words[iter] == "<>") {
            ans.trebovania[i].second.first = no_equal;
        }
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
        ans.trebovania[i].second.second = words[iter];
        ++iter;
        if (iter >= words.size()) {
            break;
        }
        ans.OR_conditionnal.emplace_back();
        if ((words[iter] == "OR")) {
            ans.OR_conditionnal[i] = true;
        } else if ((words[iter] == "ANS")) {
            ans.OR_conditionnal[i] = false;
        } else {
            return false;
        }
        ++iter;
        ++i;
    }
    if (ans.OR_conditionnal.size() >= ans.trebovania.size()) {
        return false;
    }
    res.t = ans;
    return true;

}

bool ParserSQL::ParseInsert(std::vector<std::string>& words) {
    const int kMustSize = 7;
    if (words.size() < kMustSize) {
        return false;
    }
    InsertSQL ans;
    ans.name_of_table = words[2];
    int64_t iter = 3;
    if (words[3] == "(") {
        ++iter;
        while (words[iter] != ")") {
            ans.cols.push_back(GetStringWithoutSEPLastBegin(words[iter]));
            ++iter;
            if (iter >= words.size()) {
                return false;
            }
        }
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
    }
    if (words[iter] == "VALUES") {
        ++iter;
        if (iter >= words.size() || words[iter] != "(") {
            return false;
        }
        ++iter;
        while (words[iter] != ")") {
            ans.values.push_back(GetStringWithoutSEPLastBegin(words[iter]));
            ++iter;
            if (iter >= words.size()) {
                return false;
            }
        }
    } else {
        return false;
    }
    res.t = ans;
    return true;
}

bool ParserSQL::ParseSelect(std::vector<std::string>& words) {
    const int kMustSize = 3;
    if (words.size() < kMustSize) {
        return false;
    }
    SelectSQL ans;
    int64_t iter = 1;
    if (words[iter] == "*") {
        ans.all_choose = true;
        ++iter;
        if (words[iter] != "FROM") {
            return false;
        }
    } else {
        while (words[iter] != "FROM") {
            ans.choose.push_back(GetStringWithoutSEPLastBegin(words[iter]));
            ++iter;
            if (iter >= words.size()) {
                return false;
            }
        }
        if (ans.choose.empty()) {
            return false;
        }
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }
    ans.from = words[iter];
    ++iter;
    if (iter >= words.size()) {
        res.t = ans;
        return true;
    }
    if (words[iter] != "WHERE") {
        return false;
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }

    if (words[iter] == "NOT") {
        ans.not_in_the_begin = true;
        ++iter;
    }
    int i = 0;
    while (iter < words.size()) {
        ans.trebovania.emplace_back();
        ans.trebovania[i].first = words[iter];
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
        if (words[iter] == "IS" || words[iter] == "=") {
            ans.trebovania[i].second.first = equal;
        } else if (words[iter] == "<") {
            ans.trebovania[i].second.first = less;
        } else if (words[iter] == "<=") {
            ans.trebovania[i].second.first = lessE;
        } else if (words[iter] == ">") {
            ans.trebovania[i].second.first = more;
        } else if (words[iter] == ">=") {
            ans.trebovania[i].second.first = moreE;
        } else if (words[iter] == "<>") {
            ans.trebovania[i].second.first = no_equal;
        }
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
        ans.trebovania[i].second.second = words[iter];
        ++iter;
        if (iter >= words.size()) {
            break;
        }
        ans.OR_conditionnal.emplace_back();
        if ((words[iter] == "OR")) {
            ans.OR_conditionnal[i] = true;
        } else if ((words[iter] == "AND")) {
            ans.OR_conditionnal[i] = false;
        } else {
            return false;
        }
        ++iter;
        ++i;
    }
    if (ans.OR_conditionnal.size() >= ans.trebovania.size()) {
        return false;
    }
    res.t = ans;
    return true;
}

bool ParserSQL::ParseUpdate(std::vector<std::string>& words) {
    const int kMustSize = 6;
    if (words.size() < kMustSize) {
        return false;
    }
    UpdateSQL ans;
    int64_t iter = 1;
    ans.from = words[iter];
    ++iter;
    if (words[iter] != "SET") {
        return false;
    }
    ++iter;
    while (iter < words.size() && words[iter] != "WHERE") {
        if ((iter + 2) >= words.size()) {
            return false;
        }
        if (words[iter + 1] != "=") {
            return false;
        }
        ans.for_set.emplace_back(words[iter], words[iter + 2]);
        iter = iter + 3;
    }

    if (words[iter] != "WHERE") {
        res.t = ans;
        return true;
    }
    ++iter;
    if (iter >= words.size()) {
        return false;
    }

    if (words[iter] == "NOT") {
        ans.not_in_the_begin = true;
        ++iter;
    }
    int i = 0;
    while (iter < words.size()) {
        ans.trebovania.emplace_back();
        ans.trebovania[i].first = words[iter];
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
        if (words[iter] == "IS" || words[iter] == "=") {
            ans.trebovania[i].second.first = equal;
        } else if (words[iter] == "<") {
            ans.trebovania[i].second.first = less;
        } else if (words[iter] == "<=") {
            ans.trebovania[i].second.first = lessE;
        } else if (words[iter] == ">") {
            ans.trebovania[i].second.first = more;
        } else if (words[iter] == ">=") {
            ans.trebovania[i].second.first = moreE;
        } else if (words[iter] == "<>") {
            ans.trebovania[i].second.first = no_equal;
        }
        ++iter;
        if (iter >= words.size()) {
            return false;
        }
        ans.trebovania[i].second.second = words[iter];
        ++iter;
        if (iter >= words.size()) {
            break;
        }
        ans.OR_conditionnal.emplace_back();
        if ((words[iter] == "OR")) {
            ans.OR_conditionnal[i] = true;
        } else if ((words[iter] == "ANS")) {
            ans.OR_conditionnal[i] = false;
        } else {
            return false;
        }
        ++iter;
        ++i;
    }
    if (ans.OR_conditionnal.size() >= ans.trebovania.size()) {
        return false;
    }
    res.t = ans;
    return true;
}

bool ParserSQL::ParseCreateTable(std::string& for_parse, std::string::size_type found) {
    CreateTableSQL ans;
    auto found2 = for_parse.find("("); // i_____j
    if (found2 == std::string::npos) {
        return false;
    }
    const int64_t table = 5;
    std::string name{for_parse.substr(found + table, found2 - found - table)};
    name = GetStringWithoutSpasesLastBegin(name);
    ans.name_of_table = name;

    auto found4 = for_parse.rfind(")"); // i_____j
    if (found4 == std::string::npos) {
        return false;
    }
    std::string coloms{for_parse.substr(found2 + 1, found4 - found2 - 1)};
    std::vector<std::string_view> cols = split_string(',', coloms);
    std::vector<column> col_names_types;
    for (uint64_t i = 0; i < cols.size(); ++i) {
        if (!ParseNameCol(cols[i], col_names_types)) {
            return false;
        }
    }
    ans.cols = col_names_types;
    res.t = ans;
    return true;
}

bool ParserSQL::ParseNameCol(std::string_view& cols, std::vector<column>& col_names_types) {
    std::string cols2 = GetStringWithoutSEPLastBegin(std::string(cols));
    column ans;
    auto found = cols2.find("bool");
    //BOOL
    if (found != std::string::npos) {
        ans.type = BOOL;
        ans.name = GetStringWithoutSpasesLastBegin(std::string(cols2.substr(0, found)));
        if (ans.name == "") {
            return false;
        }
    }
    //INT
    found = cols2.find("int");
    if (found != std::string::npos) {
        ans.type = INT;
        ans.name = GetStringWithoutSpasesLastBegin(std::string(cols2.substr(0, found)));
        if (ans.name == "") {
            return false;
        }
    }
    //FLOAT
    found = cols2.find("float");
    if (found != std::string::npos) {
        ans.type = FLOAT;
        ans.name = GetStringWithoutSpasesLastBegin(std::string(cols2.substr(0, found)));
        if (ans.name == "") {
            return false;
        }
    }
    //DOUBLE
    found = cols2.find("double");
    if (found != std::string::npos) {
        ans.type = DOUBLE;
        ans.name = GetStringWithoutSpasesLastBegin(std::string(cols2.substr(0, found)));
        if (ans.name == "") {
            return false;
        }
    }
    found = cols2.find("varchar");
    if (found != std::string::npos) {
        ans.type = STRING;
        ans.name = GetStringWithoutSpasesLastBegin(std::string(cols2.substr(0, found)));
        if (ans.name == "") {
            return false;
        }
        //(__)
        found = cols2.find("(");
        if (found == std::string::npos) {
            return false;
        }
        auto found2 = cols2.find(")");
        if (found2 == std::string::npos) {
            return false;
        }
        std::string_view s = cols2.substr(found + 1, found2 - found - 1);
        ans.size_for_varchar = stoi(GetStringWithoutSEPLastBegin(std::string(s)));
    }

    found = cols2.find("PRIMARY KEY");
    if (found != std::string::npos) {
        ans.IamPrimary = true;

    }

    found = cols2.find("NOT NULL");
    if (found != std::string::npos) {
        ans.InotNull = true;
    }

    found = cols2.find("FOREIGN KEY");
    if (found != std::string::npos) {
        auto found1 = cols2.find("REFERENCES", found);
        auto found2 = cols2.find('(', found);
        auto found3 = cols2.find(')', found);
        if (found1 == std::string::npos || found2 == std::string::npos || found3 == std::string::npos) {
            return false;
        }
        const int64_t referenses = 10;
        std::string name2 = cols2.substr(found1 + referenses, found2 - found1 - referenses);
        name2 = GetStringWithoutSEPLastBegin(name2);
        if (name2 == "") {
            return false;
        }
        ans.name_foreign_table = name2;
        name2 = cols2.substr(found2 + 1, found3 - found2 - 1);
        name2 = GetStringWithoutSEPLastBegin(name2);
        if (name2 == "") {
            return false;
        }
        ans.name_foreign_key = name2;
        ans.IamForeign = true;
    }
    col_names_types.push_back(ans);

    return true;
}
