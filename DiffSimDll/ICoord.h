#pragma once
namespace DiffSimDll
{
	class ICoord
	{
	protected:
		double x;
		double y;
		double z;

	public:
		virtual double getX() const = 0;
		virtual double getY() const = 0;
		virtual double getZ() const = 0;

		virtual void setX(const double&) = 0;
		virtual void setY(const double&) = 0;
		virtual void setZ(const double&) = 0;
	};
}