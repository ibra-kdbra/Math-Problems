#ifndef TUTORIALWIDGET_H
#define TUTORIALWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QGroupBox>
#include <QProgressBar>
#include <QTimer>

struct TutorialStep {
    QString title;
    QString content;
    QString example;
    QString explanation;
    QString quizQuestion;
    QStringList quizOptions;
    int correctAnswer;
};

struct Tutorial {
    QString name;
    QString description;
    QVector<TutorialStep> steps;
    QString category;
};

class TutorialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TutorialWidget(QWidget* parent = nullptr);
    ~TutorialWidget() = default;

    void loadTutorial(const QString& tutorialName);
    void resetTutorial();

signals:
    void tutorialCompleted(const QString& tutorialName);
    void quizAnswered(bool correct);

private slots:
    void nextStep();
    void previousStep();
    void checkQuizAnswer();
    void showHint();
    void updateProgress();

private:
    void setupUI();
    void loadTutorials();
    void displayCurrentStep();
    void setupQuizInterface();
    void showTutorialList();

    QStackedWidget* m_mainStack;
    QWidget* m_tutorialListWidget;
    QWidget* m_tutorialContentWidget;

    QListWidget* m_tutorialList;
    QTextBrowser* m_contentBrowser;
    QLabel* m_stepTitleLabel;
    QLabel* m_progressLabel;
    QProgressBar* m_progressBar;

    QPushButton* m_previousButton;
    QPushButton* m_nextButton;
    QPushButton* m_hintButton;
    QPushButton* m_resetButton;

    // Quiz components
    QGroupBox* m_quizGroup;
    QLabel* m_quizQuestionLabel;
    QList<QPushButton*> m_quizOptionButtons;
    QLabel* m_quizFeedbackLabel;

    QVector<Tutorial> m_tutorials;
    int m_currentTutorialIndex;
    int m_currentStepIndex;
    bool m_quizMode;

    void createSampleTutorials();
    Tutorial createGCDTutorial();
    Tutorial createPrimeTutorial();
    Tutorial createModularArithmeticTutorial();
};

#endif // TUTORIALWIDGET_H
