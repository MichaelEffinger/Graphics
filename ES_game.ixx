module;
#if WE_LACK_IMPORT_STD
#include <filesystem>
#endif

export module ES_game;

#if WE_HAVE_IMPORT_STD
import std;
#endif

import ES_easy;

export namespace ES::game {

    std::filesystem::path get_game_root();



}


std::filesystem::path ES::game::get_game_root() {
    return
#ifdef ES_GAME_ROOT_DIR
    {ES_GAME_ROOT_DIR}
#else
    ES::easy::where_am_I()
#endif
    ;
}
