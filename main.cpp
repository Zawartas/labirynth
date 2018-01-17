#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <queue>
#include <utility>
#include <unordered_map>

using namespace std;

class Matrix
{
    public:
        Matrix();
        Matrix(int rows, int columns);
        ~Matrix();
        Matrix &operator =(const Matrix &matrix);
        char &operator()(int, int);
        const char &operator()(int, int) const;
        int rows() const;
        int columns() const;
        bool &operator()(int);
        bool znajdz_pierwsza_sciezke (int, int);
        bool BFS (int);
        int xy_z(int x, int y){return x*cls + y;}
        bool is_exit(int z){
            int row = z/cls;
            int column = z%cls;
            if(get<0>(plansza[row * cls + column])=='2')
                return 1;
        return false;}
        pair<int, int> z_xy(int z){pair<int, int> para (z/cls, z%cls); return para;}
    private:
        int rws, cls;
        vector<tuple <char, bool>> plansza;
};

ostream &operator <<(ostream& , const Matrix&);
static int x_start, y_start;
Matrix wczytaj_plansze(string);

////////////////////////////////////////////////////////////////////////////////////////

static queue<int> kolejka;
static unordered_map<int, int> poprzednik;

static int STOPER = 0;

bool Matrix::BFS (int z){
    STOPER++;

    if (is_exit(z)){
        return true;}
    else {
        get<1>(plansza[z]) = true;
        if (!kolejka.empty())
            kolejka.pop();

        //DODAJEMY SASIADÓW
            if (get<0>(plansza[z-cls]) != '@' && get<1>(plansza[z-cls]) == false){
                kolejka.push(z-cls);
                poprzednik.insert({z-cls, z});}
            if (get<0>(plansza[z - 1]) != '@' && get<1>(plansza[z-1]) == false){
                kolejka.push(z - 1);
                poprzednik.insert({z-1, z});}
            if (get<0>(plansza[z + 1]) != '@' && get<1>(plansza[z+1]) == false){
                kolejka.push(z + 1);
                poprzednik.insert({z+1, z});}
            if (get<0>(plansza[z + cls]) != '@' && get<1>(plansza[z+cls]) == false){
                kolejka.push(z + cls);
                poprzednik.insert({z+cls, z});}}

    BFS(kolejka.front());

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////
    static vector<int> visited;
int main()
{
    Matrix matrix1;

    matrix1 = wczytaj_plansze("labirynth3.txt");
    cout << endl << matrix1 << endl;

    cout << "Which method: (1, 2, 3, 4): ";
    int x;
    cin >> x;
    switch(x){
        case 1:
            if (matrix1.znajdz_pierwsza_sciezke(x_start, y_start) == true) {
                    cout << "Path found.\n";

                    for (auto i: visited)
                        cout << i << " ";
                    cout << "\nLength: " << visited.size() << endl;}
            else cout << "There is no path";

            break;
        case 2:
            matrix1.BFS(matrix1.xy_z(x_start, y_start));
            x = 471;
            while (x != 487){
                visited.push_back(poprzednik.find(x)->second);
                x = visited.back();
                }

            for (auto i: visited){
                        cout << i << " ";
                        int x = matrix1.z_xy(i).first;
                        int y = matrix1.z_xy(i).second;
                        matrix1(x, y) = '-';}
                        cout << "\nLength: " << visited.size() << endl;
            break;
    }
    cout << endl << matrix1 << endl;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////

ostream &operator <<(ostream &stream, const Matrix &matrix){
    for (int row = 0; row < matrix.rows(); ++row){
        for (int column = 0; column < matrix.columns(); ++column){
            stream << matrix(row, column) << " ";}
        stream << endl;}
    return stream;}

Matrix::Matrix(): rws(0), cls(0), plansza(0) {}
Matrix::Matrix(int rows, int columns): rws(rows), cls(columns) {
    for (int i = 0; i < rws * cls; i++)
        plansza.push_back(make_tuple(' ', false));}
Matrix::~Matrix(){plansza.clear();}

Matrix &Matrix::operator =(const Matrix &matrix){
    rws = matrix.rws;
    cls = matrix.cls;
    plansza = matrix.plansza;
    return *this;}

char &Matrix::operator()(int row, int column){
    return get<0>(plansza[row * cls + column]) ;}

const char &Matrix::operator()(int row, int column) const
{
    return get<0>(plansza[row * cls + column]);
}
int Matrix::rows() const
{
    return rws;
}
int Matrix::columns() const
{
    return cls;
}

bool &Matrix::operator()(int value){

    return get<1>(plansza[value]);
}

////////////////////////////////////////////////////////////////////////////////////////

bool Matrix::znajdz_pierwsza_sciezke (int i, int j){
    STOPER++;
    if (get<0>(plansza[i * cls + j]) == '2'){
        get<0>(plansza[x_start * cls + y_start]) = '1';
        return true;}
    if (get<0>(plansza[i * cls + j]) != ' ' && get<0>(plansza[i * cls + j]) != '1')
        return false;
    get<0>(plansza[i * cls + j]) = '-';
    visited.push_back(i * cls + j);

    if (znajdz_pierwsza_sciezke(i-1, j) == true){
        return true;}
    if (znajdz_pierwsza_sciezke(i, j+1) == true){
        return true;}
    if (znajdz_pierwsza_sciezke(i+1, j) == true){
        return true;}
    if (znajdz_pierwsza_sciezke(i, j-1) == true){
        return true;}
    get<0>(plansza[i * cls + j]) = 'x';
    visited.pop_back();
    return false;
}




Matrix wczytaj_plansze(string x){

    ifstream plansza;
    char znak;
    string linijka;
    unsigned int x1 = 0, y1 = 0, maks = 0;

    plansza.open(x);

    while (getline(plansza, linijka)){
        if(linijka.size() > y1)
            y1 = linijka.size();
            x1++;}

    plansza.close();

    Matrix matrix1(x1, y1);

    plansza.open(x);

    for (int i = 0; i < x1; ++i){
        for (int j = 0; j < y1; ++j){
                plansza.get(znak);

                if (znak == '\n' && j == 0){
                    --j; continue;}
                else if (znak == '\n' || plansza.eof()){
                    while (j < y1){
                        matrix1(i, j) = ' ';
                        j++;}
                    }
                else if (znak == '@')
                    matrix1(i, j) = 64;
                else if (znak == ' ')
                    matrix1(i, j) = ' ';
                else if (znak == '1'){
                    matrix1(i, j) = '1';
                    x_start = i;
                    y_start = j;
                    }
                else if (znak == '2')
                    matrix1(i, j) = '2';
        }
    }
    plansza.close();
    return matrix1;
}
