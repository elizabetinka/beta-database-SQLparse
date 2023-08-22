//
// Created by Елизавета Кравченкова on 21.05.2023.
//

#include "working_with_string.h"

std::set<char> spliters = {',', ' ', '\n', '\t'};
std::set<char> spliters_good = {'(', ')', '<', '>', '='};

std::vector<std::string> split_string_all_sep(const std::string& str) {
    std::vector<std::string> for_ans;
    std::string str_cur;
    uint64_t q = 0;
    uint64_t len = str.length();
    while (true) {
        if (spliters_good.contains(str[q])) {
            str_cur.push_back(str[q]);
            if (str[q] == '>' || str[q] == '<') {
                ++q;
                if (q > len) {
                    break;
                }
                if (str[q] == '=') {
                    str_cur.push_back(str[q]);
                } else {
                    --q;
                }
            }
            for_ans.push_back(str_cur);
            str_cur.clear();
            ++q;
        }
        while (q < len && !spliters.contains(str[q])) {
            if (spliters_good.contains(str[q])) {
                if (!str_cur.empty()) {
                    for_ans.push_back(str_cur);
                    str_cur.clear();
                }
                str_cur.push_back(str[q]);
                for_ans.push_back(str_cur);
                str_cur.clear();
                ++q;
            } else {
                str_cur.push_back(str[q]);
                ++q;
            }
        }
        if (!str_cur.empty()) {
            for_ans.push_back(str_cur);
            str_cur.clear();
        }
        while (q < len && spliters.contains(str[q])) {
            ++q;
        }
        if (q >= len) {
            break;
        }
    }
    return for_ans;
}

std::vector<std::string_view> split_string(char sep, const std::string& str) {
    std::vector<std::string_view> for_ans;
    std::string_view friend_str{str};
    uint64_t len = str.length();
    uint64_t q = 0;
    while (str[q] == sep) {
        q++;
        if (q == len) {
            return for_ans;
        }
    }
    for (uint64_t p = q; p < len; p++) {
        if (str[p] == sep && p != q) {
            for_ans.push_back(friend_str.substr(q, p - q));
            q = p;
            while (str[q] == sep) {
                q++;
                if (q == len) {
                    return for_ans;
                }
            }
            p = q;
        }
    }
    for_ans.push_back(friend_str.substr(q));
    return for_ans;
}

// возвращает -1 если строка полностью состоит из пробелов
int64_t GetFirstLiterAfterSpaces(const std::string_view& line) {
    uint32_t iter = 0;
    while (line[iter] == ' ') {
        iter++;
        // пустая строка
        if (iter == line.length()) {
            return -1;
        }
    }
    return iter;
}

// возвращает -1 если строка полностью состоит из пробелов
int64_t GetLastLiterAfterSpaces(const std::string_view& line, size_t len) {
    uint32_t iter = len - 1;
    while (line[iter] == ' ') {
        iter--;
        // пустая строка
        if (iter == 0) {
            return -1;
        }
    }
    return iter;
}

// возвращает -1 если строка полностью состоит из разделителей
int64_t GetLastLiterAfterSEP(const std::string_view& line, size_t len) {
    uint32_t iter = len - 1;
    while (spliters.contains(line[iter])) {
        iter--;
        // пустая строка
        if (iter == 0) {
            return -1;
        }
    }
    return iter;
}

// возвращает -1 если строка полностью состоит из разделителей
int64_t GetFirstLiterAfterSEP(const std::string_view& line) {
    uint32_t iter = 0;
    while (spliters.contains(line[iter])) {
        iter++;
        // пустая строка
        if (iter == line.length()) {
            return -1;
        }
    }
    return iter;
}

std::string GetStringWithoutSpasesLastBegin(const std::string& for_parse) {
    //i______J
    int64_t f = GetFirstLiterAfterSpaces(for_parse);
    if (f == -1) {
        return "";
    }
    int64_t ff = GetLastLiterAfterSpaces(for_parse, for_parse.length());
    if (ff == -1) {
        return "";
    }
    return for_parse.substr(f, ff - f + 1);
}

std::string GetStringWithoutSEPLastBegin(const std::string& for_parse) {
    //i______J
    int64_t f = GetFirstLiterAfterSEP(for_parse);
    if (f == -1) {
        return "";
    }
    int64_t ff = GetLastLiterAfterSEP(for_parse, for_parse.length());
    if (ff == -1) {
        return "";
    }
    return for_parse.substr(f, ff - f + 1);
}
