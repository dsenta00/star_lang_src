#ifndef ENTITY_REPOSITORY_H
#define ENTITY_REPOSITORY_H

#include "orm_fw.h"
#include <list>
#include <string>
#include <functional>
#include <memory>

typedef std::unique_ptr<entity> entity_p;

/**
 * The entity_repository class.
 * All created entities are stored here.
 * Each entity type has it's own entity_repository.
 */
class entity_repository {
public:
  entity *find(const std::function<bool(entity *)> &func);
  void add(entity *e);
  void remove(entity *e);
  void sweep();
protected:
  std::list<entity_p> entities;
};

#endif // ENTITY_REPOSITORY_H
