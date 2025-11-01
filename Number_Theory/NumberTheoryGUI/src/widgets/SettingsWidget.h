#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QFormLayout>
#include <QFontComboBox>
#include <QColorDialog>
#include <QDialogButtonBox>

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget* parent = nullptr);
    ~SettingsWidget() = default;

    void loadSettings();
    void saveSettings();

signals:
    void settingsChanged();
    void themeChanged(const QString& themeName);
    void fontScaleChanged(qreal scale);
    void highContrastToggled(bool enabled);

private slots:
    void onThemeChanged(int index);
    void onFontScaleChanged(int value);
    void onHighContrastToggled(bool checked);
    void onFontFamilyChanged(const QFont& font);
    void onResetToDefaults();
    void onApplySettings();

private:
    void setupUI();
    void setupAppearanceTab();
    void setupAccessibilityTab();
    void setupPerformanceTab();
    void setupAboutTab();

    void applyTheme(const QString& themeName);
    void updatePreview();

    QTabWidget* m_tabWidget;

    // Appearance tab
    QComboBox* m_themeCombo;
    QFontComboBox* m_fontFamilyCombo;
    QSpinBox* m_fontSizeSpin;
    QCheckBox* m_darkModeCheck;
    QLabel* m_previewLabel;

    // Accessibility tab
    QSlider* m_fontScaleSlider;
    QLabel* m_fontScaleLabel;
    QCheckBox* m_highContrastCheck;
    QCheckBox* m_keyboardNavigationCheck;
    QCheckBox* m_screenReaderCheck;
    QComboBox* m_languageCombo;

    // Performance tab
    QSpinBox* m_maxPrimeLimitSpin;
    QSpinBox* m_timeoutSpin;
    QCheckBox* m_enableBenchmarkingCheck;
    QComboBox* m_threadPriorityCombo;

    // About tab
    QLabel* m_versionLabel;
    QLabel* m_buildDateLabel;
    QLabel* m_qtVersionLabel;

    // Buttons
    QDialogButtonBox* m_buttonBox;
    QPushButton* m_applyButton;
    QPushButton* m_resetButton;

    // Settings storage
    QMap<QString, QVariant> m_settings;
};

#endif // SETTINGSWIDGET_H
