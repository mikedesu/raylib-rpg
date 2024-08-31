#include "TitleScene.h"
#include "mPrint.h"
#include <string>

using namespace std;

void TitleScene_create(TitleScene& t) {
    mPrint("TitleScene_create");
    TitleScene_set_control_mode(t, CONTROL_MODE_PLAYER);
    TitleScene_set_texture_filepath(t, "title_textures.txt");
    TitleScene_set_scene_transition(t, SCENE_TRANSITION_IN);
    TitleScene_set_scene_type(t, SCENE_TYPE_TITLE);
}

void TitleScene_set_control_mode(TitleScene& t, control_mode mode) {
    t.controlmode = mode;
}
control_mode TitleScene_get_control_mode(TitleScene& t) {
    return t.controlmode;
}

void TitleScene_set_texture_filepath(TitleScene& t, string filepath) {
    t.texture_filepath = filepath;
}
string TitleScene_get_texture_filepath(TitleScene& t) {
    return t.texture_filepath;
}

void TitleScene_set_scene_transition(TitleScene& t, SceneTransition transition) {
    t.transition = transition;
}

SceneTransition TitleScene_get_scene_transition(TitleScene& t) {
    return t.transition;
}

void TitleScene_set_scene_type(TitleScene& t, SceneType type) {
    t.scenetype = type;
}

SceneType TitleScene_get_scene_type(TitleScene& t) {
    return t.scenetype;
}

void TitleScene_destroy(TitleScene& t) { }

bool TitleScene_get_has_been_initialized(TitleScene& t) {
    return t.has_been_initialized;
}

void TitleScene_set_has_been_initialized(TitleScene& t, bool value) {
    t.has_been_initialized = value;
}

bool TitleScene_get_debug_panel_on(TitleScene& t) {
    return t.debug_panel_on;
}

void TitleScene_flip_debug_panel(TitleScene& t) {
    t.debug_panel_on = !t.debug_panel_on;
}

void TitleScene_set_camera_default_values(TitleScene& t) {
    t.camera2d.target = (Vector2){0, 0};
    t.camera2d.offset = (Vector2){0, 0};
    t.camera2d.rotation = 0.0f;
    t.camera2d.zoom = 1.0f;
}

void TitleScene_load_fonts(TitleScene& t) {
    //t.global_font = LoadFontEx("resources/fonts/Ubuntu-Regular.ttf", t.global_font_size, 0, 250);
    t.global_font = LoadFontEx("fonts/hack.ttf", t.global_font_size, 0, 250);
}

bool TitleScene_init(TitleScene& t) {
    if(!TitleScene_get_has_been_initialized(t)) {
        TitleScene_set_camera_default_values(t);
        TitleScene_load_fonts(t);
        TitleScene_set_texture_filepath(t, "title_textures.txt");
        bool result = TitleScene_load_textures(t);
        if(!result) {
            mPrint("Error loading textures. Exiting...");
            return false;
        }
        t.prerendered_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        t.src_rect = {0,
                      0,
                      (float)t.prerendered_texture.texture.width,
                      (float)-t.prerendered_texture.texture.height};
        t.dst_rect = {0,
                      0,
                      (float)t.prerendered_texture.texture.width,
                      (float)t.prerendered_texture.texture.height};
        TitleScene_prerender_texture(t);
        TitleScene_set_has_been_initialized(t, true);
    }
    return true;
}

void TitleScene_draw(TitleScene& t) {
    BeginMode2D(t.camera2d);
    Color clear_color = BLACK;
    ClearBackground(clear_color);
    DrawTexturePro(t.prerendered_texture.texture, t.src_rect, t.dst_rect, t.origin, 0.0f, WHITE);
    EndMode2D();
    if(TitleScene_get_debug_panel_on(t)) {
        DrawFPS(GetScreenWidth() - 80, 10);
        TitleScene_draw_debug_panel(t);
    }
    TitleScene_incr_current_frame(t);
}

void TitleScene_incr_current_frame(TitleScene& t) {
    t.current_frame++;
}

void TitleScene_handle_input(TitleScene& t) {
    if(IsKeyPressed(KEY_D)) {
        TitleScene_flip_debug_panel(t);
    } else if(IsKeyPressed(KEY_SPACE)) {
        mPrint("Space key pressed.");
        TitleScene_set_scene_transition(t, SCENE_TRANSITION_OUT);
    }
}

void TitleScene_draw_debug_panel(TitleScene& t) {
    string camera_info_str = "Current Frame: " + to_string(t.current_frame) + "\n" +
                             "Control mode: " + to_string(t.controlmode) + "\n" +
                             "Camera target: " + to_string(t.camera2d.target.x) + ", " +
                             to_string(t.camera2d.target.y) + "\n" + "TitleScene";
    const int fontsize = t.global_font.baseSize;
    DrawTextEx(TitleScene_get_global_font(t),
               camera_info_str.c_str(),
               (Vector2){10, 10},
               fontsize,
               0.5f,
               WHITE);
}

void TitleScene_close(TitleScene& t) {
    mPrint("TitleScene_close");
    for(auto& a : t.textures)
        UnloadTexture(a.second->texture);
    t.textures.clear();
    t.sprites.clear();
    t.entity_ids.clear();
    UnloadFont(t.global_font);
    if(t.music != NULL) {
        Mix_PauseMusic();
        Mix_FreeMusic(t.music);
    }
    t.has_been_initialized = false;
    UnloadRenderTexture(t.prerendered_texture);
    mPrint("End of TitleScene_close");
}

