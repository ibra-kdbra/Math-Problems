#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QSplitter>
#include <QGroupBox>
#include <QScrollArea>

class HelpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QWidget* parent = nullptr);
    ~HelpWidget() = default;

    void showAlgorithmHelp(const QString& algorithmName);
    void showGeneralHelp();

signals:
    void linkClicked(const QUrl& url);

private slots:
    void onSearchTextChanged(const QString& text);
    void onTreeItemClicked(QTreeWidgetItem* item, int column);
    void onHomeClicked();
    void onBackClicked();
    void onForwardClicked();

private:
    void setupUI();
    void setupHelpContent();
    void populateHelpTree();
    void displayContent(const QString& content);
    QString getAlgorithmHelpContent(const QString& algorithm);
    QString getGeneralHelpContent();

    QSplitter* m_mainSplitter;
    QTreeWidget* m_helpTree;
    QTextBrowser* m_contentBrowser;

    QWidget* m_navigationWidget;
    QLineEdit* m_searchEdit;
    QPushButton* m_homeButton;
    QPushButton* m_backButton;
    QPushButton* m_forwardButton;

    QString m_currentContent;
    QStringList m_navigationHistory;
    int m_historyIndex;

    // Help content sections
    QString m_gettingStartedContent;
    QString m_algorithmReferenceContent;
    QString m_visualizationGuideContent;
    QString m_troubleshootingContent;
    QString m_aboutContent;
};

#endif // HELPWIDGET_H
