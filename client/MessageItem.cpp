#include "MessageItem.h"
#include <QPainter>
#include <QFontMetrics>
#include <QStyleOption>
#include <QDebug>

MessageItem::MessageItem(const QString& username, const QString& message,
                         const QDateTime& timestamp, bool isSystem,
                         QWidget* parent)
    : QWidget(parent)
    , username_(username)
    , message_(message)
    , timestamp_(timestamp)
    , isSystem_(isSystem)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setMinimumWidth(200);
    setMinimumHeight(50); 
    
    // Debug output
    // qDebug() << "Created MessageItem:" << username << message << isSystem;
    
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    
    setObjectName("messageItem");
}

void MessageItem::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QFont usernameFont = font();
    usernameFont.setBold(true);
    usernameFont.setPointSize(10);
    
    QFont messageFont = font();
    messageFont.setPointSize(10);
    
    QFont timestampFont = font();
    timestampFont.setPointSize(8);
    
    QColor bubbleColor = isSystem_ ? QColor(245, 245, 245) : QColor(230, 235, 240);
    QColor textColor = QColor(50, 50, 50);
    QColor usernameColor = isSystem_ ? QColor(100, 100, 100) : QColor(60, 100, 180);
    QColor timestampColor = QColor(120, 120, 120);
    
    painter.fillRect(rect(), QColor(240, 240, 240, 50));
    
    painter.setFont(usernameFont);
    QFontMetrics usernameFm = painter.fontMetrics();
    int usernameHeight = usernameFm.height();
    
    painter.setFont(messageFont);
    QFontMetrics messageFm = painter.fontMetrics();
    
    int maxWidth = width() - 40;
    QRect messageRect = messageFm.boundingRect(
        QRect(0, 0, maxWidth, 1000), 
        Qt::TextWordWrap, 
        message_
    );
    
    painter.setFont(timestampFont);
    QFontMetrics timestampFm = painter.fontMetrics();
    QString timeStr = timestamp_.toString("hh:mm");
    int timestampWidth = timestampFm.horizontalAdvance(timeStr);
    int timestampHeight = timestampFm.height();
    
    int bubblePadding = 12;
    int marginTop = 5;
    int bubbleWidth = width() - 20;
    int bubbleHeight = usernameHeight + messageRect.height() + timestampHeight + 2 * bubblePadding;
    
    bubbleHeight = qMax(bubbleHeight, 50);
    
    bubbleRect_ = QRect(10, marginTop, bubbleWidth, bubbleHeight);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bubbleColor);
    painter.drawRoundedRect(bubbleRect_, 10, 10);
    
    painter.setFont(usernameFont);
    painter.setPen(usernameColor);
    painter.drawText(20, marginTop + bubblePadding + usernameFm.ascent(), username_);
    
    painter.setFont(messageFont);
    painter.setPen(textColor);
    
    QRect textRect(20, marginTop + bubblePadding + usernameHeight + 5, 
                   maxWidth, messageRect.height());
    
    painter.drawText(textRect, Qt::TextWordWrap, message_);
    
    painter.setFont(timestampFont);
    painter.setPen(timestampColor);
    painter.drawText(
        QRect(bubbleWidth - timestampWidth - 10, 
              marginTop + bubbleHeight - timestampHeight - 8,
              timestampWidth,
              timestampHeight), 
        Qt::AlignRight | Qt::AlignBottom,
        timeStr
    );
    
    int totalHeight = bubbleHeight + marginTop * 2;
    setMinimumHeight(totalHeight);
    setMaximumHeight(totalHeight);
    
    // Debug draw bounds
    // painter.setPen(Qt::red);
    // painter.drawRect(0, 0, width()-1, height()-1);
}
