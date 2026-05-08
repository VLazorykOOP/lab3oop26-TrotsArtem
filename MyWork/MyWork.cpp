#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <windows.h>

using namespace std;

// Перелічення станів для класів з динамічну пам'ять
enum STATE {
    OK = 0,
    BAD_INIT,
    BAD_ALLOC,
    OUT_OF_BOUNDS,
    BAD_DIV
};

// ЗАДАЧА 1 ТРИКУТНИК
class Triangle {
    double a, b, c;
    int color;

public:
    // Конструктор за замовчуванням
    Triangle() : a(1.0), b(1.0), c(1.0), color(0) {}

    // Конструктор з параметрами
    Triangle(double a, double b, double c, int color) {
        if (isValid(a, b, c)) { // Якщо утворюється правильний трикутник
            this->a = a; this->b = b; this->c = c;
        }
        else { // Інакше значення за замовчуванням
            this->a = 1.0; this->b = 1.0; this->c = 1.0;
        }
        this->color = (color >= 0) ? color : 0;
    }

    // Перевірка існування (сума двох сторін більша за третю)
    bool isValid(double a, double b, double c) {
        return (a > 0 && b > 0 && c > 0 && (a + b > c) && (a + c > b) && (b + c > a));
    }

    // Отримання значень полів
    double getA() const { return a; }
    double getB() const { return b; }
    double getC() const { return c; }
    int getColor() const { return color; }

    // Встановлення сторін, перевірка коректності
    void setSides(double a, double b, double c) {
        if (isValid(a, b, c)) {
            this->a = a; this->b = b; this->c = c;
        }
        else {
            cout << " [Помилка] Некоректні сторони трикутника!\n";
        }
    }

    // Встановлення кольору із перевіркою
    void setColor(int c_val) {
        if (c_val < 0 || c_val > 10000) {
            cout << " [Помилка] Некоректний колір!\n";
            return;
        }
        this->color = c_val;
    }

    // Периметр
    double P() const { return a + b + c; }

    // Площа за формулою Герона
    double S() const {
        double p = P() / 2.0; // Півпериметр
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

    // Виведення інформації
    void printInfo() const {
        cout << "a=" << a << " b=" << b << " c=" << c << " колір=" << color;
        cout << "  Периметр=" << P() << " Площа=" << S() << "\n";
    }
};

// Меню класу Triangle
void Task_1_5_Menu() {
    cout << "\n>>> ІНІЦІАЛІЗАЦІЯ ТРИКУТНИКА <<<\n";
    double a, b, c;
    int col;

    cout << "Введіть сторони трикутника (a, b, c через пробіл): ";
    cin >> a >> b >> c;
    cout << "Введіть колір (ціле число >= 0): ";
    cin >> col;

    Triangle t(a, b, c, col); // Створення об'єкта
    int subChoice;

    do {
        cout << "\n>>> МЕНЮ: ЗАДАЧА 1.5 (ТРИКУТНИК) <<<\n";
        cout << "Поточний трикутник: "; t.printInfo();
        cout << "-----------------------------------\n";
        cout << "1. Ввести нові сторони\n";
        cout << "2. Ввести новий колір\n";
        cout << "3. Згенерувати випадковий трикутник\n";
        cout << "0. Повернутися до Головного меню\n";
        cout << "Ваш вибір: ";

        cin >> subChoice;
        // Захист від введення букв замість цифр
        if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); subChoice = -1; }

