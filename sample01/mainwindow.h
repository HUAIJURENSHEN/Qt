#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QPixmap>
#include <QTreeWidget>
#include <QString>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // 创建节点时用作type参数，自定义结点类型数据必须大于1000
    enum TreeItemType {itTopItem=1001, itGroupItem, itImageItem};
    // 目录树列的序号
    enum TreeColNum {colItem=0, colItemType, colDate};
    // 用于状态栏上显示文件名
    QLabel* labFileName;
    // 用于状态栏上显示结点标题
    QLabel* labNodeText;
    // 用于状态栏上显示图片缩放比例
    QSpinBox* spinRatio;
    // 当前的图片
    QPixmap m_pixmap;
    // 当前图片缩放比例
    float m_ratio;
public:
    MainWindow(QWidget *parent = nullptr);

    // 构建目录树表头
    void buildTreeHeader();
    // 初始化目录树
    void initTree();
    // 构建目录结点
    void addFolderItem (QTreeWidgetItem* parItem, QString dirName);
    // 提取目录名称
    QString getFinalFolderName (const QString& FullPathName);
    // 添加图片节点
    void addImageItem (QTreeWidgetItem* parItem, QString aFileName);
    // 显示一个图片节点关联的图片
    void displayImage (QTreeWidgetItem* item);
    // 遍历改变结点标题
    void changeItemCaption (QTreeWidgetItem* item);
    // 遍历修改状态
    void TrimStar(QTreeWidgetItem* item);
    // 删除一个结点
    void deleteItem (QTreeWidgetItem* parItem, QTreeWidgetItem* item);

    ~MainWindow();

private slots:
    // 添加目录
    void on_actionaddFolder_triggered();
    // 添加文件
    void on_actionaddFiles_triggered();
    // 删除结点
    void on_actionDeleteItem_triggered();
    // 结点遍历
    void on_actionScanItems_triggered();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    // 适合宽度
    void on_actionZoomFitW_triggered();
    // 实际大小
    void on_actionZoomRealSize_triggered();
    // 适合高度
    void on_actionZoomFitH_triggered();
    // 放大
    void on_actionZoomIn_triggered();
    // 缩小
    void on_actionZoomOut_triggered();
    // 窗口可见
    void on_actionDockVisible_triggered();
    // 窗口浮动
    void on_actionDockFloat_triggered();
    // 保存更改
    void on_actionSaveAll_triggered();

    void on_treeWidget_customContextMenuRequested(const QPoint& pos);
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
