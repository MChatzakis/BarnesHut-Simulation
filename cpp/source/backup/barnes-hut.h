#include <iostream>
#include <string>

using namespace std;

class PhysicalEntity
{
private:
    double X;
    double Y;
    double Vx;
    double Vy;
    double M;
    string S;

public:
    PhysicalEntity(double _X, double _Y, double _Vx, double _Vy, double _M, string _S) : X{_X}, Y{_Y}, Vx{_Vx}, Vy{_Vy}, M{_M}, S{_S} {}

    string printEntity()
    {
        string output;
        output += "[ ";
        output += to_string(X) + " " + to_string(Y) + " " + to_string(Vx) + " " + to_string(Vy) + " " + to_string(M) + " " + S;
        output += " ]";
        return output;
    }
};