#ifndef ORM_H
#define ORM_H

#include "entity_repository.h"
#include <string>
#include <functional>

/**
 * ORM interface.
 */
namespace orm {
  entity_repository *find_entity_repository(std::string entity_type);
  void add_entity_repository(std::string entity_type);
  entity *create(entity *data);
  void destroy(entity *e);
  void sweep();
  entity *select(std::string entity_type, std::function<bool(entity *)> where);
  entity *get_first(std::string entity_type);
  void remove_entity_repository(std::string entity_type);
}

#endif // ORM_H
