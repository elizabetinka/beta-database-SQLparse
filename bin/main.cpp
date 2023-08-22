#pragma once
#include <lib/MyCoolDB.h>
#include <fstream>
#include <sstream>
// Требования
// Название таблицы на английском(лучше вообще все на англйском)
// Отсутствие пробелов в названиях
// Ключевые слова все в upper_case

std::string readFile(const std::ifstream& f) {
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}
// parse file "path"
// parse comand
// save "path"
int main(int argc, char **argv){
    setlocale(LC_ALL, "");
    MyCoolDB DB;
    if (argc<3){
        std::cerr<<"Недостаточно аргументов";
        return 0;
    }
    if (strcmp(argv[1],"parse")==0){
        if (strcmp(argv[2],"file")==0){
            std::string path=argv[3];
            std::ifstream fin(path);
            if (!fin.is_open()) {
                std::cerr << " Ошибка открытия файла1";
                return 0;
            }
            std::string for_parse = readFile(fin);
            DB.Parse(for_parse);
        }
        else{
            std::string for_parse;
            int64_t iter=2;
            while (iter<argc){
                for_parse.insert(for_parse.size()-1,argv[iter]);
                ++iter;
            }
            DB.Parse(for_parse);
        }
    }
    else if (strcmp(argv[1],"save")==0){
        std::string path=argv[2];
        DB.LoadToFile(path);
    }
    else{
        std::cerr<<"Неизвестная команда";
        return 0;
    }

    return 0;

}
