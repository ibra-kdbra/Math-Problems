#ifndef ACCESSIBILITYMANAGER_H
#define ACCESSIBILITYMANAGER_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QStyle>
#include <QFont>
#include <QPalette>

class AccessibilityManager : public QObject
{
    Q_OBJECT

public:
    static AccessibilityManager* instance();

    // Font scaling
    void setFontScale(qreal scale);
    qreal fontScale() const;
    QFont scaledFont(const QFont& baseFont) const;

    // High contrast theme
    void setHighContrastEnabled(bool enabled);
    bool isHighContrastEnabled() const;
    void applyHighContrastTheme();

    // Keyboard navigation
    void setupKeyboardNavigation(QWidget* widget);
    void announceText(const QString& text);

    // Screen reader support
    void setAccessibleName(QWidget* widget, const QString& name);
    void setAccessibleDescription(QWidget* widget, const QString& description);

    // Focus management
    void setFocusOrder(const QList<QWidget*>& widgets);
    void highlightCurrentFocus(QWidget* widget);

signals:
    void fontScaleChanged(qreal scale);
    void highContrastChanged(bool enabled);
    void accessibilityAnnouncement(const QString& text);

private:
    AccessibilityManager(QObject* parent = nullptr);
    ~AccessibilityManager() = default;

    static AccessibilityManager* m_instance;

    qreal m_fontScale;
    bool m_highContrastEnabled;

    QPalette m_normalPalette;
    QPalette m_highContrastPalette;

    void createHighContrastPalette();
    void applyPaletteToApplication(const QPalette& palette);
    void scaleWidgetFonts(QWidget* widget, qreal scale);
};

#endif // ACCESSIBILITYMANAGER_H
