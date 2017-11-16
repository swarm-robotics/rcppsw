/**
 * @file base_grid2D.hpp
 *
 * @copyright 2017 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_DS_BASE_GRID2D_HPP_
#define INCLUDE_RCPPSW_DS_BASE_GRID2D_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <boost/multi_array.hpp>
#include "rcppsw/common/common.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, ds);
template<typename T>
using grid_type = typename boost::multi_array<T, 2>;
template<typename T>
using grid_view = typename grid_type<T>::template array_view<2>::type;
template<typename T>
using view_range = typename grid_type<T>::index_range;
using index_range = boost::multi_array_types::index_range;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class base_grid2D
 *
 * @brief A 2D logical grid that is overlayed over a continuous environment. It
 * discretizes the continuous arena into a grid of a specified resolution.
 */
template<typename T, typename ...Args>
class base_grid2D {
 public:
  base_grid2D(double resolution, size_t x_max, size_t y_max) :
      m_resolution(resolution),
      m_x_max(x_max),
      m_y_max(y_max) {}
  virtual ~base_grid2D(void) {}

  virtual const T& access(size_t i, size_t j) const = 0;
  virtual T& access(size_t i, size_t j) = 0;

  double resolution(void) const { return m_resolution; }

  /**
   * @brief Get the size of the X dimension of the discretized subgrid, at
   * whatever the resolution specified during object construction was.
   */
  size_t xsize(void) const {
    return static_cast<size_t>(std::ceil(m_x_max / m_resolution)); }

  /**
   * @brief Get the size of the Y dimension of the discretized subgrid, at
   * whatever the resolution specified during object construction was.
   */
  size_t ysize(void) const {
    return static_cast<size_t>(std::ceil(m_y_max / m_resolution)); }

 private:
  double m_resolution;
  size_t m_x_max;
  size_t m_y_max;
};

NS_END(ds, rcppsw);

#endif /* INCLUDE_RCPPSW_DS_BASE_GRID2D_HPP_ */