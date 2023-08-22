//
// Created by Елизавета Кравченкова on 27.04.2023.
//
#include <lib/MyCoolDB.h>
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <list>
// Требования
// Название таблицы на английском(лучше вообще все на англйском)
// Отсутствие пробелов в названиях
// Ключевые слова все в upper_case
// Логические операции допускают NOT только в начале условия, порядок выполнения OR AND просто по порядку

TEST(ParseTestSuite, CreateTable) {
    MyCoolDB DB;
    //DB.CreateTable("Лизочка",{"Предмет","1 семестр","2 семестр"},{STRING,INT,DOUBLE});
    std::string parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20) PRIMARY KEY ,
        1_semestr int NOT NULL,
        2_semestr double ))";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;
    ASSERT_TRUE(true);
}

TEST(ParseTestSuite, INSERTTable) {
    MyCoolDB DB;
    std::string parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20) PRIMARY KEY ,
        1_semestr int NOT NULL,
        2_semestr double );
        INSERT INTO  Lizochka (Predmet,1_semestr,2_semestr) VALUES ("Matan",4,3.5);
        INSERT INTO  Lizochka  VALUES ("Algosy",5,4.5);
        INSERT INTO  Lizochka  (Predmet,1_semestr) VALUES ("Proga",100);
)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;


    // Должно высветится красное, так как 2 столбец удовлетворяет требованиям NOT NULL KEY
    parse = R"(
        INSERT INTO  Lizochka  VALUES ("балет",NULL,NULL))";
    ASSERT_FALSE(DB.Parse(parse));

    // Должно высветится красное, так как 1 столбец удовлетворяет требованиям PRIMARY KEY
    parse = R"(
        INSERT INTO  Lizochka VALUES ("Matan",8,4.5))";
    ASSERT_FALSE(DB.Parse(parse));
}

TEST(ParseTestSuite, SELECTTable){
    MyCoolDB DB;
    std::string parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20)  ,
        1_semestr int,
        2_semestr double );
        INSERT INTO  Lizochka (Predmet,1_semestr,2_semestr) VALUES ("Математика",4,3.5);
        INSERT INTO  Lizochka  VALUES ("Алгосы",5,4.5);
        INSERT INTO  Lizochka  (Predmet,1_semestr) VALUES ("Прога",100);
        INSERT INTO  Lizochka  VALUES ("балет",NULL,NULL);
)";
    ASSERT_TRUE(DB.Parse(parse));

    parse = R"(
        SELECT 1_semestr, 2_semestr FROM   Lizochka;)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << "\n\n";

    parse = R"(
        SELECT * FROM   Lizochka;)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << "\n\n";

    parse = R"(
        SELECT 1_semestr, 2_semestr
        FROM   Lizochka
        WHERE Predmet="балет";)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << "\n\n";

    parse = R"(
        SELECT *
        FROM   Lizochka
        WHERE 2_semestr IS NULL;)";
    DB.Parse(parse);
    std::cout << "\n\n";

    parse = R"(
        SELECT 1_semestr, 2_semestr
        FROM   Lizochka
        WHERE 1_semestr=5 OR  2_semestr >= 3;)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << "\n\n";

    parse = R"(
        SELECT Predmet
        FROM   Lizochka
        WHERE NOT 1_semestr=5)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << "\n\n";

    ASSERT_TRUE(true);
}

TEST(ParseTestSuite, UpdateTable) {
    MyCoolDB DB;
    std::string parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20)  ,
        1_semestr int,
        2_semestr double );
        INSERT INTO  Lizochka (Predmet,1_semestr,2_semestr) VALUES ("Математика",4,3.5);
        INSERT INTO  Lizochka  VALUES ("Алгосы",5,4.5);
        INSERT INTO  Lizochka  (Predmet,1_semestr) VALUES ("Прога",100);
        INSERT INTO  Lizochka  VALUES ("балет",NULL,NULL);
)";
    ASSERT_TRUE(DB.Parse(parse));

    parse = R"(
        UPDATE Lizochka
        SET 1_semestr = 5, 2_semestr =5,
        WHERE Predmet = "балет")";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    parse = R"(
        UPDATE Lizochka
        SET 1_semestr = 5, 2_semestr =5,
         WHERE NOT 1_semestr=5)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    parse = R"(
        UPDATE Lizochka
        SET 1_semestr = NULL, 2_semestr =NULL,
         WHERE 1_semestr=5)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    // Тут должна появиться информация о том, что таких данных не найдено
    parse = R"(
        SELECT *
        FROM   Lizochka
        WHERE NOT 2_semestr IS NULL;)";
    DB.Parse(parse);
    std::cout << std::endl;

    ASSERT_TRUE(true);
}

