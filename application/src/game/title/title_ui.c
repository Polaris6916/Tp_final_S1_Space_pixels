/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/title/title_scene.h"
#include "title_ui.h"


TitleUI *TitleUI_create(TitleScene *scene)
{
    TitleUI *self = (TitleUI *)calloc(1, sizeof(TitleUI));
    AssertNew(self);

    AssetManager *assets = TitleScene_getAssetManager(scene);
    self->m_scene = scene;

    TTF_Font *font = NULL;
    font = AssetManager_getFont(assets, FONT_MAIN_TITLE);
    self->m_textTitleMain = Text_create(g_renderer, font, "Space Pixels", g_colors.blue);

    font = AssetManager_getFont(assets, FONT_NORMAL);
    self->m_textSelectLevel = Text_create(g_renderer, font, "Select level", g_colors.white);
    char tab[32] = { 0 };
    self->nb_player = 2;
    sprintf(tab, u8"Number player : %d%", self->nb_player);
    self->m_nb_joueur = Text_create(g_renderer, font, tab, g_colors.white);
    self->sound = 1;
    self->m_sound = Text_create(g_renderer, font, "Music Sound: OK", g_colors.white);
    self->sound_anim = 1;
    self->m_SoundAnim = Text_create(g_renderer, font, "Anim Sound : OK", g_colors.white);
    self->m_textQuit = Text_create(g_renderer, font, "Quit", g_colors.white);

    // Set Text level
    char *levelNames[] = {
        "Level name 1",
        "Level Demo",
    };
    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        font = AssetManager_getFont(assets, FONT_NORMAL);
        self->m_textLevels[i] = Text_create(g_renderer, font, levelNames[i], g_colors.white);
    }

    // Set Text Type Player (navet) 
    char* TabType[] = {
        "Basic navet",
        "Dash navet",
    };

    //association des deux tableaux
    for (int i = 0; i < LEVEL_COUNT + NB_TYPE_PLAYER; i++)
    {
        if (i < LEVEL_COUNT)
        {
            font = AssetManager_getFont(assets, FONT_NORMAL);
            self->m_tab_reu[i] = Text_create(g_renderer, font, levelNames[i], g_colors.white);
        }
        else
        {
            font = AssetManager_getFont(assets, FONT_NORMAL);
            self->m_tab_reu[i] = Text_create(g_renderer, font, TabType[i-2], g_colors.white);
        }
        
    }


    // Set sous-titre
    char* TabBazard[] = {
    "Choose your level :",
    "Choose your navet :",
    };
    for (int i = 0; i < 2; i++)
    {
        font = AssetManager_getFont(assets, FONT_NORMAL);
        self->m_bazard[i] = Text_create(g_renderer, font, TabBazard[i], g_colors.white);
    }

    // Calcule les dimensions de certains textes
    int h, w;
    font = AssetManager_getFont(assets, FONT_MAIN_TITLE);
    TTF_SizeText(font, "Title", &w, &h);
    self->m_hTitleMain = h;

    font = AssetManager_getFont(assets, FONT_BIG);
    TTF_SizeText(font, "Title", &w, &h);
    self->m_hTitle = h;

    font = AssetManager_getFont(assets, FONT_NORMAL);
    TTF_SizeText(font, "Text", &w, &h);
    self->m_hTexts = h;

    return self;
}

void TitleUI_destroy(TitleUI *self)
{
    if (!self) return;

    Text_destroy(self->m_textTitleMain);
    Text_destroy(self->m_textSelectLevel);
    Text_destroy(self->m_nb_joueur);
    Text_destroy(self->m_textQuit);

    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        Text_destroy(self->m_textLevels[i]);
    }

    free(self);
}

void TitleUI_render(TitleUI *self)
{
    assert(self);

    TitleUI_renderBackground(self);

    if (self->m_pageID == 0)
    {
        TitleUI_renderMainPage(self);
    }
    else
    {
        TitleUI_renderLevelPage(self);
    }
}

