# QPageBar

[点击这里切换为中文](README.zh_CN.md) 

## 1. Introduction

QPageBar is a Qt widget based on Qt 5/Qt 6, which can be used to implement the page switching function (similar to various forum page bars, where you can switch pages by clicking on the page number or entering the page number).

## 2. Usage

### 1. Import files

Simply import the ```qpagebar.h``` and ```qpagebar.cpp``` files in the ```src``` folder into the project to use. (The ```Chinese``` folder contains comments in Chinese, while the ```English``` folder contains comments in English.)

### 2. Declare the QPageBar class

There are two ways to declare the QPageBar class:

The first method has only one parent parameter:
```cpp
QPageBar(QWidget* parent = nullptr);
```
+ This constructor has only one parent parameter, so you can directly declare QPageBar in the .ui file, and Qt's ui compiler will automatically call this constructor.
+ This function declares a PageBar with a page count of zero, and you need to call the ```setCount``` method to set the page count.

The second method has three parameters:
```cpp
QPageBar(int pageTotalCount, int pageShowCount = 5, QWidget* parent = nullptr);
```
+ This function declares a PageBar with a total page count of ```pageTotalCount``` and a display page button count of ```pageShowCount```.

### 3. Implementing page switching

There are four ways to switch pages:

+ Click on the previous page button (<).
+ Click on the next page button (>).
+ Click on the page number button.
+ Enter the page number and click OK.

All four ways will activate the ```pageChanged``` signal, and the signal parameter ```int page``` is the page number to jump to. You only need to connect the ```pageChanged``` signal with the jump logic.

### 4. Other public methods are detailed in the code comments

## 3. Test program

There is a test program in the demo folder, which can be built using qmake. Simply open QPageBarDemo.pro with Qt Creator and compile and run it.