#ifndef QPAGEBAR_H
#define QPAGEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVector>

/**
* @brief This class is a widget for switching page numbers, similar to the ones at the bottom of a forum.
*/
class QPageBar : public QWidget
{
    Q_OBJECT
public:
    explicit QPageBar(QWidget* parent = nullptr);
    
    /**
    * @brief Constructor
    * @param pageTotalCount Total number of pages
    * @param pageShowCount Number of page buttons to show
    */
    explicit QPageBar(int pageTotalCount, int pageShowCount = 5, QWidget* parent = nullptr);

    /**
    * @brief Set the number of pages, where totalCount must be greater than or equal to showCount
    * @param totalCount Total number of pages
    * @param showCount Number of page buttons to show
    */
    void setCount(int totalCount, int showCount);

    /**
    * @brief Clear the page buttons and page count
    */
    void clear();

    /**
    * @brief Get the current page number
    * @return The current page number
    */
    int currentPage();

    /**
    * @brief Get the total number of pages
    * @return The total number of pages
    */
    int pageCount();

signals:
    /**
    * @brief This signal will be activated when a page is switched
    * @param page The page number
    */
    void pageChanged(int page);

protected slots:
    /**
    * @brief Triggered when a button is clicked
    * @param checked The parameter of the clicked signal, can be ignored
    */
   
    void onButtonClicked(bool checked);

private:
    /**
    * @brief Total number of pages
    */
    int m_pageTotalCount;

    /**
    * @brief Number of pages to show in the page bar
    */
    int m_pageShowCount;

    /**
    * @brief The leftmost (and smallest) page number shown in the widget
    */
    int m_pageStart;

    /**
    * @brief The currently selected page number
    */
    int m_currentPage;

    QLabel* m_countLabel;
    QLineEdit* m_pageLineEdit;
    QPushButton* m_previousButton;
    QPushButton* m_nextButton;
    QPushButton* m_jumpButton;
    QVector<QPushButton*> m_pageButtons;
};


#endif // !QPAGEBAR_H