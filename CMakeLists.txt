cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
project(learn-opengl)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(LEARN_OPENGL_CONTROL_HEADERS
    src/Control/GamePlay.h
)

set(LEARN_OPENGL_CONTROL_SOURCES
    src/Control/GamePlay.cpp
)

source_group("Control" FILES ${LEARN_OPENGL_CONTROL_HEADERS} ${LEARN_OPENGL_CONTROL_SOURCES})

set(LEARN_OPENGL_GRAPHICS_HEADERS
    src/Graphics/Application.h
    src/Graphics/CameraObject.h
    src/Graphics/GfxConfigs.h
    src/Graphics/Graphics.h
    src/Graphics/LightObject.h
    src/Graphics/Material.h
    src/Graphics/Mesh.h
    src/Graphics/PhongPipeline.h
    src/Graphics/Pipeline.h
    src/Graphics/Texture.h
    src/Graphics/VertexAttributes.h
)

set(LEARN_OPENGL_GRAPHICS_SOURCES
    src/Graphics/Application.cpp
    src/Graphics/Graphics.cpp
    src/Graphics/Mesh.cpp
    src/Graphics/PhongPipeline.cpp
    src/Graphics/Pipeline.cpp
    src/Graphics/Texture.cpp
    src/Graphics/VertexAttributes.cpp
)

source_group("Graphics" FILES ${LEARN_OPENGL_GRAPHICS_HEADERS} ${LEARN_OPENGL_GRAPHICS_SOURCES})

set(LEARN_OPENGL_MATH_HEADERS
    src/Math/mat2.h
    src/Math/mat3.h
    src/Math/mat4.h
    src/Math/Mathf.h
    src/Math/Quaternion.h
    src/Math/vec2.h
    src/Math/vec3.h
    src/Math/vec4.h
)

source_group("Math" FILES ${LEARN_OPENGL_MATH_HEADERS})

set(LEARN_OPENGL_RESOURCES_HEADERS
    src/Resources/File.h
    src/Resources/Image.h
)

set(LEARN_OPENGL_RESOURCES_SOURCES
    src/Resources/File.cpp
    src/Resources/Image.cpp
)

source_group("Resources" FILES ${LEARN_OPENGL_RESOURCES_HEADERS} ${LEARN_OPENGL_RESOURCES_SOURCES})

set(LEARN_OPENGL_SCENE_HEADERS
    src/Scene/Component.h
    src/Scene/Node.h
    src/Scene/Transform.h
)

set(LEARN_OPENGL_SCENE_SOURCES
    src/Scene/Component.cpp
    src/Scene/Node.cpp
    src/Scene/Transform.cpp
)

source_group("Scene" FILES ${LEARN_OPENGL_SCENE_HEADERS} ${LEARN_OPENGL_SCENE_SOURCES})

set(LEARN_OPENGL_UTILITIES_HEADERS
    src/Utilities/Array.h
    src/Utilities/HashMap.h
    src/Utilities/Pointer.h
    src/Utilities/String.h
    src/Utilities/Traits.h
)

source_group("Utilities" FILES ${LEARN_OPENGL_UTILITIES_HEADERS})

add_executable(GfxAttempt
    ${LEARN_OPENGL_CONTROL_HEADERS}
    ${LEARN_OPENGL_CONTROL_SOURCES}
    ${LEARN_OPENGL_GRAPHICS_HEADERS}
    ${LEARN_OPENGL_GRAPHICS_SOURCES}
    ${LEARN_OPENGL_MATH_HEADERS}
    ${LEARN_OPENGL_MATH_SOURCES}
    ${LEARN_OPENGL_RESOURCES_HEADERS}
    ${LEARN_OPENGL_RESOURCES_SOURCES}
    ${LEARN_OPENGL_SCENE_HEADERS}
    ${LEARN_OPENGL_SCENE_SOURCES}
    ${LEARN_OPENGL_UTILITIES_HEADERS}
    src/TestPlay.h
    src/TestPlay.cpp
    src/main.cpp
)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(GfxAttempt
    PRIVATE glad::glad
    PRIVATE glfw
)
