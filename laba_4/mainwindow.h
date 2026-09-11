#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QString>
#include <QRegularExpression>

/*QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE*/

class CarData
{
public:
    CarData();
    CarData(const QString& brand, const QString& model, int year,
            const QString& licensePlate, const QString& engineType,
            bool heatedSeats, bool parkingSensors, bool mats);

    // Геттеры
    QString getBrand() const;
    QString getModel() const;
    int getYear() const;
    QString getLicensePlate() const;
    QString getEngineType() const;
    bool hasHeatedSeats() const;
    bool hasParkingSensors() const;
    bool hasMats() const;

    // Сеттеры
    void setBrand(const QString& brand);
    void setModel(const QString& model);
    void setYear(int year);
    void setLicensePlate(const QString& licensePlate);
    void setEngineType(const QString& engineType);
    void setHeatedSeats(bool heatedSeats);
    void setParkingSensors(bool parkingSensors);
    void setMats(bool mats);

    // Форматированный вывод
    QString toString() const;

private:
    QString m_brand;
    QString m_model;
    int m_year;
    QString m_licensePlate;
    QString m_engineType;
    bool m_heatedSeats;
    bool m_parkingSensors;
    bool m_mats;
};

class CarValidator
{
public:
    struct ValidationResult {
        bool isValid;
        QString errorMessage;
        QString fieldName;

        ValidationResult() : isValid(true) {}
        ValidationResult(bool valid, const QString& error, const QString& field)
            : isValid(valid), errorMessage(error), fieldName(field) {}
    };

    static ValidationResult validateBrand(const QString& brand);
    static ValidationResult validateModel(const QString& model);
    static ValidationResult validateYear(const QString& yearStr);
    static ValidationResult validateLicensePlate(const QString& license);
    static ValidationResult validateEngineType(bool petrolSelected, bool dieselSelected);
    static ValidationResult validateAll(const QString& brand, const QString& model, const QString& yearStr, const QString& license, bool petrolSelected, bool dieselSelected);

private:
    static bool isUppercaseOnly(const QString& text);
    static bool isValidLicenseFormat(const QString& license);
    static bool isValidModel(const QString& model);
    static bool isValidRegion(const QString& region);
};

class CarStorage
{
public:
    static bool saveToFile(const CarData& car, const QString& filename = "result.txt");
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onReset();
    void onSave();

private:
    void setupUI();

    // UI Elements
    QLineEdit* m_brandEdit;
    QLineEdit* m_modelEdit;
    QLineEdit* m_yearEdit;
    QLineEdit* m_licenseEdit;
    QRadioButton* m_petrolRadio;
    QRadioButton* m_dieselRadio;
    QButtonGroup* m_engineGroup;
    QCheckBox* m_heatedSeatsCheck;
    QCheckBox* m_parkingSensorsCheck;
    QCheckBox* m_matsCheck;
};

#endif // MAINWINDOW_H
