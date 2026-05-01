#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <windows.h>

using namespace std;

// Стани для класів з динамічною пам'яттю
enum STATE {
    OK = 0, BAD_INIT, BAD_ALLOC, OUT_OF_BOUNDS, BAD_DIV
};

// =====================================================================
// ЗАДАЧА 1.5: КЛАС ТРИКУТНИК
// =====================================================================
class Triangle {
    double a, b, c;
    int color;
public:
    // Конструктор за замовчуванням
    Triangle() : a(1.0), b(1.0), c(1.0), color(0) {}

    // Конструктор з параметрами
    Triangle(double a, double b, double c, int color) {
        if (isValid(a, b, c)) {
            this->a = a; this->b = b; this->c = c;
        }
        else {
            this->a = 1.0; this->b = 1.0; this->c = 1.0;
        }
        this->color = (color >= 0) ? color : 0;
    }

    // Перевірка коректності сторін
    bool isValid(double a, double b, double c) {
        return (a > 0 && b > 0 && c > 0 && (a + b > c) && (a + c > b) && (b + c > a));
    }

    // Гетери
    double getA() const { return a; }
    double getB() const { return b; }
    double getC() const { return c; }
    int getColor() const { return color; }

    // Сетери з перевіркою
    void setSides(double a, double b, double c) {
        if (isValid(a, b, c)) {
            this->a = a; this->b = b; this->c = c;
        }
        else {
            cout << " [Помилка] Некоректні сторони трикутника!\n";
        }
    }

    void setColor(int c_val) {
        if (c_val < 0 || c_val > 10000) {
            cout << " [Помилка] Некоректний колір!\n";
            return;
        }
        this->color = c_val;
    }

    double P() const { return a + b + c; }

    double S() const {
        double p = P() / 2.0;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

    void printInfo() const {
        cout << "a=" << a << " b=" << b << " c=" << c << " колір=" << color;
        cout << "  Периметр=" << P() << " Площа=" << S() << "\n";
    }
};

void Task_1_5_Menu() {
    int subChoice;
    Triangle t;
    double a, b, c;
    int col;

    do {
        cout << "\n>>> ЗАДАЧА 1.5: ТРИКУТНИК <<<\n";
        cout << "Поточний трикутник: "; t.printInfo();
        cout << "-----------------------------------\n";
        cout << "1. Ввести нові сторони\n";
        cout << "2. Ввести новий колір\n";
        cout << "3. Згенерувати випадковий трикутник\n";
        cout << "0. Повернутися до Головного меню\n";
        cout << "Ваш вибір: ";

        cin >> subChoice;
        if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); subChoice = -1; }

        switch (subChoice) {
        case 1:
            cout << "Введіть a, b, c: ";
            cin >> a >> b >> c;
            t.setSides(a, b, c);
            break;
        case 2:
            cout << "Введіть колір (ціле число >= 0): ";
            cin >> col;
            t.setColor(col);
            break;
        case 3: {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<double> dist(1.0, 10.0);
            uniform_int_distribution<int> cDist(0, 255);
            // Гарантовано створюємо правильний трикутник (рівносторонній для простоти)
            double side = dist(gen);
            t.setSides(side, side, side);
            t.setColor(cDist(gen));
            cout << "Згенеровано рівносторонній трикутник.\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (subChoice != 0);
}

// =====================================================================
// ЗАДАЧА 2.5: КЛАС ВЕКТОР
// =====================================================================
class Vector {
    long* v;
    int num;
    int state;
public:
    static int count;

    // Конструктор 1: без параметрів
    Vector() {
        num = 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; v[0] = 0; }
        count++;
    }

    // Конструктор 2: розмір вектора
    Vector(int n) {
        num = (n > 0) ? n : 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; for (int i = 0; i < num; i++) v[i] = 0; }
        count++;
    }

