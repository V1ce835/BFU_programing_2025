#pragma once

#include <iostream>
#include <string>
#include <vector>

class Book {
public:
    Book() = default;
    Book(const std::string& writer, const std::vector<std::string>& vec, int pages, int cost);
    Book(const Book& other);
    ~Book();

    std::string get_w();
    std::vector<std::string> get_t();
    int get_p();
    int get_c();

    Book& operator+(const Book& other);
    Book& operator=(const Book& other);
    Book& operator+=(const Book& other);
    Book operator/(const Book& other);

    void print() const;

private:
    std::string writer_;
    std::vector<std::string> vec_;
    int pages_;
    int cost_;
};
