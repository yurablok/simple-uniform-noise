# CMake Utils
# https://github.com/yurablok/cmake-cpp-template
#
# History:
# v0.2  2022-Dec-24     Added support for Windows ARM64.
# v0.1  2022-Oct-18     First release.

# Call it before the main `project(...)`
macro(check_build_directory)
    if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
        message(FATAL_ERROR "In-source builds not allowed. Please use a build directory.")
    else()
        message("${CMAKE_BINARY_DIR}")
    endif()
    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(default_build_type "RelWithDebInfo")
        set(CMAKE_BUILD_TYPE "RelWithDebInfo")
    endif()
endmacro(check_build_directory)

# Call it after the main `project(...)` and before any `add_subdirectory(...)`
# Example:
#   init_project("client --ip=localhost" "server")
function(init_project)
    if(NOT "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
        message(WARNING "CMakeUtils.cmake is not in the root.")
        return()
    endif()

    cmake_parse_arguments(arg "" "" "" "${ARGN}")
    if(NOT DEFINED arg_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "At least one target must be specified.")
    endif()

    if(NOT "${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" STREQUAL "")
        set(CMAKE_TARGET_ARCH ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})
    else()
        set(CMAKE_TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
    endif()
    message("CMAKE_TARGET_ARCH: ${CMAKE_TARGET_ARCH}")
    if("${CMAKE_TARGET_ARCH}" MATCHES "(arm|ARM|aarch).*")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(BUILD_ARCH "arm64")
        else()
            set(BUILD_ARCH "arm32")
        endif()
    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(BUILD_ARCH "x64")
        else()
            set(BUILD_ARCH "x32")
        endif()
    endif()

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(BUILD_TYPE "RelNoDebInfo")
    elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
        set(BUILD_TYPE "Release")
    else()
        set(BUILD_TYPE "${CMAKE_BUILD_TYPE}")
    endif()

    if(UNIX)
        if(APPLE)
            set(BUILD_PLATFORM "Apple")
        else()
            set(BUILD_PLATFORM "Linux")
        endif()
    elseif(WIN32)
        set(BUILD_PLATFORM "Windows")
    else()
        set(BUILD_PLATFORM "Unknown")
    endif()

    set(BUILD_FOLDER "${BUILD_ARCH}-${BUILD_TYPE}-${BUILD_PLATFORM}")
    message("BUILD_FOLDER: ${BUILD_FOLDER}")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${BUILD_FOLDER}" PARENT_SCOPE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${BUILD_FOLDER}" PARENT_SCOPE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${BUILD_FOLDER}" PARENT_SCOPE)

    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/build/.cmake")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/workdir")

    if("${CMAKE_SYSROOT}" STREQUAL "")
        #NOTE: clangd linter config
        file(WRITE ".clangd" "CompileFlags:\n  CompilationDatabase: build/.cmake/${BUILD_FOLDER}\n")
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    endif()

    if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        #NOTE: Link-Time Global Optimization
        cmake_policy(SET CMP0069 NEW)
        set(CMAKE_POLICY_DEFAULT_CMP0069 NEW PARENT_SCOPE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE PARENT_SCOPE)
    endif()

    if(MSVC)
        if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(
                /utf-8 # Set source and execution character sets to UTF-8
                /sdl # Enable Additional Security Checks
                /MP # Build with Multiple Processes
                /permissive-
                /wd4702 # Suppress Unreachable code
            )
        else()
            add_compile_options(-fcolor-diagnostics)
        endif()

        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            add_compile_options(
                /JMC # Just My Code Debugging
                /ZI # Debug Information with Edit and Continue
            )
            add_link_options(
                /INCREMENTAL # For Edit and Continue
            )
        endif()

        #set(CMAKE_CXX_FLAGS_RELEASE = "/MD /O2 /Ob2 /DNDEBUG")

        # The /Zi option produces a separate PDB file that contains all the symbolic
        # debugging information for use with the debugger. The debugging information
        # isn't included in the object files or executable, which makes them much smaller.
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob2 /DNDEBUG" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob2 /DNDEBUG" PARENT_SCOPE)

        __write_msvs_launch_vs_json("${arg_UNPARSED_ARGUMENTS}")

    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
        # -O3 -g0   3.4 MB  default Release
        # -O3 -g1   9.5 MB
        # -O2 -g1   9.3 MB
        # -O2 -g2  41.0 MB  default RelWithDebInfo
        # -O0 -g2  35.0 MB  default Debug
        # Level 1 produces minimal information, enough for making backtraces in parts
        # of the program that you don’t plan to debug. This includes descriptions of
        # functions and external variables, and line number tables, but no information
        # about local variables.
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g1 -DNDEBUG" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g1 -DNDEBUG" PARENT_SCOPE)
        
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            add_compile_options(-fdiagnostics-color=always)
        elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(-fcolor-diagnostics)
        endif()

    else()
        message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    set(BUILD_ARCH "${BUILD_ARCH}" PARENT_SCOPE)
    set(BUILD_TYPE "${BUILD_TYPE}" PARENT_SCOPE)
    set(BUILD_PLATFORM "${BUILD_PLATFORM}" PARENT_SCOPE)
    set(BUILD_FOLDER "${BUILD_FOLDER}" PARENT_SCOPE)
