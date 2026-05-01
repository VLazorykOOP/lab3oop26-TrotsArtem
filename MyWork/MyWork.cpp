#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <windows.h>

using namespace std;

// Перелічення станів для класів, які використовують динамічну пам'ять (Вектор, Матриця)
enum STATE {
    OK = 0,             // Все добре
    BAD_INIT,           // Помилка ініціалізації
    BAD_ALLOC,          // Помилка виділення пам'яті
    OUT_OF_BOUNDS,      // Вихід за межі масиву
    BAD_DIV             // Ділення на нуль
};

// ЗАДАЧА 1.5: КЛАС ТРИКУТНИК
class Triangle {
    double a, b, c; // Сторони трикутника
    int color;      // Колір у вигляді цілого числа

public:
    // 1. Конструктор за замовчуванням (створює базовий трикутник 1, 1, 1)
    Triangle() : a(1.0), b(1.0), c(1.0), color(0) {}

    // 2. Конструктор з параметрами
    Triangle(double a, double b, double c, int color) {
        if (isValid(a, b, c)) { // Якщо сторони утворюють правильний трикутник
            this->a = a; this->b = b; this->c = c;
        }
        else { // Інакше ставимо значення за замовчуванням
            this->a = 1.0; this->b = 1.0; this->c = 1.0;
        }
        this->color = (color >= 0) ? color : 0; // Перевірка на від'ємний колір
    }

    // Метод перевірки існування трикутника (сума двох сторін має бути більшою за третю)
    bool isValid(double a, double b, double c) {
        return (a > 0 && b > 0 && c > 0 && (a + b > c) && (a + c > b) && (b + c > a));
    }

    // Гетери (методи для отримання значень полів)
    double getA() const { return a; }
    double getB() const { return b; }
    double getC() const { return c; }
    int getColor() const { return color; }

    // Сетер для сторін із перевіркою коректності
    void setSides(double a, double b, double c) {
        if (isValid(a, b, c)) {
            this->a = a; this->b = b; this->c = c;
        }
        else {
            cout << " [Помилка] Некоректні сторони трикутника!\n";
        }
    }

    // Сетер для кольору із перевіркою
    void setColor(int c_val) {
        if (c_val < 0 || c_val > 10000) {
            cout << " [Помилка] Некоректний колір!\n";
            return;
        }
        this->color = c_val;
    }

    // Метод обчислення периметра
    double P() const { return a + b + c; }

    // Метод обчислення площі за формулою Герона
    double S() const {
        double p = P() / 2.0; // Півпериметр
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

    // Виведення інформації про об'єкт у консоль
    void printInfo() const {
        cout << "a=" << a << " b=" << b << " c=" << c << " колір=" << color;
        cout << "  Периметр=" << P() << " Площа=" << S() << "\n";
    }
};

// Функція-меню для роботи з класом Triangle
void Task_1_5_Menu() {
    cout << "\n>>> ІНІЦІАЛІЗАЦІЯ ТРИКУТНИКА <<<\n";
    double a, b, c;
    int col;

    // Первинне введення даних користувачем
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
            t.setSides(side, side, side); // Створюємо рівносторонній трикутник для надійності
            t.setColor(cDist(gen));
            cout << "Згенеровано новий рівносторонній трикутник.\n";
            break;
        }
        case 0: cout << "Повернення...\n"; break;
        default: cout << "Невірний вибір.\n";
        }
    } while (subChoice != 0);
}

// ЗАДАЧА 2.5: КЛАС ВЕКТОР
class Vector {
    long* v;    // Вказівник на динамічний масив (елементи вектора)
    int num;    // Розмір вектора (кількість елементів)
    int state;  // Стан об'єкта (чи не було помилок пам'яті тощо)
public:
    static int count; // Статична змінна для підрахунку існуючих об'єктів