TEST(ParseTestSuite, DeleteTable) {
    MyCoolDB DB;
    std::string parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20) ,
        1_semestr int,
        2_semestr double );
        INSERT INTO  Lizochka (Predmet,1_semestr,2_semestr) VALUES ("Математика",4,3.5);
        INSERT INTO  Lizochka  VALUES ("Алгосы",5,4.5);
        INSERT INTO  Lizochka  (Predmet,1_semestr) VALUES ("Прога",100);
        INSERT INTO  Lizochka  VALUES ("балет",NULL,NULL);
)";
    ASSERT_TRUE(DB.Parse(parse));

    parse = R"(
        DELETE FROM Lizochka
        WHERE Predmet = "балет")";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    parse = R"(
        DELETE FROM Lizochka
        WHERE 2_semestr =NULL)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    // Тут должно высветиться отсутствие строк
    parse = R"(
        DELETE FROM Lizochka)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;


    parse = R"(
        DROP TABLE Lizochka)";
    ASSERT_TRUE(DB.Parse(parse));

    // Тут должна высветиться информация об отсутствии  таблицы
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    ASSERT_TRUE(true);
}

TEST(ParseTestSuite, ForeignTable) {
    MyCoolDB DB;
    std::string parse = R"(
        CREATE     TABLE  Univerity (
        subject varchar ( 20) PRIMARY KEY ,
        teacher_name varchar ( 20),
        time_in_year int );
        INSERT INTO  Univerity VALUES ("Матан","Далевская",59);
        INSERT INTO  Univerity  VALUES ("Прога","Xвастунов",30);
        INSERT INTO  Univerity  VALUES ("Алгосы","Пермяков",30);
)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Univerity");

    parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20) FOREIGN KEY REFERENCES  Univerity(subject) ,
        1_semestr int NOT NULL,
        2_semestr double );
        INSERT INTO  Lizochka (Predmet,1_semestr,2_semestr) VALUES ("Матан",4,3.5);
        INSERT INTO  Lizochka  VALUES ("Алгосы",5,4.5);
        INSERT INTO  Lizochka  (Predmet,1_semestr) VALUES ("Прога",100);
)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;

    // должна высветиться информация о том что такая строка не удовлетворяет требованиям FOREIGN KEY
    // а именно добавляется значение, которого нет в родительской таблице см https://www.w3schools.com/sql/sql_foreignkey.asp
    parse = R"(
        INSERT INTO  Lizochka  VALUES ("балет",NULL,NULL))";
    ASSERT_FALSE(DB.Parse(parse));

    parse = R"(
        DELETE FROM Univerity
        WHERE subject = "Матан")";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Univerity");

    // Тут демонстрируется связь между таблицами. Должна появиться надпись об отсутствии подходящих данных.
    parse = R"(
        SELECT *
        FROM   Lizochka
        WHERE Predmet="Матан";)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << std::endl;
}

TEST(ParseTestSuite, JoinTable) {
    MyCoolDB DB;
    std::string parse = R"(
        CREATE     TABLE  Univerity (
        subject varchar ( 20) PRIMARY KEY ,
        teacher_name varchar ( 20),
        time_in_year int );
        INSERT INTO  Univerity VALUES ("Matan","Dalevskayy",59);
        INSERT INTO  Univerity  VALUES ("Proga","Hvastunov",30);
        INSERT INTO  Univerity  VALUES ("Algosy","Permyykov",30);
        INSERT INTO  Univerity  VALUES ("Ballroom","Starceva",175);
        INSERT INTO  Univerity  VALUES ("Diskret","Chycharef",60);
)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Univerity");

    parse = R"(
        CREATE     TABLE  Lizochka (
        Predmet varchar ( 20) FOREIGN KEY REFERENCES  Univerity(subject) ,
        1_semestr int NOT NULL,
        2_semestr double );
        INSERT INTO  Lizochka (Predmet,1_semestr,2_semestr) VALUES ("Matan",4,3.5);
        INSERT INTO  Lizochka  VALUES ("Algosy",5,4.5);
        INSERT INTO  Lizochka  VALUES ("Algosy",7,8.5);
        INSERT INTO  Lizochka  (Predmet,1_semestr) VALUES ("Proga",100);
        INSERT INTO  Lizochka  VALUES ("Proga",100,100.3);
)";
    ASSERT_TRUE(DB.Parse(parse));
    DB.PrintTable("Lizochka");
    std::cout << std::endl;


    parse = R"(
        SELECT Univerity.subject, Univerity.teacher_name, Lizochka.1_semestr
        FROM   Lizochka
        INNER JOIN Univerity
        ON Lizochka.Predmet = Univerity.subject)";

    ASSERT_TRUE(DB.Parse(parse));
    std::cout << std::endl;

    parse = R"(
        SELECT Univerity.subject, Univerity.teacher_name, Lizochka.1_semestr
        FROM   Lizochka
        LEFT JOIN Univerity
        ON Lizochka.Predmet = Univerity.subject)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << std::endl;


    parse = R"(
        SELECT Univerity.subject, Univerity.teacher_name, Lizochka.1_semestr
        FROM   Lizochka
        RIGHT JOIN Univerity
        ON Lizochka.Predmet = Univerity.subject)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << std::endl;

    parse = R"(
        SELECT *
        FROM   Lizochka
        RIGHT JOIN Univerity
        ON Lizochka.Predmet = Univerity.subject)";
    ASSERT_TRUE(DB.Parse(parse));
    std::cout << std::endl;
    DB.LoadToFile("MyDataBase.msql");
}
