#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <queue>
#include <unordered_map>

using namespace std;
typedef tuple<char, bool> parametry_pola;

class Matrix{
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
        bool first_path (int, int);
        int xy_z(int x, int y){return x*cls + y;} //na podstawie koordynatow - zwraca numer pola
        int distance(int, int);
        bool BFS (int, int); //parametr to koordynaty pola startowego
        bool DFS (int, int); //parametr to numer koordynaty startowego
        bool GBFS (int, int, int, int); //parametr to numer koordynaty startowego i celu
        bool is_exit(int); //sprawdza czy pole jest obiektem poszukiwanym '2'
        pair<int, int> z_xy(int z){pair<int, int> para (z/cls, z%cls); return para;} //na podstawie numeru pola, zwraca jego koordynaty
    private:
        int rws, cls;
        vector<parametry_pola> plansza; //kazde pole to jakiœ wektor sk³¹dajacy sie ze zmiennej char (co jest w kratce)
                                            //i bool'a (mowi o tym czy pole bylo odwiedzone czy nie)
};

ostream &operator <<(ostream& , const Matrix&);
Matrix wczytaj_plansze(string);

static int x_start, y_start;
static int x_finish, y_finish;
static queue<int> kolejkaFIFO;
static vector<int> kolejkaLIFO;
static unordered_map<int, int> poprzednik;
static vector<int> visited;
static int algorithm_runs = 0;


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    //TUTAJ PODAJEMY NAZWE PLIKU
    char *plik = "labirynth2.txt";
    Matrix labirynt = wczytaj_plansze(plik);

    cout << endl << labirynt << endl;

    cout << "Path finding method: (1 - first_path, 2 - BFS, 3 - DFS, 4 - GBFS): ";
    int x;
    cin >> x;
    system("cls");
    switch(x){
        case 1:
            if (labirynt.first_path(x_start, y_start) == true) {
                    cout << "Path found.\n";
                    cout << "Algorithm runs: " << algorithm_runs;
                    cout << "\nLength: " << visited.size() << endl;}
            else cout << "There is no path";

            break;
        case 2:
            if (labirynt.BFS(x_start, y_start) == true){
                    cout << "Path found.\n";
                    cout << "Algorithm runs: " << algorithm_runs;
                    cout << "\nLength: " << visited.size() << endl;}
            else cout << "There is no path";
            break;
        case 3:
            if (labirynt.DFS(x_start, y_start) == true){
                    cout << "Path found.\n";
                    cout << "Algorithm runs: " << algorithm_runs;
                    cout << "\nLength: " << visited.size() << endl;}
            else cout << "There is no path";
            break;
        case 4:
            // tutaj najpierw musimy znalezc '2'
            labirynt.BFS(x_start, y_start);
            x_finish = visited.front()/labirynt.columns();
            y_finish = visited.front()%labirynt.columns();
            visited.clear();    // czyscimy zeby algorytm mogl ponownie skorzystac z tego wektora
            algorithm_runs = 0; // j.w.
            labirynt = wczytaj_plansze(plik);
            if (labirynt.GBFS(x_start, y_start, x_finish, y_finish) == true){
                    cout << "Path found.\n";
                    cout << "Algorithm runs: " << algorithm_runs;
                    cout << "\nLength: " << visited.size() << endl;}
            else cout << "There is no path";
            break;
    }
    cout << endl << labirynt << endl;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
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

const char &Matrix::operator()(int row, int column) const{
    return get<0>(plansza[row * cls + column]);
}
bool &Matrix::operator()(int value){
    return get<1>(plansza[value]);
}
int Matrix::rows() const{
    return rws;
}
int Matrix::columns() const{
    return cls;
}

////////////////////////////////////////////////////////////////////////////////////////

bool Matrix::is_exit(int z){
            int row = z/cls;
            int column = z%cls;
            if(get<0>(plansza[row * cls + column])=='2')
                return true;
        return false;}

////////////////////////////////////////////////////////////////////////////////////////