    // Конструктор 3: розмір + значення ініціалізації
    Vector(int n, long val) {
        num = (n > 0) ? n : 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; for (int i = 0; i < num; i++) v[i] = val; }
        count++;
    }

    // Конструктор копій
    Vector(const Vector& s) {
        num = s.num;
        state = s.state;
        v = new (nothrow) long[num];
        if (v && state == OK) {
            for (int i = 0; i < num; i++) v[i] = s.v[i];
        }
        else { state = BAD_ALLOC; num = 0; }
        count++;
    }

    // Операція присвоєння
    Vector& operator=(const Vector& s) {
        if (this != &s) {
            if (num != s.num) {
                if (v) delete[] v;
                num = s.num;
                v = new (nothrow) long[num];
                if (!v) { state = BAD_ALLOC; num = 0; return *this; }
            }
            state = s.state;
            for (int i = 0; i < num; i++) v[i] = s.v[i];
        }
        return *this;
    }

    // Деструктор
    ~Vector() {
        count--;
        if (v) delete[] v;
    }

    // Встановлення значення (з параметром за замовчуванням)
    void setElement(int index, long val = 0) {
        if (index >= 0 && index < num && state == OK) v[index] = val;
        else state = OUT_OF_BOUNDS;
    }

    // Отримання значення
    long getElement(int index) {
        if (index >= 0 && index < num && state == OK) return v[index];
        state = OUT_OF_BOUNDS;
        return 0;
    }

    int getState() const { return state; }
    int getNum() const { return num; }

    void Print() const {
        cout << "Вектор [" << num << "] (Стан: " << state << "): ";
        if (state == OK && v) {
            for (int i = 0; i < num; i++) cout << v[i] << " ";
        }
        cout << "\n";
    }

    // Арифметика
    Vector Add(const Vector& b) const {
        int tnum = num < b.num ? num : b.num;
        Vector tmp(tnum);
        if (tmp.state == OK) {
            for (int i = 0; i < tnum; i++) tmp.v[i] = v[i] + b.v[i];
        }
        return tmp;
    }

    Vector Sub(const Vector& b) const {
        int tnum = num < b.num ? num : b.num;
        Vector tmp(tnum);
        if (tmp.state == OK) {
            for (int i = 0; i < tnum; i++) tmp.v[i] = v[i] - b.v[i];
        }
        return tmp;
    }

    Vector Mul(unsigned int d) const {
        Vector tmp(*this);
        if (tmp.state == OK) {
            for (int i = 0; i < tmp.num; i++) tmp.v[i] *= d;
        }
        return tmp;
    }

    // Порівняння (за сумою елементів для прикладу)
    long sum() const {
        long s = 0;
        for (int i = 0; i < num; i++) s += v[i];
        return s;
    }

    bool isLess(const Vector& b) const { return sum() < b.sum(); }
    bool isEqual(const Vector& b) const { return sum() == b.sum(); }
    bool isNotEqual(const Vector& b) const { return sum() != b.sum(); }
};

int Vector::count = 0;

