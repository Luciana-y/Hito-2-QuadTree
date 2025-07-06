// main.cpp
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QTextEdit>
#include "pointQuadTree.h"

class QuadTreeWidget : public QWidget {
    Q_OBJECT

public:
    QuadTree<QString> quadtree;
    QVector<QuadTree<QString>::Node*> visitedNodes;
    QuadTree<QString>::Node* foundNode = nullptr;

    QuadTreeWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(600, 600);
    }

    void insertPoint(int x, int y, const QString& data) {
        quadtree.insert(x, y, data);
        clearHighlights();
        update();
    }

    void deletePoint(int x, int y) {
        quadtree.remove(x, y);
        clearHighlights();
        update();
    }

    void searchPoint(int x, int y) {
        visitedNodes.clear();
        foundNode = recursiveSearchTrace(quadtree.getRoot(), x, y);
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        drawNode(painter, quadtree.getRoot(), 0, 0, width(), height());
    }

    void drawNode(QPainter &painter, QuadTree<QString>::Node *node, int minX, int minY, int maxX, int maxY) {
        if (!node) return;

        int x = node->point.x;
        int y = node->point.y;

        if (visitedNodes.contains(node)) {
            painter.setBrush(Qt::yellow);
        } else {
            painter.setBrush(Qt::blue);
        }

        if (foundNode == node) {
            painter.setBrush(Qt::green);
        }

        painter.drawEllipse(QPointF(x, y), 5, 5);
        painter.drawText(x + 6, y - 6, node->point.data);

        painter.setPen(Qt::DashLine);
        painter.drawLine(x, minY, x, maxY);
        painter.drawLine(minX, y, maxX, y);
        painter.setPen(Qt::SolidLine);

        drawNode(painter, node->nw, minX, y, x, maxY);
        drawNode(painter, node->ne, x, y, maxX, maxY);
        drawNode(painter, node->sw, minX, minY, x, y);
        drawNode(painter, node->se, x, minY, maxX, y);
    }

    QuadTree<QString>::Node* recursiveSearchTrace(QuadTree<QString>::Node* node, int x, int y) {
        if (!node) return nullptr;
        visitedNodes.append(node);
        if (node->point.x == x && node->point.y == y) return node;
        if (x < node->point.x && y >= node->point.y) return recursiveSearchTrace(node->nw, x, y);
        if (x >= node->point.x && y >= node->point.y) return recursiveSearchTrace(node->ne, x, y);
        if (x < node->point.x && y < node->point.y) return recursiveSearchTrace(node->sw, x, y);
        return recursiveSearchTrace(node->se, x, y);
    }

    void clearHighlights() {
        visitedNodes.clear();
        foundNode = nullptr;
    }
};

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    QLineEdit *xInput, *yInput, *dataInput;
    QTextEdit *logOutput;
    QPushButton *insertBtn, *deleteBtn, *searchBtn;
    QuadTreeWidget *quadtreeWidget;

    ControlPanel(QuadTreeWidget *qtw, QWidget *parent = nullptr) : QWidget(parent), quadtreeWidget(qtw) {
        auto *layout = new QVBoxLayout(this);

        xInput = new QLineEdit(this);
        yInput = new QLineEdit(this);
        dataInput = new QLineEdit(this);
        xInput->setPlaceholderText("X(0-800 visible)");
        yInput->setPlaceholderText("Y(0-600 visible)");
        dataInput->setPlaceholderText("Data");

        insertBtn = new QPushButton("Insert", this);
        deleteBtn = new QPushButton("Delete", this);
        searchBtn = new QPushButton("Search", this);

        logOutput = new QTextEdit(this);
        logOutput->setReadOnly(true);

        layout->addWidget(new QLabel("Insert/Search/Delete Point:"));
        layout->addWidget(xInput);
        layout->addWidget(yInput);
        layout->addWidget(dataInput);
        layout->addWidget(insertBtn);
        layout->addWidget(deleteBtn);
        layout->addWidget(searchBtn);
        layout->addWidget(new QLabel("Log:"));
        layout->addWidget(logOutput);

        connect(insertBtn, &QPushButton::clicked, this, &ControlPanel::onInsert);
        connect(deleteBtn, &QPushButton::clicked, this, &ControlPanel::onDelete);
        connect(searchBtn, &QPushButton::clicked, this, &ControlPanel::onSearch);
    }

private slots:
    void onInsert() {
        int x = xInput->text().toInt();
        int y = yInput->text().toInt();
        QString data = dataInput->text();
        quadtreeWidget->insertPoint(x, y, data);
        logOutput->append(QString("Inserted (%1,%2): %3").arg(x).arg(y).arg(data));
    }

    void onDelete() {
        int x = xInput->text().toInt();
        int y = yInput->text().toInt();
        quadtreeWidget->deletePoint(x, y);
        logOutput->append(QString("Deleted (%1,%2)").arg(x).arg(y));
    }

    void onSearch() {
        int x = xInput->text().toInt();
        int y = yInput->text().toInt();
        quadtreeWidget->searchPoint(x, y);
        logOutput->append(QString("Searching (%1,%2)...").arg(x).arg(y));
        for (auto node : quadtreeWidget->visitedNodes) {
            logOutput->append(QString("Visited (%1,%2)").arg(node->point.x).arg(node->point.y));
        }
        if (quadtreeWidget->foundNode)
            logOutput->append("✔ Node Found");
        else
            logOutput->append("❌ Not Found");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("QuadTree Visualizer - Split View");

    auto *splitter = new QSplitter;

    QuadTreeWidget *viewWidget = new QuadTreeWidget;
    ControlPanel *controls = new ControlPanel(viewWidget);

    splitter->addWidget(viewWidget);
    splitter->addWidget(controls);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    mainWindow.setCentralWidget(splitter);
    mainWindow.resize(1000, 600);
    mainWindow.show();

    return app.exec();
}

#include "main.moc"