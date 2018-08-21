#include "Simulation.h"
#include "Atom.h"
#include<fstream>

DiffSimDll::Simulation::Simulation(const std::string& input, const std::string& output)
	: input(input), output(output), begin2theta(0), end2theta(0), step2theta(0), lambda(0), distanceFromDetector(0), distanceFromSource(0), isPrepared(false)
{
}

bool DiffSimDll::Simulation::prepare()
{
	std::ifstream in(this->input.c_str());

	try
	{
		in >> begin2theta >> end2theta >> step2theta >> lambda >> distanceFromDetector >> distanceFromSource;

		atoms.clear();
		Atom a;
		while (in >> a)
		{
			atoms.push_back(a);
		}

		isPrepared = true;
		std::cout << "Simulation parameters are:" << std::endl;
		std::cout << "2theta start: " << begin2theta << std::endl;
		std::cout << "2theta end: " << end2theta << std::endl;
		std::cout << "2theta step: " << step2theta << std::endl;
		std::cout << "lambda: " << lambda << std::endl;
		std::cout << "Distance from detector: " << distanceFromDetector << std::endl;
		std::cout << "Distance from source: " << distanceFromSource << std::endl;
	}
	catch(const std::exception& e)
	{
		isPrepared = false;
		std::cout << "Error while preparing simulation:" << std::endl << e.what() << std::endl;
	}

	if (atoms.empty())
		isPrepared = false;

	return isPrepared;
}

bool DiffSimDll::Simulation::start()
{
	if (!this->isPrepared)
		return false;

	results.clear();
	std::ofstream out(this->output.c_str());

	std::cout << std::endl << "Starting simulation:" << std::endl;

	for (double i = this->begin2theta; i < this->end2theta; i += this->step2theta)
	{
		std::cout << "Current 2theta is: " << i << " out of " << end2theta << std::endl;

		Atom source(
			-this->distanceFromSource * cos(M_PI * i / 360),
			0,
			this->distanceFromSource * sin(M_PI * i / 360)
		);

		Atom detector(
			this->distanceFromDetector * cos(M_PI * i / 360),
			0,
			this->distanceFromDetector * sin(M_PI * i / 360)
		);

		double intensity = 0;

		for (int rot1 = 0; rot1 < 180; ++rot1)
		{
			Atom sourceAfterRot1 = source.rotateByYAxis(-rot1);
			Atom detectorAfterRot1 = detector.rotateByYAxis(-rot1);

			for (int rot2 = 0; rot2 < 360; ++rot2)
			{
				Atom sourceAfterRot2 = sourceAfterRot1.rotateByZAxis(-rot2);
				Atom detectorAfterRot2 = detectorAfterRot1.rotateByZAxis(-rot2);

				double sumX = 0;
				double sumY = 0;

				for (unsigned n = 0; n < atoms.size(); ++n)
				{
					double distance = sourceAfterRot2.distanceFrom(atoms[n]) + detectorAfterRot2.distanceFrom(atoms[n]);

					sumX += atoms[n].getAtomNr() * cos(2 * M_PI * distance / this->lambda);
					sumY += atoms[n].getAtomNr() * sin(2 * M_PI * distance / this->lambda);
				}

				double sum = sumX * sumX + sumY * sumY;
				intensity = (intensity > sum) ? intensity : sum;
			}
		}

		results.insert(std::pair<double, double>(i, intensity));
		out << i << "   " << intensity << std::endl;
	}

	std::cout << "Simulation has completed" << std::endl;

	return true;
}
