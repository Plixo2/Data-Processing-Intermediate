
#include "Tokenizer.h"


#include <regex>
#include <iostream>
#include <set>


class DuplicateToken: public std::exception
{
    virtual const char* what() const throw()
    {
        return "A Token Type is found twice";
    }
} excp;

Tokenizer::~Tokenizer() {

}

Tokenizer::Tokenizer(std::vector<Token_Capture> captures) : captures(captures) {
    std::set<uint64_t> set;
    for (const auto &item : captures) {
        if(set.contains(item.type)) {
            throw excp;
        }
        set.insert(item.type);
    }
}


std::vector<Token> *Tokenizer::generate_stream(const char *char_stream, uint64_t length) {
    auto *list = new std::vector<Token>();
    length += 1;
    uint64_t char_index = 0;
    std::string as_string;
    std::string full_string;
    char *data = const_cast<char *>(char_stream);
    int size = length;
    full_string.assign(data, size);
    as_string += char_stream[char_index];

    while (char_index < length) {
        bool matched = false;
        for (Token_Capture capture: captures) {
            std::regex regex(capture.start_match);
            std::regex peek(capture.peek_match,  std::regex_constants::ECMAScript |  std::regex_constants::extended );
            if (std::regex_match(as_string, regex) &&
                (capture.peek_match.empty() || std::regex_search(full_string.substr(char_index), peek))) {
                matched = true;
                std::regex capture_regex(capture.capture_match);
                char_index++;
                while (char_index < length) {
                    as_string += char_stream[char_index];
                    char_index++;
                    if (!std::regex_match(as_string, capture_regex)) {
                        as_string.erase(as_string.length() - 1);
                        break;
                    }
                }
                if (char_index < length) {
                    char_index--;
                }
                list->push_back({capture.type, as_string, 0, 0});
                as_string.clear();
                as_string += char_stream[char_index];
                break;
            }
        }
        if (!matched) {
            std::cout << "Failed to capture " << as_string << std::endl;
            break;
        }
    }

    return list;
}
