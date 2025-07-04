list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include(CMakeFindDependencyMacro)

# detect the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(FIND_SFML_OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(FIND_SFML_OS_LINUX 1)

    if(false)
        set(FIND_SFML_USE_DRM 1)
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    set(FIND_SFML_OS_FREEBSD 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(FIND_SFML_OS_ANDROID 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    set(FIND_SFML_OS_IOS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(FIND_SFML_OS_MACOS 1)
endif()

# When installing, save whether SFML was built using system dependencies.
set(SFML_BUILT_USING_SYSTEM_DEPS OFF)

# start with an empty list
set(FIND_SFML_DEPENDENCIES_NOTFOUND)

if(SFML_BUILT_USING_SYSTEM_DEPS)
    find_dependency(Vorbis)
    find_dependency(FLAC)
else()
    find_dependency(Vorbis CONFIG PATHS "${CMAKE_CURRENT_LIST_DIR}/../../../")
    find_dependency(FLAC CONFIG PATHS "${CMAKE_CURRENT_LIST_DIR}/../../../")
endif()

if(FIND_SFML_DEPENDENCIES_NOTFOUND)
    set(FIND_SFML_ERROR "SFML found but some of its dependencies are missing (${FIND_SFML_DEPENDENCIES_NOTFOUND})")
    set(SFML_FOUND OFF)
endif()
