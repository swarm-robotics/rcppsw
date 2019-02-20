/**
 * @file sigmoid_sel_parser.cpp
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/task_allocation/sigmoid_sel_xml_parser.hpp"
#include <ext/ticpp/ticpp.h>

#include "rcppsw/utils/line_parser.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
constexpr char sigmoid_sel_xml_parser::kXMLRoot[];

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void sigmoid_sel_xml_parser::parse(const ticpp::Element& node) {
  ticpp::Element snode = node_get(const_cast<ticpp::Element&>(node), kXMLRoot);
  m_params =
      std::make_shared<std::remove_reference<decltype(*m_params)>::type>();

  m_sigmoid.parse(snode);
  m_params->sigmoid = *m_sigmoid.parse_results();
  XML_PARSE_ATTR(snode, m_params, method);
} /* parse() */

void sigmoid_sel_xml_parser::show(std::ostream& stream) const {
  stream << build_header() << m_sigmoid << XML_ATTR_STR(m_params, method)
         << std::endl
         << build_footer();
} /* show() */

bool sigmoid_sel_xml_parser::validate(void) const {
  CHECK(true == m_sigmoid.validate());
  CHECK("" != m_params->method);
  return true;

error:
  return false;
} /* validate() */

NS_END(task_allocation, rcppsw);