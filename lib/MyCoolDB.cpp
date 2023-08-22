//
// Created by Елизавета Кравченкова on 27.04.2023.
//

#include "MyCoolDB.h"

std::ostream& operator<<(std::ostream& os, const varchar& val) {
    return os << val.Get();
}

PrintTableClass::PrintTableClass(Table* table, const std::string& name) : table_(table), table_name(name) {
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        if (table_->type[i].T_enum == STRING) {
            max_widht = std::max(max_widht, table_->type[i].size_for_varchar);
        }
    }
}

void PrintTableClass::init(Table* table, const std::string& name) {
    table_ = table;
    table_name = name;
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        if (table_->type[i].T_enum == STRING) {
            max_widht = std::max(max_widht, table_->type[i].size_for_varchar);
        }
    }
}

std::string PrintTableClass::AddToMaxsize(std::string name) const {
    int64_t cut_size = name.length();
    while (cut_size < max_widht) {
        if ((cut_size + 2) <= max_widht) {
            name = " " + name + " ";
            cut_size += 2;
        } else {
            name.push_back(' ');
            cut_size++;
        }
    }
    return name;
}

std::string PrintTableClassJoin::AddToMaxsize(std::string name) const {
    int64_t cut_size = name.length();
    while (cut_size < max_widht) {
        if ((cut_size + 2) <= max_widht) {
            name = " " + name + " ";
            cut_size += 2;
        } else {
            name.push_back(' ');
            cut_size++;
        }
    }
    return name;
}

void PrintTableClass::Print(std::ofstream& fout) {
    fout << "Таблица : " << table_name << "\n";
    fout << '|';
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        fout << AddToMaxsize(table_->name_of_col[i]) << '|';
    }
    fout << std::endl;
    auto it = table_->table.begin();
    while (it != table_->table.end()) {
        PrintRow(it->second, fout);
        fout << std::endl;
        ++it;
    }
}

void PrintTableClass::PrintRow(const MyRow& it, std::ofstream& fout) {
    fout << '|';
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        switch (table_->type[i].T_enum) {
            case BOOL:
                if (!it.not_null[i]) {
                    fout << AddToMaxsize("NULL") << '|';
                } else {
                    fout << AddToMaxsize(std::to_string(std::get<bool>(it.data[i]))) << '|';
                }
                break;
            case INT:
                if (!it.not_null[i]) {
                    fout << AddToMaxsize("NULL") << '|';
                } else {
                    fout << AddToMaxsize(std::to_string(std::get<int>(it.data[i]))) << '|';
                }
                break;
            case FLOAT:
                if (!it.not_null[i]) {
                    fout << AddToMaxsize("NULL") << '|';
                } else {
                    fout << AddToMaxsize(std::to_string(std::get<float>(it.data[i]))) << '|';
                }
                break;
            case DOUBLE:
                if (!it.not_null[i]) {
                    fout << AddToMaxsize("NULL") << '|';
                } else {
                    fout << AddToMaxsize(std::to_string(std::get<double>(it.data[i]))) << '|';
                }
                break;
            case STRING:
                if (!it.not_null[i]) {
                    fout << AddToMaxsize("NULL") << '|';
                } else {
                    fout << AddToMaxsize(std::get<varchar>(it.data[i]).Get()) << '|';
                }
                break;
            default:
                break;
        }

    }
}

void PrintTableClass::PrintRowSomeCols(const MyRow& it, std::vector<int64_t>& col_number) {
    std::cout << '|';
    for (uint64_t i = 0; i < col_number.size(); ++i) {
        switch (table_->type[col_number[i]].T_enum) {
            case BOOL:
                if (!it.not_null[col_number[i]]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<bool>(it.data[col_number[i]]))) << '|';
                }
                break;
            case INT:
                if (!it.not_null[col_number[i]]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<int>(it.data[col_number[i]]))) << '|';
                }
                break;
            case FLOAT:
                if (!it.not_null[col_number[i]]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<float>(it.data[col_number[i]]))) << '|';
                }
                break;
            case DOUBLE:
                if (!it.not_null[col_number[i]]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<double>(it.data[col_number[i]]))) << '|';
                }
                break;
            case STRING:
                if (!it.not_null[col_number[i]]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::get<varchar>(it.data[col_number[i]]).Get()) << '|';
                }
                break;
            default:
                break;
        }

    }
}

void PrintTableClass::PrintHelloCols(std::vector<int64_t>& col_number) {
    std::cout << "Таблица : " << table_name << "\n";
    std::cout << '|';
    for (uint64_t i = 0; i < col_number.size(); ++i) {
        std::cout << AddToMaxsize(table_->name_of_col[col_number[i]]) << '|';
    }
    std::cout << std::endl;
}

void PrintTableClassJoin::PrintHelloCols(const std::vector<std::pair<bool, std::string>>& col_number) {
    std::cout << "Таблица : " << table_name1 << " , "<<table_name2<< "\n";
    std::cout << '|';
    for (uint64_t i = 0; i < col_number.size(); ++i) {
        std::cout << AddToMaxsize(col_number[i].second) << '|';
    }
    std::cout << std::endl;
}

void PrintTableClass::Print_Hello() {
    std::cout << "Таблица : " << table_name << "\n";
    std::cout << '|';
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        std::cout << AddToMaxsize(table_->name_of_col[i]) << '|';
    }
    std::cout << std::endl;
}

