// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_MISC_SOLVE_H
#define EIGEN_MISC_SOLVE_H

/** \class ei_solve_retval_base
  *
  */
template<typename DecompositionType, typename Rhs>
struct ei_traits<ei_solve_retval_base<DecompositionType, Rhs> >
{
  typedef typename DecompositionType::MatrixType MatrixType;
  typedef Matrix<typename Rhs::Scalar,
                 MatrixType::ColsAtCompileTime,
                 Rhs::ColsAtCompileTime,
                 Rhs::PlainMatrixType::Options,
                 MatrixType::MaxColsAtCompileTime,
                 Rhs::MaxColsAtCompileTime> ReturnMatrixType;
};

template<typename _DecompositionType, typename Rhs> struct ei_solve_retval_base
 : public ReturnByValue<ei_solve_retval_base<_DecompositionType, Rhs> >
{
  typedef typename ei_cleantype<typename Rhs::Nested>::type RhsNestedCleaned;
  typedef _DecompositionType DecompositionType;

  ei_solve_retval_base(const DecompositionType& dec, const Rhs& rhs)
    : m_dec(dec), m_rhs(rhs)
  {}

  inline int rows() const { return m_dec.cols(); }
  inline int cols() const { return m_rhs.cols(); }
  inline const DecompositionType& dec() const { return m_dec; }
  inline const RhsNestedCleaned& rhs() const { return m_rhs; }

  template<typename Dest> inline void evalTo(Dest& dst) const
  {
    static_cast<const ei_solve_retval<DecompositionType,Rhs>*>(this)->evalTo(dst);
  }

  protected:
    const DecompositionType& m_dec;
    const typename Rhs::Nested m_rhs;
};

#define EIGEN_MAKE_SOLVE_HELPERS(DecompositionType,Rhs) \
  typedef typename DecompositionType::MatrixType MatrixType; \
  typedef typename MatrixType::Scalar Scalar; \
  typedef typename MatrixType::RealScalar RealScalar; \
  typedef ei_solve_retval_base<DecompositionType,Rhs> Base; \
  using Base::dec; \
  using Base::rhs; \
  using Base::rows; \
  using Base::cols; \
  ei_solve_retval(const DecompositionType& dec, const Rhs& rhs) \
    : Base(dec, rhs) {}

#endif // EIGEN_MISC_SOLVE_H
