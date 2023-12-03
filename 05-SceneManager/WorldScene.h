#pragma once
#include "Scene.h"
#include "Game.h"

#include "GameObject.h"
#include "Map.h"

#include "Mario.h"
#include "WorldPlayer.h"
#include "PlayScene.h"

class CWorldScene :
    public CScene
{
    CMario* mario = NULL;
    CWorldPlayer* player = NULL;

    int gameTimeRemain = 0;
    bool isTurnOnCamY = false;

    void _ParseSection_SPRITES(string line);
    void _ParseSection_ANIMATIONS(string line);

    void _ParseSection_ASSETS(string line);
    void _ParseSection_OBJECTS(string line);
    void _ParseSection_TILEMAP(string line);

    void LoadAssets(LPCWSTR assetFile);
public:
    CWorldScene(int id, LPCWSTR filePath);

    virtual void Load();

    Map* map = NULL;
    vector<LPGAMEOBJECT> objects;

    virtual void Update(DWORD dt);
    virtual void Render();
    virtual void Unload();

    void PurgeDeletedObjects();

    static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