endfunction(init_project)


function(__write_msvs_launch_vs_json targets)
    set(cfgPath "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json")
    #set(cfgSize_b 0)
    #if(EXISTS "${cfgPath}")
    #    file(SIZE "${cfgPath}" cfgSize_b)
    #endif()
    #if(cfgSize_b GREATER 100)
    #    return()
    #endif()

    file(WRITE "${cfgPath}" "reload") # Reload the config by using some JSON-error

    set(json "")
    set(json "${json}{\n")
    set(json "${json}  \"NOTE\": \"This file was generated by CMakeUtils.cmake\",\n")
    set(json "${json}  \"version\": \"0.2.1\",\n")
    set(json "${json}  \"configurations\": [\n")

    macro(add appPath qtPath targetName targetArgs)
        string(REPLACE "/" "\\\\" targetPath "${CMAKE_SOURCE_DIR}/build/${appPath}/${targetName}")
        set(json "${json}    {\n")
        set(json "${json}      \"type\": \"default\",\n")
        set(json "${json}      \"project\": \"CMakeLists.txt\",\n")
        set(json "${json}      \"projectTarget\": \"${targetName}.exe (${targetPath}.exe)\",\n")
        set(json "${json}      \"name\": \"${targetName}\",\n")
        set(json "${json}      \"args\": [\n")
        set(json "${json}        \"${targetArgs}\"\n")
        set(json "${json}      ],\n")
        set(json "${json}      \"currentDir\": \"\${workspaceRoot}/workdir\",\n")
        set(json "${json}      \"env\": {\n")
        set(json "${json}        \"PATH\": \"\${env.PATH};${qtPath}/bin\"\n")
        set(json "${json}      }\n")
        set(json "${json}    },\n")
    endmacro(add)

    foreach(target ${targets})
        string(REGEX MATCH "^([_a-zA-Z0-9\\-]+)[ ]*(.*)$" matched "${target}")
        if(${CMAKE_MATCH_COUNT} EQUAL 0)
            message(FATAL_ERROR "Wrong target format (actual='${target}', expected='name args').")
        endif()

        set(targetName ${CMAKE_MATCH_1})
        set(targetArgs ${CMAKE_MATCH_2})
        message("targetName=${targetName} targetArgs=[${targetArgs}]")

        add("x32-Debug-Windows/Debug"              "${Qt5x32Path}" "${targetName}" "${targetArgs}")
        add("x32-Release-Windows/RelWithDebInfo"   "${Qt5x32Path}" "${targetName}" "${targetArgs}")
        add("x32-RelNoDebInfo-Windows/Release"     "${Qt5x32Path}" "${targetName}" "${targetArgs}")
        add("x64-Debug-Windows/Debug"              "${Qt5x64Path}" "${targetName}" "${targetArgs}")
        add("x64-Release-Windows/RelWithDebInfo"   "${Qt5x64Path}" "${targetName}" "${targetArgs}")
        add("x64-RelNoDebInfo-Windows/Release"     "${Qt5x64Path}" "${targetName}" "${targetArgs}")
        add("arm64-Debug-Windows/Debug"            "${Qt5x64Path}" "${targetName}" "${targetArgs}")
        add("arm64-Release-Windows/RelWithDebInfo" "${Qt5x64Path}" "${targetName}" "${targetArgs}")
        add("arm64-RelNoDebInfo-Windows/Release"   "${Qt5x64Path}" "${targetName}" "${targetArgs}")
    endforeach()

    set(json "${json}  ]\n")
    set(json "${json}}\n")

    file(WRITE "${cfgPath}" "${json}")
endfunction(__write_msvs_launch_vs_json)


function(copy_release_file_to_workdir frompath topath)
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        return()
    endif()

    if(MSVC)
        set(fullfrom "${CMAKE_SOURCE_DIR}/build/${BUILD_FOLDER}/${CMAKE_BUILD_TYPE}/${frompath}")
    else()
        set(fullfrom "${CMAKE_SOURCE_DIR}/build/${BUILD_FOLDER}/${frompath}")
    endif()
    set(fullto "${CMAKE_SOURCE_DIR}/workdir/bin_${BUILD_ARCH}/${topath}")

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${fullfrom}" "${fullto}"
    )
endfunction(copy_release_file_to_workdir)

macro(copy_release_app_to_workdir basename)
    if(WIN32)
        copy_release_file_to_workdir("${basename}.exe" "${basename}.exe")
    else()
        copy_release_file_to_workdir("${basename}" "${basename}")
    endif()
endmacro(copy_release_app_to_workdir)

macro(copy_release_lib_to_workdir basename)
    if(WIN32)
        copy_release_file_to_workdir("${basename}.dll" "${basename}.dll")
    else()
        copy_release_file_to_workdir("lib${basename}.so" "${basename}.so")
    endif()
endmacro(copy_release_lib_to_workdir)
