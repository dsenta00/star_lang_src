#ifndef ORM_H
#define ORM_H
#include <string>
#include <functional>
#include "entity_repository.h"

namespace orm
{
  entity_repository *findEntityRepostiory(std::string entity_type);
  void addEntityRepostiory(std::string entity_type);
  void removeEntityRepostiory(std::string entity_type);

  entity *create(entity *data);
  void destroy(entity* e);

  void sweep();
  entity *select(std::string entity_type, std::function<bool(entity *)> where);
  entity *get_first(std::string entity_type);
}

#endif // ORM_H
