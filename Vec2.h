#pragma once
#include "Definitions.h"
#include <initializer_list>
#define VEC2_SIZE 2
#define AT_X 0
#define AT_Y 1

namespace math {
	/*!
		VECTOR 2D CLASS
		===============

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
		void setY(T y) { this->y = x; }

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
			return index ? y : x;
		}

		/**
		 * @brief This method sets position of index [0,1].
		 */
		/*
		T & operator()(int index)
		{
			if (index < 0 || index > 1)
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return index ? y : x;
		}
		*/

		Vec2& operator=(const Vec2& v2)
		{
			this->x = v2.getX();
			this->y = v2.getY();
			return *this;
		}

	};
}