void PrintTableClass::Print() {
    std::cout << "Таблица : " << table_name << "\n";
    std::cout << '|';
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        std::cout << AddToMaxsize(table_->name_of_col[i]) << '|';
    }
    std::cout << std::endl;
    auto it = table_->table.begin();
    while (it != table_->table.end()) {
        PrintRow(it->second);
        std::cout << std::endl;
        ++it;
    }
}

void PrintTableClass::PrintRow(const MyRow& it) {
    std::cout << '|';
    for (uint64_t i = 0; i < table_->cout_of_col; ++i) {
        switch (table_->type[i].T_enum) {
            case BOOL:
                if (!it.not_null[i]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<bool>(it.data[i]))) << '|';
                }
                break;
            case INT:
                if (!it.not_null[i]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<int>(it.data[i]))) << '|';
                }
                break;
            case FLOAT:
                if (!it.not_null[i]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<float>(it.data[i]))) << '|';
                }
                break;
            case DOUBLE:
                if (!it.not_null[i]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::to_string(std::get<double>(it.data[i]))) << '|';
                }
                break;
            case STRING:
                if (!it.not_null[i]) {
                    std::cout << AddToMaxsize("NULL") << '|';
                } else {
                    std::cout << AddToMaxsize(std::get<varchar>(it.data[i]).Get()) << '|';
                }
                break;
            default:
                break;
        }

    }
}

void Table::add(const MyRow& row) {
    table.insert({unique_num, row});
    for (uint64_t i = 0; i < cout_of_col; ++i) {
        if (!row.not_null[i]) {
            idx[name_of_col[i]]["NULL"].push_back(unique_num);
            continue;
        }
        switch (type[i].T_enum) {
            case BOOL:
                idx[name_of_col[i]][std::get<bool>(row.data[i])].push_back(unique_num);
                break;
            case INT:
                idx[name_of_col[i]][std::get<int>(row.data[i])].push_back(unique_num);
                break;
            case FLOAT:
                idx[name_of_col[i]][std::get<float>(row.data[i])].push_back(unique_num);
                break;
            case DOUBLE:
                idx[name_of_col[i]][std::get<double>(row.data[i])].push_back(unique_num);
                break;
            case STRING:
                idx[name_of_col[i]][std::get<varchar>(row.data[i])].push_back(unique_num);
                break;
            default:
                break;
        }

    }
    ++unique_num;
}

void Table::Print() {
    PrintTableClass printer(this, table_name_);
    printer.Print();
    /*
    for (uint64_t i=0;i<name_of_col.size();++i){
        std::cout<<name_of_col[i]<<" ";
    }
    std::cout<<std::endl;

    auto it=table.begin();
    while (it!=table.end()){
        PrintRow(it->second);
        std::cout<<std::endl;
        ++it;
    }
     */
}

void Table::PrintRow(const MyRow& it) {
    PrintTableClass printer(this, table_name_);
    printer.PrintRow(it);
}

void Table::PrintRow(const MyRow& it, std::vector<int64_t>& col_number) {
    PrintTableClass printer(this, table_name_);
    printer.PrintRowSomeCols(it, col_number);
    /*
    for (uint64_t i=0;i<col_number.size();++i){
        if (foreign_keys[col_number[i]]!= nullptr && foreign_keys[col_number[i]]->idx[name_of_foreign_keys[col_number[i]]][it.data[col_number[i]]].empty()){
            std::cout<<"NULL ";
            continue;
        }
        switch (type[col_number[i]].T_enum) {
            case BOOL:
                if (!it.not_null[col_number[i]]){
                    std::cout<<"NULL ";
                }
                else{
                    std::cout<<std::get<bool>(it.data[col_number[i]])<<" ";
                }
                break;
            case INT:
                if (!it.not_null[col_number[i]]){
                    std::cout<<"NULL ";
                }
                else {
                    std::cout << std::get<int>(it.data[col_number[i]]) << " ";
                }
                break;
            case FLOAT:
                if (!it.not_null[col_number[i]]){
                    std::cout<<"NULL ";
                }
                else {
                    std::cout << std::get<float>(it.data[col_number[i]]) << " ";
                }
                break;
            case DOUBLE:
                if (!it.not_null[col_number[i]]){
                    std::cout<<"NULL ";
                }
                else {
                    std::cout << std::get<double>(it.data[col_number[i]]) << " ";
                }
                break;
            case STRING:
                if (!it.not_null[col_number[i]]){
                    std::cout<<"NULL ";
                }
                else {
                    std::cout << std::get<varchar>(it.data[col_number[i]]) << " ";
                }
                break;
            default:
                break;
        }

    }
    */
}

