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
  void change_id(entity *e, std::string new_id);
  void destroy(entity *e);
  void sweep();
  entity *select(std::string entity_type, std::function<bool(entity *)> where);
  entity *select(std::string entity_type, std::string id);
  entity *get_first(std::string entity_type);
  void remove_entity_repository(std::string entity_type);
}

#define ORM_SELECT(__ENTITY_TYPE__, __WHERE__) \
  (__ENTITY_TYPE__ *)orm::select(#__ENTITY_TYPE__, [&] (entity *e) { \
  __ENTITY_TYPE__ *obj = (__ENTITY_TYPE__ *)e; \
  (void)obj; \
  return __WHERE__; \
})

#define ORM_DESTROY(__ENTITY__) \
  orm::destroy((entity *)__ENTITY__)

#endif // ORM_H
