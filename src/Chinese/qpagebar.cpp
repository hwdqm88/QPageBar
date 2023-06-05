#include "include/utils/qpagebar.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

QPageBar::QPageBar(QWidget* parent) :
    QWidget(parent),
    m_pageTotalCount(0),
    m_pageShowCount(0),
    m_countLabel(new QLabel(tr(u8"共") + QString::number(0) + tr(u8"页"), this)),
    m_pageLineEdit(new QLineEdit(this)),
    m_jumpButton(new QPushButton(tr(u8"确定"), this))
{
    // 1. 初始化layout以及各种控件
    QHBoxLayout* hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setSpacing(3);

    m_previousButton = new QPushButton(tr("<"), this);
    m_previousButton->setWhatsThis("-1");
    m_previousButton->setMaximumWidth(50);
    m_previousButton->setEnabled(false);

    m_nextButton = new QPushButton(">");
    m_nextButton->setWhatsThis("-2");
    QLabel* label1 = new QLabel(tr(u8"前往"), this);
    QLabel* label2 = new QLabel(tr(u8"页"), this);

    m_pageLineEdit->setMaximumWidth(50);

    QFont font;
    font.setPointSize(10);
    m_countLabel->setFont(font);
    m_pageLineEdit->setFont(font);
    label1->setFont(font);
    label2->setFont(font);
    m_previousButton->setFont(font);
    m_nextButton->setFont(font);
    m_jumpButton->setFont(font);


    // 2. 把控件加入layout中
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(m_countLabel);
    hBoxLayout->addWidget(m_previousButton);
    hBoxLayout->addWidget(m_nextButton);
    hBoxLayout->addWidget(label1);
    hBoxLayout->addWidget(m_pageLineEdit);
    hBoxLayout->addWidget(label2);
    hBoxLayout->addWidget(m_jumpButton);

    // 3. 绑定3个信号，实现三种方式页码的跳转（第4种方式是点击页码按钮跳转，这个信号在生成button的时候绑定）
    connect(m_previousButton, &QPushButton::clicked, this, &QPageBar::onButtonClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &QPageBar::onButtonClicked);
    connect(m_jumpButton, &QPushButton::clicked, this, [&]() {
        bool isOk;
        int page = m_pageLineEdit->text().toInt(&isOk);
        if (!isOk || page < 1 || page > m_pageTotalCount)
            QMessageBox::critical(nullptr, tr(u8"页码错误"), 
                tr(u8"请输入正确的页码，范围1~%1").arg(m_pageTotalCount), tr(u8"确定"));
        else
        {
            if (m_currentPage == page)
                return;

            // 如果要跳转的页面当前已经显示，则直接切换
            if (m_pageStart <= page && m_pageStart + m_pageShowCount - 1 >= page)
            {
                m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
                m_pageButtons[page - m_pageStart]->setChecked(true);
            }
            // 如果没有显示，情况1：要跳转的页面比显示的最小页码要小
            else if (m_pageStart > page)
            {
                for (int i = 0; i < m_pageShowCount; i++)
                {
                    m_pageButtons[i]->setWhatsThis(QString::number(page + i));
                    m_pageButtons[i]->setText(QString::number(page + i));
                    m_pageButtons[i]->setChecked(false);
                }
                m_pageButtons[0]->setChecked(true);
                m_pageStart = page;
            }
            // 情况2：要跳转的页面比显示的最大页码要大
            else
            {
                for (int i = 0; i < m_pageShowCount; i++)
                {
                    m_pageButtons[i]->setWhatsThis(QString::number(page - m_pageShowCount + i + 1));
                    m_pageButtons[i]->setText(QString::number(page - m_pageShowCount + i + 1));
                    m_pageButtons[i]->setChecked(false);
                }
                m_pageButtons[m_pageShowCount - 1]->setChecked(true);
                m_pageStart = page - m_pageShowCount + 1;
            }
            m_currentPage = page;
            emit pageChanged(page);
        }
    });
}

QPageBar::QPageBar(int pageTotalCount, int pageShowCount, QWidget* parent) : QPageBar(parent)
{
    setCount(pageTotalCount, pageShowCount);
    if (pageTotalCount > 0)
    {
        m_pageButtons[0]->setChecked(true);
        emit pageChanged(1);
    }
}

