#ifndef ENTITY_H
#define ENTITY_H
#include "orm_fw.h"
#include "relationship_type.h"
#include <cstdint>
#include <map>
#include <vector>
#include <string>

typedef std::shared_ptr<relationship> relationship_p;

/**
 * The entity class.
 * Each entity has relations with another entity.
 */
class entity {
protected:
  bool marked;
  std::string id;
  std::string type;
  std::map<std::string, relationship_p> relation;
public:
  entity(std::string type, const uint64_t id);
  entity(std::string type, std::string id);

  std::string get_id();
  std::string get_entity_type();
  bool get_marked();
  void set_marked(bool marked);

  bool have_relations();

  void add_entity(std::string relationship_name, entity *e);
  void add_entities(std::string relationship_name, relationship *er);
  void remove_entity(std::string relationship_name, entity *e);
  void remove_entity(entity *e);
  void notify_remove(std::string relationship_name, entity *e);
  entity *back(std::string relationship_name);

  relationship *get_relationship(std::string relationship_name);
  void add_relationship(std::string relationship_name, relationship_type type);
  void remove_relationship(std::string relationship_name);

  void remove_all_relationships();
  virtual ~entity();
};

#endif // ENTITY_H