    // 1. Конструктор за замовчуванням (створює масив на 1 елемент)
    Vector() {
        num = 1;
        v = new (nothrow) long[num]; // nothrow запобігає вильоту програми при нестачі пам'яті
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; v[0] = 0; }
        count++;
    }

    // 2. Конструктор із заданим розміром
    Vector(int n) {
        num = (n > 0) ? n : 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; for (int i = 0; i < num; i++) v[i] = 0; } // Заповнюємо нулями
        count++;
    }

    // 3. Конструктор: заданий розмір + ініціалізація певним значенням
    Vector(int n, long val) {
        num = (n > 0) ? n : 1;
        v = new (nothrow) long[num];
        if (!v) { state = BAD_ALLOC; num = 0; }
        else { state = OK; for (int i = 0; i < num; i++) v[i] = val; }
        count++;
    }

    // 4. Конструктор копіювання (глибоке копіювання динамічного масиву)
    Vector(const Vector& s) {
        num = s.num;
        state = s.state;
        v = new (nothrow) long[num];
        if (v && state == OK) {
            for (int i = 0; i < num; i++) v[i] = s.v[i]; // Копіюємо кожен елемент
        }
        else { state = BAD_ALLOC; num = 0; }
        count++;
    }

    // Оператор присвоєння
    Vector& operator=(const Vector& s) {
        if (this != &s) { // Захист від самоприсвоєння (a = a)
            if (num != s.num) {
                if (v) delete[] v; // Видаляємо стару пам'ять
                num = s.num;
                v = new (nothrow) long[num]; // Виділяємо нову під новий розмір
                if (!v) { state = BAD_ALLOC; num = 0; return *this; }
            }
            state = s.state;
            for (int i = 0; i < num; i++) v[i] = s.v[i];
        }
        return *this;
    }

    // Деструктор (звільняє пам'ять при видаленні об'єкта)
    ~Vector() {
        count--;
        if (v) delete[] v;
    }

    // Встановити елемент за індексом
    void setElement(int index, long val = 0) {
        if (index >= 0 && index < num && state == OK) v[index] = val;
        else state = OUT_OF_BOUNDS; // Помилка: вийшли за межі масиву
    }

    // Отримати елемент за індексом
    long getElement(int index) {
        if (index >= 0 && index < num && state == OK) return v[index];
        state = OUT_OF_BOUNDS;
        return 0;
    }

    // Гетери для стану і розміру
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

    // Метод додавання двох векторів (по-елементно)
    Vector Add(const Vector& b) const {
        int tnum = num < b.num ? num : b.num; // Беремо мінімальний розмір
        Vector tmp(tnum);
        if (tmp.state == OK) {
            for (int i = 0; i < tnum; i++) tmp.v[i] = v[i] + b.v[i];
        }
        return tmp;
    }

    // Метод віднімання векторів
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
        Vector tmp(*this); // Створюємо копію поточного вектора
        if (tmp.state == OK) {
            for (int i = 0; i < tmp.num; i++) tmp.v[i] *= d;
        }
        return tmp;
    }

    // Допоміжний метод: сума всіх елементів (для порівняння)
    long sum() const {
        long s = 0;
        for (int i = 0; i < num; i++) s += v[i];
        return s;
    }

    // Методи порівняння векторів (на основі суми їх елементів)
    bool isLess(const Vector& b) const { return sum() < b.sum(); }
    bool isEqual(const Vector& b) const { return sum() == b.sum(); }
    bool isNotEqual(const Vector& b) const { return sum() != b.sum(); }
};

int Vector::count = 0; // Ініціалізація статичної змінної

// Функція-меню для вектора
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
            v1.setElement(idx, val); // Змінюємо конкретний елемент
            break;
        }
        case 2: {
            Vector res = v1.Add(v2); // Додаємо
            cout << "Результат: "; res.Print();
            break;
        }
        case 3: {
            Vector res = v1.Sub(v2); // Віднімаємо
            cout << "Результат: "; res.Print();
            break;
        }
        case 4: {
            unsigned int scalar;
            cout << "Введіть множник (>0): "; cin >> scalar;
            Vector res = v1.Mul(scalar); // Множимо
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

// ЗАДАЧА 3.5: КЛАС МАТРИЦЯ
class Matrix {
    int** mat;      // Вказівник на масив вказівників
    int rows, cols;
    int state;      // Стан матриці

    void allocate() {
        mat = new (nothrow) int* [rows]; // Виділення пам'яті під рядки
        if (!mat) { state = BAD_ALLOC; rows = 0; cols = 0; return; }
        for (int i = 0; i < rows; i++) {
            mat[i] = new (nothrow) int[cols]; // Виділення пам'яті під стовпці
            if (!mat[i]) { state = BAD_ALLOC; return; }
        }
        state = OK;
    }

    // Приватний метод для безпечного очищення пам'яті
    void freeMem() {
        if (mat) {
            for (int i = 0; i < rows; i++) delete[] mat[i]; // Видаляємо стовпці
            delete[] mat; // Видаляємо масив вказівників (рядки)
            mat = nullptr;
        }
    }

public:
    static int count;

    // 1. Конструктор за замовчуванням (матриця 3x3)
    Matrix() {
        rows = 3; cols = 3; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = 0;
        }
        count++;
    }

    // 2. Конструктор квадратної матриці (n x n)
    Matrix(int n) {
        rows = (n > 0) ? n : 3; cols = rows; allocate();
        if (state == OK) {
            for (int i = 0; i < rows; i++) for (int j = 0; j < cols; j++) mat[i][j] = 0;
        }
        count++;
    }

    // 3. Конструктор матриці довільного розміру + значення ініціалізації
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

    // Оператор присвоєння (видаляє стару пам'ять і виділяє нову)
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

    // Встановлення елемента матриці (рядки і стовпці нумеруються з 0)
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

    // Множення матриць
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

    // Множення матриці на скаляр
    Matrix MulScalar(short d) const {
        Matrix tmp(*this);
        if (tmp.state == OK) {
            for (int i = 0; i < tmp.rows; i++)
                for (int j = 0; j < tmp.cols; j++) tmp.mat[i][j] *= d;
        }
        return tmp;
    }

    // Сума елементів матриці для порівняння
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

// Функція-меню для матриці
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