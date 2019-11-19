/**
 * \file sigmoid_parser.hpp
 *
 * \copyright 2018 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_MATH_CONFIG_XML_SIGMOID_PARSER_HPP_
#define INCLUDE_RCPPSW_MATH_CONFIG_XML_SIGMOID_PARSER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <memory>

#include "rcppsw/common/common.hpp"
#include "rcppsw/math/config/sigmoid_config.hpp"
#include "rcppsw/config/xml/xml_config_parser.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, math, config, xml);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \class sigmoid_parser
 * \ingroup math config xml
 *
 * \brief Parses XML configuration for sigmoids into \ref sigmoid_config.
 */
class sigmoid_parser : public rcppsw::config::xml::xml_config_parser {
 public:
  using config_type = sigmoid_config;

  /**
   * \brief The root tag that all task sigmoid configuration values should lie
   * under in the XML tree.
   */
  static constexpr char kXMLRoot[] = "sigmoid";

  void parse(const ticpp::Element& node) override RCSW_COLD;
  bool validate(void) const override RCSW_ATTR(pure, cold);
  RCSW_COLD std::string xml_root(void) const override { return kXMLRoot; }

 private:
  RCSW_COLD rcppsw::config::base_config* config_get_impl(void) const override {
    return m_config.get();
  }

  /* clang-format off */
  std::unique_ptr<sigmoid_config> m_config{nullptr};
  /* clang-format on */
};

NS_END(xml, config, math, rcppsw);

#endif /* INCLUDE_RCPPSW_MATH_CONFIG_XML_SIGMOID_PARSER_HPP_ */
