#pragma once

#include "Standard.hpp"
#include <type_traits>
#include <ratio>
#include "Math.hpp"

/**
 * The unit system in the lwirth-lib is based entirely on the International System of Units (SI, Wikipedia: https://en.wikipedia.org/wiki/International_System_of_Units)
 * It tries to mimic it as closely as possible.
 *
 * The type of the value member is always the base-SI-unit. For example: Length::m_value represents meters.
 */

namespace lw
{
	/**
	 * L = LengthDim; M = MassDim; T = TimeDim; I = ElectricCurrentDim; Θ = TemperaturDim; N = ChemicalAmountDim; J = LuminousIntensity; A = AngleDim
	 */
	template<class L, class M, class T, class I, class Θ, class N, class J, class A>
	class Unit
	{
	private:
		f32 m_value; //always in base-unit

	public:
		constexpr Unit()
			: m_value(0.f)
		{}

		explicit constexpr Unit(f32 value)
			: m_value(value)
		{}

		constexpr Unit& operator+=(const Unit& rhs)
		{
			m_value += rhs.m_value;
			return *this;
		}

		constexpr Unit& operator-=(const Unit& rhs)
		{
			m_value -= rhs.m_value;
			return *this;
		}
	};

	

#define DEFINE_UNIT(L, M, T, I, Θ, N, J, A, name)\
	using name = Unit<std::ratio<L>, std::ratio<M>, std::ratio<T>, std::ratio<I>, std::ratio<Θ>, std::ratio<N>, std::ratio<J>, std::ratio<A>>;
	
#define DEFINE_UNIT_LITERAL(UnitType, litIdentifier, exponent)\
	constexpr UnitType operator"" _##litIdentifier(f128 value) { return UnitType(static_cast<f32>(value * lw::pow(10, exponent))); } \
	constexpr UnitType operator"" _##litIdentifier(u64 value) { return UnitType(static_cast<f32>(value * lw::pow(10, exponent))); }

	DEFINE_UNIT(0, 0, 0, 0, 0, 0, 0, 0, Number);

	DEFINE_UNIT(1, 0, 0, 0, 0, 0, 0, 0, Length);
	DEFINE_UNIT_LITERAL(Length, µm, -6);
	DEFINE_UNIT_LITERAL(Length, mm, -3);
	DEFINE_UNIT_LITERAL(Length, cm, -2);
	DEFINE_UNIT_LITERAL(Length, dm, -1);
	DEFINE_UNIT_LITERAL(Length, m, 0);
	DEFINE_UNIT_LITERAL(Length, km, 3);
	DEFINE_UNIT(2, 0, 0, 0, 0, 0, 0, 0, Area);
	DEFINE_UNIT_LITERAL(Area, mm2, -6);
	DEFINE_UNIT_LITERAL(Area, cm2, -4);
	DEFINE_UNIT_LITERAL(Area, dm2, -2);
	DEFINE_UNIT_LITERAL(Area, m2, 0);
	DEFINE_UNIT_LITERAL(Area, a, 2);
	DEFINE_UNIT_LITERAL(Area, ha, 4);
	DEFINE_UNIT_LITERAL(Area, km2, 6);
	DEFINE_UNIT(3, 0, 0, 0, 0, 0, 0, 0, Volume);
	DEFINE_UNIT(4, 0, 0, 0, 0, 0, 0, 0, HyperVolume);

	DEFINE_UNIT(0, 1, 0, 0, 0, 0, 0, 0, Mass);
	DEFINE_UNIT(-3, 1, 0, 0, 0, 0, 0, 0, Density);

	DEFINE_UNIT(0, 0, 1, 0, 0, 0, 0, 0, Time);
	DEFINE_UNIT(0, 0, -1, 0, 0, 0, 0, 0, Frequency);
	DEFINE_UNIT(1, 0, -1, 0, 0, 0, 0, 0, Speed);
	DEFINE_UNIT(1, 0, -2, 0, 0, 0, 0, 0, Acceleration);
	DEFINE_UNIT(1, 0, -3, 0, 0, 0, 0, 0, Jerk);
	DEFINE_UNIT(1, 0, -4, 0, 0, 0, 0, 0, Snap);
	DEFINE_UNIT(1, 0, -5, 0, 0, 0, 0, 0, Crackle);
	DEFINE_UNIT(1, 0, -6, 0, 0, 0, 0, 0, Pop);
	DEFINE_UNIT(1, 1, -2, 0, 0, 0, 0, 0, Force);
	DEFINE_UNIT(1, 1, -1, 0, 0, 0, 0, 0, Momentum)
	DEFINE_UNIT(-1, 1, -2, 0, 0, 0, 0, 0, Pressure);
	DEFINE_UNIT(2, 1, -2, 0, 0, 0, 0, 0, Energy);
	DEFINE_UNIT(2, 1, -3, 0, 0, 0, 0, 0, Power);

	DEFINE_UNIT(0, 0, 0, 1, 0, 0, 0, 0, ElectricCurrent);
	DEFINE_UNIT(0, 0, 1, 1, 0, 0, 0, 0, ElectricCharge);
	DEFINE_UNIT(1, 1, -3, -1, 0, 0, 0, 0, ElectricFieldStrength);
	DEFINE_UNIT(2, 1, 3, -1, 0, 0, 0, 0, ElectricPotential);
	DEFINE_UNIT(2, 1, -3, -2, 0, 0, 0, 0, ElectricalResistance);

	DEFINE_UNIT(0, 0, 0, 0, 1, 0, 0, 0, Temperature);
	DEFINE_UNIT(2, 1, -2, 0, -1, 0, 0, 0, Entropy);

	DEFINE_UNIT(0, 0, 0, 0, 0, 1, 0, 0, ChemicalAmount);

	DEFINE_UNIT(0, 0, 0, 0, 0, 0, 1, 0, LuminousIntensity);

	DEFINE_UNIT(0, 0, 0, 0, 0, 0, 0, 1, Angle);
	DEFINE_UNIT(0, 0, 0, 0, 0, 0, 0, 2, SolidAngle);
	DEFINE_UNIT(0, 0, -1, 0, 0, 0, 0, 1, AngularSpeed);
	DEFINE_UNIT(0, 0, -2, 0, 0, 0, 0, 1, AngularAcceleration);
	DEFINE_UNIT(0, 0, -3, 0, 0, 0, 0, 1, AngularJerk);
	DEFINE_UNIT(0, 0, -4, 0, 0, 0, 0, 1, AngularSnap);
	DEFINE_UNIT(0, 0, -5, 0, 0, 0, 0, 1, AngularCrackle);
	DEFINE_UNIT(0, 0, -6, 0, 0, 0, 0, 1, AngularPop);
	

	
}

