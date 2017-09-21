#ifndef ENTITY_REPOSITORY_H
#define ENTITY_REPOSITORY_H
#include "orm_fw.h"
#include <list>
#include <string>
#include <functional>
#include <memory>

typedef std::unique_ptr<entity> entity_p;

/**
 * @brief The entity_repository class.
 * All created entities are stored here.
 * Each entity type has it's own entity_repository.
 */
class entity_repository {
protected:
  std::string entity_type;
  std::list<entity_p> entities;
public:
  entity_repository(std::string entity_type);

  std::string &get_entity_type();

  entity *find(const std::function<bool(entity *)> &func);
  void add(entity *e);
  void remove(entity *e);

  void sweep();
  void clear();
};

#endif // ENTITY_REPOSITORY_H
