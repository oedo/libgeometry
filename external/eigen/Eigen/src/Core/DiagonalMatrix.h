// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009 Gael Guennebaud <g.gael@free.fr>
// Copyright (C) 2007-2009 Benoit Jacob <jacob.benoit.1@gmail.com>
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

#ifndef EIGEN_DIAGONALMATRIX_H
#define EIGEN_DIAGONALMATRIX_H

#ifndef EIGEN_PARSED_BY_DOXYGEN
template<typename Derived>
class DiagonalBase : public AnyMatrixBase<Derived>
{
  public:
    typedef typename ei_traits<Derived>::DiagonalVectorType DiagonalVectorType;
    typedef typename DiagonalVectorType::Scalar Scalar;

    enum {
      RowsAtCompileTime = DiagonalVectorType::SizeAtCompileTime,
      ColsAtCompileTime = DiagonalVectorType::SizeAtCompileTime,
      MaxRowsAtCompileTime = DiagonalVectorType::MaxSizeAtCompileTime,
      MaxColsAtCompileTime = DiagonalVectorType::MaxSizeAtCompileTime,
      IsVectorAtCompileTime = 0,
      Flags = 0
    };

    typedef Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime, 0, MaxRowsAtCompileTime, MaxColsAtCompileTime> DenseMatrixType;

    inline const Derived& derived() const { return *static_cast<const Derived*>(this); }
    inline Derived& derived() { return *static_cast<Derived*>(this); }

    DenseMatrixType toDenseMatrix() const { return derived(); }
    template<typename DenseDerived>
    void evalTo(MatrixBase<DenseDerived> &other) const;
    template<typename DenseDerived>
    void addTo(MatrixBase<DenseDerived> &other) const
    { other.diagonal() += diagonal(); }
    template<typename DenseDerived>
    void subTo(MatrixBase<DenseDerived> &other) const
    { other.diagonal() -= diagonal(); }

    inline const DiagonalVectorType& diagonal() const { return derived().diagonal(); }
    inline DiagonalVectorType& diagonal() { return derived().diagonal(); }

    inline int rows() const { return diagonal().size(); }
    inline int cols() const { return diagonal().size(); }

    template<typename MatrixDerived>
    const DiagonalProduct<MatrixDerived, Derived, OnTheLeft>
    operator*(const MatrixBase<MatrixDerived> &matrix) const;

    inline const DiagonalWrapper<NestByValue<CwiseUnaryOp<ei_scalar_inverse_op<Scalar>, DiagonalVectorType> > >
    inverse() const
    {
      return diagonal().cwiseInverse().nestByValue();
    }
};

template<typename Derived>
template<typename DenseDerived>
void DiagonalBase<Derived>::evalTo(MatrixBase<DenseDerived> &other) const
{
  other.setZero();
  other.diagonal() = diagonal();
}
#endif

/** \class DiagonalMatrix
  *
  * \brief Represents a diagonal matrix with its storage
  *
  * \param _Scalar the type of coefficients
  * \param SizeAtCompileTime the dimension of the matrix, or Dynamic
  * \param MaxSizeAtCompileTime the dimension of the matrix, or Dynamic. This parameter is optional and defaults
  *        to SizeAtCompileTime. Most of the time, you do not need to specify it.
  *
  * \sa class DiagonalWrapper
  */
template<typename _Scalar, int SizeAtCompileTime, int MaxSizeAtCompileTime>
struct ei_traits<DiagonalMatrix<_Scalar,SizeAtCompileTime,MaxSizeAtCompileTime> >
 : ei_traits<Matrix<_Scalar,SizeAtCompileTime,SizeAtCompileTime,0,MaxSizeAtCompileTime,MaxSizeAtCompileTime> >
{
  typedef Matrix<_Scalar,SizeAtCompileTime,1,0,MaxSizeAtCompileTime,1> DiagonalVectorType;
};