void QPageBar::setCount(int totalCount, int showCount)
{
    if (totalCount < showCount)
    {
        qDebug() << "totalCount必须大于或等于showCount";
        return;
    }

    // 1. 首先要把原有的页码按钮清空
    clear();
    
    // 2. 初始化成员变量
    m_pageTotalCount = totalCount;
    m_pageShowCount = showCount;
    m_currentPage = 1;
    m_pageStart = 1;

    m_countLabel->setText(tr(u8"共") + QString::number(m_pageTotalCount) + tr(u8"页"));

    // 3. 加入页码按钮，并且绑定信号
    for (int i = 0; i < showCount; i++)
    {
        QFont font;
        font.setPointSize(10);
        QPushButton* button = new QPushButton(QString::number(i + 1), this);
        button->setFont(font);
        qobject_cast<QHBoxLayout*>(layout())->insertWidget(i + 3, button);
        button->setWhatsThis(QString::number(i + 1));
        button->setCheckable(true);
        m_pageButtons.append(button);
        connect(button, &QPushButton::clicked, this, &QPageBar::onButtonClicked);
    }
    m_pageButtons[0]->setChecked(true);
}

void QPageBar::clear()
{
    for (QPushButton* button : m_pageButtons)
    {
        layout()->removeWidget(button);
        disconnect(button);
        delete button;
    }
    m_pageButtons.clear();
    m_pageShowCount = 0;
    m_pageTotalCount = 0;
}

int QPageBar::currentPage()
{
    return m_currentPage;
}

int QPageBar::pageCount()
{
    return m_pageTotalCount;
}

void QPageBar::onButtonClicked(bool checked)
{
    if (m_pageTotalCount == 0 || m_pageShowCount == 0)
        return;

    int index = qobject_cast<QPushButton*>(sender())->whatsThis().toInt();
    switch (index)
    {
    case -1:    // -1是 < 按钮
        if (m_currentPage == 1)
            break;
        // 如果当前页面不是显示的开头，直接切换即可
        if (m_currentPage > m_pageStart)
        {
            m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
            m_pageButtons[m_currentPage - m_pageStart - 1]->setChecked(true);
        }
        // 如果当前页面是显示的开头，就要整体往前切换
        else
        {
            for (int i = 0; i < m_pageShowCount; i++)
            {
                m_pageButtons[i]->setWhatsThis(QString::number(m_pageStart + i - 1));
                m_pageButtons[i]->setText(QString::number(m_pageStart + i - 1));
                m_pageButtons[i]->setChecked(false);
            }
            m_pageButtons[0]->setChecked(true);
            m_pageStart--;
        }
        m_currentPage--;
        emit pageChanged(m_currentPage);
        
        // 如果已经到最末尾了就禁用>按钮，所以这里要恢复>按钮
        m_nextButton->setEnabled(true);

        // 如果到了最开头就禁用<按钮
        if (m_currentPage == 1)
            m_previousButton->setEnabled(false);

        break;

    case -2:    // -2是 > 按钮
        if (m_currentPage == m_pageTotalCount)
            break;
        // 如果当前页面不是显示的结尾，直接切换即可
        if (m_currentPage < m_pageStart + m_pageShowCount - 1)
        {
            m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
            m_pageButtons[m_currentPage - m_pageStart + 1]->setChecked(true);
        }
        // 如果当前页面是显示的结尾，就要整体往后切换
        else
        {
            for (int i = 0; i < m_pageShowCount; i++)
            {
                m_pageButtons[i]->setWhatsThis(QString::number(m_pageStart + i + 1));
                m_pageButtons[i]->setText(QString::number(m_pageStart + i + 1));
                m_pageButtons[i]->setChecked(false);
            }
            m_pageButtons[m_pageShowCount - 1]->setChecked(true);
            m_pageStart++;
        }
        m_currentPage++;
        emit pageChanged(m_currentPage);

        // 如果已经到最开头了就禁用<按钮，所以这里要恢复<按钮
        m_previousButton->setEnabled(true);

        // 如果到了最末尾就禁用>按钮
        if (m_currentPage == m_pageTotalCount)
            m_nextButton->setEnabled(false);

        break;

    default:    // 其他都是页号按钮
        m_pageButtons[index - m_pageStart]->setChecked(true);
        if (index == m_currentPage)
            break;
        m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
        if (index == 1)
        {
            m_previousButton->setEnabled(false);
            m_nextButton->setEnabled(true);
        }
        else if (index == m_pageTotalCount)
        {
            m_nextButton->setEnabled(false);
            m_previousButton->setEnabled(true);
        }
        else
        {
            m_previousButton->setEnabled(true);
            m_nextButton->setEnabled(true);
        }
        m_currentPage = index;
        emit pageChanged(m_currentPage);

        break;
    }
}
