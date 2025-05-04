#include "MessageListWidget.h"
#include "MessageItem.h"
#include <QScrollBar>
#include <qDebug>

MessageListWidget::MessageListWidget(QWidget* parent)
    : QScrollArea(parent)
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    contentWidget_ = new QWidget(this);
    contentWidget_->setObjectName("messageContentWidget");
    
    layout_ = new QVBoxLayout(contentWidget_);
    layout_->setAlignment(Qt::AlignTop);
    layout_->setSpacing(10);
    layout_->setContentsMargins(5, 5, 5, 5);
    
    setWidget(contentWidget_);
    
    setStyleSheet("QScrollArea { background-color: #f5f5f5; border: 1px solid #ddd; }");
    contentWidget_->setStyleSheet("QWidget#messageContentWidget { background-color: #f5f5f5; }");
}

void MessageListWidget::addMessage(const QString& from, const QString& content, 
                                 const QDateTime& timestamp, bool isSystem)
{
    MessageItem* item = new MessageItem(from, content, timestamp, isSystem, contentWidget_);
    // qDebug() << "Adding message from:" << from << "content:" << content << "isSystem:" << isSystem;
    
    layout_->addWidget(item);
    item->show();
    
    scrollToBottom();
}

void MessageListWidget::clear()
{
    QLayoutItem* item;
    while ((item = layout_->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    
    // qDebug() << "MessageListWidget cleared";
}

void MessageListWidget::scrollToBottom()
{
    QScrollBar* scrollBar = verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}
