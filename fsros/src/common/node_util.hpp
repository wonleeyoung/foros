/*
 * Copyright (c) 2021 42dot All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AKIT_FAILSAFE_FSROS_COMMON_NODE_UTIL_HPP_
#define AKIT_FAILSAFE_FSROS_COMMON_NODE_UTIL_HPP_

#include <string>

namespace akit {
namespace failsafe {
namespace fsros {

class NodeUtil {
 public:
  static std::string get_node_name(uint32_t node_id) {
    return "node" + std::to_string(node_id);
  }

  static std::string get_service_name(const char *cluster_name,
                                      uint32_t node_id,
                                      const char *service_name) {
    return std::string(cluster_name) + "/node" + std::to_string(node_id) +
           service_name;
  }
};

}  // namespace fsros
}  // namespace failsafe
}  // namespace akit

#endif  // AKIT_FAILSAFE_FSROS_COMMON_NODE_UTIL_HPP_