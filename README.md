Another Fairygui runtime basically copied from https://github.com/u0u0/Cocos2d-Lua-Community but compatible with our cocos2d-x branch. It should also work with other v3 branches with minor changes.

Install in cocos2d-x project:

    git submodule add https://github.com/WuJiayiSH/FairyGUI-cocos2dx-runtime.git frameworks\FairyGUI-cocos2dx-runtime

Update your CMakeLists.txt:

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/frameworks/FairyGUI-cocos2dx-runtime)
    include(${CMAKE_CURRENT_SOURCE_DIR}/frameworks/FairyGUI-cocos2dx-runtime/CMakeLists.txt)
    set(GAME_HEADER ${GAME_HEADER} ${FAIRYGUI_HEADER})
    set(GAME_SOURCE ${GAME_SOURCE} ${FAIRYGUI_SRC})

Register lua bindings in AppDelegate.cpp(Lua specific):

    #include "lua_fairygui.hpp"
    ...
    register_fairygui_manual(luaState);

You're good to go but might need this line to get UI music work(Lua specific):

    fairygui.UIConfig.onMusicCallback = function(path)
        audio.playSound(path)
    end
