#pragma once

enum class WindowedMode { WINDOWED, FULLSCREEN };


enum class PhysicsObjectType {
    UNDEFINED,
    GAME_OBJECT,
    GLASS,
    DOOR,
    WINDOW,
    SCENE_MESH,
    RAGDOLL_RIGID,
    CSG_OBJECT_ADDITIVE,
    CSG_OBJECT_SUBTRACTIVE,
    LIGHT,
    HEIGHT_MAP,
};
