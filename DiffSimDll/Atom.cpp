#include "Atom.h"

DiffSimDll::Atom::Atom(const int &atomNr, const double &x, const double &y, const double &z)
{
	this->atomNr = atomNr;
	this->x = x;
	this->y = y;
	this->z = z;
}

DiffSimDll::Atom::Atom()
	: Atom(0, 0.0, 0.0, 0.0)
{
}

DiffSimDll::Atom::Atom(const double &x, const double &y, const double &z)
	: Atom(0,x,y,z)
{
}

DiffSimDll::Atom::Atom(const Atom& a)
	: Atom(a.getAtomNr(), a.getX(), a.getY(), a.getZ())
{
}

double DiffSimDll::Atom::getX() const
{
	return x;
}

double DiffSimDll::Atom::getY() const
{
	return y;
}

double DiffSimDll::Atom::getZ() const
{
	return z;
}

int DiffSimDll::Atom::getAtomNr() const
{
	return atomNr;
}

void DiffSimDll::Atom::setX(const double &x)
{
	this->x = x;
}

void DiffSimDll::Atom::setY(const double &y)
{
	this->y = y;
}

void DiffSimDll::Atom::setZ(const double &z)
{
	this->z = z;
}

void DiffSimDll::Atom::setAtomNr(const int &atomNr)
{
	this->atomNr = atomNr;
}

double DiffSimDll::Atom::distanceFrom(const Atom& a) const
{
	return sqrt(
		(this->x - a.getX()) * (this->x - a.getX()) +
		(this->y - a.getY()) * (this->y - a.getY()) +
		(this->z - a.getZ()) * (this->z - a.getZ())
	);
}

DiffSimDll::Atom DiffSimDll::Atom::rotateByXAxis(const double& angle) const
{
	Atom atom(this->atomNr, this->x, this->y, this->z);

	atom.setY(this->y * cos(M_PI * angle / 180) + this->z * sin(M_PI * angle / 180));
	atom.setZ(-this->y * sin(M_PI * angle / 180) + this->z * cos(M_PI * angle / 180));

	return atom;
}

DiffSimDll::Atom DiffSimDll::Atom::rotateByYAxis(const double& angle) const
{
	Atom atom(this->atomNr, this->x, this->y, this->z);

	atom.setZ(this->z * cos(M_PI * angle / 180) + this->x * sin(M_PI * angle / 180));
	atom.setX(-this->z * sin(M_PI * angle / 180) + this->x * cos(M_PI * angle / 180));

	return atom;
}

DiffSimDll::Atom DiffSimDll::Atom::rotateByZAxis(const double& angle) const
{
	Atom atom(this->atomNr, this->x, this->y, this->z);

	atom.setX(this->x * cos(M_PI * angle / 180) + this->y * sin(M_PI * angle / 180));
	atom.setY(-this->x * sin(M_PI * angle / 180) + this->y * cos(M_PI * angle / 180));

	return atom;
}

void DiffSimDll::Atom::readFrom(std::istream& in)
{
	int atomNr;
	double x, y, z;
	in >> atomNr >> x >> y >> z;
	this->atomNr = atomNr;
	this->x = x;
	this->y = y;
	this->z = z;
}

void DiffSimDll::Atom::printOn(std::ostream& out) const
{
	out << this->getAtomNr() << "   " << this->getX() << "   " << this->getY() << "   " << this->getZ() << std::endl;
}
