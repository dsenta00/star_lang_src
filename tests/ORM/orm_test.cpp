/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../test_assert.h"
#include "error_log.h"
#include "ORM/relationship.h"
#include "orm_test.h"
#include "ORM/orm.h"

class class2;

class class1 : public object {
public:
    class1() : object::object("class1", "class1")
    {
        this->master_relationship_add("class1_class2", ONE_TO_MANY);
        this->master_relationship_add("class1_class1", ONE_TO_ONE);
    }

    void addClass1(class1 *c1)
    {
        this->master_relationship_add_object("class1_class1", (object *) c1);
    }

    void addClass2(class2 *c2)
    {
        this->master_relationship_add_object("class1_class2", (object *) c2);
    }
};

class class3;

class class2 : public object {
public:
    int number;

    explicit class2(int i = 0) : object::object("class2", i)
    {
        number = i;
        this->master_relationship_add("class2_class3", ONE_TO_MANY);
    }

    void addClass3(class3 *c3)
    {
        this->master_relationship_add_object("class2_class3", (object *) c3);
    }
};

class class3 : public object {
public:
    int class2_number;
    int number;

    explicit class3(int i = 0, int class2_number = 0) : object::object("class3", i)
    {
        this->number = i;
        this->class2_number = class2_number;
        this->slave_relationship_add("class3_class1", ONE_TO_MANY);
    }

    void addClass1(class1 *c1)
    {
        this->master_relationship_add_object("class3_class1", (object *) c1);
    }
};

/**
 * @brief orm_test_basic
 */
