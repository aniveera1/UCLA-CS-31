// Code for Project 1
// Report poll results

#include <iostream>
using namespace std;   // pp. 38-39 in Savitch 6/e explains this line

int main()
{
    int numberSurveyed;
    int forHillary;
    int forDonald;
    
    cout << "How many registered voters were surveyed? ";
    cin >> numberSurveyed;
    cout << "How many of them say they will vote for Hillary? ";
    cin >> forHillary;
    cout << "How many of them say they will vote for Donald? ";
    cin >> forDonald;
    
    double pctHillary = 100.0 * forHillary / numberSurveyed;
    double pctDonald = 100.0 * forDonald / numberSurveyed;
    
    cout.setf(ios::fixed);       // see pp. 32-33 in Savitch 6/e
    cout.precision(1);
    
    cout << endl;
    cout << pctHillary << "% say they will vote for Hillary." << endl;
    cout << pctDonald << "% say they will vote for Donald." << endl;
    
    if (forHillary > forDonald)
        cout << "Hillary is predicted to win the election." << endl;
    else
        cout << "Donald is predicted to win the election." << endl;
}