        switch (subChoice) {
        case 1:
            cout << "Введіть нові a, b, c: ";
            cin >> a >> b >> c;
            t.setSides(a, b, c);
            break;
        case 2:
            cout << "Введіть новий колір (ціле число >= 0): ";
            cin >> col;
            t.setColor(col);
            break;
        case 3: {
            // Генерація випадкових чисел
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<double> dist(1.0, 10.0);
            uniform_int_distribution<int> cDist(0, 255);

            double side = dist(gen);
            t.setSides(side, side, side); // Створення рівносторонній трикутника
            t.setColor(cDist(gen));
            cout << "Згенеровано новий рівносторонній трикутник.\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (subChoice != 0);
}

// ЗАДАЧА 2 ВЕКТОР
class Vector {
    long* v;    // Вказівник на динамічний масив
    int num;    // Розмір вектора
    int state;  // Стан об'єкта
public:
    static int count; // Статична змінна для підрахунку існуючих об'єктів

    // Конструктор за замовчуванням
    Vector() {
        num = 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; v[0] = 0; }
        count++;
    }

    // Конструктор із заданим розміром
    Vector(int n) {
        num = (n > 0) ? n : 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; for (int i = 0; i < num; i++) v[i] = 0; } // Заповнення нулями
        count++;
    }

    // Конструктор заданого розміру + ініціалізація з певними значеннями
    Vector(int n, long val) {
        num = (n > 0) ? n : 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; for (int i = 0; i < num; i++) v[i] = val; }
        count++;
    }

    // Конструктор копіювання
    Vector(const Vector& s) {
        num = s.num;
        state = s.state;
        v = new (nothrow) long[num];
        if (v && state == OK) {
            for (int i = 0; i < num; i++) v[i] = s.v[i]; // Копіювання кожен елемент
        }
        else { state = BAD_ALLOC; num = 0; }
        count++;
    }

    // Присвоєння
    Vector& operator=(const Vector& s) {
        if (this != &s) { // Захист від самоприсвоєння
            if (num != s.num) {
                if (v) delete[] v; // Видалення старої пам'яті
                num = s.num;
                v = new (nothrow) long[num]; // Виділення нової
                if (!v) { state = BAD_ALLOC; num = 0; return *this; }
            }
            state = s.state;
            for (int i = 0; i < num; i++) v[i] = s.v[i];
        }
        return *this;
    }

    // Звільняє пам'яті при видаленні об'єкта
    ~Vector() {
        count--;
        if (v) delete[] v;
    }

    // Встановлення елементу за індексом
    void setElement(int index, long val = 0) {
        if (index >= 0 && index < num && state == OK) v[index] = val;
        else state = OUT_OF_BOUNDS;
    }

    // Отримання елементу за індексом
    long getElement(int index) {
        if (index >= 0 && index < num && state == OK) return v[index];
        state = OUT_OF_BOUNDS;
        return 0;
    }

    // Отримання стану і розміру
    int getState() const { return state; }
    int getNum() const { return num; }

    // Виведення вектора
    void Print() const {
        cout << "Вектор [" << num << "] (Стан: " << state << "): ";
        if (state == OK && v) {
            for (int i = 0; i < num; i++) cout << v[i] << " ";
        }
        cout << "\n";
    }

    // Додавання двох векторів
    Vector Add(const Vector& b) const {
        int tnum = num < b.num ? num : b.num; 
        Vector tmp(tnum);
        if (tmp.state == OK) {
            for (int i = 0; i < tnum; i++) tmp.v[i] = v[i] + b.v[i];
        }
        return tmp;
    }

    // Віднімання векторів
    Vector Sub(const Vector& b) const {
        int tnum = num < b.num ? num : b.num;
        Vector tmp(tnum);
        if (tmp.state == OK) {
            for (int i = 0; i < tnum; i++) tmp.v[i] = v[i] - b.v[i];
        }
        return tmp;
    }

    // Множення всіх елементів вектора на число
    Vector Mul(unsigned int d) const {
        Vector tmp(*this); // Створення копії
        if (tmp.state == OK) {
            for (int i = 0; i < tmp.num; i++) tmp.v[i] *= d;
        }
        return tmp;
    }

    // Сума для порівняння
    long sum() const {
        long s = 0;
        for (int i = 0; i < num; i++) s += v[i];
        return s;
    }

    // Порівняння векторів
    bool isLess(const Vector& b) const { return sum() < b.sum(); }
    bool isEqual(const Vector& b) const { return sum() == b.sum(); }
    bool isNotEqual(const Vector& b) const { return sum() != b.sum(); }
};