static void orm_test_basic()
{
    ASSERT_OK;
    class1 *c1 = (class1 *) orm::create((object *) new class1());
    ASSERT_OK;

    ASSERT_EQUALS(c1->get_id(), "class1");
    ASSERT_FALSE(c1->get_marked(), "get_marked should be false");

    c1->master_relationship_add_object("konan", nullptr);
    ASSERT_ERROR(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
    ERROR_LOG_CLEAR;

    class2 *c2 = (class2 *) orm::create((object *) new class2());
    c1->addClass2(c2);
    ASSERT_OK;

    relationship *c1_relationship = c1->master_relationship_get("class1_class2");
    relationship *c2_relationship = c2->slave_relationship_get("class1_class2");

    ASSERT_NOT_NULL(c1_relationship);
    ASSERT_NOT_NULL(c2_relationship);
    ASSERT_EQUALS(c1_relationship->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(c2_relationship->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(c1_relationship->size(), 1);
    ASSERT_EQUALS(c2_relationship->size(), 1);

    ASSERT_EQUALS((class2 *) c1->master_relationship_back("class1_class2"), c2);
    ASSERT_EQUALS((class1 *) c2->slave_relationship_back("class1_class2"), c1);

    ORM_DESTROY(c2);

    ASSERT_OK;
    ASSERT_EQUALS(orm::get_first("class1"), c1);
    ASSERT_NULL(orm::get_first("class2"));
    ASSERT_EQUALS(c1_relationship->size(), 0);

    ORM_DESTROY(c1);

    ASSERT_OK;
    ASSERT_NULL(orm::get_first("class1"));
    ASSERT_NULL(orm::get_first("class2"));
}

/**
 * @brief orm_test_advanced1
 */
void orm_test_advanced1()
{
    class1 *c1 = (class1 *) orm::create((object *) new class1());

    for (uint32_t i = 0; i < 16; i++)
    {
        c1->addClass2((class2 *) orm::create((object *) new class2(i)));
    }

    relationship *c1_relationship = c1->master_relationship_get("class1_class2");

    ASSERT_NOT_NULL(c1_relationship);
    ASSERT_EQUALS(c1_relationship->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(c1_relationship->size(), 16);

    for (object *o : *c1_relationship)
    {
        relationship *c2_relationship = o->slave_relationship_get("class1_class2");

        ASSERT_NOT_NULL(c2_relationship);
        ASSERT_EQUALS(c2_relationship->get_type(), ONE_TO_MANY);
        ASSERT_EQUALS(c2_relationship->size(), 1);
    }

    /*
     * test foreach with two parameters
     */
    c1_relationship->for_each([&](object *e1, object *e2) {
        class2 *c2_1 = (class2 *) e1;
        class2 *c2_2 = (class2 *) e2;

        ASSERT_EQUALS(c2_2->number, c2_1->number + 1);
        return FOREACH_CONTINUE;
    });

    c1_relationship->for_each([&](object *e1, object *e2) {
        (void) e1;
        class2 *c2_2 = (class2 *) e2;

        if (c2_2->number % 2 == 1)
        {
            orm::destroy(e2);
            return FOREACH_IT2_REMOVED;
        }

        return FOREACH_CONTINUE;
    });

    c1_relationship = c1->master_relationship_get("class1_class2");

    ASSERT_NOT_NULL(c1_relationship);
    ASSERT_EQUALS(c1_relationship->size(), 8);

    for (object *e : *c1_relationship)
    {
        ASSERT_EQUALS(((class2 *) e)->number % 2, 0);

        relationship *c2_relationship = e->slave_relationship_get("class1_class2");

        ASSERT_NOT_NULL(c2_relationship);
        ASSERT_EQUALS(c2_relationship->size(), 1);
    };

    /*
     * test sort
     */
    c1_relationship->sort([&](object *e1, object *e2) {
        return ((class2 *) e1)->number > ((class2 *) e2)->number;
    });

    c1_relationship->for_each([&](object *e1, object *e2) {
        class2 *c2_1 = (class2 *) e1;
        class2 *c2_2 = (class2 *) e2;

        ASSERT_EQUALS(c2_1->number, c2_2->number + 2);
        return FOREACH_CONTINUE;
    });

    /*
     * test orm destroy
     */
    ORM_DESTROY(c1);

    ASSERT_NULL(orm::get_first("class1"));
    ASSERT_NULL(orm::get_first("class2"));
}

/**
 * @brief orm_test_advanced2
 */
static void orm_test_advanced2()
{
    class1 *c1 = (class1 *) orm::create((object *) new class1());

    for (uint32_t i = 0; i < 16; i++)
    {
        class2 *c2 = (class2 *) orm::create((object *) new class2(i));

        for (uint32_t j = 0; j < 16; j++)
        {
            class3 *c3 = (class3 *) orm::create((object *) new class3(j, i));
            c2->addClass3(c3);
        }

        c1->addClass2(c2);
    }

    /*
     * Check first relations.
     */
    relationship *c1_relationship = c1->master_relationship_get("class1_class2");

    ASSERT_NOT_NULL(c1_relationship);
    ASSERT_EQUALS(c1_relationship->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(c1_relationship->size(), 16);

    for (object *class2_entity : *c1_relationship)
    {
        relationship *c2_relationship = class2_entity->slave_relationship_get("class1_class2");

        ASSERT_NOT_NULL(c2_relationship);
        ASSERT_EQUALS(c2_relationship->get_type(), ONE_TO_MANY);
        ASSERT_EQUALS(c2_relationship->size(), 1);

        relationship *c3_relationship = class2_entity->master_relationship_get("class2_class3");

        ASSERT_NOT_NULL(c3_relationship);
        ASSERT_EQUALS(c3_relationship->get_type(), ONE_TO_MANY);
        ASSERT_EQUALS(c3_relationship->size(), 16);

        for (object *class3_entity : *c3_relationship)
        {
            relationship *c3_relationship = class2_entity->slave_relationship_get("class1_class2");

            ASSERT_NOT_NULL(c3_relationship);
            ASSERT_EQUALS(c3_relationship->get_type(), ONE_TO_MANY);
            ASSERT_EQUALS(c3_relationship->size(), 1);
        }
    }

    class2 *c2;

    c2 = ORM_SELECT(class2, e->get_id() == "1");
    ASSERT_NOT_NULL(c2);
    ASSERT_EQUALS(c2->number, 1);

    ORM_DESTROY(c2);
    c2 = ORM_SELECT(class2, e->get_id() == "1");
    ASSERT_NULL(c2);

    class3 *c3;
    c3 = ORM_SELECT(class3, obj->class2_number == 1);
    ASSERT_NULL(c3);

    c3 = ORM_SELECT(class3, obj->class2_number == 2);
    ASSERT_NOT_NULL(c3);

    ORM_DESTROY(c1);

    ASSERT_OK;
    ASSERT_NULL(orm::get_first("class1"));
    ASSERT_NULL(orm::get_first("class2"));
    ASSERT_NULL(orm::get_first("class3"));
}

#define CLASS1_CHECK(__MIDDLE__, __FRONT__, __BACK__) \
do { \
ASSERT_TRUE((__MIDDLE__)->master_relationship_get("class1_class1")->front() == __FRONT__, #__FRONT__" should be front!"); \
ASSERT_TRUE((__MIDDLE__)->slave_relationship_get("class1_class1")->front() == __BACK__, #__BACK__" should be back!"); \
}while(0)

/**
 * @brief orm_test_cyclic_relations1
 */
static void orm_test_cyclic_relations1()
{
    class1 *c1_1 = (class1 *) orm::create((object *) new class1());
    class1 *c1_2 = (class1 *) orm::create((object *) new class1());
    class1 *c1_3 = (class1 *) orm::create((object *) new class1());

    c1_1->addClass1(c1_2);
    c1_2->addClass1(c1_3);
    c1_3->addClass1(c1_1);

    class1 *c;

    c = ORM_SELECT(class1, obj == c1_1);
    ASSERT_NOT_NULL(c);

    c = ORM_SELECT(class1, obj == c1_2);
    ASSERT_NOT_NULL(c);

    c = ORM_SELECT(class1, obj == c1_3);
    ASSERT_NOT_NULL(c);

    CLASS1_CHECK(c1_1, c1_2, c1_3);
    CLASS1_CHECK(c1_1, c1_2, c1_3);
    CLASS1_CHECK(c1_1, c1_2, c1_3);

    ORM_DESTROY(c1_1);

    c = ORM_SELECT(class1, obj == c1_1);
    ASSERT_NULL(c);

    c = ORM_SELECT(class1, obj == c1_2);
    ASSERT_NULL(c);

    c = ORM_SELECT(class1, obj == c1_3);
    ASSERT_NULL(c);
}

/**
 * @brief orm_test_cyclic_relations2
 */
static void orm_test_cyclic_relations2()
{
    class1 *c1 = (class1 *) orm::create((object *) new class1());
    class2 *c2 = (class2 *) orm::create((object *) new class2());
    class3 *c3 = (class3 *) orm::create((object *) new class3());

    c1->addClass2(c2);
    c2->addClass3(c3);
    c3->addClass1(c1);

    c1 = ORM_SELECT(class1, obj == c1);
    ASSERT_NOT_NULL(c1);

    c2 = ORM_SELECT(class2, obj == c2);
    ASSERT_NOT_NULL(c2);

    c3 = ORM_SELECT(class3, obj == c3);
    ASSERT_NOT_NULL(c3);

    ORM_DESTROY(c1);

    c1 = ORM_SELECT(class1, obj == c1);
    ASSERT_NULL(c1);

    c2 = ORM_SELECT(class2, obj == c2);
    ASSERT_NULL(c2);

    c3 = ORM_SELECT(class3, obj == c3);
    ASSERT_NULL(c3);
}

/**
 * @brief orm_test_change_id
 */
static void orm_test_change_id()
{
    auto c1 = (class1 *) orm::create((object *) new class1());
    auto c2 = (class1 *) orm::create((object *) new class1());
    auto c3 = (class1 *) orm::create((object *) new class1());

    orm::change_id(c1, "miljenko");
    orm::change_id(c2, "ivan");
    orm::change_id(c3, "jure");

    ASSERT_EQUALS(c1->get_id(), "miljenko");
    ASSERT_EQUALS(c2->get_id(), "ivan");
    ASSERT_EQUALS(c3->get_id(), "jure");

    class1 *c;

    c = (class1 *) orm::select("class1", "class1");
    ASSERT_NULL(c);

    c = (class1 *) orm::select("class1", "miljenko");
    ASSERT_EQUALS(c, c1);

    c = (class1 *) orm::select("class1", "ivan");
    ASSERT_EQUALS(c, c2);

    c = (class1 *) orm::select("class1", "jure");
    ASSERT_EQUALS(c, c3);
}

/**
 * @brief orm_test_switch_relations1
 */
static void orm_test_switch_relations1()
{
    auto c1_1 = (class1 *) orm::create((object *) new class1());
    auto c1_2 = (class1 *) orm::create((object *) new class1());

    orm::change_id(c1_1, "miljenko");
    orm::change_id(c1_2, "ivan");

    auto c2 = (class2 *) orm::create((object *) new class2());

    c1_1->addClass2(c2);
    c1_2->addClass2(c2);

    auto r = c1_1->master_relationship_get("class1_class2");
    ASSERT_NOT_NULL(r);
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 1);

    r = c1_2->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 1);

    r = c2->slave_relationship_get("class1_class2");
    ASSERT_NOT_NULL(r);
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 2);

    ORM_DESTROY(c2);

    c2 = ORM_SELECT(class2, true);
    ASSERT_NULL(c2);

    r = c1_1->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 0);

    r = c1_2->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 0);
}

/**
 * @brief orm_test_switch_relations2
 */
static void orm_test_switch_relations2()
{
    auto c1_1 = (class1 *) orm::create((object *) new class1());
    auto c1_2 = (class1 *) orm::create((object *) new class1());

    orm::change_id(c1_1, "miljenko");
    orm::change_id(c1_2, "ivan");

    auto c2 = (class2 *) orm::create((object *) new class2());

    c1_1->addClass2(c2);
    c1_2->addClass2(c2);

    auto r = c1_1->master_relationship_get("class1_class2");
    ASSERT_NOT_NULL(r);
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 1);

    r = c1_2->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 1);

    r = c2->slave_relationship_get("class1_class2");
    ASSERT_NOT_NULL(r);
    ASSERT_EQUALS(r->get_type(), ONE_TO_MANY);
    ASSERT_EQUALS(r->size(), 2);

    c1_2->master_relationship_remove_object("class1_class2", c2);

    c2 = ORM_SELECT(class2, true);
    ASSERT_NOT_NULL(c2);

    r = c2->slave_relationship_get("class1_class2");
    ASSERT_EQUALS(r->size(), 1);
    ASSERT_EQUALS(r->front(), c1_1);

    r = c1_1->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->size(), 1);
    ASSERT_EQUALS(r->front(), c2);

    r = c1_2->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->size(), 0);

    c1_1->master_relationship_remove_object("class1_class2", c2);

    c2 = ORM_SELECT(class2, true);
    ASSERT_NULL(c2);

    r = c1_1->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->size(), 0);

    r = c1_2->master_relationship_get("class1_class2");
    ASSERT_EQUALS(r->size(), 0);
}

/**
 * Test ORM.
 */
void orm_test()
{
    RUN_TEST(orm_test_basic());
    RUN_TEST(orm_test_advanced1());
    RUN_TEST(orm_test_advanced2());
    RUN_TEST(orm_test_cyclic_relations1());
    RUN_TEST(orm_test_cyclic_relations2());
    RUN_TEST(orm_test_change_id());
    RUN_TEST(orm_test_switch_relations1());
    RUN_TEST(orm_test_switch_relations2());
}