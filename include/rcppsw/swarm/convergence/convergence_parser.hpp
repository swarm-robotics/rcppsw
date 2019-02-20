/**
 * @file convergence_parser.hpp
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

#ifndef INCLUDE_RCPPSW_SWARM_CONVERGENCE_CONVERGENCE_PARSER_HPP_
#define INCLUDE_RCPPSW_SWARM_CONVERGENCE_CONVERGENCE_PARSER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "rcppsw/swarm/convergence/convergence_params.hpp"
#include "rcppsw/common/common.hpp"
#include "rcppsw/params/xml_param_parser.hpp"
#include "rcppsw/swarm/convergence/positional_entropy_parser.hpp"
#include "rcppsw/swarm/convergence/interactivity_parser.hpp"
#include "rcppsw/swarm/convergence/angular_order_parser.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, swarm, convergence);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class convergence_parser
 * @ingroup params
 *
 * @brief Parses XML parameters related to calculating swarm convergence into
 * \ref convergence_params.
 */
class convergence_parser : public rcppsw::params::xml_param_parser {
 public:
  explicit convergence_parser(uint level)
      : xml_param_parser(level),
        m_params(std::make_shared<std::remove_reference<decltype(*m_params)>::type>()),
        m_pos_entropy(level + 1),
        m_interactivity(level + 1),
        m_ang_order(level + 1) {}
  ~convergence_parser(void) override = default;

  /**
   * @brief The root tag that all loop functions relating to convergence parameters
   * should lie under in the XML tree.
   */
  static constexpr char kXMLRoot[] = "convergence";

  bool validate(void) const override;
  void parse(const ticpp::Element& node) override;

  std::string xml_root(void) const override { return kXMLRoot; }

  std::shared_ptr<convergence_params> parse_results(void) const { return m_params; }

 private:
  std::shared_ptr<rcppsw::params::base_params> parse_results_impl(
      void) const override {
    return m_params;
  }

  /* clang-format off */
  std::shared_ptr<convergence_params> m_params;
  positional_entropy_parser           m_pos_entropy;
  interactivity_parser                m_interactivity;
  angular_order_parser                m_ang_order;
  /* clang-format on */
};

NS_END(convergence, swarm, rcppsw);

#endif /* INCLUDE_RCPPSW_SWARM_CONVERGENCE_CONVERGENCE_PARSER_HPP_ */