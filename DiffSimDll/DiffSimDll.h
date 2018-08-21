#pragma once
#define DLLAPI __declspec(dllexport)

#include "Atom.h"
#include <vector>
#include <map>
#include <fstream>

namespace DiffSimDll
{
	extern "C"
	{
		typedef void(__stdcall *ReportProgressCallback)(int);
		typedef bool(__stdcall *CancellationPendingCallback)();

		struct DLLAPI WorkProgressInteropNegotiator
		{
			ReportProgressCallback progressCallback;
			CancellationPendingCallback cancellationPending;
			bool cancel;
		};

		DLLAPI int __stdcall StartDiffractionSimulation(WorkProgressInteropNegotiator& negotiator, const char*, const char*);
	}

	class Simulation
	{
	public:
		Simulation(WorkProgressInteropNegotiator& n, const std::string&, const std::string&);
		bool prepare();
		bool start();

	private:
		WorkProgressInteropNegotiator negotiator;
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