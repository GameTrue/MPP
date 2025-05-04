#include "Message.h"

Message::Message(const std::string& from, const std::string& to, const std::string& content)
    : from(from), to(to), content(content) {}

std::string Message::getFrom() const { return from; }
std::string Message::getTo() const { return to; }
std::string Message::getContent() const { return content; }
