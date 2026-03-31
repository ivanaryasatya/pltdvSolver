#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;

// ANSI Color Codes
namespace Color {
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN = "\033[36m";
    const string BOLD = "\033[1m";
}

enum class Op { LEQ, GEQ, LT, GT, EQ };

string opToString(Op op) {
    switch (op) {
        case Op::LEQ: return "<=";
        case Op::GEQ: return ">=";
        case Op::LT:  return "<";
        case Op::GT:  return ">";
        case Op::EQ:  return "=";
    }
    return "";
}

string cleanFloat(double val) {
    if (val == 0) return "0";
    string str = to_string(val);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (str.back() == '.') str.pop_back();
    return str;
}

struct Inequality {
    double a, b, c; // ax + by [op] c
    Op op;

    string toString() const {
        string s = "";
        
        if (a != 0) {
            if (a == 1) s += "x";
            else if (a == -1) s += "-x";
            else s += cleanFloat(a) + "x";
        }
        if (b != 0) {
            if (b > 0 && !s.empty()) s += " + ";
            else if (b < 0 && !s.empty()) s += " - ";
            else if (b < 0 && s.empty()) s += "-";
            
            double absB = abs(b);
            if (absB == 1) s += "y";
            else s += cleanFloat(absB) + "y";
        }
        if (s.empty()) s = "0";

        return s + " " + opToString(op) + " " + cleanFloat(c);
    }

    bool satisfies(double x, double y) const {
        double val = a * x + b * y;
        const double EPS = 1e-7;
        switch (op) {
            case Op::LEQ: return val <= c + EPS;
            case Op::GEQ: return val >= c - EPS;
            case Op::LT:  return val < c - EPS;
            case Op::GT:  return val > c + EPS;
            case Op::EQ:  return abs(val - c) <= EPS;
        }
        return false;
    }
};

struct Point {
    double x, y;
    bool operator==(const Point& other) const {
        return abs(x - other.x) < 1e-7 && abs(y - other.y) < 1e-7;
    }
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void enableANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

class PltdvSolver {
private:
    vector<Inequality> system;
    double objP = 0, objQ = 0;
    bool hasObj = false;

    bool getIntersection(const Inequality& in1, const Inequality& in2, Point& p) {
        double D = in1.a * in2.b - in2.a * in1.b;
        if (abs(D) < 1e-7) return false; // Parallel lines
        p.x = (in1.c * in2.b - in2.c * in1.b) / D;
        p.y = (in1.a * in2.c - in2.a * in1.c) / D;
        // Fix -0
        if (abs(p.x) < 1e-7) p.x = 0;
        if (abs(p.y) < 1e-7) p.y = 0;
        return true;
    }

public:
    void addInequality(double a, double b, Op op, double c) {
        system.push_back(Inequality{a, b, c, op});
    }

    void setObjective(double p, double q) {
        objP = p;
        objQ = q;
        hasObj = true;
    }

    void clearSystem() {
        system.clear();
        hasObj = false;
        objP = objQ = 0;
    }

    void addNonNegativity() {
        addInequality(1, 0, Op::GEQ, 0); // x >= 0
        addInequality(0, 1, Op::GEQ, 0); // y >= 0
    }
    
    void removeInequality(int index) {
        if (index >= 1 && index <= system.size()) {
            system.erase(system.begin() + index - 1);
            cout << Color::GREEN << "Pertidaksamaan ke-" << index << " berhasil dihapus.\n" << Color::RESET;
        } else {
            cout << Color::RED << "Indeks tidak valid.\n" << Color::RESET;
        }
    }

    void displaySystem() const {
        cout << Color::CYAN << Color::BOLD << "\n--- Sistem Pertidaksamaan Saat Ini ---\n" << Color::RESET;
        if (system.empty()) {
            cout << Color::YELLOW << "Sistem masih kosong.\n" << Color::RESET;
        } else {
            for (size_t i = 0; i < system.size(); ++i) {
                cout << "[" << i + 1 << "] " << system[i].toString() << "\n";
            }
        }
        if (hasObj) {
            cout << "\n" << Color::MAGENTA << "Fungsi Objektif: Z = ";
            
            string z_str = "";
            if (objP != 0) {
                if (objP == 1) z_str += "x";
                else if (objP == -1) z_str += "-x";
                else z_str += cleanFloat(objP) + "x";
            }
            if (objQ != 0) {
                if (objQ > 0 && !z_str.empty()) z_str += " + ";
                else if (objQ < 0 && !z_str.empty()) z_str += " - ";
                else if (objQ < 0 && z_str.empty()) z_str += "-";
                
                double absQ = abs(objQ);
                if (absQ == 1) z_str += "y";
                else z_str += cleanFloat(absQ) + "y";
            }
            if (z_str.empty()) z_str = "0";

            cout << z_str << "\n" << Color::RESET;
        }
        cout << "--------------------------------------\n";
    }