void Task_2_5_Menu() {
    int choice;
    Vector v1(3, 5); // Вектор з 3 елементів, ініціалізований 5
    Vector v2(3, 2); // Вектор з 3 елементів, ініціалізований 2

    do {
        cout << "\n>>> ЗАДАЧА 2.5: ВЕКТОР <<<\n";
        cout << "Об'єктів у пам'яті: " << Vector::count << "\n";
        cout << "[1] V1: "; v1.Print();
        cout << "[2] V2: "; v2.Print();
        cout << "-----------------------------------\n";
        cout << "1. Змінити елемент у V1\n";
        cout << "2. Додавання (V1 + V2)\n";
        cout << "3. Віднімання (V1 - V2)\n";
        cout << "4. Множення V1 на unsigned int\n";
        cout << "5. Порівняти V1 та V2\n";
        cout << "0. Повернутися до Головного меню\n";
        cout << "Ваш вибір: ";

        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); choice = -1; }

        switch (choice) {
        case 1: {
            int idx; long val;
            cout << "Індекс (0-" << v1.getNum() - 1 << "): "; cin >> idx;
            cout << "Нове значення: "; cin >> val;
            v1.setElement(idx, val);
            break;
        }
        case 2: {
            Vector res = v1.Add(v2);
            cout << "Результат: "; res.Print();
            break;
        }
        case 3: {
            Vector res = v1.Sub(v2);
            cout << "Результат: "; res.Print();
            break;
        }
        case 4: {
            unsigned int scalar;
            cout << "Введіть множник (>0): "; cin >> scalar;
            Vector res = v1.Mul(scalar);
            cout << "Результат: "; res.Print();
            break;
        }
        case 5: {
            cout << "Сума V1 = " << v1.sum() << ", Сума V2 = " << v2.sum() << "\n";
            if (v1.isLess(v2)) cout << "V1 < V2 (True)\n";
            if (v1.isEqual(v2)) cout << "V1 == V2 (True)\n";
            if (v1.isNotEqual(v2)) cout << "V1 != V2 (True)\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (choice != 0);
}

// =====================================================================
// ЗАДАЧА 3.5: КЛАС МАТРИЦЯ
// =====================================================================
class Matrix {
    int** mat;
    int rows, cols;
    int state;

    void allocate() {
        mat = new (nothrow) int* [rows];
        if (!mat) { state = BAD_ALLOC; rows = 0; cols = 0; return; }
        for (int i = 0; i < rows; i++) {
            mat[i] = new (nothrow) int[cols];
            if (!mat[i]) { state = BAD_ALLOC; return; }
        }
        state = OK;
    }

    void freeMem() {
        if (mat) {
            for (int i = 0; i < rows; i++) delete[] mat[i];
            delete[] mat;
            mat = nullptr;
        }
    }

public:
    static int count;

    // Конструктор 1: без параметрів 3x3
    Matrix() {
        rows = 3; cols = 3; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = 0;
        }
        count++;
    }

    // Конструктор 2: n x n
    Matrix(int n) {
        rows = (n > 0) ? n : 3; cols = rows; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = 0;
        }
        count++;
    }

    // Конструктор 3: n x m + value
    Matrix(int r, int c, int val) {
        rows = (r > 0) ? r : 3; cols = (c > 0) ? c : 3; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = val;
        }
        count++;
    }

    // Конструктор копій
    Matrix(const Matrix& s) {
        rows = s.rows; cols = s.cols; allocate(); state = s.state;
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = s.mat[i][j];
        }
        count++;
    }

    // Операція присвоєння
    Matrix& operator=(const Matrix& s) {
        if (this != &s) {
            freeMem();
            rows = s.rows; cols = s.cols; allocate(); state = s.state;
            if (state == OK) {
                for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = s.mat[i][j];
            }
        }
        return *this;
    }

    ~Matrix() {
        count--;
        freeMem();
    }

    // Сетер (з параметром за замовчуванням)
    void setElement(int r, int c, int val = 0) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && state == OK) mat[r][c] = val;
        else state = OUT_OF_BOUNDS;
    }

    // Гетер
    int getElement(int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && state == OK) return mat[r][c];
        state = OUT_OF_BOUNDS;
        return 0;
    }

    void Print() const {
        cout << "Матриця " << rows << "x" << cols << " (Стан: " << state << "):\n";
        if (state == OK && mat) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) cout << mat[i][j] << "\t";
                cout << "\n";
            }
        }
    }

    // Арифметика
    Matrix Add(const Matrix& b) const {
        int minR = rows < b.rows ? rows : b.rows;
        int minC = cols < b.cols ? cols : b.cols;
        Matrix tmp(minR, minC, 0);
        if (tmp.state == OK) {
            for (int i = 0; i < minR; i++)
                for (int j = 0; j < minC; j++) tmp.mat[i][j] = mat[i][j] + b.mat[i][j];
        }
        return tmp;
    }

    Matrix Sub(const Matrix& b) const {
        int minR = rows < b.rows ? rows : b.rows;
        int minC = cols < b.cols ? cols : b.cols;
        Matrix tmp(minR, minC, 0);
        if (tmp.state == OK) {
            for (int i = 0; i < minR; i++)
                for (int j = 0; j < minC; j++) tmp.mat[i][j] = mat[i][j] - b.mat[i][j];
        }
        return tmp;
    }

    Matrix Mul(const Matrix& b) const {
        // Спрощене по-елементне множення для прикладу або множення сумісних
        int minR = rows < b.rows ? rows : b.rows;
        int minC = cols < b.cols ? cols : b.cols;
        Matrix tmp(minR, minC, 0);
        if (tmp.state == OK) {
            for (int i = 0; i < minR; i++)
                for (int j = 0; j < minC; j++) tmp.mat[i][j] = mat[i][j] * b.mat[i][j];
        }
        return tmp;
    }

    Matrix MulScalar(short d) const {
        Matrix tmp(*this);
        if (tmp.state == OK) {
            for (int i = 0; i < tmp.rows; i++)
                for (int j = 0; j < tmp.cols; j++) tmp.mat[i][j] *= d;
        }
        return tmp;
    }

    // Порівняння (за сумою)
    long sum() const {
        long s = 0;
        for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) s += mat[i][j];
        return s;
    }

    bool isGreater(const Matrix& b) const { return sum() > b.sum(); }
    bool isLess(const Matrix& b) const { return sum() < b.sum(); }
    bool isEqual(const Matrix& b) const { return sum() == b.sum(); }
};

