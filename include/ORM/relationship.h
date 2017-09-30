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
class relationship {
public:
  relationship(std::string relationship_name, relationship_type type);
  std::string &get_name();
  relationship_type get_type();
  entity *find(const std::function<bool(entity *)> &func);
  void sort(const std::function<bool(entity *, entity *)> &func);
  void for_each(const std::function<foreach_result(entity *, entity *)> &func);
  void for_each(const std::function<void(entity *)> &func);
  void add_entity(entity *e);
  void remove_entity(entity *e);
  std::vector<entity *> &get_entities();
  entity *front();
  entity *back();
  uint32_t num_of_entities();
protected:
  std::string relationship_name;
  relationship_type type;
  std::vector<entity *> entities;
};

#endif // RELATIONSHIP_H