void Table::Insert_with_col(const InsertSQL& value) {
    if (value.cols.size() != value.values.size()) {
        throw "Недостаточное количество аргументов";
    }
    MyRow ins;
    ins.not_null.resize(cout_of_col, false);
    ins.data.resize(cout_of_col);
    for (uint64_t i = 0; i < value.cols.size(); ++i) {
        if (!index_of_col.contains(value.cols[i])) {
            throw "Я не знаю ничего о таких столбцах";
        }
        int idx_col = index_of_col[value.cols[i]];
        if (value.cols[i] == "NULL") {
            ins.data[i] = "NULL";
            continue;
        }
        try {
            switch (type[idx_col].T_enum) {
                case BOOL:
                    if (value.values[i] == "TRUE") {
                        ins.data[idx_col] = true;
                    } else if (value.values[i] == "FALSE") {
                        ins.data[idx_col] = false;
                    } else {
                        throw "неверный тип";
                    }
                case INT:
                    ins.data[idx_col] = std::stoi(value.values[i]);
                    break;
                case FLOAT:
                    ins.data[idx_col] = std::stof(value.values[i]);
                    break;
                case DOUBLE:
                    ins.data[idx_col] = std::stod(value.values[i]);
                    break;
                case STRING:
                    varchar var(value.values[i].substr(1, value.values[i].size() - 2), type[idx_col].size_for_varchar);
                    ins.data[idx_col] = var;
                    break;
            }
        }
        catch (...) {
            throw "Неверный тип";
        }
        ins.not_null[idx_col] = true;
    }
    for (uint64_t i = 0; i < cout_of_col; ++i) {
        if (foreign_keys[i] != nullptr) {
            if (!ins.not_null[i] || foreign_keys[i]->idx[name_of_foreign_keys[i]][ins.data[i]].empty()) {
                throw "Не удовлетворяет требованиям FOREIGN KEY";
            }
        }
        if (primary_keys[i]) {
            if (!ins.not_null[i] || !idx[name_of_col[i]][ins.data[i]].empty()) {
                throw "Не удовлетворяет требованиям PRIMARY KEY";
            }
        } else if (not_null_keys[i]) {
            if (!ins.not_null[i]) {
                throw "Не удовлетворяет требованиям NOT NULL KEY";
            }
        }
    }

    add(ins);
}

void Table::Insert(const InsertSQL& value) {
    if (!value.cols.empty()) {
        return Insert_with_col(value);
    }
    if (cout_of_col != value.values.size()) {
        throw "Недостаточное количество аргументов";
    }

    MyRow ins;
    ins.not_null.resize(cout_of_col, false);
    ins.data.resize(cout_of_col);

    for (uint64_t i = 0; i < value.values.size(); ++i) {
        if (value.values[i] == "NULL") {
            ins.data[i] = "NULL";
            continue;
        }
        try {
            switch (type[i].T_enum) {
                case BOOL:
                    if (value.values[i] == "TRUE") {
                        ins.data[i] = true;
                    } else if (value.values[i] == "FALSE") {
                        ins.data[i] = false;
                    } else {
                        throw "неверный тип";
                    }
                case INT:
                    ins.data[i] = std::stoi(value.values[i]);
                    break;
                case FLOAT:
                    ins.data[i] = std::stof(value.values[i]);
                    break;
                case DOUBLE:
                    ins.data[i] = std::stod(value.values[i]);
                    break;
                case STRING:
                    varchar var(value.values[i].substr(1, value.values[i].size() - 2), type[i].size_for_varchar);
                    ins.data[i] = var;
                    break;
            }
        }
        catch (...) {
            throw "Неверный тип";
        }
        ins.not_null[i] = true;
    }
    for (uint64_t i = 0; i < cout_of_col; ++i) {
        if (foreign_keys[i] != nullptr) {
            if (!ins.not_null[i] || foreign_keys[i]->idx[name_of_foreign_keys[i]][ins.data[i]].empty()) {
                throw "Не удовлетворяет требованиям FOREIGN KEY";
            }
        }
        if (primary_keys[i]) {
            if (!ins.not_null[i] || !idx[name_of_col[i]][ins.data[i]].empty()) {
                throw "Не удовлетворяет требованиям PRIMARY KEY";
            }
        } else if (not_null_keys[i]) {
            if (!ins.not_null[i]) {
                throw "Не удовлетворяет требованиям NOT NULL KEY";
            }
        }
    }
    add(ins);
}

void Table::Select(const SelectSQL& value) {
    PrintTableClass printer(this, table_name_);
    if (!value.trebovania.empty()) {
        std::set < int64_t > true_index = find_indexs(value.trebovania, value.OR_conditionnal, value.not_in_the_begin);
        if (true_index.empty()) {
            std::cout << "Нет данных, удовлетворяющих данным требованиям";
            return;
        }
        if (value.all_choose) {
            printer.Print_Hello();
            auto it = true_index.begin();
            while (it != true_index.end()) {
                PrintRow(table[*it]);
                std::cout << std::endl;
                ++it;
            }
        } else {
            std::vector<int64_t> col_number;
            for (uint64_t i = 0; i < value.choose.size(); ++i) {
                if (!index_of_col.contains(value.choose[i])) {
                    throw "Неизвестные данные о таблице";
                }
                col_number.push_back(index_of_col[value.choose[i]]);
            }
            printer.PrintHelloCols(col_number);
            auto it = true_index.begin();
            while (it != true_index.end()) {
                PrintRow(table[*it], col_number);
                std::cout << std::endl;
                ++it;
            }
        }
    } else {
        if (value.all_choose) {
            printer.Print_Hello();
            auto it = table.begin();
            while (it != table.end()) {
                PrintRow(it->second);
                std::cout << std::endl;
                ++it;
            }
        } else {
            std::vector<int64_t> col_number;
            for (uint64_t i = 0; i < value.choose.size(); ++i) {
                if (!index_of_col.contains(value.choose[i])) {
                    throw "Неизвестные данные о таблице";
                }
                col_number.push_back(index_of_col[value.choose[i]]);
            }
            printer.PrintHelloCols(col_number);
            auto it = table.begin();
            while (it != table.end()) {
                PrintRow(it->second, col_number);
                std::cout << std::endl;
                ++it;
            }
        }
    }
}

