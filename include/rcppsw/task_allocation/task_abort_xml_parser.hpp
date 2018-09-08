/**
 * @file task_abort_parser.hpp
 *
 * @copyright 2018 John Harwell, All rights reserved.
 *
 * This file is part of FORDYCA.
 *
 * FORDYCA is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * FORDYCA is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * FORDYCA.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef INCLUDE_RCPPSW_TASK_ALLOCATION_TASK_ABORT_XML_PARSER_HPP_
#define INCLUDE_RCPPSW_TASK_ALLOCATION_TASK_ABORT_XML_PARSER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "rcppsw/common/common.hpp"
#include "rcppsw/params/xml_param_parser.hpp"
#include "rcppsw/task_allocation/abort_params.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class task_abort_xml_parser
 * @ingroup task_allocation
 *
 * @brief Parses XML parameters for relating to the subtask selection.
 */
class task_abort_xml_parser: public rcppsw::params::xml_param_parser,
                             public er::client<task_abort_xml_parser> {
 public:
  task_abort_xml_parser(const std::string& er_parent,
                        uint level)
      : xml_param_parser(er_parent, level),
        ER_CLIENT_INIT(er_parent) {}

  /**
   * @brief The root tag that all task task_abort parameters should lie
   * under in the XML tree.
   */
  static constexpr char kXMLRoot[] = "task_abort";

  void show(std::ostream& stream) const override;
  bool validate(void) const override;
  void parse(const ticpp::Element& node) override;

  std::string xml_root(void) const override { return kXMLRoot; }
  bool parsed(void) const override { return m_parsed; }

  std::shared_ptr<abort_params> parse_results(void) const {
    return m_params;
  }

 private:
  std::shared_ptr<rcppsw::params::base_params> parse_results_impl(void) const override {
    return m_params;
  }

  // clang-format off
  bool                          m_parsed{false};
  std::shared_ptr<abort_params> m_params{nullptr};
  // clang-format on
};

NS_END(task_allocation, rcppsw);

#endif /* INCLUDE_RCPPSW_TASK_ALLOCATION_TASK_ABORT_XML_PARSER_HPP_ */
