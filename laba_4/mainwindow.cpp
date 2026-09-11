#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpressionValidator>

/*MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}*/

CarData::CarData()
    : m_year(0)
    , m_heatedSeats(false)
    , m_parkingSensors(false)
    , m_mats(false)
{
}

CarData::CarData(const QString& brand, const QString& model, int year,
                 const QString& licensePlate, const QString& engineType,
                 bool heatedSeats, bool parkingSensors, bool mats)
    : m_brand(brand)
    , m_model(model)
    , m_year(year)
    , m_licensePlate(licensePlate)
    , m_engineType(engineType)
    , m_heatedSeats(heatedSeats)
    , m_parkingSensors(parkingSensors)
    , m_mats(mats)
{
}

QString CarData::getBrand() const { return m_brand; }
QString CarData::getModel() const { return m_model; }
int CarData::getYear() const { return m_year; }
QString CarData::getLicensePlate() const { return m_licensePlate; }
QString CarData::getEngineType() const { return m_engineType; }
bool CarData::hasHeatedSeats() const { return m_heatedSeats; }
bool CarData::hasParkingSensors() const { return m_parkingSensors; }
bool CarData::hasMats() const { return m_mats; }

void CarData::setBrand(const QString& brand) { m_brand = brand; }
void CarData::setModel(const QString& model) { m_model = model; }
void CarData::setYear(int year) { m_year = year; }
void CarData::setLicensePlate(const QString& licensePlate) { m_licensePlate = licensePlate; }
void CarData::setEngineType(const QString& engineType) { m_engineType = engineType; }
void CarData::setHeatedSeats(bool heatedSeats) { m_heatedSeats = heatedSeats; }
void CarData::setParkingSensors(bool parkingSensors) { m_parkingSensors = parkingSensors; }
void CarData::setMats(bool mats) { m_mats = mats; }

QString CarData::toString() const
{
    QString result;
    QTextStream out(&result);

    out << "========================================\n";
    out << "Марка: " << m_brand << "\n";
    out << "Модель: " << m_model << "\n";
    out << "Год выпуска: " << m_year << "\n";
    out << "Гос. номер: " << m_licensePlate << "\n";
    out << "Двигатель: " << m_engineType << "\n";
    out << "Опции:\n";

    if (m_heatedSeats) out << "  - Подогрев сидений\n";
    if (m_parkingSensors) out << "  - Парктроники\n";
    if (m_mats) out << "  - Коврики\n";
    if (!m_heatedSeats && !m_parkingSensors && !m_mats) {
        out << "  - Не выбрано\n";
    }
    out << "========================================\n\n";

    return result;
}


CarValidator::ValidationResult CarValidator::validateBrand(const QString& brand)
{
    if (brand.isEmpty()) {
        return ValidationResult(false, "Поле 'Марка' не заполнено", "Марка");
    }

    if (!isUppercaseOnly(brand)) {
        return ValidationResult(false,
                                "Марка должна содержать только заглавные буквы (A-Z, А-Я)",
                                "Марка");
    }

    return ValidationResult();
}

CarValidator::ValidationResult CarValidator::validateModel(const QString& model)
{
    if (model.isEmpty()) {
        return ValidationResult(false, "Поле 'Модель' не заполнено", "Модель");
    }

    if (!isUppercaseOnly(model)) {
        return ValidationResult(false,
                                "Модель должна содержать только заглавные буквы (A-Z, А-Я,Ё, 0-9)",
                                "Модель");
    }

    return ValidationResult();
}

CarValidator::ValidationResult CarValidator::validateYear(const QString& yearStr)
{
    if (yearStr.isEmpty()) {
        return ValidationResult(false, "Поле 'Год выпуска' не заполнено", "Год выпуска");
    }

    QRegularExpression yearRegex("^\\d{4}$");
    if (!yearRegex.match(yearStr).hasMatch()) {
        return ValidationResult(false,
                                "Год выпуска должен содержать ровно 4 цифры",
                                "Год выпуска");
    }

    int year = yearStr.toInt();
    if (year < 1900 || year > 2026) {
        return ValidationResult(false,
                                "Год выпуска должен быть между 1900 и 2026",
                                "Год выпуска");
    }

    return ValidationResult();
}