    void solve() {
        if (system.empty()) {
            cout << Color::RED << "Error: Sistem pertidaksamaan kosong! Tambahkan minimal 1 pertidaksamaan.\n" << Color::RESET;
            return;
        }

        vector<Point> vertices;

        // Ensure we catch single point intersection for pairs
        for (size_t i = 0; i < system.size(); ++i) {
            for (size_t j = i + 1; j < system.size(); ++j) {
                Point p;
                if (getIntersection(system[i], system[j], p)) {
                    // Check if point satisfies all other inequalities
                    bool valid = true;
                    for (size_t k = 0; k < system.size(); ++k) {
                        if (k != i && k != j) {
                            if (!system[k].satisfies(p.x, p.y)) {
                                valid = false;
                                break;
                            }
                        }
                    }
                    if (valid) {
                        // Avoid adding duplicates
                        bool duplicate = false;
                        for (const auto& v : vertices) {
                            if (v == p) { duplicate = true; break; }
                        }
                        if (!duplicate) vertices.push_back(p);
                    }
                }
            }
        }

        cout << Color::GREEN << Color::BOLD << "\n--- Hasil Penyelesaian ---\n" << Color::RESET;
        if (vertices.empty()) {
            cout << Color::RED << "Tidak ditemukan titik sudut pada daerah penyelesaian.\n";
            cout << "Penyebab: Sistem mungkin tidak konsisten (tidak ada solusi), atau daerah bersolusi terbuka tanpa persimpangan.\n" << Color::RESET;
            return;
        }

        cout << Color::YELLOW << "Titik Sudut (Vertices) Daerah Penyelesaian:\n" << Color::RESET;
        double maxZ = -numeric_limits<double>::infinity();
        double minZ = numeric_limits<double>::infinity();
        vector<Point> maxPoints, minPoints;

        cout << left << setw(15) << "Titik (x, y)";
        if (hasObj) cout << " | Nilai Z";
        cout << "\n------------------------------------------------\n";

        for (const auto& v : vertices) {
            string pointStr = "(" + cleanFloat(v.x) + ", " + cleanFloat(v.y) + ")";
            cout << left << setw(15) << pointStr;
            
            if (hasObj) {
                double z = objP * v.x + objQ * v.y;
                if (abs(z) < 1e-7) z = 0; // Fix -0
                cout << " | " << cleanFloat(z);
                
                if (z > maxZ + 1e-7) { maxZ = z; maxPoints = {v}; }
                else if (abs(z - maxZ) < 1e-7) { maxPoints.push_back(v); }

                if (z < minZ - 1e-7) { minZ = z; minPoints = {v}; }
                else if (abs(z - minZ) < 1e-7) { minPoints.push_back(v); }
            }
            cout << "\n";
        }

        if (hasObj) {
            cout << Color::CYAN << Color::BOLD << "\nKesimpulan Optimasi:\n" << Color::RESET;
            cout << "Nilai Maksimum (" << Color::GREEN << cleanFloat(maxZ) << Color::RESET << ") berada di titik: ";
            for (const auto& p : maxPoints) cout << "(" << cleanFloat(p.x) << ", " << cleanFloat(p.y) << ") ";
            cout << "\n";
            
            cout << "Nilai Minimum (" << Color::RED << cleanFloat(minZ) << Color::RESET << ") berada di titik: ";
            for (const auto& p : minPoints) cout << "(" << cleanFloat(p.x) << ", " << cleanFloat(p.y) << ") ";
            cout << "\n";
        }
        cout << "------------------------------------------------\n";
    }
    
