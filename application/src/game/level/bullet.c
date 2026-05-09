/*
    Copyright (c) Arnaud BANNIER and Nicolas BODIN.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "game/level/bullet.h"
#include "game/level/level_scene.h"
#include "utils/common.h"

Bullet *Bullet_create(LevelScene *scene, Vec2 position, Vec2 velocity, int type, float angle, int damage, int playerID)
{
    Bullet *self = (Bullet *)calloc(1, sizeof(Bullet));
    AssertNew(self);

    self->m_position = position;
    self->m_velocity = velocity;
    self->m_type = type;
    self->m_scene = scene;
    self->m_angle = angle;
    self->m_damage = damage;
    self->m_playerID = playerID;
    self->m_state = BULLET_STATE_ACTIVE;

    self->m_angle_direction = 0;

    AssetManager *assets = LevelScene_getAssetManager(scene);
    switch (type)
    {
    default:
    case BULLET_PLAYER_DEFAULT:
        //* TODO 10: Tir du joueur
        self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BULLET_PLAYER_DEFAULT);
        //*/
        self->m_extent = Vec2_set(8 * PIX_TO_WORLD, 16 * PIX_TO_WORLD);
        self->m_radius = 0.05f;
        self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 0.4f, -1);
        break;

    case BULLET_FIGHTER:
        // TODO 15 : Tir d'un enemie
        self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BULLET_FIGHTER);
        //*/
        self->m_extent = Vec2_set(8 * PIX_TO_WORLD, 16 * (1.0f / 48.0f));
        self->m_radius = 0.05f;
        self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 0.4f, -1);
        break;

    case BULLET_TORPEDO:
        // TODO 15 : Tir d'un enemie
        self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BULLET_TORPEDO);
        //*/
        self->m_extent = Vec2_set(8 * PIX_TO_WORLD, 16 * PIX_TO_WORLD);
        self->m_radius = 0.05f;
        self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 0.4f, -1);
        break;

    case BULLET_BATTLECRUISER:
        // TODO 15 : Tir d'un enemie
        self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BULLET_BATTLECRUISER);
        //*/
        self->m_extent = Vec2_set(32 * PIX_TO_WORLD, 64 * PIX_TO_WORLD);
        self->m_radius = 0.05f;
        self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 0.4f, -1);
        break;

    case BULLET_FRIGATE:
        // TODO 15 : Tir d'un enemie
        self->m_spriteSheet = AssetManager_getSpriteSheet(assets, SPRITE_BULLET_FRIGATE);
        //*/
        self->m_extent = Vec2_set(16 * PIX_TO_WORLD, 32 * PIX_TO_WORLD);
        self->m_radius = 0.05f;
        self->m_anim = SpriteAnim_create(self->m_spriteSheet->rectCount, 0.4f, -1);
        break;
    }


    return self;
}

void Bullet_destroy(Bullet *self)
{
    if (!self) return;

    SpriteAnim_destroy(self->m_anim);
    free(self);
}

void Bullet_update(Bullet *self)
{
    float delta = Timer_getDelta(g_time);

    Vec2 angle_direction_normal = { 0.0f, 1.0f };

    int id_joueur_proche = Player_is_arroundest(self);
    Player* joueur_proche = LevelScene_getPlayer(self->m_scene, id_joueur_proche);

    switch (self->m_type)
    {
    default:
        break;

    case BULLET_FIGHTER:
        // Met à jour la position
        //* TODO 10: Tir du joueur
        self->m_position = Vec2_add(
            self->m_position,
            Vec2_scale(self->m_velocity, delta)
        );
        break;
        //*/

    case BULLET_PLAYER_DEFAULT:
       // Met à jour la position
       //* TODO 10: Tir du joueur
        self->m_position = Vec2_add(
            self->m_position,
            Vec2_scale(self->m_velocity, delta)
        );
        break;
        //*/

    case BULLET_TORPEDO:
        Vec2 acceleration =
        {
            0,
            joueur_proche->m_position.y - self->m_position.y
        };

        self->m_velocity = Vec2_add(self->m_velocity, Vec2_scale(acceleration, delta));

        self->m_position = Vec2_add(
            self->m_position,
            Vec2_scale(self->m_velocity, delta)
        );

        self->m_angle = Vec2_signedAngleDeg(self->m_velocity, angle_direction_normal);
        break;

    case BULLET_BATTLECRUISER:
        // Met à jour la position
       //* TODO 10: Tir du joueur
        self->m_position = Vec2_add(
            self->m_position,
            Vec2_scale(self->m_velocity, delta)
        );

        break;
        //*/

    case BULLET_FRIGATE:
        Vec2 direction = {
            joueur_proche->m_position.x - self->m_position.x,
            joueur_proche->m_position.y - self->m_position.y
        };

        direction = Vec2_scale(Vec2_normalize(direction), 3);
        direction = Vec2_scale(direction, 1.5 * delta); // Ajuster la vitesse en fonction de delta
        self->m_position = Vec2_add(self->m_position, direction);

        self->m_angle = Vec2_signedAngleDeg(direction, angle_direction_normal);
        break;
    }
    

    // Met à jour l'animation
    if (self->m_anim) SpriteAnim_update(self->m_anim, delta);
}

void Bullet_render(Bullet *self)
{
    assert(self);
    LevelScene *scene = self->m_scene;
    AssetManager *assets = LevelScene_getAssetManager(scene);
    Camera *camera = LevelScene_getCamera(scene);

    float scale = Camera_getWorldToViewScale(camera);
    SDL_FRect dst = { 0 };
    dst.h = self->m_extent.y * scale;
    dst.w = self->m_extent.x * scale;
    Camera_worldToView(camera, self->m_position, &dst.x, &dst.y);
    dst.x -= 0.50f * dst.w;
    dst.y -= 0.50f * dst.h;

    SDL_Rect *src = NULL;
    SDL_Texture *texture = NULL;
    SpriteSheet *spriteSheet = NULL;
    int index = 0;

    //* TODO 10: Tir du joueur
    spriteSheet = self->m_spriteSheet;
    index = SpriteAnim_getFrameIndex(self->m_anim);
    src = &(spriteSheet->rects[index]);
    texture = spriteSheet->texture;
    SDL_RenderCopyExF(g_renderer, texture, src, &dst, self->m_angle, NULL, 0);
    //*/
}

void Bullet_drawGizmos(Bullet *self, Gizmos *gizmos)
{
    Gizmos_drawCircle(gizmos, self->m_position, self->m_radius);
}

int Player_is_arroundest(Bullet *self)
{
    int nb_player = LevelScene_getPlayerCount(self->m_scene);
    int id_player=0;
    float distance_min = 10000;
    float distance;
    for (int i = 0; i < nb_player; i++)
    {
        Player *joueur = LevelScene_getPlayer(self->m_scene, i);
        distance = Vec2_distanceSquared(self->m_position, joueur->m_position);
        if (distance_min > distance && joueur->m_state != PLAYER_STATE_DEAD)
        {
            id_player = i;
            distance_min = distance;
        }
    }
    return id_player;
}