void Table::Update(const UpdateSQL& value) {
    std::vector<int64_t> col_number;
    for (uint64_t i = 0; i < value.for_set.size(); ++i) {
        if (!index_of_col.contains(value.for_set[i].first)) {
            throw "Неизвестные данные о таблице";
        }
        col_number.push_back(index_of_col[value.for_set[i].first]);
    }
    if (!value.trebovania.empty()) {
        std::set < int64_t > true_index = find_indexs(value.trebovania, value.OR_conditionnal, value.not_in_the_begin);
        if (true_index.empty()) {
            std::cout << "Нет данных, удовлетворяющих данным требованиям";
            return;
        }
        auto it = true_index.begin();
        while (it != true_index.end()) {
            for (uint64_t i = 0; i < col_number.size(); ++i) {
                object val = string_to_object(value.for_set[i].second, col_number[i]);
                if (children_keys[col_number[i]] != nullptr && val != table[*it].data[col_number[i]]) {
                    children_keys[col_number[i]]->SetNull(name_of_children_keys[col_number[i]],
                                                          table[*it].data[col_number[i]]);
                }
                if (!table[*it].not_null[col_number[i]]) {
                    auto it2 = std::find(idx[value.for_set[i].first]["NULL"].begin(),
                                         idx[value.for_set[i].first]["NULL"].end(), *it);
                    idx[value.for_set[i].first]["NULL"].erase(it2);
                } else {
                    auto it2 = std::find(idx[value.for_set[i].first][table[*it].data[col_number[i]]].begin(),
                                         idx[value.for_set[i].first][table[*it].data[col_number[i]]].end(), *it);
                    idx[value.for_set[i].first][table[*it].data[col_number[i]]].erase(it2);
                }
                table[*it].data[col_number[i]] = val;
                if (value.for_set[i].second == "NULL") {
                    idx[value.for_set[i].first]["NULL"].push_back(*it);
                    table[*it].not_null[col_number[i]] = false;
                } else {
                    idx[value.for_set[i].first][val].push_back(*it);
                    table[*it].not_null[col_number[i]] = true;
                }
            }
            ++it;
        }

    } else {
        auto it = table.begin();
        while (it != table.end()) {
            for (uint64_t i = 0; i < col_number.size(); ++i) {
                object val = string_to_object(value.for_set[i].second, col_number[i]);
                if (children_keys[col_number[i]] != nullptr && val != it->second.data[col_number[i]]) {
                    children_keys[col_number[i]]->SetNull(name_of_children_keys[col_number[i]],
                                                          it->second.data[col_number[i]]);
                }
                if (!it->second.not_null[col_number[i]]) {
                    auto it2 = std::find(idx[value.for_set[i].first]["NULL"].begin(),
                                         idx[value.for_set[i].first]["NULL"].end(), it->first);
                    idx[value.for_set[i].first]["NULL"].erase(it2);
                } else {
                    idx[value.for_set[i].first][it->second.data[col_number[i]]].erase(
                            std::find(idx[value.for_set[i].first][it->second.data[col_number[i]]].begin(),
                                      idx[value.for_set[i].first][it->second.data[col_number[i]]].end(), it->first));
                }
                it->second.data[col_number[i]] = val;
                if (value.for_set[i].second == "NULL") {
                    idx[value.for_set[i].first]["NULL"].push_back(it->first);
                    it->second.not_null[col_number[i]] = false;
                } else {
                    idx[value.for_set[i].first][val].push_back(it->first);
                    it->second.not_null[col_number[i]] = true;
                }
            }
            ++it;
        }
    }
}

void Table::Delete(const DeleteSQL& value) {
    if (!value.trebovania.empty()) {
        std::set < int64_t > true_index = find_indexs(value.trebovania, value.OR_conditionnal, value.not_in_the_begin);
        if (true_index.empty()) {
            std::cout << "Нет данных, удовлетворяющих данным требованиям";
            return;
        }
        auto it = true_index.begin();
        while (it != true_index.end()) {
            for (uint64_t i = 0; i < name_of_col.size(); ++i) {
                if (children_keys[i] != nullptr) {
                    children_keys[i]->SetNull(name_of_children_keys[i], table[*it].data[index_of_col[name_of_col[i]]]);
                }
                if (!table[*it].not_null[index_of_col[name_of_col[i]]]) {
                    auto it2 = std::find(idx[name_of_col[i]]["NULL"].begin(), idx[name_of_col[i]]["NULL"].end(), *it);
                    idx[name_of_col[i]]["NULL"].erase(it2);
                } else {
                    auto it2 = std::find(idx[name_of_col[i]][table[*it].data[index_of_col[name_of_col[i]]]].begin(),
                                         idx[name_of_col[i]][table[*it].data[index_of_col[name_of_col[i]]]].end(), *it);
                    idx[name_of_col[i]][table[*it].data[index_of_col[name_of_col[i]]]].erase(it2);
                }
            }
            table.erase(*it);
            ++it;
        }

    } else {
        unique_num = 0;
        for (uint64_t i = 0; i < name_of_col.size(); ++i) {
            if (children_keys[i] != nullptr) {
                children_keys[i]->SetNullall(name_of_children_keys[i]);
            }
            idx[name_of_col[i]].clear();
        }
        table.clear();
    }
}

