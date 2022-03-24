
#ifndef DATA_PROCESSING_INTERMEDIATE_TOKENIZER_H
#define DATA_PROCESSING_INTERMEDIATE_TOKENIZER_H


#include <string>
#include <vector>


typedef struct {
    uint64_t type;
    std::string start_match;
    std::string capture_match;
    std::string peek_match;
} Token_Capture;
typedef struct {
    uint64_t type;
    std::string raw_string;
    uint64_t char_stream_begin;
    uint64_t char_stream_end;
} Token;

class Tokenizer {
private:
    std::vector<Token_Capture> captures;
public:
    Tokenizer(std::vector<Token_Capture> captures);
    ~Tokenizer();
    std::vector<Token>* generate_stream(const char* char_stream,uint64_t length);
};


#endif //DATA_PROCESSING_INTERMEDIATE_TOKENIZER_H
