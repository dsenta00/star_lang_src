#include "box_assert.h"
#include "box_monitor.h"
#include "ORM/entity.h"
#include "ORM/relationship.h"
#include "ORM/orm_test.h"
#include "ORM/orm.h"

class class2;

class class1 : public entity {
public:
  class1() : entity::entity("class1", "class1")
  {
    this->add_relationship("class1_class2", ONE_TO_MANY);
  }

  void addClass2(class2 *c2)
  {
    this->add_entity("class1_class2", (entity *) c2);
    entity *e = (entity *) c2;
    e->add_entity("class1_class2", (entity *) this);
  }
};

class class2 : public entity {
public:
  int number;

  class2(int i = 0) : entity::entity("class2", i)
  {
    number = i;
    this->add_relationship("class1_class2", MANY_TO_ONE);
  }
};

/**
 * @brief orm_test_basic
 */
static void orm_test_basic()
{
  ASSERT_OK;
  class1 *c1 = (class1 *) orm::create((entity *) new class1());
  ASSERT_OK;

  c1->add_entity("konan", nullptr);
  ASSERT_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
  BOX_ERROR_CLEAR;

  class2 *c2 = (class2 *) orm::create((entity *) new class2());
  c1->addClass2(c2);
  ASSERT_OK;

  relationship *c1_relationship = c1->get_relationship("class1_class2");
  relationship *c2_relationship = c2->get_relationship("class1_class2");

  ASSERT_TRUE(c1_relationship != nullptr, "relationship class1_class2 should exist!");
  ASSERT_TRUE(c2_relationship != nullptr, "relationship class1_class2 should exist!");
  ASSERT_TRUE(c1_relationship->get_type() == ONE_TO_MANY, "relationship should be ONE_TO_MANY");
  ASSERT_TRUE(c2_relationship->get_type() == MANY_TO_ONE, "relationship should be MANY_TO_ONE");
  ASSERT_TRUE(c1_relationship->num_of_entities() == 1, "number of entities should be 1! (%u)",
              c1_relationship->num_of_entities());
  ASSERT_TRUE(c2_relationship->num_of_entities() == 1, "number of entities should be 1! (%u)",
              c2_relationship->num_of_entities());

  class2 *c2_1 = (class2 *) c1->back("class1_class2");
  ASSERT_TRUE(c2_1 == c2, "back() should return 0x%X (0x%X)", c2, c2_1);

  class1 *c1_1 = (class1 *) c2->back("class1_class2");
  ASSERT_TRUE(c1_1 == c1, "back() should return 0x%X (0x%X)", c1, c1_1);

  orm::destroy(c2);
  ASSERT_OK;
  ASSERT_TRUE(orm::get_first("class1") == nullptr, "class1 repository should not have entities");
  ASSERT_TRUE(orm::get_first("class2") == nullptr, "class2 repository should not have entities");

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * @brief orm_test_advanced
 */
void orm_test_advanced()
{
  class1 *c1 = (class1 *) orm::create((entity *) new class1());

  for (uint32_t i = 0; i < 16; i++)
  {
    c1->addClass2((class2 *) orm::create((entity *) new class2(i)));
  }

  relationship *c1_relationship = c1->get_relationship("class1_class2");

  ASSERT_TRUE(c1_relationship != nullptr, "relationship class1_class2 should exist!");
  ASSERT_TRUE(c1_relationship->get_type() == ONE_TO_MANY, "relationship should be ONE_TO_MANY");
  ASSERT_TRUE(c1_relationship->num_of_entities() == 16, "number of entities should be 16! (%u)",
              c1_relationship->num_of_entities());

  std::vector<entity *> &entities = c1->get_relationship("class1_class2")->get_entities();
  for (entity *e : entities)
  {
    relationship *c2_relationship = e->get_relationship("class1_class2");

    ASSERT_TRUE(c2_relationship != nullptr, "relationship class1_class2 should exist!");
    ASSERT_TRUE(c2_relationship->get_type() == MANY_TO_ONE, "relationship should be MANY_TO_ONE");
    ASSERT_TRUE(c2_relationship->num_of_entities() == 1, "number of entities should be 1! (%u)",
                c1_relationship->num_of_entities());
  }

  /*
   * test foreach with two parameters
   */
  c1_relationship->for_each([&](entity *e1, entity *e2) {
    class2 *c2_1 = (class2 *) e1;
    class2 *c2_2 = (class2 *) e2;

    ASSERT_TRUE(c2_2->number == (c2_1->number + 1), "they should follow each other (%u) (%u)", c2_1->number,
                c2_2->number);
    return FOREACH_CONTINUE;
  });

  c1_relationship->for_each([&](entity *e1, entity *e2) {
    (void) e1;
    class2 *c2_2 = (class2 *) e2;

    if (c2_2->number % 2 == 1)
    {
      orm::destroy(e2);
      return FOREACH_IT2_REMOVED;
    }

    return FOREACH_CONTINUE;
  });

  c1_relationship = c1->get_relationship("class1_class2");

  ASSERT_TRUE(c1_relationship != nullptr, "relationship class1_class2 should exist!");
  ASSERT_TRUE(c1_relationship->get_type() == ONE_TO_MANY, "relationship should be ONE_TO_MANY");
  ASSERT_TRUE(c1_relationship->num_of_entities() == 8, "number of entities should be 8! (%u)",
              c1_relationship->num_of_entities());

  for (entity *e : entities)
  {
    ASSERT_TRUE(((class2 *) e)->number % 2 == 0, "number should be pair (%u)", ((class2 *) e)->number);

    relationship *c2_relationship = e->get_relationship("class1_class2");

    ASSERT_TRUE(c2_relationship != nullptr, "relationship class1_class2 should exist!");
    ASSERT_TRUE(c2_relationship->get_type() == MANY_TO_ONE, "relationship should be MANY_TO_ONE");
    ASSERT_TRUE(c2_relationship->num_of_entities() == 1, "number of entities should be 1! (%u)",
                c1_relationship->num_of_entities());
  }

  /*
   * test sort
   */
  c1_relationship->sort([&](entity *e1, entity *e2) {
    return ((class2 *) e1)->number > ((class2 *) e2)->number;
  });

  c1_relationship->for_each([&](entity *e1, entity *e2) {
    class2 *c2_1 = (class2 *) e1;
    class2 *c2_2 = (class2 *) e2;

    ASSERT_TRUE(c2_1->number == (c2_2->number + 2), "they should follow each other (%u) (%u)", c2_1->number,
                c2_2->number);
    return FOREACH_CONTINUE;
  });

  /*
   * test orm destroy
   */
  orm::destroy((entity *) c1);

  ASSERT_TRUE(orm::get_first("class1") == nullptr, "class1 repository should not have entities");
  ASSERT_TRUE(orm::get_first("class2") == nullptr, "class2 repository should not have entities");

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test ORM.
 */
void orm_test()
{
  orm::add_entity_repository("class1");
  orm::add_entity_repository("class2");

  printf("%s()\r\n", __FUNCTION__);
  orm_test_basic();
  orm_test_advanced();
  printf("\r\n\r\n");

  orm::remove_entity_repository("class1");
  orm::remove_entity_repository("class2");
}