object Table::string_to_object(const std::string& value, int64_t i) {
    object need_val;
    if (value == "NULL") {
        need_val = value;
        return need_val;
    }
    try {
        switch (type[i].T_enum) {
            case BOOL:
                if (value == "TRUE") {
                    need_val = true;
                } else if (value == "FALSE") {
                    need_val = false;
                } else {
                    throw "неверный тип";
                }
            case INT:
                need_val = std::stoi(value);
                break;
            case FLOAT:
                need_val = std::stof(value);
                break;
            case DOUBLE:
                need_val = std::stod(value);
                break;
            case STRING:
                if (value == "NULL") {
                    need_val = value;
                } else {
                    varchar var(value.substr(1, value.size() - 2), type[i].size_for_varchar);
                    need_val = var;
                }
                break;
        }
    }
    catch (...) {
        throw "Неверный тип";
    }
    return need_val;
}

std::set<int64_t>
Table::find_indexsORstr(const std::vector<std::pair<std::string, std::pair<int, std::string>>>& trebovania) {
    std::set < int64_t > idxes;
    for (uint64_t i = 0; i < trebovania.size(); ++i) {
        int64_t index_need_col = index_of_col[trebovania[i].first];
        std::string need_val_str = trebovania[i].second.second;
        object need_val = string_to_object(need_val_str, index_need_col);
        if (need_val_str == "NULL" && trebovania[i].second.first == equal) {
            idxes.insert(idx[trebovania[i].first]["NULL"].begin(),
                         idx[trebovania[i].first]["NULL"].end());
        }
        if (trebovania[i].second.first == equal && need_val_str != "NULL") {
            idxes.insert(idx[trebovania[i].first][need_val].begin(),
                         idx[trebovania[i].first][need_val].end());
        } else {
            auto it = table.begin();
            while (it != table.end()) {
                object val = it->second.data[index_need_col];
                if (!it->second.not_null[index_need_col]) {
                    ++it;
                    continue;
                }
                switch (trebovania[i].second.first) {
                    case less:
                        if (val < need_val) {
                            idxes.insert(it->first);
                        }
                        break;
                    case more:
                        if (val > need_val) {
                            idxes.insert(it->first);
                        }
                        break;
                    case moreE:
                        if (val >= need_val) {
                            idxes.insert(it->first);
                        }
                        break;
                    case lessE:
                        if (val <= need_val) {
                            idxes.insert(it->first);
                        }
                        break;
                    case no_equal:
                        if (val != need_val) {
                            idxes.insert(it->first);
                        }
                        break;
                    default:
                        break;
                }
                ++it;
            }
        }

    }
    return idxes;
}

std::set<int64_t>
Table::find_indexsANDstr(const std::vector<std::pair<std::string, std::pair<int, std::string>>>& trebovania) {
    std::vector<std::pair<std::string, std::pair<int, std::string>>> first = {trebovania[0]};
    std::set < int64_t > idxes = find_indexsORstr(first);
    for (uint64_t i = 1; i < trebovania.size(); ++i) {
        int64_t index_need_col = index_of_col[trebovania[i].first];
        std::string need_val_str = trebovania[i].second.second;
        object need_val = string_to_object(need_val_str, i);

        auto it = idxes.begin();
        object val = table[*it].data[index_need_col];
        while (it != idxes.end()) {
            switch (trebovania[i].second.first) {
                case equal:
                    if (val != need_val) {
                        idxes.erase(it);
                    }
                    break;
                case less:
                    if (val >= need_val) {
                        idxes.erase(it);
                    }
                    break;
                case more:
                    if (val <= need_val) {
                        idxes.erase(it);
                    }
                    break;
                case moreE:
                    if (val < need_val) {
                        idxes.erase(it);
                    }
                    break;
                case lessE:
                    if (val > need_val) {
                        idxes.erase(it);
                    }
                    break;
                case no_equal:
                    if (val == need_val) {
                        idxes.erase(it);
                    }
                    break;
                default:
                    break;
            }
            ++it;
        }
    }
    return idxes;
}

std::set<int64_t> Table::find_indexs(const std::vector<std::pair<std::string, std::pair<int, std::string>>>& trebovania,
                                     const std::vector<bool>& OR_conditionnal, const bool not_in_the_begin) {
    std::vector<std::pair<std::string, std::pair<int, std::string>>> first = {trebovania[0]};
    std::set < int64_t > ans = find_indexsORstr(first);
    for (uint64_t i = 0; i < OR_conditionnal.size(); ++i) {
        std::vector<std::pair<std::string, std::pair<int, std::string>>> tr = {trebovania[i + 1]};
        if (OR_conditionnal[i]) {
            ans.merge(find_indexsORstr(tr));
        } else {
            std::set < int64_t > ans1;
            std::set < int64_t > ans2 = find_indexsANDstr(tr);
            std::set_intersection(ans.begin(), ans.end(), ans2.begin(), ans2.end(), std::inserter(ans1, ans1.begin()));
            ans = ans1;
        }
    }

    if (not_in_the_begin) {
        std::set < int64_t > ans3;
        std::set < int64_t > ans2;
        auto it = table.begin();
        while (it != table.end()) {
            ans2.insert(it->first);
            ++it;
        }
        std::set_difference(ans2.begin(), ans2.end(), ans.begin(), ans.end(), std::inserter(ans3, ans3.begin()));
        ans = ans3;

    }
    return ans;
}

void Table::SetNull(const std::string& key, const object& value) {
    std::vector<int64_t> values_id = idx[key][value];
    int64_t col_num = index_of_col[key];
    idx[key][value].clear();
    for (uint64_t i = 0; i < values_id.size(); ++i) {
        idx[key]["NULL"].push_back(values_id[i]);
        table[values_id[i]].data[col_num] = "NULL";
        table[values_id[i]].not_null[col_num] = false;
    }
}

