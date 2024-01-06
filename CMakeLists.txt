# определяем минимальную версию CMake, при помощи которой можно обрабатывать текущий файл проекта
cmake_minimum_required(VERSION 3.10)

# устанавливаем кэшированную переменную PATCH_VERSION со значением "2"
set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
# устанавливаем переменную PROJECT_VERSION
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

# устанавливаем имя проекта и версию
project(helloworld VERSION ${PROJECT_VESRION})
# ищем и подключаем библиотеку Boost с компонентом unit_test_framework (фреймворк для модульного тестирования) в проект 
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# копируем файл version.h.in в файл version.h, при этом выполняя подстановку переменных внутри файла
configure_file(version.h.in version.h)

# создаем исполняемый файл helloworld_cli на основе main.cpp
add_executable(helloworld_cli main.cpp)
# создаем библиотеку с именем helloworld на основе lib.cpp
add_library(helloworld lib.cpp)
# создаем исполняемый файл test_version на основе test_version.cpp
add_executable(test_version test_version.cpp)

# устанавливаем свойства для целей (targets) helloworld_cli, helloworld и test_version, 
# указывая, что для них требуется стандарт C++ 14 и он обязателен для компиляции
set_target_properties(helloworld_cli helloworld test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
# добавляем директорию "${CMAKE_BINARY_DIR}" в качестве приватной директории для цели helloworld; 
# это означает, что эта директория будет доступна только для цели helloworld
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)
# устанавливаем свойства для цели test_version, добавляя определение препроцессора BOOST_TEST_DYN_LINK
# и директорию ${Boost_INCLUDE_DIR} в качестве директории для поиска заголовочных файлов
set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

# связываем зависимости для целей helloworld_cli и test_version
target_link_libraries(helloworld_cli PRIVATE
    helloworld
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    helloworld
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
endif()

# указываем, что при установке (копировании) цели с именем "helloworld_cli", 
# ее исполняемый файл должен быть скопирован в папку "bin"
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

# устанавливаем генератор упаковки на "DEB" - будут сгенерирован пакет .deb для упаковки проекта
set(CPACK_GENERATOR DEB)

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

# добавляем тест с именем "test_version", который запускает исполняемый файл с именем "test_version"
add_test(test_version test_version)