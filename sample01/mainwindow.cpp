#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 状态栏
    // 文本框
    labNodeText = new QLabel("Node Title", this);
    labNodeText->setMinimumWidth(200);
    ui->statusbar->addWidget(labNodeText);
    // 缩放比率
    spinRatio = new QSpinBox(this);
    spinRatio->setRange(0,2000);
    spinRatio->setValue(100);
    spinRatio->setSuffix(" %");
    spinRatio->setReadOnly(true);
    spinRatio->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->statusbar->addWidget(spinRatio);
    // 总路径
    labFileName = new QLabel("File Name", this);
    ui->statusbar->addPermanentWidget(labFileName);

    // 初始化
    buildTreeHeader();
    initTree();
    setCentralWidget(ui->scrollArea);

}

void MainWindow::buildTreeHeader()
{
    ui->treeWidget->clear();
    QTreeWidgetItem* header = new QTreeWidgetItem();
    header->setText(MainWindow::colItem, "Directories and Files");
    header->setText(MainWindow::colItemType, "Node Type");
    header->setText(MainWindow::colDate, "Last modified date");
    header->setTextAlignment(colItem, Qt::AlignHCenter|Qt::AlignVCenter);
    header->setTextAlignment(colItemType, Qt::AlignHCenter|Qt::AlignVCenter);
    ui->treeWidget->setHeaderItem(header);
}

void MainWindow::initTree()
{
    // 初始化树，并创建一个顶层节点
    QIcon icon("qrc:/good/logo.jpeg");
    QTreeWidgetItem* item = new QTreeWidgetItem(MainWindow::itTopItem);
    item->setIcon(MainWindow::colItem, icon);
    item->setText(MainWindow::colItem, "Photo");
    item->setText(MainWindow::colItemType, "Top Item");
    // 设置为选中状态等
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    item->setCheckState(MainWindow::colItem, Qt::Checked);
    // 添加顶层结点到树中
    ui->treeWidget->addTopLevelItem(item);
}

void MainWindow::on_actionaddFolder_triggered()
{
    QString dir = QFileDialog::getExistingDirectory();
    if(dir.isEmpty())
        return;
    QTreeWidgetItem* parItem = ui->treeWidget->currentItem();
    if(parItem == nullptr)
        return;
    if(parItem->type() != itImageItem)
        addFolderItem(parItem, dir);
}

void MainWindow::addFolderItem(QTreeWidgetItem* parItem, QString dirName)
{
    QIcon icon("qrc:/good/logo.jpeg");
    QString NodeText = getFinalFolderName(dirName);
    QTreeWidgetItem* item = new QTreeWidgetItem(MainWindow::itGroupItem);
    item->setIcon(colItem, icon);
    item->setText(colItem, NodeText);
    item->setText(colItemType, "Group Item");
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    item->setCheckState(colItem, Qt::Checked);
    item->setData(colItem, Qt::UserRole, QVariant(dirName));
    parItem->addChild(item);
}

QString MainWindow::getFinalFolderName(const QString& FullPathName)
{
    // 从完整的目录名称中获取最后的文件名称
    int cnt = FullPathName.length();
    int i = FullPathName.lastIndexOf("/");
    QString str = FullPathName.right(cnt-i-1);
    return str;
}

void MainWindow::on_actionaddFiles_triggered()
{
    // 添加图片结点
    QStringList files = QFileDialog::getOpenFileNames(this, "Choose File", "", "Images(*.jpg)");
    if(files.empty())
        return;
    QTreeWidgetItem* parItem, *item;
    item = ui->treeWidget->currentItem();
    if(item == nullptr)
        item = ui->treeWidget->topLevelItem(0);  // 取顶层结点
    // 否则如果当前结点是图片节点，选取其父节点作为添加位置
    if(item->type() == itImageItem)
        parItem = item->parent();
    else
        parItem = item;
    for(int i = 0; i < files.size(); ++i)
    {
        QString aFilename = files.at(i);
        addImageItem(parItem, aFilename);
    }
    parItem->setExpanded(true); // 展开父结点
}

void MainWindow::addImageItem(QTreeWidgetItem* parItem, QString aFileName)
{
    QIcon icon("qrc:/good/logo.jpeg");
    QFileInfo fileInfo(aFileName);
    QString NodeText = fileInfo.fileName();
    QDateTime birthDate = fileInfo.lastModified();

    QTreeWidgetItem* item = new QTreeWidgetItem(MainWindow::itImageItem);
    item->setIcon(colItem, icon);
    item->setText(colItem, NodeText);
    item->setText(colItemType, "Image Item");
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    item->setCheckState(colItem, Qt::Checked);
    item->setData(colItem, Qt::UserRole, QVariant(aFileName));
    parItem->addChild(item);
}

void MainWindow::on_actionDeleteItem_triggered()
{
    // 删除节点
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item==nullptr)
        return;
    QTreeWidgetItem* parItem = item->parent();
    deleteItem(parItem, item);
}