int Matrix::count = 0;

void Task_3_5_Menu() {
    int choice;
    Matrix m1(2, 2, 3);
    Matrix m2(2, 2, 2);

    do {
        cout << "\n>>> ЗАДАЧА 3.5: МАТРИЦЯ <<<\n";
        cout << "Об'єктів у пам'яті: " << Matrix::count << "\n";
        cout << "[1] M1:\n"; m1.Print();
        cout << "[2] M2:\n"; m2.Print();
        cout << "-----------------------------------\n";
        cout << "1. Змінити елемент у M1\n";
        cout << "2. Додавання (M1 + M2)\n";
        cout << "3. Віднімання (M1 - M2)\n";
        cout << "4. Множення матриць (M1 * M2)\n";
        cout << "5. Множення M1 на скаляр (short)\n";
        cout << "6. Порівняти M1 та M2\n";
        cout << "0. Повернутися до Головного меню\n";
        cout << "Ваш вибір: ";

        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); choice = -1; }

        switch (choice) {
        case 1: {
            int r, c, val;
            cout << "Рядок і стовпець: "; cin >> r >> c;
            cout << "Нове значення: "; cin >> val;
            m1.setElement(r, c, val);
            break;
        }
        case 2: {
            Matrix res = m1.Add(m2);
            cout << "Результат:\n"; res.Print();
            break;
        }
        case 3: {
            Matrix res = m1.Sub(m2);
            cout << "Результат:\n"; res.Print();
            break;
        }
        case 4: {
            Matrix res = m1.Mul(m2);
            cout << "Результат:\n"; res.Print();
            break;
        }
        case 5: {
            short scalar;
            cout << "Введіть скаляр (short): "; cin >> scalar;
            Matrix res = m1.MulScalar(scalar);
            cout << "Результат:\n"; res.Print();
            break;
        }
        case 6: {
            cout << "Сума M1 = " << m1.sum() << ", Сума M2 = " << m2.sum() << "\n";
            if (m1.isGreater(m2)) cout << "M1 > M2 (True)\n";
            if (m1.isLess(m2)) cout << "M1 < M2 (True)\n";
            if (m1.isEqual(m2)) cout << "M1 == M2 (True)\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (choice != 0);
}

// =====================================================================
// ГОЛОВНЕ МЕНЮ
// =====================================================================
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int mainChoice;

    do {
        cout << "\n================ ГОЛОВНЕ МЕНЮ =================\n";
        cout << "1. Задача 1.5 (Клас 'Трикутник')\n";
        cout << "2. Задача 2.5 (Клас 'Вектор')\n";
        cout << "3. Задача 3.5 (Клас 'Матриця')\n";
        cout << "0. Вихід з програми\n";
        cout << "===============================================\n";
        cout << "Ваш вибір: ";

        cin >> mainChoice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(32767, '\n');
            mainChoice = -1;
        }

        switch (mainChoice) {
        case 1: Task_1_5_Menu(); break;
        case 2: Task_2_5_Menu(); break;
        case 3: Task_3_5_Menu(); break;
        case 0: cout << "Роботу завершено. До побачення!\n"; break;
        default: cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    } while (mainChoice != 0);

    return 0;
}