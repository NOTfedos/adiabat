#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>

#include <gmpxx.h>


using std::vector;
using std::rand;
using std::pow;
using std::cout;
using std::ofstream;
using std::endl;
using std::string;
using std::setprecision;
using std::fixed;


const unsigned long N = pow(10, 5);  // количество молекул
const unsigned int A = 2, B = 2, C = 1;  // габариты параллелепипеда
const unsigned int COUNT_LIMIT = pow(10, 2);  // количество итераций процесса
const mpf_class dt = pow(2.0, -6);
const mpf_class v_c = pow(10.0, -2) / dt;  // скорость расширения сосуда
const double M = pow(10, -10);


string output_filename = "data.txt";


class Body{
public:
    mpf_class x, y, z, vx, vy, vz;

    Body(){};

    Body(const double& a, const double& b, const double& c){
        x = a * (double) rand() / RAND_MAX;
        y = b * (double) rand() / RAND_MAX;
        z = c * (double) rand() / RAND_MAX;
        vx = (double) (1 - 2 * rand() / RAND_MAX);
        vy = (double) (1 - 2 * rand() / RAND_MAX);
        vy = (double) (1 - 2 * rand() / RAND_MAX);  // распределение может быть не Максвелловским (?)
    }

    // обновление координат
    void move(){
        this->x += vx * dt;
        this->y += vy * dt;
        this->z += vz * dt;
    }
};

class BoxContainer{
// параллелограм в начале координат
public:
    mpf_class a, b, c;

    BoxContainer(double a, double b, double c){
        this->a = a;  // по X координате
        this->b = b;  // по Y координате
        this->c = c;  // по Z координате
    }

    mpf_class get_volume() const{
        return a * b * c;
    }

    mpf_class get_square() const{
        return 2*(a*b + b*c + a*c);
    }

    mpf_class collide(Body& molecula) const{
        mpf_class force = 0;
        if ((molecula.x < 0) || (molecula.x > a)){
            molecula.vx = -1 * molecula.vx;
            force += 2 * abs(molecula.vx) * M / dt;
        }
        if ((molecula.y < 0) || (molecula.y > b)){
            molecula.vy = -1 * molecula.vy;
            force = 2 * abs(molecula.vy) * M / dt;
        }
        if ((molecula.z < 0) || (molecula.z > c)){
            molecula.vz = -1 * molecula.vz;
            force = 2 * abs(molecula.vz) * M / dt;
        }
        return force;
    }

};



template<class T> class MyVector{  // неизменяемый вектор
public:
    MyVector(unsigned int n=0){
        this->arr = new T[n];
        this->self_size = n;
    }

    void set_at(T& x, const unsigned int& i){
        if ((i < 0) || (i>=this->self_size)){
            return ;
        }
        this->arr[i] = x; 
    }

    Body& get_at(const unsigned int& i){
        return this->arr[i];
    }  

    T& operator[](int i){
        return this->get_at(i);
    }

    unsigned int size() const{
        return this->self_size;
    }

private:
    unsigned int self_size;
    T *arr;
};


MyVector<Body> spawn_molecules(){
    MyVector<Body> arr(N);
    for (int i=0; i < N; i++){
        arr[i] = Body(A, B, C);
    }
    return arr;
}


int main(){

    long int start = clock();
    srand(clock());

    ofstream outf;
    outf.open(output_filename);

    BoxContainer box = BoxContainer(A, B, C);
    MyVector<Body> arr = spawn_molecules();//

    long int counter = 0;
    mpf_class force = 0;  // изменение импульса
    mpf_class pressure = 0;  // давление


    while(counter < COUNT_LIMIT){

        pressure = 0.0;

        // проводим серию измерений давления
        for (int t = 0; t < 20; t++){
            force = 0.0;
            for (unsigned long i=0; i < N; i++){
                arr[i].move();
                force += box.collide( arr[i] );
            }

            outf << force / box.get_square() << " " <<  box.get_volume() << endl;  // вывод данных
        }


        // отладочный вывод
        cout << "Progress " << fixed << (double) counter / COUNT_LIMIT * 100 << setprecision(0) << "%" << endl;
        //cout << pressure << " " << box.get_volume() << endl;

        box.c += dt * v_c;
        counter++;
    }

    
    long int end = clock();
    cout << "Calc ended in " << (end - start) / CLOCKS_PER_SEC / 60 << " minutes" << endl;
    cout << "Output written into " << output_filename << endl;


    outf.close();

    return 0;
}