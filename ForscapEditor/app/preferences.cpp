#include "preferences.h"
#include "ui_preferences.h"

#include "forscape_common.h"
#include "keywordsubstitutioneditor.h"
#include <typeset_themes.h>
#include <typeset_view.h>
#include <QColorDialog>
#include <QScrollBar>
#include <QSettings>
#include <QWindow>
#include "symbolsubstitutioneditor.h"
#include "typeset_integral_preference.h"

#define COLOUR_SETTING "COLOUR_PRESET"
#define INTEGRAL_LAYOUT_SETTING "VERT_INTS"
#define DISLAY_COMMA_SETTING "NUM_COMMAS"

Preferences::Preferences(QSettings& settings, QWidget* parent) :
    QWidget(parent), ui(new Ui::Preferences), settings(settings){
    ui->setupUi(this);

    if(settings.contains(INTEGRAL_LAYOUT_SETTING)){
        bool vertical_integrals = settings.value(INTEGRAL_LAYOUT_SETTING).toBool();
        ui->integralCheckBox->setChecked(vertical_integrals);
        Forscape::Typeset::integral_bounds_vertical = vertical_integrals;
    }

    if(settings.contains(DISLAY_COMMA_SETTING)){
        bool display_commas = settings.value(DISLAY_COMMA_SETTING).toBool();
        ui->numberCommaCheckBox->setChecked(display_commas);
        Forscape::Typeset::display_commas_in_numbers = display_commas;
    }

    for(int i = 0; i < Forscape::Typeset::NUM_COLOUR_PRESETS; i++){
        std::string_view name = Forscape::Typeset::getPresetName(i).data();
        ui->colour_dropdown->addItem(name.data());
    }

    if(settings.contains(COLOUR_SETTING)){
        QString colour_preset = settings.value(COLOUR_SETTING).toString();
        if(colour_preset == "Custom") addCustomDropdownIfNotPresent();
        else for(size_t i = 0; i < Forscape::Typeset::NUM_COLOUR_PRESETS; i++)
            if(colour_preset == Forscape::Typeset::getPresetName(i).data()){
                Forscape::Typeset::setPreset(i);
                ui->colour_dropdown->setCurrentIndex(static_cast<int>(i));
            }
    }

    for(size_t i = 0; i < Forscape::Typeset::NUM_COLOUR_ROLES; i++){
        std::string_view name = Forscape::Typeset::getColourName(i).data();
        if(!settings.contains(name.data())) continue;
        Forscape::Typeset::setColour(i, settings.value(name.data()).value<QColor>());
    }

    connect(ui->colour_dropdown, SIGNAL(activated(int)), this, SLOT(onPresetSelect(int)));

    connect(ui->colour_table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(onColourSelect(QTableWidgetItem*)) );

    ui->colour_table->setColumnCount(1);
    QTableWidgetItem* col_item = new QTableWidgetItem("Colour");
    ui->colour_table->setHorizontalHeaderItem(0, col_item);
    ui->colour_table->setRowCount(Forscape::Typeset::NUM_COLOUR_ROLES);

    for(int i = 0; i < Forscape::Typeset::NUM_COLOUR_ROLES; i++){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setBackground(Forscape::Typeset::getColour(i));
        item->setFlags(item->flags() & ~Qt::ItemFlag::ItemIsEditable);
        ui->colour_table->setItem(i, 0, item);

        QTableWidgetItem* row_item = new QTableWidgetItem();
        row_item->setText(Forscape::Typeset::getColourName(i).data());
        ui->colour_table->setVerticalHeaderItem(i, row_item);
    }

    keyword_editor = new KeywordSubstitutionEditor(settings, ui->scrollArea);
    ui->scrollArea->setWidget(keyword_editor);

    symbol_editor = new SymbolSubstitutionEditor(settings, this);
    ui->symbolLayout->insertWidget(1, symbol_editor);
}

Preferences::~Preferences(){
    settings.setValue(INTEGRAL_LAYOUT_SETTING, Forscape::Typeset::integral_bounds_vertical);
    settings.setValue(DISLAY_COMMA_SETTING, Forscape::Typeset::display_commas_in_numbers);

    settings.setValue(COLOUR_SETTING, ui->colour_dropdown->currentText());
    for(size_t i = 0; i < Forscape::Typeset::NUM_COLOUR_ROLES; i++){
        std::string_view name = Forscape::Typeset::getColourName(i).data();
        const QColor& colour = Forscape::Typeset::getColour(i);
        settings.setValue(name.data(), colour);
    }

    delete ui;
}

void Preferences::onPresetSelect(int index){
    if(index > Forscape::Typeset::NUM_COLOUR_PRESETS) return;

    Forscape::Typeset::setPreset(index);

    for(size_t i = 0; i < Forscape::Typeset::NUM_COLOUR_ROLES; i++){
        QTableWidgetItem* item = ui->colour_table->item(static_cast<int>(i), 0);
        item->setBackground(Forscape::Typeset::getColour(i));
    }

    removeCustomDropdownIfPresent();
    emit colourChanged();
    updateWindows();
}

void Preferences::onColourSelect(QTableWidgetItem* item){
    int role = item->row();
    const QColor& current = Forscape::Typeset::getColour(role);
    QColor colour = QColorDialog::getColor(current, this, Forscape::Typeset::getColourName(role).data(), QColorDialog::DontUseNativeDialog);
    if(!colour.isValid() || colour == current) return;
    item->setBackground(colour);
    Forscape::Typeset::setColour(role, colour);
    addCustomDropdownIfNotPresent();
    emit colourChanged();
    QCoreApplication::processEvents();
    updateWindows();
}

void Preferences::addCustomDropdownIfNotPresent(){
    if(ui->colour_dropdown->count() != Forscape::Typeset::NUM_COLOUR_PRESETS) return;
    ui->colour_dropdown->addItem("Custom");
    ui->colour_dropdown->setCurrentIndex(Forscape::Typeset::NUM_COLOUR_PRESETS);
}

void Preferences::removeCustomDropdownIfPresent(){
    if(ui->colour_dropdown->count() == Forscape::Typeset::NUM_COLOUR_PRESETS) return;
    ui->colour_dropdown->removeItem(ui->colour_dropdown->count()-1);
}

void Preferences::updateWindows() const {
    for(auto window : QGuiApplication::topLevelWindows())
        window->requestUpdate();
}

void Preferences::on_keywordDefaultsButton_clicked(){
    keyword_editor->resetDefaults();
}

void Preferences::on_keywordAddButton_clicked(){
    keyword_editor->addSlot();
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    auto scrollbar = ui->scrollArea->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}


void Preferences::on_symbolsDefaultsButton_clicked(){
    symbol_editor->resetDefaults();
}

void Preferences::on_symbolsAddButton_clicked(){
    symbol_editor->addRow();
}

void Preferences::on_integralCheckBox_toggled(bool checked){
    Forscape::Typeset::integral_bounds_vertical = checked;

    for(Forscape::Typeset::View* view : Forscape::Typeset::View::all_views)
        view->updateModel();
}


void Preferences::on_numberCommaCheckBox_toggled(bool checked){
    Forscape::Typeset::display_commas_in_numbers = checked;

    for(Forscape::Typeset::View* view : Forscape::Typeset::View::all_views)
        view->update();
}

