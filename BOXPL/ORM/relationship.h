#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H
#include "orm_fw.h"
#include "relationship_type.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>

typedef enum {
  FOREACH_CONTINUE,
  FOREACH_IT2_REMOVED,
  FOREACH_IT1_REMOVED
} foreach_result;

/**
 * The relationship class.
 * Contains and manages all entity objects in relationship.
 */
class relationship
{
  std::string relationship_name;
  relationship_type type;
  std::vector<entity *> entities;
  entity *parent_entity;

  void notify_removing(entity *entity_to_remove);
public:
  relationship(entity *parent_entity, std::string relationship_name, relationship_type type);
  std::string &get_name();
  void set_name(std::string relationship_name);
  relationship_type get_type();
  entity *get_entity(std::string id = "single");
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
