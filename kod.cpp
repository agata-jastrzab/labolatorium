
#include <iostream>
#include <string>
#include <regex>
#include <stdexcept>
#include <iomanip>
#include <sstream>

using namespace std;

class Osoba {
private:
    string imie;
    string nazwisko;
    string pesel;

    static bool czyPoprawnyPesel(const string& pesel) {
        if (!regex_match(pesel, regex("\\d{11}"))) return false;

        int wagi[10] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3};
        int suma = 0;
        for (int i = 0; i < 10; i++) {
            suma += (pesel[i] - '0') * wagi[i];
        }

        int kontrolna = (10 - (suma % 10)) % 10;
        return kontrolna == (pesel[10] - '0');
    }

    string getDataUrodzenia() const {
        int rok = stoi(pesel.substr(0, 2));
        int miesiac = stoi(pesel.substr(2, 2));
        int dzien = stoi(pesel.substr(4, 2));
        int stulecie = 1900;

        if (miesiac >= 1 && miesiac <= 12) stulecie = 1900;
        else if (miesiac >= 21 && miesiac <= 32) { miesiac -= 20; stulecie = 2000; }
        else if (miesiac >= 41 && miesiac <= 52) { miesiac -= 40; stulecie = 2100; }
        else if (miesiac >= 61 && miesiac <= 72) { miesiac -= 60; stulecie = 2200; }
        else if (miesiac >= 81 && miesiac <= 92) { miesiac -= 80; stulecie = 1800; }

        rok += stulecie;
        return to_string(dzien) + "/" + to_string(miesiac) + "/" + to_string(rok);
    }

public:
    Osoba(string imie, string nazwisko, string pesel)
        : imie(imie), nazwisko(nazwisko) {
        if (!czyPoprawnyPesel(pesel)) {
            throw invalid_argument("Niepoprawny PESEL!");
        }
        this->pesel = pesel;
    }

    string getImie() const { return imie; }
    string getNazwisko() const { return nazwisko; }
    string getPesel() const { return pesel; }

    void setImie(string i) { imie = i; }
    void setNazwisko(string n) { nazwisko = n; }
    void setPesel(string p) {
        if (!czyPoprawnyPesel(p)) {
            throw invalid_argument("Niepoprawny PESEL!");
        }
        pesel = p;
    }

    virtual string toString() const {
        return "Imię i nazwisko: " + imie + " " + nazwisko + "; data urodzenia: " + getDataUrodzenia();
    }

    virtual ~Osoba() = default;
};

class Pracownik : public Osoba {
private:
    string stanowisko;
    double pensja;

public:
    Pracownik(string imie, string nazwisko, string pesel, string stanowisko, double pensja)
        : Osoba(imie, nazwisko, pesel), stanowisko(stanowisko), pensja(pensja) {}

    string getStanowisko() const { return stanowisko; }
    double getPensja() const { return pensja; }

    void setStanowisko(string s) { stanowisko = s; }
    void setPensja(double p) { pensja = p; }

    double obliczRoczneWynagrodzenie() const {
        return pensja * 12;
    }

    string toString() const override {
        ostringstream ss;
        ss << Osoba::toString()
           << "; stanowisko: " << stanowisko
           << "; pensja: " << fixed << setprecision(2) << pensja;
        return ss.str();
    }
};

class Menadzer : public Pracownik {
private:
    int liczbaPodwladnych;
    double dodatekDoPensji;

public:
    Menadzer(string imie, string nazwisko, string pesel, string stanowisko, double pensja, int liczbaPodwladnych)
        : Pracownik(imie, nazwisko, pesel, stanowisko, pensja),
          liczbaPodwladnych(liczbaPodwladnych),
          dodatekDoPensji(liczbaPodwladnych * 100.0) {}

    int getLiczbaPodwladnych() const { return liczbaPodwladnych; }
    double getDodatekDoPensji() const { return dodatekDoPensji; }

    void setLiczbaPodwladnych(int l) {
        liczbaPodwladnych = l;
        dodatekDoPensji = l * 100.0;
    }

    void setDodatekDoPensji(double d) { dodatekDoPensji = d; }

    double obliczRoczneWynagrodzenie() const override {
        return (getPensja() + dodatekDoPensji) * 12;
    }

    string toString() const override {
        ostringstream ss;
        ss << Pracownik::toString()
           << "; liczba podwładnych: " << liczbaPodwladnych
           << "; dodatek do pensji: " << fixed << setprecision(2) << dodatekDoPensji;
        return ss.str();
    }
};

int main() {
    try {
        Osoba osoba("Arleta", "Nok", "99631963360");
        Pracownik pracownik("Zbigniew", "Sober", "03231705509", "Programista", 8500.0);
        Menadzer menadzer("Adam", "Weber", "45631795508", "Kierownik działu", 12000.0, 10);

        cout << osoba.toString() << endl;
        cout << pracownik.toString() << endl;
        cout << menadzer.toString() << endl;

        cout << "Roczne wynagrodzenie pracownika: " << pracownik.obliczRoczneWynagrodzenie() << endl;
        cout << "Roczne wynagrodzenie menadżera: " << menadzer.obliczRoczneWynagrodzenie() << endl;
    } catch (const exception& e) {
        cerr << "Błąd: " << e.what() << endl;
    }

    return 0;
}