void Table::SetNullall(const std::string& key) {
    int64_t col_num = index_of_col[key];
    idx[key].clear();
    auto it = table.begin();
    while (it != table.end()) {
        idx[key]["NULL"].push_back(it->first);
        it->second.data[col_num] = "NULL";
        it->second.not_null[col_num] = false;
        ++it;
    }
}

std::set<object> Table::GetValuesInThisKey(const std::string& key) {
    std::set < object > ans;
    auto it = idx[key].begin();
    while (it != idx[key].end()) {
        if (!it->second.empty()) {
            ans.insert(it->first);
        }
        ++it;
    }
    return ans;
}

Table::Table(std::vector<column>& types, std::vector<Table*>& foreign_tables, std::string table_name) {
    int64_t size = types.size();
    name_of_col.resize(size);
    primary_keys.resize(size);
    not_null_keys.resize(size);
    table_name_ = table_name;
    foreign_keys.resize(size);
    children_keys.resize(size);
    name_of_children_keys.resize(size);
    name_of_foreign_keys.resize(size);
    type.resize(size);
    for (uint64_t i = 0; i < size; ++i) {
        type[i].T_enum = types[i].type;
        type[i].size_for_varchar = types[i].size_for_varchar;
        name_of_col[i] = types[i].name;
        primary_keys[i] = types[i].IamPrimary;
        not_null_keys[i] = types[i].InotNull;
        foreign_keys[i] = foreign_tables[i];
        name_of_foreign_keys[i] = types[i].name_foreign_key;
        index_of_col.insert({name_of_col[i], i});
        cout_of_col = size;
        idx.insert({name_of_col[i], std::unordered_map < object, std::vector<int64_t>>()});
    }
}

void MyCoolDB::InnerJoin(const JoinSQL& value, Table& Table1, Table& Table2, int type) {
    std::set < object > t1 = Table1.GetValuesInThisKey(value.key_from_table1);
    std::set < object > t2 = Table2.GetValuesInThisKey(value.key_from_table2);
    std::set < object > t3;
    std::set_intersection(t1.begin(), t1.end(), t2.begin(), t2.end(), std::inserter(t3, t3.begin()));
    std::vector<std::pair<std::vector<int64_t>, std::vector<int64_t>>> indexes;
    auto it = t3.begin();
    while (it != t3.end()) {
        std::vector<int64_t> vec1 = Table1.idx[value.key_from_table1][*it];
        std::vector<int64_t> vec2 = Table2.idx[value.key_from_table2][*it];
        std::pair<std::vector<int64_t>, std::vector<int64_t>> pair = {vec1, vec2};
        indexes.emplace_back(pair);
        ++it;
    }
    PrintTableClassJoin printer(&Table1,&Table2,Table1.table_name_,Table2.table_name_);
    printer.PrintHelloCols(value.choose);
    int64_t num_col_in1 = Table1.index_of_col[value.key_from_table1];
    int64_t num_col_in2 = Table2.index_of_col[value.key_from_table2];
    for (uint64_t i = 0; i < indexes.size(); ++i) {
        std::vector<int64_t> first = indexes[i].first;
        std::vector<int64_t> second = indexes[i].second;
        for (uint64_t iter1 = 0; iter1 < first.size(); ++iter1) {
            for (uint64_t iter2 = 0; iter2 < second.size(); ++iter2) {
                std::cout << '|';
                for (uint64_t col = 0; col < value.choose.size(); ++col) {
                    if (value.choose[col].first) {
                        num_col_in1 = Table1.index_of_col[value.choose[col].second];
                        switch (Table1.type[num_col_in1].T_enum) {
                            case BOOL:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<bool>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }

                                break;
                            case INT:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<int>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }
                                break;
                            case FLOAT:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<float>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }

                                break;
                            case DOUBLE:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<double>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }
                                break;
                            case STRING:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::get<varchar>(Table1.table[first[iter1]].data[num_col_in1]).Get()) <<'|';
                                }
                            default:
                                break;
                        }
                    }
                    else {
                        num_col_in2 = Table2.index_of_col[value.choose[col].second];
                        switch (Table2.type[num_col_in2].T_enum) {
                            case BOOL:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<bool>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }
                                break;
                            case INT:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<int>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }
                                break;
                            case FLOAT:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<float>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }
                                break;
                            case DOUBLE:

                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<double>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }

                                break;
                            case STRING:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::get<varchar>(Table2.table[second[iter2]].data[num_col_in2]).Get())
                                              <<'|';
                                }
                            default:
                                break;
                        }
                    }

                }
                std::cout << std::endl;
            }
        }
    }
}

