
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"

#include "SensorCircuit.h" // 1. include the source file of your model

/*

  Example circuit (Sensor Circuit):

     (1)     .---------------.    (2)             
       .-----| nodei   nodej |-----.
      +|     |               |     |   
  vin (~)    | SensorCircuit |     <  Rout  
       |     '---------------'     |
       |             | nodek       |
       '-------------+-------------' 
	                -+-
				     '
                    (0)        

*/

int main()
{
	const double h = 1e-2;
	const double tmax = 10.0;

	const double Va = 10.0;
	const double f = 1.0;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("vin (V)", "vout (V)");

	Simulator simulator(2, 0); // nnodes, nsignals
	
	// netlist:
	VoltageSource Vin(1, 0, 0.0, Va, f);
	SensorCircuit sensor(1, 2, 0, 10.0, 0.1, 0.1, 0.1);
	Resistor Rout(2, 0, 10.0);

	// add devices:
	simulator.AddDevice(Vin);
	simulator.AddDevice(sensor);
	simulator.AddDevice(Rout);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{
		plotter.AddRow(simulator.GetTime(), Vin.GetVoltage(), Rout.GetVoltage());
		simulator.Step();
	}

	plotter.Plot();

	return 0;
}