CarValidator::ValidationResult CarValidator::validateLicensePlate(const QString& license)
{
    if (license.isEmpty()) {
        return ValidationResult(false, "Поле 'Гос. номер' не заполнено", "Гос. номер");
    }

    if (!isValidLicenseFormat(license)) {
        return ValidationResult(false,
                                "Гос. номер должен соответствовать формату:\n"
                                "A111AA22 или A111AA222\n"
                                "Буквы из набора: A, B, C, E, H, K, M, O, P, T, X, Y (или русские аналоги)\n"
                                "Для трехзначного региона допустимы только 101 или 111",
                                "Гос. номер");
    }

    return ValidationResult();
}

CarValidator::ValidationResult CarValidator::validateEngineType(bool petrolSelected, bool dieselSelected)
{
    if (!petrolSelected && !dieselSelected) {
        return ValidationResult(false,
                                "Необходимо выбрать тип двигателя (Бензин или Дизель)",
                                "Двигатель");
    }

    return ValidationResult();
}

CarValidator::ValidationResult CarValidator::validateAll(const QString& brand, const QString& model,
                                                         const QString& yearStr, const QString& license,
                                                         bool petrolSelected, bool dieselSelected)
{
    ValidationResult result = validateBrand(brand);
    if (!result.isValid) return result;

    result = validateModel(model);
    if (!result.isValid) return result;

    result = validateYear(yearStr);
    if (!result.isValid) return result;

    result = validateLicensePlate(license);
    if (!result.isValid) return result;

    result = validateEngineType(petrolSelected, dieselSelected);
    if (!result.isValid) return result;

    return ValidationResult();
}

bool CarValidator::isUppercaseOnly(const QString& text)
{
    QRegularExpression uppercaseRegex("^[A-ZА-ЯЁ]+$");
    return uppercaseRegex.match(text).hasMatch();
}

bool CarValidator::isValidModel(const QString& model)
{
    QRegularExpression modelRegex("^[A-ZА-ЯЁ0-9\\-\\s]+$");
    return modelRegex.match(model).hasMatch();
}

bool CarValidator::isValidLicenseFormat(const QString& license)
{
    QString upperLicense = license.toUpper();

    // Разрешенные буквы для госномеров
    QString letters = "ABCEHKMOPTXYАВСЕНКМОРТХУ";
    QString pattern = QString("^[%1]\\d{3}[%1]{2}\\d{2,3}$").arg(letters);

    QRegularExpression licenseRegex(pattern);
    if (!licenseRegex.match(upperLicense).hasMatch()) {
        return false;
    }

    // Проверяем регион
    QString region = upperLicense.mid(6);
    if (region.length() == 3) {
        return isValidRegion(region);
    }

    return true;
}

bool CarValidator::isValidRegion(const QString& region)
{
    return (region == "101" || region == "111");
}


