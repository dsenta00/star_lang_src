TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../ORM/entity.cpp \
    ../ORM/entity_repository.cpp \
    ../ORM/orm.cpp \
    ../ORM/relationship.cpp \
    ../ORM/orm_test.cpp \
    ../box_array.cpp \
    ../box_array_test.cpp \
    ../box_assert.cpp \
    ../box_data.cpp \
    ../box_data_test.cpp \
    ../box_instruction.cpp \
    ../box_memory_chunk.cpp \
    ../box_memory_chunk_test.cpp \
    ../box_monitor.cpp \
    ../box_virtual_memory.cpp \
    ../box_virtual_memory_test.cpp \
    ../main.cpp \
    ../box_error.cpp \
    ../memory.cpp \
    ../box_memory_chunk_if.cpp \
    ../test.cpp

HEADERS += \
    ../ORM/entity.h \
    ../ORM/entity_repository.h \
    ../ORM/orm.h \
    ../ORM/relationship.h \
    ../ORM/relationship_type.h \
    ../ORM/orm_test.h \
    ../box_array.h \
    ../box_array_test.h \
    ../box_assert.h \
    ../box_data.h \
    ../box_data_test.h \
    ../box_instruction.h \
    ../box_memory_chunk.h \
    ../box_memory_chunk_test.h \
    ../box_monitor.h \
    ../box_virtual_memory.h \
    ../box_virtual_memory_test.h \
    ../box_error.h \
    ../memory.h \
    ../box_data_type.h \
    ../box_memory_chunk_if.h \
    ../test.h

DISTFILES +=
