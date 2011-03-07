/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __EIGEN_WRAPPERS_HPP__
#define __EIGEN_WRAPPERS_HPP__

#include "Eigen/Core"
#include "Eigen/Geometry"

// Define a set of lightweight wrappers around Eigen datatypes used inside omegalib
// wrappers use low-case names to avoid clashes with their Eigen base classes.
namespace omega { namespace math 
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	template< size_t M, typename T = float >
	class vector: public Eigen::Matrix<T, M, 1>
	{
	public:
		typedef Eigen::Matrix<T, M, 1> Base;

		vector(): Base() {}

		template< typename OtherDerived > 
		vector(const Eigen::EigenBase< OtherDerived > &other): Base(other) {}

		vector(const T &x, const T &y): Base(x, y) {}

		vector(const T &x, const T &y, const T &z): Base(x, y, z) {}

		template< typename OtherDerived, int ColsAtCompileTime >
		vector(const Eigen::RotationBase< OtherDerived, ColsAtCompileTime > &r): Base(r) {}

		template<typename OtherDerived >
		vector(const Eigen::ReturnByValue< OtherDerived > &other): Base(other) {}

		template<int N>
		vector(const Eigen::Matrix<T, M, N> &other): Base(other) {}

		template< typename OtherDerived >
		vector(const Eigen::MatrixBase< OtherDerived > &other): Base(other) {}

		vector(const T &x, const T &y, const T &z, const T &w): Base(x, y, z, w) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	template< size_t M, size_t N, typename T = float >
	class matrix: public Eigen::Matrix<T, M, N>
	{
	public:
		typedef Eigen::Matrix<T, M, N> Base;

		matrix(): Base() {}

		template< typename OtherDerived > 
		matrix(const Eigen::EigenBase< OtherDerived > &other): Base(other) {}

		//matrix(const T &x, const T &y): Matrix(x, y) {}

		//matrix(const T &x, const T &y, const T &z): Matrix (x, y, z) {}

		template< typename OtherDerived, size_t ColsAtCompileTime>
		matrix(const Eigen::RotationBase< OtherDerived, ColsAtCompileTime > &r): Base (r) {}

		template<typename OtherDerived >
		matrix(const Eigen::ReturnByValue< OtherDerived > &other): Base(other) {}

		matrix(const Base&other): Base (other) {}

		template< typename OtherDerived >
		matrix(const Eigen::MatrixBase< OtherDerived > &other): Base(other) {}

		matrix(const T &x, const T &y, const T &z, const T &w): Base (x, y, z, w) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	template< int Dim, typename T = float, int Mode = Eigen::Projective, int Options = 0 >
	class transform: public Eigen::Transform<T, Dim, Mode, Options>
	{
	public:
		typedef Eigen::Transform<T, Dim, Mode, Options> Base;

		transform(): Base() {}

		template<typename OtherDerived >
		transform(const Eigen::EigenBase< OtherDerived > &other): Base (other) {}

		template<typename OtherScalarType >
		transform (const Eigen::Transform< OtherScalarType, Dim, Mode, Options > &other): Base (other) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	template < typename Scalar >
	class quaternion : public Eigen::Quaternion<Scalar>
	{
	public:
		typedef Eigen::Quaternion<Scalar> Base;

		quaternion (const Scalar *data): Base(data) {}

		template<typename Derived >	
		quaternion (const Eigen::MatrixBase< Derived > &other): Base (other) {}

		quaternion (const typename Base::AngleAxisType &aa): Base (aa) {}

		quaternion (Scalar w, Scalar x, Scalar y, Scalar z): Base (w, x, y, z) {}

		quaternion (): Base() {}

		template<class Derived >
		quaternion (const Eigen::QuaternionBase< Derived > &other): Base (other) {}
	};
}; // namespace math
}; //namespace omega

#endif