void TitleScene_cleanup(TitleScene& t) { }
void TitleScene_update(TitleScene& t) { }

void TitleScene_prerender_texture(TitleScene& t) {
    int scale = 1;
    float x = 0, y = 0;
    Vector2 origin = (Vector2){0, 0};
    Color clear_color = BLACK;
    shared_ptr<texture_info> title_info = t.textures["title"];
    shared_ptr<texture_info> presents_info = t.textures["presents"];
    float w_src = title_info->texture.width, h_src = title_info->texture.height;
    float w_dst = w_src, h_dst = h_src;
    Rectangle src_rect = {0, 0, w_src, h_src}, dst_rect = {0, 0, w_dst, h_dst};
    BeginDrawing();
    BeginTextureMode(t.prerendered_texture);
    BeginMode2D(t.camera2d);
    ClearBackground(clear_color);
    if(title_info == nullptr) {
        mPrint("Error: title_info is null.");
        return;
    } else if(presents_info == nullptr) {
        mPrint("Error: presents_info is null.");
        return;
    }
    while(w_dst < GetScreenWidth() && h_dst < GetScreenHeight()) {
        scale *= 2;
        w_dst *= 2;
        h_dst *= 2;
    }
    w_dst /= 2;
    h_dst /= 2;
    scale /= 2;
    x = GetScreenWidth() / 2.0f - w_dst / 2.0f;
    y = GetScreenHeight() / 2.0f - h_dst;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.width = w_dst;
    dst_rect.height = h_dst;
    DrawTexturePro(title_info->texture, src_rect, dst_rect, origin, 0, WHITE);
    w_src = presents_info->texture.width;
    h_src = presents_info->texture.height;
    src_rect = {0, 0, w_src, h_src};
    dst_rect.y = y - h_src * scale / 4;
    dst_rect.width = w_src * scale / 4;
    dst_rect.height = h_src * scale / 4;
    DrawTexturePro(presents_info->texture, src_rect, dst_rect, origin, 0, WHITE);
    EndMode2D();
    EndTextureMode();
    EndDrawing();
}

bool TitleScene_load_texture(TitleScene& t,
                             const char* asset_name,
                             const char* asset_path,
                             const int num_frames,
                             const int contexts,
                             const int is_player,
                             const int width,
                             const int height) {

    string asset_name_str = string(asset_name);
    Texture2D tt = LoadTexture(asset_path);
    texture_info tx;
    if(tt.id == 0) {
        mPrint("Error loading texture: " + string(asset_path));
        return false;
    }
    if(asset_name_str.find("tile-") != string::npos) {
        //  apply custom dithering
        Image img = LoadImageFromTexture(tt);
        ImageDither(&img, 4, 4, 4, 4);
        Texture2D ttt = LoadTextureFromImage(img);
        UnloadImage(img);
        tt = ttt;
    }
    tx.texture = tt;
    tx.num_frames = num_frames;
    tx.contexts = contexts;
    tx.is_player = is_player;
    tx.asset_path = asset_path;
    tx.width = width;
    tx.height = height;
    t.textures[asset_name] = make_shared<texture_info>(tx);
    return true;
}

bool TitleScene_load_textures(TitleScene& t) {
    FILE* file = nullptr;
    char line[256];
    char *asset_name = nullptr, *num_frames = nullptr, *contexts = nullptr, *is_player = nullptr,
         *width = nullptr, *height = nullptr, *asset_path = nullptr;
    bool retval = false;
    if(t.texture_filepath == "") {
        mPrint("Error: texture_filepath not set.");
    } else {
        file = fopen(t.texture_filepath.c_str(), "r");
        if(file == NULL) {
            mPrint("Error opening file: " + t.texture_filepath);
        } else {
            while(fgets(line, sizeof(line), file)) {
                // if the line begins with a hashtag, it is a comment
                if(line[0] == '#') {
                    continue;
                }
                asset_name = strtok(line, " ");
                num_frames = strtok(NULL, " ");
                contexts = strtok(NULL, " ");
                is_player = strtok(NULL, " ");
                width = strtok(NULL, " ");
                height = strtok(NULL, " ");
                asset_path = strtok(NULL, " ");
                // check to see if asset path has newline at end
                if(asset_path[strlen(asset_path) - 1] == '\n') {
                    asset_path[strlen(asset_path) - 1] = '\0';
                }
                bool result = TitleScene_load_texture(t,
                                                      asset_name,
                                                      asset_path,
                                                      atoi(num_frames),
                                                      atoi(contexts),
                                                      atoi(is_player),
                                                      atoi(width),
                                                      atoi(height));
                if(!result) {
                    mPrint("Error loading texture: " + string(asset_path));
                }
            }
            fclose(file);
            retval = true;
        }
    }
    return retval;
}

Font& TitleScene_get_global_font(TitleScene& t) {

    return t.global_font;
}

//void TitleScene_set_popup_manager(TitleScene& t, shared_ptr<PopupManager> pm) {
//void TitleScene_set_popup_manager(TitleScene& t, PopupManager& pm) {
//    t.popup_manager = pm;
//}

void TitleScene_set_id(TitleScene& t, Scene_id id) {
    t.id = id;
}

Scene_id TitleScene_get_id(TitleScene& t) {
    return t.id;
}

float TitleScene_get_alpha(TitleScene& t) {
    return t.alpha;
}

void TitleScene_set_alpha(TitleScene& t, float a) {
    t.alpha = a;
}
