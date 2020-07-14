#pragma once
#include "Definitions.h"
#include <ostream>
namespace math {
	/*!
		Vec2 class 
		==========

		This class represents a vec2(x,y) strucutre and its basic operations.
	 */
	template <class T=int>
	class Vec2
	{
	private:
		T x, y;
	public:
		/**
		 * default constructor.
		 * 
		 */
		Vec2() : x(0),y(0) {}

		/**
		 * 2º constructor.
		 *
		 * @param x position.
		 * @param y position.
		 */
		Vec2(T x, T y) : x(x), y(y) {}

		/**
		 * 3º copy constructor.
		 *
		 * @param v Vec2 to copy.
		 */
		Vec2(const Vec2& v) { x = v.getX(); y = v.getY(); }

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
		 * @brief This method sets x position.
		 * @param x.
		 */
		void setX(T x) { this->x  = x; }

		/**
		 * @brief This method sets y position.
		 * @param y.
		 */
		void setY(T y) { this->y = y; }

		/**
		 * @brief This method sets xy position.
		 * @param x.
		 * @param y.
		 */
		void setXY(T x, T y) { this->x = x; this->y = y;}

		/**
		 * @brief This method gets position from index [0,1].
		 */
		T operator[](int index)
		{
			if (index < 0 || index > 1)
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return (index == 0) ? x : y;
		}

		/**
		 * @brief This method checks if Vec2 is diferent from another Vec2.
		 * It compares x and y data.
		 */
		bool operator!=(const Vec2& v2)
		{
			return x != v2.getX() || y != v2.getY() ? true : false;
		}

		/**
		 * @brief This method overloads the multiplication operator.
		 */
		Vec2& operator*(const T& mult)
		{
			this->x *= mult;
			this->y *= mult;
			return *this;
		}

		/**
		 * @brief This method overloads the assignment operator.
		 */
		Vec2& operator=(const Vec2& v2)
		{
			this->x = v2.getX();
			this->y = v2.getY();
			return *this;
		}

		/**
		 * @brief This method overloads the - operator.
		 */
		Vec2& operator-(const Vec2& v2)
		{
			this->x -= v2.getX();
			this->y -= v2.getY();
			return *this;
		}

		/**
		 * @brief This method overloads the + operator.
		 */
		Vec2& operator+(const Vec2& v2)
		{
			this->x += v2.getX();
			this->y += v2.getY();
			return *this;
		}

		/**
		 * @brief This method increments the x and y position as a matrix of width mod.
		 */
		void rightShiftCursor(int mod)
		{
			if (getX() == (mod - 1))
			{
				setX(0);
				setY(getY() + 1);
			}
			else
				setX((getX() + 1) % mod);
		}

		/**
		 * @brief This method decrements the x and y position as a matrix of width mod.
		 */
		void leftShiftCursor(int mod)
		{
			if (getX() == 0)
			{
				if (getY() > 0)
				{
					setY(getY() - 1);
					setX(mod - 1);
				}
			}
			else
				setX(getX() - 1);
		}

		/**
		 * @brief This method executes n times rightShiftCursor.
		 * @see rightShiftCursor
		 */
		void rightShiftCursorNTimes(int x, int mod)
		{
			for (int i = 0; i < x; i++)
				rightShiftCursor(mod);
		}

		/**
		 * @brief This method executes n times leftShiftCursor.
		 * @see leftShiftCursor
		 */
		void leftShiftCursorNTimes(int x, int mod)
		{
			for (int i = 0; i < x; i++)
				leftShiftCursor(mod);
		}

		//friend std::ostream& operator<<(std::ostream& os, const Vec2& dt);
	};	
}
