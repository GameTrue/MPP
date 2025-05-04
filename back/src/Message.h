#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public:
    Message(const std::string& from, const std::string& to, const std::string& content);
    std::string getFrom() const;
    std::string getTo() const;
    std::string getContent() const;

private:
    std::string from;
    std::string to;
    std::string content;
};

#endif
