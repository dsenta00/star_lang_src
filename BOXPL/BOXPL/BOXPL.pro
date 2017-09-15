TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../ORM/entity.cpp \
    ../ORM/entity_repository.cpp \
    ../ORM/orm.cpp \
    ../ORM/relationship.cpp \
    ../box_array.cpp \
    ../box_assert.cpp \
    ../box_data.cpp \
    ../box_instruction.cpp \
    ../box_memory_chunk.cpp \
    ../box_monitor.cpp \
    ../box_virtual_memory.cpp \
    ../main.cpp \
    ../box_error.cpp \
    ../memory.cpp \
    ../box_memory_chunk_if.cpp \
    ../ORM/orm_test.cpp \
    ../tests/box_array_test.cpp \
    ../tests/box_data_test.cpp \
    ../tests/box_memory_chunk_test.cpp \
    ../tests/box_virtual_memory_test.cpp \
    ../tests/test.cpp \
    ../box_method.cpp

HEADERS += \
    ../ORM/entity.h \
    ../ORM/entity_repository.h \
    ../ORM/orm.h \
    ../ORM/relationship.h \
    ../ORM/relationship_type.h \
    ../ORM/orm_test.h \
    ../box_array.h \
    ../box_assert.h \
    ../box_data.h \
    ../box_instruction.h \
    ../box_memory_chunk.h \
    ../box_monitor.h \
    ../box_virtual_memory.h \
    ../box_error.h \
    ../memory.h \
    ../box_data_type.h \
    ../box_memory_chunk_if.h \
    ../ORM/entity.h \
    ../ORM/entity_repository.h \
    ../ORM/orm.h \
    ../tests/box_array_test.h \
    ../tests/box_memory_chunk_test.h \
    ../tests/box_virtual_memory_test.h \
    ../tests/test.h \
    ../box_fw.h \
    ../box_method.h \
    ../ORM/orm_fw.h

DISTFILES +=
