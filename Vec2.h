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
		T xy[VEC2_SIZE];
	public:
		/**
		 * default constructor.
		 * 
		 */
		Vec2() { xy[AT_X] = 0; xy[AT_Y] = 0; }

		/**
		 * 2º constructor.
		 * 
		 * @param x position.
		 * @param y position.
		 */
		Vec2(T x, T y) { xy[AT_X] = x; xy[AT_Y] = y; }

		/**
		 * @brief This method gets x position.
		 * \return x.
		 */
		T getX() const { return xy[AT_X]; }

		/**
		 * @brief This method gets y position.
		 * \return y.
		 */
		T getY() const { return xy[AT_Y]; }

		/**
		 * @brief This method sets x position.
		 * @param x.
		 */
		void setX(T x) { xy[AT_X] = x; }

		/**
		 * @brief This method sets y position.
		 * @param y.
		 */
		void setY(T y) { xy[AT_Y] = y; }

		/**
		 * @brief This method sets xy position.
		 * @param x.
		 * @param y.
		 */
		void setXY(T x, T y) { xy[AT_X] = x; xy[AT_Y] = y; }


		/**
		 * @brief This method gets position from index [0,1].
		 */
		T operator[](int index)
		{
			if (index < 0 || index > 1)
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return xy[index];
		}

		/**
		 * @brief This method sets position of index [0,1].
		 */
		T& operator()(int index)
		{
			if (index < 0 || index > 1)
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return xy[index];
		}

	};
}

