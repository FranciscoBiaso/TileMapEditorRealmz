#pragma once
#include "Definitions.h"

namespace math {
	/*!
		Vec3 class
		==========

		This class represents a Vec3(x,y) strucutre and its basic operations.
	 */
	template <class T = int>
	class Vec3
	{
	private:
		T x, y, z;
	public:
		/**
		 * default constructor.
		 *
		 */
		Vec3() : x(0), y(0), z(0) {}

		/**
		 * 2º constructor.
		 *
		 * @param x position.
		 * @param y position.
		 */
		Vec3(T x, T y, T z) : x(x), y(y), z(z){}

		/**
		 * 3º copy constructor.
		 *
		 * @param v Vec3 to copy.
		 */
		Vec3(const Vec3& v) { x = v.getX(); y = v.getY(); z = v.getZ();}


		/**
		 * @brief This method gets x position.
		 * \return x.
		 */
		T getX() const { return x; }

		/**
		 * @brief This method gets y position.
		 * \return y.
		 */
		T getY() const { return y; }

		/**
		 * @brief This method gets y position.
		 * \return y.
		 */
		T getZ() const { return z; }

		/**
		 * @brief This method sets x position.
		 * @param x.
		 */
		void setX(T x) { this->x = x; }

		/**
		 * @brief This method sets y position.
		 * @param y.
		 */
		void setY(T y) { this->y = y; }

		/**
		 * @brief This method sets y position.
		 * @param y.
		 */
		void setZ(T z) { this->z = z; }

		/**
		 * @brief This method sets xy position.
		 * @param x.
		 * @param y.
		 */
		void setXYZ(T x, T y, T z) { this->x = x; this->y = y; this->z = z;}

		/**
		 * @brief This method gets position from index [0,1].
		 */
		T operator[](int index)
		{
			if (index < 0 || index > 1)
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return (index == 0) ? x : ((index == 1) ? y : z);
		}

		/**
		 * @brief This method checks if Vec3 is diferent from another Vec3.
		 * It compares x and y data.
		 */
		bool operator!=(const Vec3& v2)
		{
			return x != v2.getX() || y != v2.getY()  || z != v2.getZ() ? true : false;
		}

		/**
		 * @brief This method overloads the multiplication operator.
		 */
		Vec3& operator*(const T& mult)
		{
			this->x *= mult;
			this->y *= mult;
			this->z *= mult;
			return *this;
		}

		/**
		 * @brief This method overloads the assignment operator.
		 */
		Vec3& operator=(const Vec3& v2)
		{
			this->x = v2.getX();
			this->y = v2.getY();
			this->z = v2.getZ();
			return *this;
		}
	};
}

