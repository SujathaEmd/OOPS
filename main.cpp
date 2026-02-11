#include <iostream>
#include <thread>
#include <atomic>

using namespace std;
atomic<int> count(0);

template <typename T>
class Array
{
    T arr[5];

public:
    void set(int index, T value)
    {
        if (index >= 0 && index < 5)
            arr[index] = value;
    }

    T get(int index)
    {
        if (index >= 0 && index < 5)
            return arr[index];
        return T();   // return default value
    }
};

void work(void)
{
    for(int cnt = 0; cnt < 1000; cnt++) 
	count++;
}

int main()
{
    Array<int> int_var;
    Array<float> f_var;
    thread task1(work);
    thread task2(work);

    // store values
    int_var.set(0, 10);
    int_var.set(1, 20);
    int_var.set(2, 30);
    int_var.set(3, 40);
    int_var.set(4, 50);

    // print values using get()
    cout << int_var.get(0) << endl;
    cout << int_var.get(1) << endl;
    cout << int_var.get(2) << endl;
    cout << int_var.get(3) << endl;
    cout << int_var.get(4) << endl;
    
    // store values
    f_var.set(0, 10.01);
    f_var.set(1, 20.02);
    f_var.set(2, 30.03);
    f_var.set(3, 40.04);
    f_var.set(4, 50.05);

    // print values using get()
    cout << f_var.get(0) << endl;
    cout << f_var.get(1) << endl;
    cout << f_var.get(2) << endl;
    cout << f_var.get(3) << endl;
    cout << f_var.get(4) << endl;
    
    task1.join();
    task2.join();
    cout << count;

    return 0;
}