void MyCoolDB::LeftJoin(const std::string& key_from_table1, const std::string& key_from_table2,
                        const std::string& name_of_table1, const std::string& name_of_table2, Table& Table1,
                        Table& Table2, std::vector<std::pair<bool, std::string>>& choose) {
    std::set < object > t3 = Table1.GetValuesInThisKey(key_from_table1);
    std::vector<std::pair<std::vector<int64_t>, std::vector<int64_t>>> indexes;
    auto it = t3.begin();
    while (it != t3.end()) {
        if (Table2.idx[key_from_table2].contains(*it)) {
            indexes.emplace_back(Table1.idx[key_from_table1][*it], Table2.idx[key_from_table2][*it]);
        } else {
            std::vector<int64_t> empty_vec;
            indexes.emplace_back(Table1.idx[key_from_table1][*it], empty_vec);
        }
        ++it;
    }
    PrintTableClassJoin printer(&Table1,&Table2,Table1.table_name_,Table2.table_name_);
    printer.PrintHelloCols(choose);
    int64_t num_col_in1 = Table1.index_of_col[key_from_table1];
    int64_t num_col_in2 = Table2.index_of_col[key_from_table2];
    for (uint64_t i = 0; i < indexes.size(); ++i) {
        std::vector<int64_t> first = indexes[i].first;
        std::vector<int64_t> second = indexes[i].second;
        for (uint64_t iter1 = 0; iter1 < first.size(); ++iter1) {
            if (second.empty()) {
                std::cout << '|';
                for (uint64_t col = 0; col < choose.size(); ++col) {
                    if (choose[col].first) {
                        num_col_in1 = Table1.index_of_col[choose[col].second];
                        switch (Table1.type[num_col_in1].T_enum) {
                            case BOOL:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<bool>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }

                                break;
                            case INT:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<int>(Table1.table[first[iter1]].data[num_col_in1]))) << '|';
                                }
                                break;
                            case FLOAT:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<float>(Table1.table[first[iter1]].data[num_col_in1]))) << '|';
                                }

                                break;
                            case DOUBLE:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<double>(Table1.table[first[iter1]].data[num_col_in1]))) << '|';
                                }
                                break;
                            case STRING:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::get<varchar>(Table1.table[first[iter1]].data[num_col_in1]).Get()) << '|';
                                }
                            default:
                                break;
                        }
                    }
                    else {
                        std::cout <<printer.AddToMaxsize("NULL")<<'|';
                    }
                }
                std::cout << std::endl;
            }
            for (uint64_t iter2 = 0; iter2 < second.size(); ++iter2) {
                std::cout << '|';
                for (uint64_t col = 0; col < choose.size(); ++col) {
                    if (choose[col].first) {
                        num_col_in1 = Table1.index_of_col[choose[col].second];
                        switch (Table1.type[num_col_in1].T_enum) {
                            case BOOL:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<bool>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }

                                break;
                            case INT:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<int>(Table1.table[first[iter1]].data[num_col_in1]))) <<'|';
                                }
                                break;
                            case FLOAT:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<float>(Table1.table[first[iter1]].data[num_col_in1]))) << '|';
                                }

                                break;
                            case DOUBLE:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<double>(Table1.table[first[iter1]].data[num_col_in1]))) << '|';
                                }
                                break;
                            case STRING:
                                if (!Table1.table[first[iter1]].not_null[num_col_in1]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::get<varchar>(Table1.table[first[iter1]].data[num_col_in1]).Get()) << '|';
                                }
                            default:
                                break;
                        }
                    } else {
                        num_col_in2 = Table2.index_of_col[choose[col].second]; //куда пропал teacher
                        switch (Table2.type[num_col_in2].T_enum) {
                            case BOOL:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<bool>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }
                                break;
                            case INT:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<int>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }
                                break;
                            case FLOAT:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<float>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }
                                break;
                            case DOUBLE:

                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout << printer.AddToMaxsize(std::to_string(std::get<double>(Table2.table[second[iter2]].data[num_col_in2]))) <<'|';
                                }

                                break;
                            case STRING:
                                if (!Table2.table[second[iter2]].not_null[num_col_in2]) {
                                    std::cout <<printer.AddToMaxsize("NULL")<<'|';
                                } else {
                                    std::cout <<printer.AddToMaxsize( std::get<varchar>(Table2.table[second[iter2]].data[num_col_in2]).Get())
                                              << '|';
                                }
                            default:
                                break;
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
    }
}

void MyCoolDB::RightJoin(const std::string& key_from_table1, const std::string& key_from_table2,
                         const std::string& name_of_table1, const std::string& name_of_table2, Table& Table1,
                         Table& Table2, std::vector<std::pair<bool, std::string>>& choose) {
    for (uint64_t i = 0; i < choose.size(); ++i) {
        if (choose[i].first) {
            choose[i].first = false;
        } else {
            choose[i].first = true;
        }
    }
    LeftJoin(key_from_table2, key_from_table1, name_of_table2, name_of_table1, Table2, Table1, choose);
}

void MyCoolDB::LoadToFile(const std::string& path) {
    std::ofstream fout;
    fout.open(path, std::ios::out);
    if (!fout.is_open()) {
        std::cerr << " Файл для выгрузки не был создан";
        return;
    }
    auto it = DB.begin();
    while (it != DB.end()) {
        PrintTableClass t(&it->second, it->first);
        t.Print(fout);
        fout << '\n';
        ++it;
    }
}

void MyCoolDB::PrintTable(const std::string& name) {
    if (!DB.contains(name)) {
        std::cout << "У меня нет информации о такой таблице\n";
        return;
    }
    PrintTableClass t(&DB[name], name);
    t.Print();
}

