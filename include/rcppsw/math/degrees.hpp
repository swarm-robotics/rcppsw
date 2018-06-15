/**
 * @file degrees.hpp
 *
 * @copyright 2018 John Harwell, All rights reserved.
 *
 * This file is part of RCPPSW.
 *
 * RCPPSW is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * RCPPSW is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * RCPPSW.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef INCLUDE_RCPPSW_MATH_DEGREES_HPP_
#define INCLUDE_RCPPSW_MATH_DEGREES_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cmath>
#include <limits>

#include "rcppsw/common/common.hpp"
#include "rcppsw/math/range.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, math);

class radians;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class degrees
 * @ingroup math
 *
 * @brief Used to store an angle value in degrees (duh).
 */
class degrees {
 public:
  static const float kDEGREES_TO_RADIANS;

  degrees(void) : m_value(0.0) {}

  /**
   * @brief Initialize from the specified value in DEGREES.
   */
  explicit degrees(float value) : m_value(value) {}
  explicit degrees(const radians& r);

  float value(void) const { return m_value; }
  void set(float value) { m_value = value; }

  float abs_value(void) const { return std::abs(m_value); }

  /**
   * @brief Normalizes the value in the range [-180, 180]
   */
  degrees& signed_normalize(void) {
    kSignedRange.wrap_value(*this);
    return (*this);
  }

  /**
   * @brief Normalizes the value in the range [0, 360]
   */
  degrees& unsigned_normalize(void) {
    kUnsignedRange.wrap_value(*this);
    return (*this);
  }

  degrees& operator+(void) {
    return *this;
  }

  degrees operator-(void) const {
    return degrees(-m_value);
  }

  degrees& operator+=(const degrees& other) {
    m_value += other.m_value;
    return *this;
  }

  degrees& operator+=(float value) {
    m_value += value;
    return *this;
  }

  degrees& operator-=(const degrees& other) {
    m_value -= other.m_value;
    return *this;
  }
  degrees& operator-=(float value) {
    m_value -= value;
    return *this;
  }
  degrees& operator*=(float value) {
    m_value *= value;
    return *this;
  }

  degrees& operator/=(float value) {
    m_value /= value;
    return *this;
  }

  degrees operator+(const degrees& other) const {
    degrees res(*this);
    res += other;
    return res;
  }

  degrees operator-(const degrees& other) const {
    degrees res(*this);
    res -= other;
    return res;
  }

  degrees operator*(float value) const {
    degrees res(*this);
    res *= value;
    return res;
  }

  float operator/(const degrees& other) const {
    return m_value / other.m_value;
  }

  degrees operator/(float value) const {
    degrees res(*this);
    res /= value;
    return res;
  }

  bool operator<(const degrees& other) const {
    return m_value < other.m_value;
  }

  bool operator<=(const degrees& other) const {
    return m_value <= other.m_value;
  }

  bool operator>(const degrees& other) const {
    return m_value > other.m_value;
  }

  bool operator>=(const degrees& other) const {
    return m_value >= other.m_value;
  }

  bool operator==(const degrees& other) const {
    return std::fabs(m_value - other.m_value) < std::numeric_limits<float>::epsilon();
  }

  bool operator!=(const degrees& other) const {
    return !(*this == other);
  }

 private:
  /**
   * @brief The signed normalization range [-180, 180]
   */
  static const range<degrees> kSignedRange;
  /**
   * @brief The unsigned normalization range [0, 360]
   */
  static const range<degrees> kUnsignedRange;

  float m_value;
};

/*******************************************************************************
 * Operators
 ******************************************************************************/
std::ostream& operator<<(std::ostream& stream, const degrees& d);

NS_END(math, rcppsw);

#endif /* INCLUDE_RCPPSW_MATH_DEGREES_HPP_ */
