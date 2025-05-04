#pragma once

#include <QScrollArea>
#include <QVBoxLayout>
#include <QDateTime>

class MessageListWidget : public QScrollArea
{
    Q_OBJECT

public:
    explicit MessageListWidget(QWidget* parent = nullptr);
    
    void addMessage(const QString& from, const QString& content, 
                   const QDateTime& timestamp, bool isSystem = false);
    void clear();

private:
    QWidget* contentWidget_;
    QVBoxLayout* layout_;
    void scrollToBottom();
};
