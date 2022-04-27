## local variable for managing the game target ##
set(GAME ${PROJECT_NAME})

## glob is useful but not reliant in compilation so just use it for headers ##
file(GLOB_RECURSE HEADER_LIST
        LIST_DIRECTORIES FALSE
        CONFIGURE_DEPENDS

        # ty olly
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h

        # Helper stuff
        helper/ZOrderData.hpp

        # 'Interfaces', because they can't go anywhere else (i don't think...)
        gameobjects/mapstuff/IInteractable.hpp
        gameobjects/mapstuff/IHackable.hpp
        gameobjects/mapstuff/IPickable.hpp
        gameobjects/mapstuff/IKeyUnlockable.hpp)

list(APPEND HEADER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

## source files get listed here ##
set(GAME_SOURCES
        # --- The carry ---
        main.cpp

        # --- Scenes ---
        scenes/MainMenuScene.cpp
        scenes/LevelSelectScene.cpp
        scenes/CharacterSelectScene.cpp
        scenes/GameScene.cpp
        scenes/EndOfLevelScene.cpp
        scenes/GameData.cpp

        scenes/levels/LevelBase.cpp
        scenes/levels/Level1.cpp
        scenes/levels/Level2.cpp
        scenes/levels/Level3.cpp

        # --- Gameobjects ---
        # Characters
        gameobjects/characters/CharacterBase.cpp
        gameobjects/characters/PlayerCharacter.cpp
        gameobjects/characters/NonPlayerCharacter.cpp

        gameobjects/characters/playercharacters/Hacker.cpp
        gameobjects/characters/playercharacters/Thief.cpp
        gameobjects/characters/playercharacters/Marksman.cpp
        gameobjects/characters/playercharacters/Bruiser.cpp

        gameobjects/characters/nonplayercharacters/Guard.cpp
        gameobjects/characters/nonplayercharacters/Civilian.cpp

        # Map Stuff
        gameobjects/mapstuff/MapObject.cpp
        gameobjects/mapstuff/MapManager.cpp

        gameobjects/mapstuff/alarm/Alarm.cpp
        gameobjects/mapstuff/alarm/AlarmManager.cpp

        gameobjects/mapstuff/doors/DoorBase.cpp
        gameobjects/mapstuff/doors/AutomaticDoor.cpp
        gameobjects/mapstuff/doors/ManualDoor.cpp
        gameobjects/mapstuff/doors/LockedDoor.cpp

        gameobjects/mapstuff/camera/SecurityCamera.cpp

        gameobjects/mapstuff/tripwire/TripWire.cpp

        gameobjects/mapstuff/collectables/CollectableBase.cpp
        gameobjects/mapstuff/collectables/PickupableCollectable.cpp

        gameobjects/mapstuff/collectables/consumables/MedKit.cpp
        gameobjects/mapstuff/collectables/consumables/MissionObjectiveObject.cpp

        gameobjects/mapstuff/collectables/weapons/WeaponBase.cpp
        gameobjects/mapstuff/collectables/weapons/Pistol.cpp
        gameobjects/mapstuff/collectables/weapons/Rifle.cpp

        gameobjects/mapstuff/collectables/key/Key.cpp

        gameobjects/mapstuff/computer/ComputerTerminal.cpp

        gameobjects/mapstuff/bush/Bush.cpp

        gameobjects/mapstuff/endpoint/EndPoint.cpp

        gameobjects/mapstuff/vent/Vent.cpp

        # Projectiles
        gameobjects/projectiles/ProjectileBase.cpp
        gameobjects/projectiles/KnockOutDart.cpp
        gameobjects/projectiles/Bullet.cpp

        # Misc
        gameobjects/misc/Timer.cpp

        # Managers
        gameobjects/managers/MusicManager.cpp

        # --- Testing ---
        testing/BenTestScene.cpp
        testing/JamesTestScene.cpp
        testing/MatteoTestScene.cpp
        testing/NorbertTestScene.cpp)

## the ASGE Game, which spawns the Game window ##
add_executable(${GAME} ${HEADER_LIST} ${GAME_SOURCES})

## configure the target. Dependency targets go in the LIBS variable i.e. soloud ##
list(APPEND LIBS "ASGEGameLib" "soloud" "magic_enum::magic_enum" "tmxlite")
target_include_directories(${GAME} PRIVATE ${HEADER_FOLDER})
configure_target(${GAME} "${HEADER_FOLDER}" "${HEADER_LIST}" "${LIBS}")
add_asge_dependency(${GAME})

## icon support, because why not ##
if (WIN32)
    set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/game.rc)
elseif (APPLE)
    set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/game.icns)
    set_source_files_properties(
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/game.icns PROPERTIES
            MAXOSX_PACKAGE_LOCATION "resources")
endif()

## game data ##
add_gamedata(${GAME} ${CMAKE_CURRENT_SOURCE_DIR}/data)
add_dependencies(${GAME} ${GAME}-GD)