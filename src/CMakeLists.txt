## local variable for managing the gamelib target
set(GAMELIB "ASGEGameLib")

## glob is useful but not reliant in compilation so just use it for headers ##
file(GLOB_RECURSE HEADER_LIST
        LIST_DIRECTORIES FALSE
        CONFIGURE_DEPENDS

        ${CMAKE_SOURCE_DIR}/include/${GAMELIB}/*.hpp
        ${CMAKE_SOURCE_DIR}/include/${GAMELIB}/*.h
        *.hpp *.h)

## cpp files go here, [h]pp files are globbed so don't need to be explicitly added ##
set(GAMELIB_SOURCES
        # Driver
        ${GAMELIB}/Driver.cpp
        ${GAMELIB}/Scene.cpp
        ${GAMELIB}/Component.cpp
        ${GAMELIB}/HelperMethods.cpp

        # Driver Handlers
        ${GAMELIB}/handlers/InputHandler.cpp
        ${GAMELIB}/handlers/SceneHandler.cpp
        ${GAMELIB}/handlers/ThreadHandler.cpp
        ${GAMELIB}/handlers/RenderHandler.cpp
        ${GAMELIB}/handlers/AudioHandler.cpp
        ${GAMELIB}/handlers/CollisionHandler.cpp

        # Components
        ${GAMELIB}/components/tilemap/TiledBasedTileMap.cpp
        ${GAMELIB}/components/tilemap/StringBasedTileMap.cpp
        ${GAMELIB}/components/tilemap/PathFinder.cpp

        ${GAMELIB}/components/ui/GamePadInputUI.cpp

        ${GAMELIB}/components/animator/Animator.cpp

        ${GAMELIB}/components/persistence/FileStream.cpp

        ${GAMELIB}/components/gamepad/GamePad.cpp

        ${GAMELIB}/components/camera/Camera.cpp
        ${GAMELIB}/components/camera/HUD.cpp

        ${GAMELIB}/components/collision/Collider.cpp
        ${GAMELIB}/components/collision/HitResult.cpp
        ${GAMELIB}/components/collision/Raycast.cpp

        ${GAMELIB}/components/player/CharacterPicker.cpp
        ${GAMELIB}/components/player/NamePicker.cpp

        ${GAMELIB}/components/map/TileSquare.cpp

        # Containers
        ${GAMELIB}/containers/Vector2.cpp
        ${GAMELIB}/containers/Float4.cpp)

## create the game library
add_library(${GAMELIB} STATIC ${GAMELIB_SOURCES} "${HEADER_LIST}")
add_library(libs::${GAMELIB} ALIAS ${GAMELIB})
target_include_directories(${GAMELIB} PRIVATE ${CMAKE_SOURCE_DIR}/include/${GAMELIB} ${CMAKE_SOURCE_DIR}/src/${GAMELIB})
target_compile_options(${GAMELIB} PUBLIC $<$<STREQUAL:x${CMAKE_CXX_SIMULATE_ID},xMSVC>:/EHsc>)

## set the build target properties ##
set(LINK_LIBS "magic_enum::magic_enum" "soloud" "tmxlite")
configure_target(${GAMELIB} ${CMAKE_SOURCE_DIR}/include "${HEADER_LIST}" "${LINK_LIBS}" )
add_asge_dependency(${GAMELIB})