int Vector::count = 0; // Статичної змінної

// Меню класу Vector
void Task_2_5_Menu() {
    int n1, n2;
    long val1, val2;

    cout << "\n>>> ІНІЦІАЛІЗАЦІЯ ВЕКТОРІВ <<<\n";
    cout << "Введіть РОЗМІР першого вектора та ПОЧАТКОВЕ ЗНАЧЕННЯ елементів (через пробіл): ";
    cin >> n1 >> val1;
    Vector v1(n1, val1);

    cout << "Введіть РОЗМІР другого вектора та ПОЧАТКОВЕ ЗНАЧЕННЯ елементів (через пробіл): ";
    cin >> n2 >> val2;
    Vector v2(n2, val2);

    int choice;
    do {
        cout << "\n>>> МЕНЮ: ЗАДАЧА 2.5 (ВЕКТОР) <<<\n";
        cout << "Об'єктів у пам'яті: " << Vector::count << "\n";
        cout << "[1] V1: "; v1.Print();
        cout << "[2] V2: "; v2.Print();
        cout << "-----------------------------------\n";
        cout << "1. Змінити певний елемент у V1\n";
        cout << "2. Додавання (V1 + V2)\n";
        cout << "3. Віднімання (V1 - V2)\n";
        cout << "4. Множення V1 на число (unsigned int)\n";
        cout << "5. Порівняти V1 та V2 (за сумою елементів)\n";
        cout << "0. Повернутися до Головного меню\n";
        cout << "Ваш вибір: ";

        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); choice = -1; }

        switch (choice) {
        case 1: {
            int idx; long val;
            cout << "Введіть індекс (0-" << v1.getNum() - 1 << "): "; cin >> idx;
            cout << "Введіть нове значення: "; cin >> val;
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
            cout << "Сума елементів V1 = " << v1.sum() << ", Сума елементів V2 = " << v2.sum() << "\n";
            if (v1.isLess(v2)) cout << "V1 < V2\n";
            else if (v1.isEqual(v2)) cout << "V1 == V2\n";
            else cout << "V1 > V2\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (choice != 0);
}

// ЗАДАЧА 3 МАТРИЦЯ
class Matrix {
    int** mat;      // Вказівник на масив вказівників
    int rows, cols;
    int state;

    void allocate() {
        mat = new (nothrow) int* [rows]; // Виділення пам'яті під рядки
        if (!mat) { state = BAD_ALLOC; rows = 0; cols = 0; return; }
        for (int i = 0; i < rows; i++) {
            mat[i] = new (nothrow) int[cols]; // Виділення пам'яті під стовпці
            if (!mat[i]) { state = BAD_ALLOC; return; }
        }
        state = OK;
    }

    // Безпечне очищення пам'яті
    void freeMem() {
        if (mat) {
            for (int i = 0; i < rows; i++) delete[] mat[i]; // Видалення стовпців
            delete[] mat; // Видалення масиву вказівників
            mat = nullptr;
        }
    }

public:
    static int count;

    // Конструктор за замовчуванням (матриця 3x3)
    Matrix() {
        rows = 3; cols = 3; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = 0;
        }
        count++;
    }

    // Конструктор квадратної матриці
    Matrix(int n) {
        rows = (n > 0) ? n : 3; cols = rows; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = 0;
        }
        count++;
    }

    // Конструктор матриці довільного розміру
    Matrix(int r, int c, int val) {
        rows = (r > 0) ? r : 3; cols = (c > 0) ? c : 3; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = val;
        }
        count++;
    }

    // Конструктор копіювання
    Matrix(const Matrix& s) {
        rows = s.rows; cols = s.cols; allocate(); state = s.state;
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = s.mat[i][j];
        }
        count++;
    }

    // Оператор присвоєння
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

    // Деструктор
    ~Matrix() {
        count--;
        freeMem();
    }

    // Встановлення елемента матриці
    void setElement(int r, int c, int val = 0) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && state == OK) mat[r][c] = val;
        else state = OUT_OF_BOUNDS;
    }

    // Елементи матриці
    int getElement(int r, int c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols && state == OK) return mat[r][c];
        state = OUT_OF_BOUNDS;
        return 0;
    }

    // Виведення матриці
    void Print() const {
        cout << "Матриця " << rows << "x" << cols << " (Стан: " << state << "):\n";
        if (state == OK && mat) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) cout << mat[i][j] << "\t";
                cout << "\n";
            }
        }
    }

    // Додавання
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

    // Віднімання
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

    // Множення
    Matrix Mul(const Matrix& b) const {
        int minR = rows < b.rows ? rows : b.rows;
        int minC = cols < b.cols ? cols : b.cols;
        Matrix tmp(minR, minC, 0);
        if (tmp.state == OK) {
            for (int i = 0; i < minR; i++)
                for (int j = 0; j < minC; j++) tmp.mat[i][j] = mat[i][j] * b.mat[i][j];
        }
        return tmp;
    }

    // Множення на скаляр
    Matrix MulScalar(short d) const {
        Matrix tmp(*this);
        if (tmp.state == OK) {
            for (int i = 0; i < tmp.rows; i++)
                for (int j = 0; j < tmp.cols; j++) tmp.mat[i][j] *= d;
        }
        return tmp;
    }

    // Сума елементів для порівняння
    long sum() const {
        long s = 0;
        for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) s += mat[i][j];
        return s;
    }

    // Порівняння
    bool isGreater(const Matrix& b) const { return sum() > b.sum(); }
    bool isLess(const Matrix& b) const { return sum() < b.sum(); }
    bool isEqual(const Matrix& b) const { return sum() == b.sum(); }
};