template<typename _Scalar, int SizeAtCompileTime, int MaxSizeAtCompileTime>
class DiagonalMatrix
  : public DiagonalBase<DiagonalMatrix<_Scalar,SizeAtCompileTime,MaxSizeAtCompileTime> >
{
  public:
    #ifndef EIGEN_PARSED_BY_DOXYGEN
    typedef typename ei_traits<DiagonalMatrix>::DiagonalVectorType DiagonalVectorType;
    typedef const DiagonalMatrix& Nested;
    typedef _Scalar Scalar;
    #endif

  protected:

    DiagonalVectorType m_diagonal;

  public:

    /** const version of diagonal(). */
    inline const DiagonalVectorType& diagonal() const { return m_diagonal; }
    /** \returns a reference to the stored vector of diagonal coefficients. */
    inline DiagonalVectorType& diagonal() { return m_diagonal; }

    /** Default constructor without initialization */
    inline DiagonalMatrix() {}

    /** Constructs a diagonal matrix with given dimension  */
    inline DiagonalMatrix(int dim) : m_diagonal(dim) {}

    /** 2D constructor. */
    inline DiagonalMatrix(const Scalar& x, const Scalar& y) : m_diagonal(x,y) {}

    /** 3D constructor. */
    inline DiagonalMatrix(const Scalar& x, const Scalar& y, const Scalar& z) : m_diagonal(x,y,z) {}

    /** Copy constructor. */
    template<typename OtherDerived>
    inline DiagonalMatrix(const DiagonalBase<OtherDerived>& other) : m_diagonal(other.diagonal()) {}

    #ifndef EIGEN_PARSED_BY_DOXYGEN
    /** copy constructor. prevent a default copy constructor from hiding the other templated constructor */
    inline DiagonalMatrix(const DiagonalMatrix& other) : m_diagonal(other.diagonal()) {}
    #endif

    /** generic constructor from expression of the diagonal coefficients */
    template<typename OtherDerived>
    explicit inline DiagonalMatrix(const MatrixBase<OtherDerived>& other) : m_diagonal(other)
    {}

    /** Copy operator. */
    template<typename OtherDerived>
    DiagonalMatrix& operator=(const DiagonalBase<OtherDerived>& other)
    {
      m_diagonal = other.diagonal();
      return *this;
    }

    #ifndef EIGEN_PARSED_BY_DOXYGEN
    /** This is a special case of the templated operator=. Its purpose is to
      * prevent a default operator= from hiding the templated operator=.
      */
    DiagonalMatrix& operator=(const DiagonalMatrix& other)
    {
      m_diagonal = other.m_diagonal();
      return *this;
    }
    #endif

    /** Resizes to given size. */
    inline void resize(int size) { m_diagonal.resize(size); }
    /** Sets all coefficients to zero. */
    inline void setZero() { m_diagonal.setZero(); }
    /** Resizes and sets all coefficients to zero. */
    inline void setZero(int size) { m_diagonal.setZero(size); }
    /** Sets this matrix to be the identity matrix of the current size. */
    inline void setIdentity() { m_diagonal.setOnes(); }
    /** Sets this matrix to be the identity matrix of the given size. */
    inline void setIdentity(int size) { m_diagonal.setOnes(size); }
};

/** \class DiagonalWrapper
  *
  * \brief Expression of a diagonal matrix
  *
  * \param _DiagonalVectorType the type of the vector of diagonal coefficients
  *
  * This class is an expression of a diagonal matrix, but not storing its own vector of diagonal coefficients,
  * instead wrapping an existing vector expression. It is the return type of MatrixBase::asDiagonal()
  * and most of the time this is the only way that it is used.
  *
  * \sa class DiagonalMatrix, class DiagonalBase, MatrixBase::asDiagonal()
  */
template<typename _DiagonalVectorType>
struct ei_traits<DiagonalWrapper<_DiagonalVectorType> >
{
  typedef _DiagonalVectorType DiagonalVectorType;
  typedef typename DiagonalVectorType::Scalar Scalar;
  enum {
    RowsAtCompileTime = DiagonalVectorType::SizeAtCompileTime,
    ColsAtCompileTime = DiagonalVectorType::SizeAtCompileTime,
    MaxRowsAtCompileTime = DiagonalVectorType::SizeAtCompileTime,
    MaxColsAtCompileTime = DiagonalVectorType::SizeAtCompileTime,
    Flags = 0
  };
};

template<typename _DiagonalVectorType>
class DiagonalWrapper
  : public DiagonalBase<DiagonalWrapper<_DiagonalVectorType> >, ei_no_assignment_operator
{
  public:
    #ifndef EIGEN_PARSED_BY_DOXYGEN
    typedef _DiagonalVectorType DiagonalVectorType;
    typedef DiagonalWrapper Nested;
    #endif

    /** Constructor from expression of diagonal coefficients to wrap. */
    inline DiagonalWrapper(const DiagonalVectorType& diagonal) : m_diagonal(diagonal) {}

    /** \returns a const reference to the wrapped expression of diagonal coefficients. */
    const DiagonalVectorType& diagonal() const { return m_diagonal; }

  protected:
    const typename DiagonalVectorType::Nested m_diagonal;
};

/** \returns a pseudo-expression of a diagonal matrix with *this as vector of diagonal coefficients
  *
  * \only_for_vectors
  *
  * Example: \include MatrixBase_asDiagonal.cpp
  * Output: \verbinclude MatrixBase_asDiagonal.out
  *
  * \sa class DiagonalWrapper, class DiagonalMatrix, diagonal(), isDiagonal()
  **/
template<typename Derived>
inline const DiagonalWrapper<Derived>
MatrixBase<Derived>::asDiagonal() const
{
  return derived();
}

/** \returns true if *this is approximately equal to a diagonal matrix,
  *          within the precision given by \a prec.
  *
  * Example: \include MatrixBase_isDiagonal.cpp
  * Output: \verbinclude MatrixBase_isDiagonal.out
  *
  * \sa asDiagonal()
  */
template<typename Derived>
bool MatrixBase<Derived>::isDiagonal
(RealScalar prec) const
{
  if(cols() != rows()) return false;
  RealScalar maxAbsOnDiagonal = static_cast<RealScalar>(-1);
  for(int j = 0; j < cols(); ++j)
  {
    RealScalar absOnDiagonal = ei_abs(coeff(j,j));
    if(absOnDiagonal > maxAbsOnDiagonal) maxAbsOnDiagonal = absOnDiagonal;
  }
  for(int j = 0; j < cols(); ++j)
    for(int i = 0; i < j; ++i)
    {
      if(!ei_isMuchSmallerThan(coeff(i, j), maxAbsOnDiagonal, prec)) return false;
      if(!ei_isMuchSmallerThan(coeff(j, i), maxAbsOnDiagonal, prec)) return false;
    }
  return true;
}

#endif // EIGEN_DIAGONALMATRIX_H