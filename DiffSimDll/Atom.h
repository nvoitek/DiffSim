#pragma once

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ICoord.h"

namespace DiffSimDll
{
	class Atom : public ICoord
	{
	public:
		double getX() const;
		double getY() const;
		double getZ() const;
		void setX(const double &);
		void setY(const double &);
		void setZ(const double &);

		int getAtomNr() const;
		void setAtomNr(const int&);

		Atom();
		Atom(const int&, const double&, const double&, const double&);
		Atom(const double&, const double&, const double&);
		Atom(const Atom&);

		double distanceFrom(const Atom&) const;

		Atom rotateByXAxis(const double& angle) const;
		Atom rotateByYAxis(const double& angle) const;
		Atom rotateByZAxis(const double& angle) const;

		void readFrom(std::istream&);
		void printOn(std::ostream&) const;

	private:
		int atomNr;
		double x;
		double y;
		double z;
	};

	inline std::istream& operator >> (std::istream& in, Atom& a)
	{
		a.readFrom(in);
		return in;
	}

	inline std::ostream& operator << (std::ostream& out, const Atom& a)
	{
		a.printOn(out);
		return out;
	}
}
