#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>

class ServerGUI : public QMainWindow {
    Q_OBJECT

public:
    ServerGUI(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Server Management Console");
        resize(900, 600);

        // Центральный виджет
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Основной макет
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Меню
        createMenus();

        // Разделитель: список пользователей и сообщения
        QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

        // Список пользователей
        userListWidget = new QListWidget();
        userListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        userListWidget->addItem("User1");
        userListWidget->addItem("User2");
        userListWidget->addItem("User3");
        connect(userListWidget, &QListWidget::itemSelectionChanged, this, &ServerGUI::onUserSelected);

        // Кнопки управления пользователями
        QPushButton *kickButton = new QPushButton("Kick User");
        QPushButton *banButton = new QPushButton("Ban User");
        connect(kickButton, &QPushButton::clicked, this, &ServerGUI::onKickUser);
        connect(banButton, &QPushButton::clicked, this, &ServerGUI::onBanUser);

        QVBoxLayout *userControlLayout = new QVBoxLayout();
        userControlLayout->addWidget(kickButton);
        userControlLayout->addWidget(banButton);
        userControlLayout->addStretch();

        QWidget *userControlWidget = new QWidget();
        userControlWidget->setLayout(userControlLayout);

        // Лист сообщений
        messageTextEdit = new QTextEdit();
        messageTextEdit->setReadOnly(true);
        
        // Область для отображения сообщений и пользователей
        splitter->addWidget(userListWidget);
        
        QWidget *messageContainer = new QWidget();
        QVBoxLayout *messageLayout = new QVBoxLayout(messageContainer);
        
        QLabel *messagesLabel = new QLabel("Messages:");
        
        messageLayout->addWidget(messagesLabel);
        messageLayout->addWidget(messageTextEdit);
        
        splitter->addWidget(messageContainer);
        
        splitter->setStretchFactor(1, 3); // Сделать область сообщений больше

        mainLayout->addWidget(splitter);
    }

private slots:
    void onUserSelected() {
        QList<QListWidgetItem*> selectedItems = userListWidget->selectedItems();
        if (!selectedItems.isEmpty()) {
            QString username = selectedItems.first()->text();
            // Обновите список сообщений для выбранного пользователя
            messageTextEdit->append(QString("Selected user: %1").arg(username));
            // Здесь можно реализовать загрузку приватных сообщений или другой логики
            loadMessagesForUser(username);
        }
    }

    void onKickUser() {
        QList<QListWidgetItem*> selectedItems = userListWidget->selectedItems();
        if (!selectedItems.isEmpty()) {
            QString username = selectedItems.first()->text();
            // Отправьте команду отключения пользователя на сервер
            messageTextEdit->append(QString("Kicked user: %1").arg(username));
            // Реализуйте вызов API или сокет-сообщение для отключения пользователя
            kickUser(username);
            delete selectedItems.first(); // Удаляем из списка
        }
    }

    void onBanUser() {
       QList<QListWidgetItem*> selectedItems = userListWidget->selectedItems();
       if (!selectedItems.isEmpty()) {
           QString username = selectedItems.first()->text();
           messageTextEdit->append(QString("Banned user: %1").arg(username));
           banUser(username);
           delete selectedItems.first(); 
       }
    }

private:
    QListWidget *userListWidget;
    QTextEdit *messageTextEdit;

    void createMenus() {
       QMenuBar *menuBar = this->menuBar();

       QMenu *fileMenu = menuBar->addMenu("&File");
       QAction *exitAction = fileMenu->addAction("&Exit");
       connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

       QMenu *helpMenu = menuBar->addMenu("&Help");
       QAction *aboutAction = helpMenu->addAction("&About");
       connect(aboutAction, &QAction::triggered, this, []() {
           QMessageBox::about(nullptr, "About", "Server Management GUI\nBuilt with Qt");
       });
    }

    void loadMessagesForUser(const QString &username) {
       // Здесь реализуйте загрузку приватных сообщений или всей истории чата с этим пользователем.
       messageTextEdit->append(QString("Loading messages for %1...").arg(username));
    }

    void kickUser(const QString &username) {
       // Реализуйте отправку команды отключения пользователя на сервер.
    }

    void banUser(const QString &username) {
       // Реализуйте отправку команды бана.
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ServerGUI window;
    window.show();

    return app.exec();
}
