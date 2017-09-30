#ifndef ENTITY_REPOSITORY_H
#define ENTITY_REPOSITORY_H

#include "orm_fw.h"
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <memory>

typedef std::shared_ptr<entity> entity_p;

/**
 * The entity_repository class.
 *
 * All created entities of same type are stored right here.
 * Each entity type has its own entity_repository.
 */
class entity_repository {
public:
  entity *find(const std::function<bool(entity *)> &func);
  entity *get(std::string &id);
  void add(entity *e);
  void remove(entity *e);
  void change_id(entity *e, std::string &new_id);
  void sweep();
  ~entity_repository();
protected:
  /*
   * key -> entity ID
   * values array
   */
  std::map<std::string, std::vector<entity_p>> entity_map;
};

#endif // ENTITY_REPOSITORY_H