bool CarStorage::saveToFile(const CarData& car, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << car.toString();
    file.close();

    return true;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QFormLayout* formLayout = new QFormLayout();


    m_brandEdit = new QLineEdit();
    m_modelEdit = new QLineEdit();
    m_yearEdit = new QLineEdit();
    QRegularExpressionValidator* yearValidator = new QRegularExpressionValidator(
        QRegularExpression("\\d{4}"), this);
    m_yearEdit->setValidator(yearValidator);
    m_licenseEdit = new QLineEdit();


    m_petrolRadio = new QRadioButton("Бензин");
    m_dieselRadio = new QRadioButton("Дизель");
    m_engineGroup = new QButtonGroup(this);
    m_engineGroup->setExclusive(true);
    m_engineGroup->addButton(m_petrolRadio);
    m_engineGroup->addButton(m_dieselRadio);

    QHBoxLayout* engineLayout = new QHBoxLayout();
    engineLayout->addWidget(m_petrolRadio);
    engineLayout->addWidget(m_dieselRadio);
    engineLayout->addStretch();


    m_heatedSeatsCheck = new QCheckBox("Подогрев сидений");
    m_parkingSensorsCheck = new QCheckBox("Парктроники");
    m_matsCheck = new QCheckBox("Коврики");

    QVBoxLayout* optionsLayout = new QVBoxLayout;
    optionsLayout->addWidget(m_heatedSeatsCheck);
    optionsLayout->addWidget(m_parkingSensorsCheck);
    optionsLayout->addWidget(m_matsCheck);

    QPushButton* resetButton = new QPushButton("Сброс");
    /*resetButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"      // Красный фон
        "   color: black;"                    // Белый текст
        "   border: none;"                    // Без рамки
        "   border-radius: 0px;"              // Скругленные углы
        "   font-size: 14px;"                 // Размер шрифта
        "   font-weight: bold;"               // Жирный шрифт
        "}"
        "QPushButton:hover {"
        "   background-color: #d32f2f;"       // Темнее при наведении
        "}"
        "QPushButton:pressed {"
        "   background-color: #b71c1c;"       // Еще темнее при нажатии
        "}"
        );*/
    QPushButton* saveButton = new QPushButton("Сохранить");
    /*saveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4caf50;"      // Зеленый фон
        "   color: black;"                    // Белый текст
        "   border: none;"                    // Без рамки
        "   border-radius: 0px;"              // Скругленные углы
        "   font-size: 14px;"                 // Размер шрифта
        "   font-weight: bold;"               // Жирный шрифт
        "}"
        "QPushButton:hover {"
        "   background-color: #388e3c;"       // Темнее при наведении
        "}"
        "QPushButton:pressed {"
        "   background-color: #2e7d32;"       // Еще темнее при нажатии
        "}"
        );*/

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(resetButton);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();

    // Собираем форму
    formLayout->addRow("Марка:", m_brandEdit);
    formLayout->addRow("Модель:", m_modelEdit);
    formLayout->addRow("Год выпуска:", m_yearEdit);
    formLayout->addRow("Гос. номер:", m_licenseEdit);
    formLayout->addRow("Двигатель:", engineLayout);
    formLayout->addRow("Опции:", optionsLayout);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    // Подключаем сигналы
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSave);

    setWindowTitle("Авто");
    resize(500, 400);
}

void MainWindow::onReset()
{
    m_brandEdit->clear();
    m_modelEdit->clear();
    m_yearEdit->clear();
    m_licenseEdit->clear();

    bool wasExclusive = m_engineGroup->exclusive();
    m_engineGroup->setExclusive(false);
    m_petrolRadio->setChecked(false);
    m_dieselRadio->setChecked(false);
    m_engineGroup->setExclusive(wasExclusive);

    m_heatedSeatsCheck->setChecked(false);
    m_parkingSensorsCheck->setChecked(false);
    m_matsCheck->setChecked(false);

    m_brandEdit->setStyleSheet("");
    m_modelEdit->setStyleSheet("");
    m_yearEdit->setStyleSheet("");
    m_licenseEdit->setStyleSheet("");

    m_brandEdit->setFocus();
}

void MainWindow::onSave()
{
    // Получаем данные
    QString brand = m_brandEdit->text().trimmed();
    QString model = m_modelEdit->text().trimmed();
    QString yearStr = m_yearEdit->text().trimmed();
    QString license = m_licenseEdit->text().trimmed().toUpper();
    bool petrolSelected = m_petrolRadio->isChecked();
    bool dieselSelected = m_dieselRadio->isChecked();

    // Валидация
    CarValidator::ValidationResult result = CarValidator::validateAll(
        brand, model, yearStr, license, petrolSelected, dieselSelected);

    if (!result.isValid) {
        QMessageBox::warning(this, "Ошибка валидации", result.errorMessage);

        // Устанавливаем фокус на поле с ошибкой
        if (result.fieldName == "Марка") m_brandEdit->setFocus();
        else if (result.fieldName == "Модель") m_modelEdit->setFocus();
        else if (result.fieldName == "Год выпуска") m_yearEdit->setFocus();
        else if (result.fieldName == "Гос. номер") m_licenseEdit->setFocus();

        return;
    }

    // Создаем объект CarData
    CarData car(
        brand,
        model,
        yearStr.toInt(),
        license,
        petrolSelected ? "Бензин" : "Дизель",
        m_heatedSeatsCheck->isChecked(),
        m_parkingSensorsCheck->isChecked(),
        m_matsCheck->isChecked()
        );

    // Сохраняем в файл
    if (CarStorage::saveToFile(car)) {
        QMessageBox::information(this, "",  "Данные успешно сохранены в result.txt");
        onReset(); // Очищаем форму
    } else {
        QMessageBox::warning(this, "", "Не удалось сохранить данные в файл");
    }
}

