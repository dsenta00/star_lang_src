#ifndef ENTITY_REPOSITORY_H
#define ENTITY_REPOSITORY_H
#include <list>
#include <string>
#include <functional>
#include <memory>

#ifndef ENTITY
#define ENTITY
  class entity;
#endif

typedef std::unique_ptr<entity> entity_p;

class entity_repository
{
  std::string entity_type;
  std::list<entity_p> entities;
public:
  entity_repository(std::string entity_type);

  std::string &getEntityType();

  entity *find(const std::function<bool(entity *)> &func);
  entity_repository *add(entity *e);
  entity_repository *remove(entity *e);

  void sweep();
  void clear();
};

#endif // ENTITY_REPOSITORY_H
