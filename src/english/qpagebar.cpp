#include "qpagebar.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

QPageBar::QPageBar(QWidget* parent) :
    QWidget(parent),
    m_pageTotalCount(0),
    m_pageShowCount(0),
    m_countLabel(new QLabel(tr("Total pages: ") + QString::number(0), this)),
    m_pageLineEdit(new QLineEdit(this)),
    m_jumpButton(new QPushButton(tr("Ok"), this))
{
    // 1. Initialize the layout and various widgets. If you need to adjust the size or font of the widgets, modify this part of the code.
    // 1.1 Set horizontal layout
    QHBoxLayout* hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setSpacing(3);

    // 1.2 Initialize the previous page button
    m_previousButton = new QPushButton(tr("<"), this);
    m_previousButton->setWhatsThis("-1"); // Record which button the current button belongs to through WhatsThis to facilitate logic processing after button click
    m_previousButton->setEnabled(false);

    // 1.3 Initialize the next page button
    m_nextButton = new QPushButton(">");
    m_nextButton->setWhatsThis("-2"); // Record which button the current button belongs to through WhatsThis to facilitate logic processing after button click
    QLabel* label1 = new QLabel(tr("Go to page No."), this);

    // 1.4 Set the size of the page number input box
    m_pageLineEdit->setMaximumWidth(75);

    // 1.5 Set the font
    QFont font;
    font.setPointSize(10);
    m_countLabel->setFont(font);
    m_pageLineEdit->setFont(font);
    label1->setFont(font);
    m_previousButton->setFont(font);
    m_nextButton->setFont(font);
    m_jumpButton->setFont(font);

    // 2. Add widgets to the layout
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(m_countLabel);
    hBoxLayout->addWidget(m_previousButton);
    hBoxLayout->addWidget(m_nextButton);
    hBoxLayout->addWidget(label1);
    hBoxLayout->addWidget(m_pageLineEdit);
    hBoxLayout->addWidget(m_jumpButton);


    // 3. Bind 3 signals to achieve 3 ways of page jumping (the 4th way is to click the page number button, which is bound when generating the button)
    // 3.1 Logic of the previous page button
    connect(m_previousButton, &QPushButton::clicked, this, &QPageBar::onButtonClicked);
    // 3.2 Logic of the next page button
    connect(m_nextButton, &QPushButton::clicked, this, &QPageBar::onButtonClicked);
    // 3.3 Logic of the jump button
    connect(m_jumpButton, &QPushButton::clicked, this, [&]() {
        bool isOk;
        int page = m_pageLineEdit->text().toInt(&isOk);
        if (!isOk || page < 1 || page > m_pageTotalCount)
        {
            QMessageBox errMsgBox(QMessageBox::Critical, tr("Page number error"),
                                  tr("Please enter the correct page number, range 1~%1").arg(m_pageTotalCount));
            errMsgBox.exec();
        }
        else
        {
            if (m_currentPage == page)
                return;

            // If the page to be jumped to is already displayed, switch directly
            if (m_pageStart <= page && m_pageStart + m_pageShowCount - 1 >= page)
            {
                m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
                m_pageButtons[page - m_pageStart]->setChecked(true);
            }
            // If not displayed, case 1: the page to be jumped to is smaller than the minimum displayed page number
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
            // Case 2: the page to be jumped to is larger than the maximum displayed page number
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
        m_pageButtons[0]->setChecked(true);
}

void QPageBar::setCount(int totalCount, int showCount)
{
    if (totalCount < showCount) // if the total count is smaller than the show count
    {
        qDebug() << "totalCount必须大于或等于showCount"; // totalCount must be greater than or equal to showCount
        return;
    }

    // 1. First, clear the existing page number buttons
    clear();

    // 2. Initialize member variables
    m_pageTotalCount = totalCount;
    m_pageShowCount = showCount;
    m_currentPage = 1;
    m_pageStart = 1;

    m_countLabel->setText(tr("Total pages: ") + QString::number(m_pageTotalCount)); // set the count label text to show the total number of pages

    // 3. Add page number buttons and bind signals to them
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
    case -1:    // -1 is the < button
        if (m_currentPage == 1)
            break;
        // If the current page is not the beginning, switch directly.
        if (m_currentPage > m_pageStart)
        {
            m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
            m_pageButtons[m_currentPage - m_pageStart - 1]->setChecked(true);
        }
        // If the current page is the beginning, shift the entire page to the previous one.
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
        
        // If it has reached the end, disable the > button. This line restores it.
        m_nextButton->setEnabled(true);
        
        // If it has reached the beginning, disable the < button.
        if (m_currentPage == 1)
            m_previousButton->setEnabled(false);
        
        break;

    case -2: // -2 represents the > button
        if (m_currentPage == m_pageTotalCount)
            break;
        // If the current page is not the end of the displayed pages, switch directly
        if (m_currentPage < m_pageStart + m_pageShowCount - 1)
        {
            m_pageButtons[m_currentPage - m_pageStart]->setChecked(false);
            m_pageButtons[m_currentPage - m_pageStart + 1]->setChecked(true);
        }
        // If the current page is the end of the displayed pages, shift the entire list to the right
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

        // If we have reached the beginning of the list, re-enable the < button
        m_previousButton->setEnabled(true);

        // If we have reached the end of the list, disable the > button
        if (m_currentPage == m_pageTotalCount)
            m_nextButton->setEnabled(false);

        break;

    default: // All other cases represent individual page buttons
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