bool MyCoolDB::Parse(std::string& for_parsing) {
    ParserSQL p;
    std::vector<std::string_view> comands = split_string(';', for_parsing);
    for (uint64_t i = 0; i < comands.size(); ++i) {
        std::string comand = std::string(comands[i]);
        if (GetStringWithoutSEPLastBegin(comand) == "") {
            continue;
        }
        if (!p.Parse(comand)) {
            std::cerr << "Не удалось распознать, что вы хотели\n";
            return false;
        }
        Result res = p.res;
        switch (res.operations) {
            case CREATETABLE:
                try {
                    CreateTable(std::get<CreateTableSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
            case INSERT:
                try {
                    Insert(std::get<InsertSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
            default:
            case SELECT:
                try {
                    Select(std::get<SelectSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
            case UPDATE:
                try {
                    Update(std::get<UpdateSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
            case DELETE:
                try {
                    Delete(std::get<DeleteSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
            case DROP:
                try {
                    Drop(std::get<DropSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
            case SelectJoin:
                try {
                    Join(std::get<JoinSQL>(res.t));
                } catch (const char* error_message) {
                    std::cerr << error_message << '\n';
                    return false;
                }
                break;
        }
    }

    return true;
}

void MyCoolDB::CreateTable(CreateTableSQL t) {
    std::vector<Table*> foreign_tables(t.cols.size());
    for (uint64_t i = 0; i < t.cols.size(); ++i) {
        if (t.cols[i].IamForeign) {
            if (!DB.contains(t.cols[i].name_foreign_table)) {
                throw "Неизвестная таблица для FOREIGN KEY";
            }
            if (!DB[t.cols[i].name_foreign_table].IhaveThisColumn(t.cols[i].name_foreign_key)) {
                throw "Неизвестная таблица для FOREIGN KEY";
            }
            foreign_tables[i] = &DB[t.cols[i].name_foreign_table];
        }
    }
    DB.insert({t.name_of_table, Table(t.cols, foreign_tables, t.name_of_table)});
    PrintTableClass printer(&DB[t.name_of_table], t.name_of_table);

    GiveChildrens(t);
}

void MyCoolDB::GiveChildrens(CreateTableSQL& t) {
    for (uint64_t i = 0; i < t.cols.size(); ++i) {
        if (t.cols[i].IamForeign) {
            int64_t idx_for_key_in_your_table = DB[t.cols[i].name_foreign_table].index_of_col[t.cols[i].name_foreign_key];
            DB[t.cols[i].name_foreign_table].children_keys[idx_for_key_in_your_table] = &DB[t.name_of_table];
            DB[t.cols[i].name_foreign_table].name_of_children_keys[idx_for_key_in_your_table] = t.cols[i].name;
        }
    }
}

void MyCoolDB::Insert(const InsertSQL& value) {
    if (!DB.contains(value.name_of_table)) {
        throw "Я не знаю ничего об этой таблице";
    }
    DB[value.name_of_table].Insert(value);
}

void MyCoolDB::Select(const SelectSQL& value) {
    if (!DB.contains(value.from)) {
        throw "Я не знаю ничего об этой таблице";
    }
    DB[value.from].Select(value);
}

void MyCoolDB::Delete(const DeleteSQL& value) {
    if (!DB.contains(value.name_of_table)) {
        throw "Я не знаю ничего об этой таблице";
    }
    DB[value.name_of_table].Delete(value);
}

void MyCoolDB::Drop(const DropSQL& value) {
    if (!DB.contains(value.name_of_table)) {
        throw "Я не знаю ничего об этой таблице";
    }
    DB.erase(value.name_of_table);
}

void MyCoolDB::Update(const UpdateSQL& value) {
    if (!DB.contains(value.from)) {
        throw "Я не знаю ничего об этой таблице";
    }
    DB[value.from].Update(value);
}

void MyCoolDB::Join(JoinSQL value) {
    if (!DB.contains(value.name_of_table1)) {
        throw "Я не знаю ничего о 1 таблице";
    }
    if (!DB.contains(value.name_of_table2)) {
        throw "Я не знаю ничего о 2 таблице";
    }
    if (!DB[value.name_of_table1].IhaveThisColumn(value.key_from_table1)) {
        throw "Неизвестный столбец для 1 таблицы";
    }
    if (!DB[value.name_of_table2].IhaveThisColumn(value.key_from_table2)) {
        throw "Неизвестный столбец для 2 таблицы";
    }
    auto y = DB[value.name_of_table2].type[DB[value.name_of_table2].index_of_col[value.key_from_table2]];
    auto y2 = DB[value.name_of_table1].type[DB[value.name_of_table1].index_of_col[value.key_from_table1]];
    if (y.T_enum != y2.T_enum) {
        throw "Типы этих ключей несовместимы";
    }
    if (value.all_choose){
        for (uint64_t i=0;i<DB[value.name_of_table1].cout_of_col;++i){
            value.choose.emplace_back(true,DB[value.name_of_table1].name_of_col[i]);
        }
        for (uint64_t i=0;i<DB[value.name_of_table2].cout_of_col;++i){
            value.choose.emplace_back(false,DB[value.name_of_table2].name_of_col[i]);
        }
    }
    switch (value.join_type) {
        case INNER:
            InnerJoin(value, DB[value.name_of_table1], DB[value.name_of_table2], y.T_enum);
            break;
        case LEFT:
            LeftJoin(value.key_from_table1, value.key_from_table2, value.name_of_table1, value.name_of_table2,
                     DB[value.name_of_table1], DB[value.name_of_table2], value.choose);
            break;
        case RIGHT:
            RightJoin(value.key_from_table1, value.key_from_table2, value.name_of_table1, value.name_of_table2,
                      DB[value.name_of_table1], DB[value.name_of_table2], value.choose);
            break;
    }
}