int Matrix::count = 0;

// Меню класу Matrix
void Task_3_5_Menu() {
    int r1, c1, val1;
    int r2, c2, val2;

    cout << "\n>>> ІНІЦІАЛІЗАЦІЯ МАТРИЦЬ <<<\n";
    cout << "ПЕРША матриця. Введіть кількість РЯДКІВ, СТОВПЦІВ та ПОЧАТКОВЕ ЗНАЧЕННЯ: ";
    cin >> r1 >> c1 >> val1;
    Matrix m1(r1, c1, val1);

    cout << "ДРУГА матриця. Введіть кількість РЯДКІВ, СТОВПЦІВ та ПОЧАТКОВЕ ЗНАЧЕННЯ: ";
    cin >> r2 >> c2 >> val2;
    Matrix m2(r2, c2, val2);

    int choice;
    do {
        cout << "\n>>> МЕНЮ: ЗАДАЧА 3.5 (МАТРИЦЯ) <<<\n";
        cout << "Об'єктів у пам'яті: " << Matrix::count << "\n";
        cout << "[1] M1:\n"; m1.Print();
        cout << "[2] M2:\n"; m2.Print();
        cout << "-----------------------------------\n";
        cout << "1. Змінити певний елемент у M1\n";
        cout << "2. Додавання (M1 + M2)\n";
        cout << "3. Віднімання (M1 - M2)\n";
        cout << "4. По-елементне множення (M1 * M2)\n";
        cout << "5. Множення M1 на скаляр (short)\n";
        cout << "6. Порівняти M1 та M2 (за сумою)\n";
        cout << "0. Повернутися до Головного меню\n";
        cout << "Ваш вибір: ";

        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(32767, '\n'); choice = -1; }

        switch (choice) {
        case 1: {
            int r, c, val;
            cout << "Введіть індекси (рядок і стовпець, починаючи з 0): "; cin >> r >> c;
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
            cout << "Сума елементів M1 = " << m1.sum() << ", Сума елементів M2 = " << m2.sum() << "\n";
            if (m1.isGreater(m2)) cout << "M1 > M2\n";
            else if (m1.isLess(m2)) cout << "M1 < M2\n";
            else cout << "M1 == M2\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (choice != 0);
}

// ГОЛОВНЕ МЕНЮ
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

        // Захист від некоректного вводу
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
