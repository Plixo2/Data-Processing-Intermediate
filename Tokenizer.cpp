
#include "Tokenizer.h"


#include <regex>
#include <iostream>


Tokenizer::~Tokenizer() {

}

Tokenizer::Tokenizer(std::vector<Token_Capture> captures) : captures(captures) {
}


std::vector<Token> *Tokenizer::generate_stream(const char *char_stream, uint64_t length) {
    auto *list = new std::vector<Token>();

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
            std::regex peek(capture.peek_match);
            if (std::regex_match(as_string, regex) &&
                (capture.peek_match.empty() || std::regex_match(full_string.substr(char_index), peek))) {
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
