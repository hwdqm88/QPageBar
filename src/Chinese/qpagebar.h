#ifndef QPAGEBAR_H
#define QPAGEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVector>

/**
* @brief 这个类是切换页码的控件，类似于论坛最下面的那种
*/
class QPageBar : public QWidget
{
    Q_OBJECT
public:
    explicit QPageBar(QWidget* parent = nullptr);
    
    /**
    * @brief 构造函数
    * @param pageTotalCount 全部的页面数量
    * @param pageShowCount 显示的页码按钮数量 
    */
    explicit QPageBar(int pageTotalCount, int pageShowCount = 5, QWidget* parent = nullptr);

    /**
    * @brief 设置页面的数量，totalCount必须大于等于showCount
    * @param totalCount 全部的页面数量
    * @param showCount 显示的页码按钮数量
    */
    void setCount(int totalCount, int showCount);

    /**
    * @brief 清空页面按钮和数量
    */
    void clear();

    /**
    * @brief 获取当前页面号
    * @return 当前页号
    */
    int currentPage();

    /**
    * @brief 获取页面数量
    * @return 页面数量
    */
    int pageCount();

signals:
    /**
    * @brief 当页面切换时，激活这一信号
    * @param page 页号
    */
    void pageChanged(int page);

protected slots:
    /**
    * @brief 点击按钮后触发这一信号
    * @param checked clicked信号的参数，可忽略
    */
    void onButtonClicked(bool checked);

private:
    /**
    * @brief 页面总数
    */
    int m_pageTotalCount;

    /**
    * @brief 在pageBar中显示的页面数
    */
    int m_pageShowCount;

    /**
    * @brief 显示的页面中最左面的（也就是最小的）页码
    */
    int m_pageStart;

    /**
    * @brief 当前选中的页码
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
