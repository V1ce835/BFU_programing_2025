#include "Header.h"
#include <algorithm>  

Book::Book(const std::string& writer, const std::vector<std::string>& vec, int pages, int cost)
    : writer_(writer), vec_(vec), pages_(pages), cost_(cost) {
}

Book::Book(const Book& other)
    : writer_(other.writer_), vec_(other.vec_), pages_(other.pages_), cost_(other.cost_) {
}

Book::~Book() {
    vec_.clear();
    std::cout << "dtor" << std::endl;
}

std::string Book::get_w() {
    return writer_;
}

std::vector<std::string> Book::get_t() {
    return vec_;
}

int Book::get_p() {
    return pages_;
}

int Book::get_c() {
    return cost_;
}

Book& Book::operator=(const Book& other) {
    if (this != &other) {
        writer_ = other.writer_;
        vec_ = other.vec_;
        pages_ = other.pages_;
        cost_ = other.cost_;
    }
    return *this;
}

Book& Book:: operator+(const Book& other) {
    pages_ += other.pages_;
    cost_ = (cost_ + other.cost_) * 0.85;

    if (writer_ == other.writer_) {
        for (const auto& work : other.vec_) {
            bool exists = false;
            for (const auto& vec : vec_) {
                if (vec == work) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                vec_.push_back(work);
            }
        }
    }
    else {
        std::cout << "Error";
    }
    return *this;
}

Book& Book::operator+=(const Book& other) {
    pages_ += other.pages_;
    cost_ = (cost_ + other.cost_) * 0.85; 
    std::string gg = writer_;
    writer_ = writer_ + " и " + other.writer_;

    for (auto& work : vec_) {
        work = gg + work;
    }
    for (const auto& work : other.vec_) {
        vec_.push_back(other.writer_ + work);
    }
    return *this;
}

Book Book::operator/(const Book& other) {
    Book result;
    result.writer_ = writer_;
    result.pages_ = static_cast<int>((pages_ + other.pages_) * 0.7); 
    result.cost_ = static_cast<int>((cost_ + other.cost_) * 1.1);     

    std::vector<std::string> selected;

  
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    if (!vec_.empty()) {
        int count1 = 1 + rand() % vec_.size();
        std::vector<int> used;
        for (int i = 0; i < count1; i++) {
            int idx;
            do {
                idx = rand() % vec_.size();
            } while (std::find(used.begin(), used.end(), idx) != used.end());
            used.push_back(idx);

            if (writer_ == other.writer_) {
                selected.push_back(vec_[idx]);
            }
            else {
                selected.push_back(writer_ + vec_[idx]);
            }
        }
    }
    if (!other.vec_.empty()) {
        int count2 = 1 + rand() % other.vec_.size();
        std::vector<int> usedIndexes;
        for (int i = 0; i < count2; i++) {
            int idx;
            do {
                idx = rand() % other.vec_.size();
            } while (std::find(usedIndexes.begin(), usedIndexes.end(), idx) != usedIndexes.end());
            usedIndexes.push_back(idx);
            std::string work;
            if (writer_ == other.writer_) {
                work = other.vec_[idx];
            }
            else {
                work = other.writer_ + other.vec_[idx];
            }
            if (std::find(selected.begin(), selected.end(), work) == selected.end()) {
                selected.push_back(work);
            }
        }
    }
    result.vec_ = selected;
    return result;
}

void Book::print() const {
    std::cout << "[" << writer_ << ", (";
    for (size_t i = 0; i < vec_.size(); i++) {
        std::cout << vec_[i];
        if (i < vec_.size() - 1) std::cout << ",";
    }
    std::cout << "), " << pages_ << ", " << cost_ << "]" << std::endl;
}