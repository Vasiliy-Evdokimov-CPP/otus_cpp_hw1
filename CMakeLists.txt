# определяем минимальную версию CMake, при помощи которой можно обрабатывать текущий файл проекта
cmake_minimum_required(VERSION 3.14)

# устанавливаем кэшированную переменную PATCH_VERSION со значением "2"
set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
# устанавливаем переменную PROJECT_VERSION
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
# переменной @PROJECT_VERSION_PATCH@, используемой в version.h.in,
# присваивается значение именно из полного номера версии, указанного на этом шаге
# (если я правильно понял)

# устанавливаем имя проекта и версию
project(helloworld VERSION ${PROJECT_VESRION})

# копируем файл version.h.in в файл version.h, при этом выполняя подстановку переменной внутри файла
configure_file(version.h.in version.h)

# создаем исполняемый файл helloworld_cli на основе main.cpp
add_executable(helloworld_cli main.cpp)
# создаем библиотеку с именем helloworld на основе lib.cpp
add_library(helloworld lib.cpp)
# создаем исполняемый файл test_version на основе test_version.cpp
add_executable(test_version test_version.cpp)

# GTest 

# создаем исполняемый файл gtest_version на основе gtest_version.cpp
add_executable(gtest_version gtest_version.cpp)

# подключаем GTest
# https://github.com/google/googletest/blob/main/docs/quickstart-cmake.md
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# устанавливаем свойства для целей (targets) helloworld_cli, helloworld и test_version, 
# указывая, что для них требуется стандарт C++ 14 и он обязателен для компиляции
set_target_properties(
    helloworld_cli 
    helloworld 
    test_version
    gtest_version 
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)
# добавляем директорию "${CMAKE_BINARY_DIR}" в качестве приватной директории для цели helloworld; 
# это означает, что эта директория будет доступна только для цели helloworld
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)

# связываем зависимости для целей
target_link_libraries(helloworld_cli PRIVATE
    helloworld
)

target_link_libraries(test_version
    helloworld
)

target_link_libraries(gtest_version
    helloworld
    GTest::gtest_main
)

# определяем для целей опции компиляторов (MSVC и прочих)
if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )

    # устанавливаем генератор упаковки на "ZIP" - файл релиза будет упакован в .zip архив
    set(CPACK_GENERATOR ZIP)

else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    # устанавливаем генератор упаковки на "DEB" - будут сгенерирован пакет .deb для упаковки проекта
    set(CPACK_GENERATOR DEB)

endif()

# указываем, что при установке (копировании) цели с именем "helloworld_cli", 
# ее исполняемый файл должен быть скопирован в папку "bin"
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

# устанавливаем переменные, используемые CPack (инструмент для упаковки проекта в различные форматы пакетов) для версии пакета
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

# устанавливаем контактный email для пакета при использовании CPack
set(CPACK_PACKAGE_CONTACT vasiliyevdokimov84@gmail.com)

# включаем использование CPack в проекте
include(CPack)

# включаем поддержку тестирования в проекте
enable_testing()

# включаем использование GoogleTest
include(GoogleTest)
gtest_discover_tests(gtest_version)

# добавляем тест CTest
add_test(NAME TestVersion COMMAND test_version)
set_tests_properties(TestVersion PROPERTIES FAIL_REGULAR_EXPRESSION "FAIL")