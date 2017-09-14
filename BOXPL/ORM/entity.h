#ifndef ENTITY_H
#define ENTITY_H
#include "relationship_type.h"
#include <cstdint>
#include <map>
#include <vector>
#include <string>

#ifndef RELATIONSHIP
#define RELATIONSHIP
  class relationship;
#endif

typedef std::shared_ptr<relationship> relationship_p;

/**
 * @brief The entity class
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

  std::string getId();
  std::string getType();
  bool getMarked();
  void setMarked(bool marked);

  bool hasRelations();

  entity *addEntity(std::string relationship_name, entity *e);
  entity *addEntities(std::string relationship_name, relationship *er);
  entity *removeEntity(std::string relationship_name, entity *e);
  entity *removeEntity(entity *e);
  void notifyRemove(std::string relationship_name, entity *e);
  entity *back(std::string relationship_name);

  relationship *getRelationship(std::string relationship_name);
  void addRelationship(std::string relationship_name, relationship_type type);
  void removeRelationship(std::string relationship_name);

  void removeAllRelationships();
  virtual ~entity();
};

#endif // ENTITY_H