void TitleUI_update(TitleUI *self)
{
    if (self->m_pageID == 0)
    {
        TitleUI_updateMainPage(self);
    }
    else
    {
        TitleUI_updateLevelPage(self);
    }

}

void TitleUI_drawGizmos(TitleUI *self, Gizmos *gizmos)
{
}

void TitleUI_updateMainPage(TitleUI *self)
{
    TitleScene *scene = self->m_scene;
    AssetManager *assets = TitleScene_getAssetManager(scene);
    Input *input = TitleScene_getInput(scene);
    GameConfig *gameConfig = TitleScene_getGameConfig(scene);

    // Modifie la s�lection
    int prevSelection = self->m_selection;
    if (input->upPressed)
    {
        self->m_selection--;
    }
    if (input->downPressed)
    {
        self->m_selection++;
    }

    self->m_selection = Int_clamp(self->m_selection, 0, 4);
    if (prevSelection != self->m_selection)
    {
        Game_playSoundFX(assets, SOUND_UI_SELECT);
    }

    // Couleurs des textes s�lectionnables
    Text *texts[5] = {
        self->m_textSelectLevel,
        self->m_nb_joueur,
        self->m_sound,
        self->m_SoundAnim,
        self->m_textQuit
    };
    for (int i = 0; i < 5; i++)
    {
        if (i == self->m_selection)
        {
            Text_setColor(texts[i], g_colors.white);
        }
        else
        {
            Text_setColor(texts[i], g_colors.whiteSemi);
        }
    }

    // Action du joueur sur la s�lection
    if (input->validatePressed)
    {
        if (self->m_selection == 0)
        {
            // On va vers la page de s�lection du niveau
            self->m_pageID = 1;
            self->m_selection = 0;
        }
        else if (self->m_selection == 1)
        {
            if (gameConfig->playerCount == 2)
            {
                gameConfig->playerCount = 1;
                self->nb_player = 1;
            }
            else
            {
                gameConfig->playerCount = 2;
                self->nb_player = 2;
            }
            TTF_Font* font = NULL;
            font = AssetManager_getFont(assets, FONT_NORMAL);
            char tab[32] = { 0 };
            sprintf(tab, u8"Number player : %d%", self->nb_player);
            self->m_nb_joueur = Text_create(g_renderer, font, tab, g_colors.white);
        }
        else if (self->m_selection == 2)
        {
            if (self->sound == 0)
            {
                self->sound = 1;
                TTF_Font* font = NULL;
                font = AssetManager_getFont(assets, FONT_NORMAL);
                self->m_sound = Text_create(g_renderer, font, "Music Sound : OK", g_colors.white);
                Game_setMusicVolume(1.0);
            }
            else
            {
                self->sound = 0;
                TTF_Font* font = NULL;
                font = AssetManager_getFont(assets, FONT_NORMAL);
                self->m_sound = Text_create(g_renderer, font, "Music Sound : pas OK", g_colors.white);
                Game_setMusicVolume(0.0);
            }
        }
        else if (self->m_selection == 3)
        {
            if (self->sound_anim == 0)
            {
                self->sound_anim = 1;
                TTF_Font* font = NULL;
                font = AssetManager_getFont(assets, FONT_NORMAL);
                self->m_SoundAnim = Text_create(g_renderer, font, "Anim Sound : OK", g_colors.white);
                gameConfig->sound_tirEnemy = 1;
                for (int i = 0; i < nb_sound_anim; i++)
                {
                    Game_setSoundVolume(assets, i, 1.0);
                }
            }
            else
            {
                self->sound_anim = 0;
                TTF_Font* font = NULL;
                font = AssetManager_getFont(assets, FONT_NORMAL);
                self->m_SoundAnim = Text_create(g_renderer, font, "Anim Sound : pas OK", g_colors.white);
                gameConfig->sound_tirEnemy = 0;
                for (int i = 0; i < nb_sound_anim; i++)
                {
                    Game_setSoundVolume(assets, i, 0.0);
                }
            }
        }
        else if (self->m_selection == 4)
        {
            // On quitte le jeu
            gameConfig->nextScene = GAME_SCENE_QUIT;
            TitleScene_quit(scene);
        }
        Game_playSoundFX(assets, SOUND_UI_VALIDATE);
    }
}

