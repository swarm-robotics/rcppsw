/**
 * \file grid3D_overlay.hpp
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

#ifndef INCLUDE_RCPPSW_DS_GRID3D_OVERLAY_HPP_
#define INCLUDE_RCPPSW_DS_GRID3D_OVERLAY_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <limits>

#include "rcppsw/ds/base_grid3D.hpp"
#include "rcppsw/ds/grid_overlay.hpp"
#include "rcppsw/er/client.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, ds);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \class grid3D_overlay
 * \ingroup ds
 *
 * \brief A 3D logical grid overlayed over a continuous environment using a
 * \a contiguous array of the template parameter type (\see \ref grid_overlay).
 *
 * \tparam T The type of the grid element (probably a cell of some kind). Must
 * have must have a zero parameter constructor available or it won't compile
 * (this is a limitation of the boost underneath). Furthermore, T must also have
 * a copy constructor available, as move semantics are not supported with the
 * underlying library.
 */
template <typename T>
class grid3D_overlay : public base_grid3D<T>,
                       public grid_overlay<math::vector3d>,
                       public er::client<grid3D_overlay<T>> {
 public:
  using typename base_grid3D<T>::index_range;
  using typename base_grid3D<T>::grid_view;
  using typename base_grid3D<T>::const_grid_view;
  using typename base_grid3D<T>::grid_type;

  using base_grid3D<T>::access;
  using base_grid3D<T>::xdsize;
  using base_grid3D<T>::ydsize;
  using base_grid3D<T>::zdsize;

  using grid_overlay<math::vector3d>::resolution;
  using grid_overlay<math::vector3d>::originr;
  using grid_overlay<math::vector3d>::origind;

  grid3D_overlay(const math::vector3d& origin,
                 const math::vector3d& dim,
                 const types::discretize_ratio& grid_res,
                 const types::discretize_ratio& field_res)
      : grid_overlay(origin, grid_res, field_res),
        ER_CLIENT_INIT("rcppsw.ds.grid3D_overlay"),
        mc_dim(dim),
        m_cells(boost::extents[static_cast<typename index_range::index>(xdsize())]
                              [typename index_range::index(ydsize())]
                              [typename index_range::index(zdsize())]) {
    double remx = std::remainder(mc_dim.x(), resolution().v());
    double remy = std::remainder(mc_dim.y(), resolution().v());
    double remz = std::remainder(mc_dim.z(), resolution().v());

    /*
     * Some values of dimensions and grid resolution might not be able to be
     * represented exactly, so we can't just assert that the mod result =
     * 0.0. Instead, we verify that IF the mod result is > 0.0 that it is also
     * VERY close to the grid resolution.
     */
    ER_ASSERT(remx <= 1.0 / ONEE9,
              "X dimension (%f) not an even multiple of resolution (%f)",
              mc_dim.x(),
              resolution().v());
    ER_ASSERT(remy <= 1.0 / ONEE9,
              "Y dimension (%f) not an even multiple of resolution (%f)",
              mc_dim.y(),
              resolution().v());
    ER_ASSERT(remz <= 1.0 / ONEE9,
              "Z dimension (%f) not an even multiple of resolution (%f)",
              mc_dim.z(),
              resolution().v());
  }

  virtual ~grid3D_overlay(void) = default;

  size_t xdsize(void) const override {
    return static_cast<size_t>(mc_dim.x() / resolution().v());
  }

  size_t ydsize(void) const override {
    return static_cast<size_t>(mc_dim.y() / resolution().v());
  }

  size_t zdsize(void) const override {
    return static_cast<size_t>(mc_dim.z() / resolution().v());
  }

  /**
   * \brief Get the size of the X dimension (non-discretized).
   */
  double xrsize(void) const { return mc_dim.x(); }

  /**
   * \brief Get the size of the Y dimension (non-discretized).
   */
  double yrsize(void) const { return mc_dim.y(); }

  /**
   * \brief Get the size of the Y dimension (non-discretized).
   */
  double zrsize(void) const { return mc_dim.z(); }

  /**
   * \brief Get the real dimensions of the grid; that is, continuous floating
   * point.
   */

  const math::vector3d& dimsr(void) const { return mc_dim; }

  /**
   * \brief Get the discrete dimensions of the grid.
   */
  math::vector3z dimsd(void) const {
    return math::dvec2zvec(mc_dim, resolution().v());
  }

  T& access(size_t i, size_t j, size_t k) override {
    return m_cells[static_cast<typename index_range::index>(i)]
                  [static_cast<typename index_range::index>(j)]
                  [static_cast<typename index_range::index>(k)];
  }

 private:
  grid_type& grid(void) override { return m_cells; }
  const grid_type& grid(void) const override { return m_cells; }

  /* clang-format off */
  const math::vector3d          mc_dim;

  grid_type                     m_cells;
  /* clang-format on */
};

NS_END(ds, rcppsw);

#endif /* INCLUDE_RCPPSW_DS_GRID3D_OVERLAY_HPP_ */
