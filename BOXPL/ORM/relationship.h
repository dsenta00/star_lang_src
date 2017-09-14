#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H
#include <vector>
#include <string>
#include "relationship_type.h"
#include <functional>
#include <memory>

#ifndef ENTITY
#define ENTITY
  class entity;
#endif

typedef enum {
  FOREACH_CONTINUE,
  FOREACH_IT2_REMOVED,
  FOREACH_IT1_REMOVED
} foreach_result;

class relationship
{
  std::string relationship_name;
  relationship_type type;
  std::vector<entity *> entities;
  entity *parent_entity;

  void notify_removing(entity *entity_to_remove);
public:
  relationship(entity *parent_entity, std::string relationship_name, relationship_type type);
  std::string getName();
  relationship *setName(std::string relationship_name);
  relationship_type getType();
  entity *getEntity(std::string id = "single");
  entity *find(const std::function<bool(entity *)> &func);
  relationship *sort(const std::function<bool(entity *, entity *)> &func);
  relationship *for_each(const std::function<foreach_result(entity *, entity *)> &func);
  relationship *for_each(const std::function<void(entity *)> &func);
  relationship *addEntity(entity *e);
  relationship *addEntities(relationship *r);
  relationship *removeEntity(entity *e);
  std::vector<entity *> &getEntities();
  entity *front();
  entity *back();
  void delete_all();
  uint32_t numOfEntities();
};

#endif // RELATIONSHIP_H