void TitleUI_updateLevelPage(TitleUI *self)
{
    TitleScene *scene = self->m_scene;
    AssetManager *assets = TitleScene_getAssetManager(scene);
    Input *input = TitleScene_getInput(scene);
    GameConfig *gameConfig = TitleScene_getGameConfig(scene);

    // Modifie la s�lection
    int prevSelection = self->m_selection;
    if (input->upPressed)
    {
        self->m_selection--;
    }
    if (input->downPressed)
    {
        self->m_selection++;
    }

    self->m_selection = Int_clamp(self->m_selection, 0, LEVEL_COUNT + NB_TYPE_PLAYER-1);
    if (prevSelection != self->m_selection)
    {
        Game_playSoundFX(assets, SOUND_UI_SELECT);
    }

    // Couleurs des textes s�lectionnables (level)
    for (int i = 0; i < 4; i++)
    {
        //if ((self->m_selection < LEVEL_COUNT))
        //{
            if (i == self->m_selection)
            {
                Text_setColor(self->m_tab_reu[i], g_colors.green);
            }
            else
            {
                Text_setColor(self->m_tab_reu[i], g_colors.red);
            }
        //}

    }

    if (self->m_selection < LEVEL_COUNT)
    {
          // Action du joueur sur la s�lection
        if (input->validatePressed)
        {
            // On commence le niveau
            gameConfig->nextScene = GAME_SCENE_LEVEL;
            gameConfig->levelID = self->m_selection;
            TitleScene_quit(scene);
            Game_playSoundFX(assets, SOUND_UI_VALIDATE);
        }
        else if (input->cancelPressed)
        {
            // On retroune vers la page principale
            self->m_pageID = 0;
            self->m_selection = 0;
            Game_playSoundFX(assets, SOUND_UI_CANCEL);
        }
    }
    else if (self->m_selection >= LEVEL_COUNT)
    {
        // Action du joueur sur la s�lection 
        if (input->validatePressed)
        {
            if (self->m_selection == 2)
            {
                gameConfig->playerType = self->m_selection - LEVEL_COUNT;
                Text_setColor(self->m_tab_reu[self->m_selection], g_colors.blue);
                Text_setColor(self->m_tab_reu[self->m_selection+1], g_colors.white);
                Game_playSoundFX(assets, SOUND_UI_VALIDATE);
            }
            else if (self->m_selection == 3)
            {
                gameConfig->playerType = self->m_selection - LEVEL_COUNT;
                Text_setColor(self->m_tab_reu[self->m_selection], g_colors.blue);
                Text_setColor(self->m_tab_reu[self->m_selection - 1], g_colors.white);
                Game_playSoundFX(assets, SOUND_UI_VALIDATE);
            }
        }
        else if (input->cancelPressed)
        {
            // On retroune vers la page principale
            self->m_pageID = 0;
            self->m_selection = 0;
            Game_playSoundFX(assets, SOUND_UI_CANCEL);
        }
    }
}

void TitleUI_renderBackground(TitleUI *self)
{
    assert(self);
    TitleScene *scene = self->m_scene;
    AssetManager *assets = TitleScene_getAssetManager(scene);
    Camera *camera = TitleScene_getCamera(scene);
    SpriteSheet *spriteSheet = NULL;
    float scale = Camera_getWorldToViewScale(camera);

    SDL_FRect dstFRect = { 0 };
    dstFRect.w = 1024 * PIX_TO_WORLD * scale;
    dstFRect.h = 1024 * PIX_TO_WORLD * scale;

    dstFRect.x = 0;
    spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BACKGROUND_PURPLE_NEBULA);
    SpriteSheet_renderCopyF(spriteSheet, 0, g_renderer, &dstFRect, 0.0, NULL, 0);

    dstFRect.x += dstFRect.w;
    SpriteSheet_renderCopyF(spriteSheet, 0, g_renderer, &dstFRect, 0.0, NULL, 0);
}

