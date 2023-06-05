# QPageBar

[English README](README.md) 

## 一、简介

QPageBar是一个基于Qt 5/Qt 6的Qt控件，可以实现切换页面的功能（类似各种论坛底部的PageBar，点击页号或者输入页号就可以切换页面）。   

## 二、用法

### 1. 引入文件

将src文件夹中的```qpagebar.h``` ```qpagebar.cpp```直接导入项目中即可使用。（```Chinese```文件夹中是中文注释，```English```文件夹中是英文注释）

代码中的注释是

### 2. 声明QPageBar类

QPageBar有两种声明方式：

第一种只有一个parent参数：
```cpp
QPageBar(QWidget* parent = nullptr);
```
+ 这个构造函数只有一个parent参数，所以可以直接在.ui文件里声明QPageBar，Qt的ui编译会自动调用这个构造函数。
+ 此函数会声明一个页面数量为空的PageBar，需要调用```setCount```方法来设置页面数量。

第二种有三个参数：
```cpp
QPageBar(int pageTotalCount, int pageShowCount = 5, QWidget* parent = nullptr);
```
+ 此函数会声明一个页面总数是pageTotalCount、显示的页码按钮数量是pageShowCount的PageBar

### 3. 切换页面的实现

切换页面有4种方式：

+ 点击上一页按钮（<）
+ 点击下一页按钮（>）
+ 点击页码按钮
+ 输入页面后点击确定

这四种方式都会激活```pageChanged```信号，信号的参数```int page```是要跳转的页码。您只需要将```pageChanged```信号和跳转的逻辑进行```connect```即可。

### 4. 其他public方法在代码中有详细注释

## 三、测试程序

demo文件夹中有测试程序，使用qmake构建，直接用Qt Creator打开QPageBarDemo.pro即可编译运行。