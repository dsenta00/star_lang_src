TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../box_array.cpp \
    ../box_array_test.cpp \
    ../box_assert.cpp \
    ../box_data.cpp \
    ../box_data_member.cpp \
    ../box_data_member_test.cpp \
    ../box_data_test.cpp \
    ../box_instruction.cpp \
    ../box_member_collection.cpp \
    ../box_member_collection_test.cpp \
    ../box_memory_chunk.cpp \
    ../box_memory_chunk_test.cpp \
    ../box_method_member_collection.cpp \
    ../box_monitor.cpp \
    ../box_object_member_collection.cpp \
    ../box_virtual_memory.cpp \
    ../box_virtual_memory_test.cpp \
    ../main.cpp \
    ../box_error.cpp

HEADERS += \
    ../box_array.h \
    ../box_array_test.h \
    ../box_assert.h \
    ../box_data.h \
    ../box_data_member.h \
    ../box_data_member_test.h \
    ../box_data_test.h \
    ../box_instruction.h \
    ../box_member_collection.h \
    ../box_member_collection_test.h \
    ../box_memory_chunk.h \
    ../box_memory_chunk_test.h \
    ../box_method_member_collection.h \
    ../box_monitor.h \
    ../box_object_member_collection.h \
    ../box_virtual_memory.h \
    ../box_virtual_memory_test.h \
    ../box_error.h \
    ../memory.h