    // Check if the system is empty
    bool isEmpty() const {
        return system.empty();
    }
    size_t size() const {
        return system.size();
    }
};

void printHeader() {
    cout << Color::BLUE << Color::BOLD;
    cout << "====================================================\n";
    cout << "   [ PLTDV Solver - Optimasi Program Linear C++ ]   \n";
    cout << "                  by ivan aryasatya                 \n";
    cout << "====================================================\n" << Color::RESET;
}

void pauseMenu() {
    cout << "\n" << Color::YELLOW << "Tekan Enter untuk melanjutkan..." << Color::RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    enableANSI();
    PltdvSolver solver;
    int choice;

    while (true) {
        clearScreen();
        printHeader();
        solver.displaySystem();

        cout << Color::BOLD << "Menu Utama:\n" << Color::RESET;
        cout << "1. Tambah Pertidaksamaan       (Contoh: Masukkan '2x + 3y <= 12')\n";
        cout << "2. Tambah Syarat Non-Negatif   (Contoh: Otomatis tambah 'x >= 0' dan 'y >= 0')\n";
        cout << "3. Hapus Pertidaksamaan        (Contoh: Hapus persamaan salah di daftar nomor 2)\n";
        cout << "4. Set Fungsi Objektif         (Contoh: Masukkan target Z = '500x + 400y')\n";
        cout << "5. Selesaikan & Cari Ekstrem   (Contoh: Cari Nilai Maks/Min & Titik Potong)\n";
        cout << "6. Reset Sistem                (Contoh: Bersihkan sistem untuk soal baru)\n";
        cout << "0. Keluar                      (Contoh: Tutup aplikasi)\n";
        cout << "\nPilih opsi [0-6]: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            break;
        }
        else if (choice == 1) {
            double a, b, c;
            int opChoice;
            Op op;
            cout << "\n--- Tambah Pertidaksamaan ---\n";
            cout << "Format: ax + by [operator] c\n";
            cout << "Masukkan koefisien a (untuk x): "; 
            if(!(cin >> a)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            cout << "Masukkan koefisien b (untuk y): "; 
            if(!(cin >> b)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            
            cout << "Pilih Operator:\n";
            cout << "1. <=\n2. >=\n3. <\n4. >\n5. =\n";
            cout << "Pilihan [1-5]: ";
            if(!(cin >> opChoice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            
            cout << "Masukkan konstanta c: "; 
            if(!(cin >> c)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            
            switch (opChoice) {
                case 1: op = Op::LEQ; break;
                case 2: op = Op::GEQ; break;
                case 3: op = Op::LT; break;
                case 4: op = Op::GT; break;
                case 5: op = Op::EQ; break;
                default: 
                    cout << Color::RED << "Operator tidak valid.\n" << Color::RESET; 
                    pauseMenu();
                    continue;
            }
            solver.addInequality(a, b, op, c);
            cout << Color::GREEN << "Pertidaksamaan berhasil ditambahkan.\n" << Color::RESET;
            pauseMenu();
        }
        else if (choice == 2) {
            solver.addNonNegativity();
            cout << Color::GREEN << "Syarat non-negatif ditambahkan (x >= 0, y >= 0).\n" << Color::RESET;
            pauseMenu();
        }
        else if (choice == 3) {
            if (solver.isEmpty()) {
                cout << Color::RED << "Sistem kosong, tidak ada yang bisa dihapus.\n" << Color::RESET;
                pauseMenu();
                continue;
            }
            int idx;
            cout << "Masukkan nomor pertidaksamaan yang ingin dihapus (1-" << solver.size() << "): ";
            if (cin >> idx) {
                solver.removeInequality(idx);
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            pauseMenu();
        }
        else if (choice == 4) {
            double p, q;
            cout << "\n--- Set Fungsi Objektif ---\n";
            cout << "Format: Z = px + qy\n";
            cout << "Masukkan koefisien p (untuk x): "; 
            if(!(cin >> p)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            cout << "Masukkan koefisien q (untuk y): "; 
            if(!(cin >> q)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            solver.setObjective(p, q);
            cout << Color::GREEN << "Fungsi objektif berhasil di-set.\n" << Color::RESET;
            pauseMenu();
        }
        else if (choice == 5) {
            clearScreen();
            printHeader();
            solver.displaySystem();
            solver.solve();
            pauseMenu();
        }
        else if (choice == 6) {
            solver.clearSystem();
            cout << Color::GREEN << "Sistem berhasil di-reset sepenuhnya.\n" << Color::RESET;
            pauseMenu();
        }
    }

    clearScreen();
    cout << Color::GREEN << Color::BOLD << "Terima kasih telah menggunakan PLTDV Solver!\n" << Color::RESET;
    return 0;
}
