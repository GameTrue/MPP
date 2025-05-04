#pragma once

#include <QWidget>
#include <QLabel>
#include <QDateTime>

class MessageItem : public QWidget
{
    Q_OBJECT

public:
    explicit MessageItem(const QString& username, const QString& message, 
                        const QDateTime& timestamp, bool isSystem = false, 
                        QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString username_;
    QString message_;
    QDateTime timestamp_;
    bool isSystem_;
    QRect bubbleRect_;
};
