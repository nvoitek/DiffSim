#pragma once

#include "Atom.h"
#include <vector>
#include <map>
namespace DiffSimDll
{
	class Simulation
	{
	public:
		Simulation(const std::string&, const std::string&);
		bool prepare();
		bool start();

	private:
		bool isPrepared;
		double begin2theta;
		double end2theta;
		double step2theta;
		double lambda;
		double distanceFromDetector;
		double distanceFromSource;
		std::string input;
		std::string output;
		std::vector<Atom> atoms;
		std::map<double, double> results;
	};
}