void MainWindow::deleteItem(QTreeWidgetItem* parItem, QTreeWidgetItem* item)
{
    // 彻底删除一个节点以及其子节点，需要递归调用
    if(item->childCount() > 0)
    {
        int count = item->childCount();
        QTreeWidgetItem* tempParItem = item;
        for(int i = count - 1; i >= 0; --i)
            deleteItem(tempParItem, tempParItem->child(i));
    }
    // 递归删除完子节点之后删除自身，此步通过自身的父节点来实现
    parItem->removeChild(item);
    delete item;
}

void MainWindow::on_actionScanItems_triggered()
{
    // 节点的遍历
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
        changeItemCaption(item);   // 更改结点标题
    }
}

void MainWindow::changeItemCaption(QTreeWidgetItem* item)
{
    QString str = "*" + item->text(colItem);
    item->setText(colItem, str);
    if(item->childCount() > 0)
    {
        for(int i = 0; i < item->childCount(); ++i)
            changeItemCaption(item->child(i));
    }
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if(current == nullptr)
        return;
    if(current == previous)
        return;
    int var = current->type();
    switch(var)
    {
    case itTopItem:
        ui->actionaddFolder->setEnabled(true);
        ui->actionaddFiles->setEnabled(true);
        ui->actionDeleteItem->setEnabled(false);
        break;
    case itGroupItem:
        ui->actionaddFolder->setEnabled(true);
        ui->actionaddFiles->setEnabled(true);
        ui->actionDeleteItem->setEnabled(true);
        break;
    case itImageItem:
        ui->actionaddFolder->setEnabled(false);
        ui->actionaddFiles->setEnabled(true);
        ui->actionDeleteItem->setEnabled(true);
        displayImage(current);
    }
}

void MainWindow::displayImage(QTreeWidgetItem* item)
{
    QString filename = item->data(colItem, Qt::UserRole).toString();
    labFileName->setText(filename);
    labNodeText->setText(item->text(colItem));
    m_pixmap.load(filename);
    ui->actionZoomFitH->trigger();
    ui->actionZoomFitH->setEnabled(true);
    ui->actionZoomFitW->setEnabled(true);
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionZoomRealSize->setEnabled(true);
    ui->label->setPixmap(m_pixmap);  // 对label容器设置图片
}

void MainWindow::on_actionZoomRealSize_triggered()
{
    m_ratio = 1;
    spinRatio->setValue(100);
    ui->label->setPixmap(m_pixmap);
}

void MainWindow::on_actionZoomFitW_triggered()
{
    int W = ui->scrollArea->width();
    int realW = m_pixmap.width();
    m_ratio = float(W)/realW;
    spinRatio->setValue(100 * m_ratio);
    QPixmap pix = m_pixmap.scaledToWidth(W - 30);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actionZoomFitH_triggered()
{
    int H = ui->scrollArea->height();
    int realH = m_pixmap.height();
    m_ratio = float(H)/realH;
    spinRatio->setValue(100 * m_ratio);
    QPixmap pix = m_pixmap.scaledToHeight(H - 30);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actionZoomIn_triggered()
{
    m_ratio = 1.2 * m_ratio;
    spinRatio->setValue(100 * m_ratio);
    int w = m_ratio * m_pixmap.width();
    int h = m_ratio * m_pixmap.height();
    QPixmap pix = m_pixmap.scaled(w,h);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actionZoomOut_triggered()
{
    m_ratio = 0.8 * m_ratio;
    spinRatio->setValue(100 * m_ratio);
    int w = m_ratio * m_pixmap.width();
    int h = m_ratio * m_pixmap.height();
    QPixmap pix = m_pixmap.scaled(w,h);
    ui->label->setPixmap(pix);
}

void MainWindow::on_actionDockVisible_triggered()
{
    bool tmp = ui->dockWidget->isVisible();
    ui->dockWidget->setVisible(!tmp);
}

void MainWindow::on_actionDockFloat_triggered()
{
    bool tmp = ui->dockWidget->isFloating();
    ui->dockWidget->setFloating(!tmp);
}

void MainWindow::on_actionSaveAll_triggered()
{
    // 保存所有更改的本质实际上就是去除名字前面的 * 号
    // 节点的遍历
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
        TrimStar(item);   // 更改结点标题
    }
}

void MainWindow::TrimStar(QTreeWidgetItem* item)
{
    QString str = item->text(colItem);
    while(str.left(1).operator == ("*"))
    {
        str = str.right(str.length() - 1);
    }
    item->setText(colItem, str);
    if(item->childCount() > 0)
    {
        for(int i = 0; i < item->childCount(); ++i)
            TrimStar(item->child(i));
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint& pos)
{
    Q_UNUSED(pos);
    QMenu* menuList = new QMenu(this);
    menuList->addAction(ui->actionaddFolder);
    menuList->addAction(ui->actionaddFiles);
    menuList->addAction(ui->actionSaveAll);
    menuList->addSeparator();
    menuList->addAction(ui->actionZoomIn);
    menuList->addAction(ui->actionZoomOut);
    menuList->exec(QCursor::pos());   // 在鼠标光标位置显示快捷菜单
    delete menuList;  //菜单显示完后，需要删除
}

MainWindow::~MainWindow()
{
    delete ui;
}