void TitleUI_renderMainPage(TitleUI *self)
{
    const int xPadding = 20;
    const int yPadding = 10;
    const int wUI = Game_getWidth() / 2;
    const int hUI = self->m_hTitleMain + 2 * self->m_hTexts + 2 * yPadding;
    int x = Game_getWidth() / 2;
    int y = (Game_getHeight() - hUI) / 2;

    SDL_Rect dstRect = { 0 };
    SDL_Texture *texture = NULL;

    dstRect.w = wUI;
    dstRect.h = hUI+110;
    dstRect.x = x - dstRect.w / 2;
    dstRect.y = y;
    Game_setRenderDrawColor(g_colors.cyan, 127);
    SDL_RenderFillRect(g_renderer, &dstRect);

    y += yPadding;

    int w = 0, h = 0;
    texture = Text_getTexture(self->m_textTitleMain);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dstRect.x = x - w / 2;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);

    y += self->m_hTitleMain;

    Text *texts[5] = {
        self->m_textSelectLevel,
        self->m_nb_joueur,
        self->m_sound,
        self->m_SoundAnim,
        self->m_textQuit
    };
    for (int i = 0; i < 5; i++)
    {
        texture = Text_getTexture(texts[i]);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        dstRect.x = x - w / 2;
        dstRect.y = y;
        dstRect.w = w;
        dstRect.h = h;
        SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);

        y += self->m_hTexts;
    }
}

void TitleUI_renderLevelPage(TitleUI *self)
{
    const int xPadding = 20;
    const int yPadding = 10;
    const int wUI = Game_getWidth() / 2;
    const int hUI = self->m_hTitleMain + 2 * self->m_hTexts + 2 * yPadding;
    int x = Game_getWidth() / 2;
    int y = (Game_getHeight() - hUI) / 2;

    SDL_Rect dstRect = { 0 };
    SDL_Texture* texture = NULL;

    dstRect.w = wUI;
    dstRect.h = hUI +40 ;
    dstRect.x = x - dstRect.w / 2;
    dstRect.y = y;
    Game_setRenderDrawColor(g_colors.cyan, 127);
    SDL_RenderFillRect(g_renderer, &dstRect);

    y += yPadding;

    int w = 0, h = 0;
    texture = Text_getTexture(self->m_textTitleMain);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dstRect.x = x - w / 2;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);
    y += self->m_hTitleMain;


    //Affichage des sous-titres
    x = Game_getWidth() / 2 - 450;
    y = (Game_getHeight() - hUI) / 2 + 100;

    texture = Text_getTexture(self->m_bazard[1]);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);

    texture = Text_getTexture(self->m_bazard[0]);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    dstRect.x = x +500;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
    SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);
    y += self->m_hTitleMain;

    //*Affichage des choix
    //Affichage les Levels
    y = (Game_getHeight() - hUI) / 2 + 100 + self->m_hTexts;
    for (int i = 0; i < LEVEL_COUNT + NB_TYPE_PLAYER; i++)
    {
        if (i < LEVEL_COUNT)
        {
            texture = Text_getTexture(self->m_tab_reu[i]);
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            dstRect.x = x + 550;
            dstRect.y = y;
            dstRect.w = w;
            dstRect.h = h;
            SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);

            y += self->m_hTexts;
        }
        else
        {
            texture = Text_getTexture(self->m_tab_reu[i]);
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            dstRect.x = x + 50;
            dstRect.y = y - 75;
            dstRect.w = w;
            dstRect.h = h;
            SDL_RenderCopy(g_renderer, texture, NULL, &dstRect);

            y += self->m_hTexts;
        }
    }
}