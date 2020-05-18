/**
 * \file grid2D_overlay.hpp
 *
 * \copyright 2018 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_DS_GRID2D_OVERLAY_HPP_
#define INCLUDE_RCPPSW_DS_GRID2D_OVERLAY_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>

#include "rcppsw/ds/base_grid2D.hpp"
#include "rcppsw/er/client.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, ds);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \class grid2D_overlay
 * \ingroup ds
 *
 * \brief A 2D logical grid overlayed over a continuous environment using a
 * \a contiguous array of the template parameter type.
 *
 * \tparam T The type of the grid element (probably a cell of some kind). Must
 * have must have a zero parameter constructor available or it won't compile
 * (this is a limitation of the boost underneath). Furthermore, T must also have
 * a copy constructor available, as move semantics are not supported with the
 * underlying library.
 */
template <typename T>
class grid2D_overlay final : public base_grid2D<T>,
  er::client<grid2D_overlay<T>> {
 public:
  using typename base_grid2D<T>::index_range;
  using typename base_grid2D<T>::grid_view;
  using typename base_grid2D<T>::const_grid_view;
  using typename base_grid2D<T>::grid_type;

  using base_grid2D<T>::access;
  using base_grid2D<T>::xdsize;
  using base_grid2D<T>::ydsize;

  /**
   * \param resolution The discretization unit for the grid.
   * \param dims The real size in X,Y which will be discretized into
   *             X/resolution discrete elements along the X dimension and
   *             Y/resolution discrete elements along the Y dimension.
   */
  grid2D_overlay(const math::vector2d& dims,
                 const types::discretize_ratio& resolution)
      : ER_CLIENT_INIT("rcppsw.ds.grid2D_overlay"),
        mc_resolution(resolution),
        mc_dim(dims),
        m_cells(boost::extents[static_cast<typename index_range::index>(xdsize())]
                [typename index_range::index(ydsize())]) {
    double modx = std::fmod(mc_dim.x(), mc_resolution.v());
    double mody = std::fmod(mc_dim.y(), mc_resolution.v());

    /*
     * Some values of dimensions and grid resolution might not be able to be
     * represented exactly, so we can't just assert that the mod result =
     * 0.0. Instead, we verify that IF the mod result is > 0.0 that it is also
     * VERY close to the grid resolution.
     */
    if (modx >= std::numeric_limits<double>::epsilon()) {
      ER_ASSERT(std::fabs(mc_resolution.v() - modx) <= 1.0 / ONEE9,
                "X dimension (%f) not an even multiple of resolution (%f)",
                mc_dim.x(),
                mc_resolution.v());
    }
    if (mody >= std::numeric_limits<double>::epsilon()) {
      ER_ASSERT(std::fabs(mc_resolution.v() - mody) <= 1.0 / ONEE9,
                "Y dimension (%f) not an even multiple of resolution (%f)",
                mc_dim.y(),
                mc_resolution.v());
    }
  }

  /**
   * \brief Return the resolution of the grid.
   */
  const types::discretize_ratio& resolution(void) const { return mc_resolution; }

  /**
   * \brief Get a reference to a the cell within the grid at coordinates (i, j)
   *
   * \return Reference to the cell, of type T.
   */
  T& access(size_t i, size_t j) override {
    return m_cells[static_cast<typename index_range::index>(i)]
                  [static_cast<typename index_range::index>(j)];
  }

  size_t xdsize(void) const override {
    return static_cast<size_t>(std::ceil(mc_dim.x() / mc_resolution.v()));
  }

  size_t ydsize(void) const override {
    return static_cast<size_t>(std::ceil(mc_dim.y() / mc_resolution.v()));
  }

  /**
   * \brief Get the size of the X dimension (non-discretized).
   */
  double xrsize(void) const { return mc_dim.x(); }

  /**
   * \brief Get the size of the Y dimension (non-discretized).
   */
  double yrsize(void) const { return mc_dim.y(); }

  const math::vector2d& dimsr(void) const { return mc_dim; }
  math::vector2z dimsd(void) const {
    return math::dvec2zvec(mc_dim, mc_resolution.v());
  }

 private:
  grid_type& grid(void) override { return m_cells; }
  const grid_type& grid(void) const override { return m_cells; }

  /* clang-format off */
  const types::discretize_ratio mc_resolution;
  const math::vector2d          mc_dim;

  grid_type                     m_cells;
  /* clang-format on */
};

NS_END(ds, rcppsw);

#endif /* INCLUDE_RCPPSW_DS_GRID2D_OVERLAY_HPP_ */