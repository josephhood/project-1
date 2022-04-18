

#include "Simulator.h"

class SensorCircuit : public Device
{
public:

    // Constructor:

    SensorCircuit(int nodei, int nodej, int nodek,
        double C1, double L1, double R1, double R2);

    // Device interface:

    void Step(double t, double dt);
    void Init();

    // Viewable functions:

    double GetInputCurrent();

    // Member variables:

    int nodei;
    int nodej;
    int nodek;
    int int1;
    int int2;

    double C1;
    double L1;
    double R1;
    double R2;

    double g1;
    double g2;
};

SensorCircuit::SensorCircuit(int nodei, int nodej, int nodek,
    double C1, double L1, double R1, double R2)
{
    this->nodei = nodei;
    this->nodej = nodej;
    this->nodek = nodek;

    this->C1 = C1;
    this->L1 = L1;
    this->R1 = R1;
    this->R2 = R2;
}

void SensorCircuit::Init()
{
    int1 = GetNextNode();
    int2 = GetNextNode();

    g1 = 1 / R1;
    g2 = 1 / R2;
}

void SensorCircuit::Step(double t, double h)
{   
    /*
    
                   L1    (int2)   R2
     (nodei) o-----UUU-----o-----VVV-----.-------o (nodej)
                    o      |             |
                 (int1)    > R1         === C1
                           |             |
                           +-------------'
                           |
                           o
                        (nodek)

    */

    // R1:
    AddJacobian(nodek, nodek,  g1);
    AddJacobian(nodek, int2,  -g1);
    AddJacobian(int2,  nodek, -g1);
    AddJacobian(int2,  int2,   g1);

    // R2:
    AddJacobian(nodej, nodej,  g2);
    AddJacobian(nodej, int2,  -g2);
    AddJacobian(int2, nodej,  -g2);
    AddJacobian(int2, int2,    g2);

    // C1:
    AddJacobian(nodej, nodej,  C1 / h);
    AddJacobian(nodej, nodek, -C1 / h);
    AddJacobian(nodek, nodej, -C1 / h);
    AddJacobian(nodek, nodek,  C1 / h);

    // L1:
    AddJacobian(nodei, int1, 1);
    AddJacobian(int1, int2, -1);
    AddJacobian(int1, nodei, 1);
    AddJacobian(int2, int1, -1);
    AddJacobian(int2, int2, -L1 / h);

    AddBEquivalent(nodej, C1 / h * GetStateDifference(nodej, nodek));
    AddBEquivalent(nodek, -C1 / h * GetStateDifference(nodej, nodek));  
    AddBEquivalent(int2, -L1 / h * GetState(int2));
}

double SensorCircuit::GetInputCurrent()
{
    // current = g * v - b:
    //return C / GetTimeStep() * GetVoltage() - GetBEquivalent(nodei);
    return 0.0;
}