bool Matrix::BFS (int x, int y){
algorithm_runs++;
    int z = xy_z(x, y);

    if (is_exit(z)){
        //odtwarzanie najkrotszej sciezki po poprzednikach '2'
        int start = xy_z(x_start, y_start);
        visited.push_back(z);
        while (z != start){
                visited.push_back(poprzednik[z]);
                z = visited.back();
                if(get<0>(plansza[z]) != '1')
                    get<0>(plansza[z]) = '-';}
    return true;}

    else if (!is_exit(z)){
        if (get<0>(plansza[z]) != '1' && get<0>(plansza[z]) != '2')
            get<0>(plansza[z]) = 'x';
        if (!kolejkaFIFO.empty())
            kolejkaFIFO.pop();

        //DODAJEMY SASIADÓW
            if (get<0>(plansza[z-cls]) != '@' && get<1>(plansza[z-cls]) != true){
                    get<1>(plansza[z-cls]) = true;
                    kolejkaFIFO.push(z-cls);
                    poprzednik.insert({z-cls, z});}
            if (get<0>(plansza[z - 1]) != '@' && get<1>(plansza[z-1]) != true){
                    get<1>(plansza[z-1]) = true;
                    kolejkaFIFO.push(z - 1);
                    poprzednik.insert({z-1, z});}
            if (get<0>(plansza[z + 1]) != '@' && get<1>(plansza[z+1]) != true){
                    get<1>(plansza[z+1]) = true;
                    kolejkaFIFO.push(z + 1);
                    poprzednik.insert({z+1, z});}
            if (get<0>(plansza[z + cls]) != '@' && get<1>(plansza[z+cls]) != true){
                    get<1>(plansza[z+cls]) = true;
                    kolejkaFIFO.push(z + cls);
                    poprzednik.insert({z+cls, z});}                }
    else if (kolejkaFIFO.size() == 0) return false;

    x = z_xy(kolejkaFIFO.front()).first;
    y = z_xy(kolejkaFIFO.front()).second;
    BFS(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////

bool Matrix::DFS (int x, int y){
algorithm_runs++;
    int z = xy_z(x, y);

    if (is_exit(z)){
        //odtwarzanie najkrotszej sciezki po poprzednikach '2'
        int start = xy_z(x_start, y_start);
        visited.push_back(z);
        while (z != start){
                visited.push_back(poprzednik.find(z)->second);
                z = visited.back();
                if(get<0>(plansza[z]) != '1')
                    get<0>(plansza[z]) = '-';}
        return true;}
    else if (!is_exit(z)) {
        if (get<0>(plansza[z]) != '1' && get<0>(plansza[z]) != '2')
            get<0>(plansza[z]) = 'x';
        if (!kolejkaLIFO.empty())
            kolejkaLIFO.pop_back();

        //DODAJEMY SASIADÓW
            if (get<0>(plansza[z-cls]) != '@' && get<1>(plansza[z-cls]) != true){
                    get<1>(plansza[z - cls]) = true;
                    kolejkaLIFO.push_back(z-cls);
                    poprzednik.insert({z-cls, z});}
            if (get<0>(plansza[z - 1]) != '@' && get<1>(plansza[z-1]) != true){
                    get<1>(plansza[z - 1]) = true;
                    kolejkaLIFO.push_back(z - 1);
                    poprzednik.insert({z-1, z});}
            if (get<0>(plansza[z + 1]) != '@' && get<1>(plansza[z+1]) != true){
                    get<1>(plansza[z + 1]) = true;
                    kolejkaLIFO.push_back(z + 1);
                    poprzednik.insert({z+1, z});}
            if (get<0>(plansza[z + cls]) != '@' && get<1>(plansza[z+cls]) != true){
                    get<1>(plansza[z + cls]) = true;
                    kolejkaLIFO.push_back(z + cls);
                    poprzednik.insert({z+cls, z});}}
    else if (kolejkaLIFO.size() == 0) return false;

    x = z_xy(kolejkaLIFO.back()).first;
    y = z_xy(kolejkaLIFO.back()).second;
    DFS(x, y);
}
////////////////////////////////////////////////////////////////////////////////////////

vector<pair<int, int>> kolejka_GBFS;

////////////////////////////////////////////////////////////////////////////////////////

int Matrix::distance(int a , int b){

int ax = a/cls;
int ay = a%cls;
int bx = b/cls;
int by = b%cls;

return sqrt((ax-bx)*(ax-bx))+sqrt((ay-by)*(ay-by));}

////////////////////////////////////////////////////////////////////////////////////////

bool Matrix::GBFS (int x, int y, int Fx, int Fy){ //ten algorytm WIE juz gdzie jest koniec F
algorithm_runs++;

    int z = xy_z(x, y);

    if (is_exit(z)){
        //odtwarzanie najkrotszej sciezki po poprzednikach '2'
        int start = xy_z(x_start, y_start);
        visited.push_back(z);
        while (z != start){
                visited.push_back(poprzednik.find(z)->second);
                z = visited.back();
                if(get<0>(plansza[z]) != '1')
                    get<0>(plansza[z]) = '-';}
        return true;}
    else if (!is_exit(z)) {
        if (get<0>(plansza[z]) != '1' && get<0>(plansza[z]) != '2')
            get<0>(plansza[z]) = 'x';
            if (!kolejka_GBFS.empty())
                kolejka_GBFS.pop_back();

        //DODAJEMY SASIADÓW
            if (get<0>(plansza[z-cls]) != '@' && get<0>(plansza[z-cls]) != '1' && get<1>(plansza[z-cls]) != true){
                    kolejka_GBFS.push_back(make_pair(distance(xy_z(Fx, Fy),z-cls), z-cls));
                    get<1>(plansza[z-cls]) = true;
                    poprzednik.insert({z-cls, z});}
            if (get<0>(plansza[z-1]) != '@' && get<0>(plansza[z-1]) != '1' && get<1>(plansza[z-1]) != true){
                    kolejka_GBFS.push_back(make_pair(distance(xy_z(Fx, Fy),z-1), z-1));
                    get<1>(plansza[z-1]) = true;
                    poprzednik.insert({z-1, z});}
            if (get<0>(plansza[z+1]) != '@' && get<0>(plansza[z+1]) != '1' && get<1>(plansza[z+1]) != true){
                    kolejka_GBFS.push_back(make_pair(distance(xy_z(Fx, Fy),z+1), z+1));
                    get<1>(plansza[z+1]) = true;
                    poprzednik.insert({z+1, z});}
            if (get<0>(plansza[z+cls]) != '@' && get<0>(plansza[z+cls]) != '1' && get<1>(plansza[z+cls]) != true){
                    kolejka_GBFS.push_back(make_pair(distance(xy_z(Fx, Fy),z+cls), z+cls));
                    get<1>(plansza[z+cls]) = true;
                    poprzednik.insert({z+cls, z});}}
    else if (kolejka_GBFS.size() == 0) return false;

    sort(kolejka_GBFS.begin(), kolejka_GBFS.end());
    reverse(kolejka_GBFS.begin(), kolejka_GBFS.end());

//    for (vector<tuple<int, int>>::iterator it = kolejka_GBFS.end() - 4; it != kolejka_GBFS.end(); it++)
//        cout << "pole nr: " << get<0>(kolejka_GBFS.back())<< " odleglosc: "<<get<1>(kolejka_GBFS.back()) << " ";

    x = z_xy(get<1>(kolejka_GBFS.back())).first;
    y = z_xy(get<1>(kolejka_GBFS.back())).second;

    GBFS(x, y, x_finish, y_finish);
}

////////////////////////////////////////////////////////////////////////////////////////

bool Matrix::first_path (int i, int j){
algorithm_runs++;
    if (get<0>(plansza[i * cls + j]) == '2'){
        get<0>(plansza[x_start * cls + y_start]) = '1';
        return true;}
    if (get<0>(plansza[i * cls + j]) != ' ' && get<0>(plansza[i * cls + j]) != '1')
        return false;
    get<0>(plansza[i * cls + j]) = '-';
    visited.push_back(i * cls + j);

    if (first_path(i-1, j) == true){
        return true;}
    if (first_path(i, j+1) == true){
        return true;}
    if (first_path(i+1, j) == true){
        return true;}
    if (first_path(i, j-1) == true){
        return true;}
    get<0>(plansza[i * cls + j]) = 'x';
    visited.pop_back();
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////

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
    int k = 0;
    for (int i = 0; i < x1; ++i){
        for (int j = 0; j < y1; ++j){
                plansza.get(znak);
                matrix1(k++) = false;
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
