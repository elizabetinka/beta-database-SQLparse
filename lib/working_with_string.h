//
// Created by Елизавета Кравченкова on 21.05.2023.
//

#pragma once

#include <iostream>
#include <vector>
#include <set>


std::vector<std::string> split_string_all_sep(const std::string& str);

std::vector<std::string_view> split_string(char sep, const std::string& str);

// возвращает -1 если строка полностью состоит из пробелов
int64_t GetFirstLiterAfterSpaces(const std::string_view& line);

// возвращает -1 если строка полностью состоит из пробелов
int64_t GetLastLiterAfterSpaces(const std::string_view& line, size_t len);

std::string GetStringWithoutSpasesLastBegin(const std::string& for_parse);

// возвращает -1 если строка полностью состоит из разделителей
int64_t GetLastLiterAfterSEP(const std::string_view& line, size_t len);

// возвращает -1 если строка полностью состоит из разделителей
int64_t GetFirstLiterAfterSEP(const std::string_view& line);

std::string GetStringWithoutSEPLastBegin(const std::string& for_parse);
