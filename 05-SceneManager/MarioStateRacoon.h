#pragma once
#pragma once
#include "BaseMarioState.h"


////RACOON MARIO
#define ID_ANI_RACOON_MARIO_IDLE_RIGHT 3400
#define ID_ANI_RACOON_MARIO_IDLE_LEFT 3401

#define ID_ANI_RACOON_MARIO_WALKING_RIGHT 3500
#define ID_ANI_RACOON_MARIO_WALKING_LEFT 3501

#define ID_ANI_RACOON_MARIO_RUNNING_RIGHT 3600
#define ID_ANI_RACOON_MARIO_RUNNING_LEFT 3601

#define ID_ANI_RACOON_MARIO_JUMP_WALK_RIGHT 3700
#define ID_ANI_RACOON_MARIO_JUMP_WALK_LEFT 3701

#define ID_ANI_RACOON_MARIO_JUMP_RUN_RIGHT 3800
#define ID_ANI_RACOON_MARIO_JUMP_RUN_LEFT 3801

#define ID_ANI_RACOON_MARIO_SIT_RIGHT 3900
#define ID_ANI_RACOON_MARIO_SIT_LEFT 3901

#define ID_ANI_RACOON_MARIO_BRACE_RIGHT 3100
#define ID_ANI_RACOON_MARIO_BRACE_LEFT 3101


#define ID_ANI_RACOON_MARIO_ATTACK_FROM_LEFT 3201
#define ID_ANI_RACOON_MARIO_ATTACK_FROM_RIGHT 3200

#define ID_ANI_RACOON_MARIO_FLYING_RIGHT 3300
#define ID_ANI_RACOON_MARIO_FLYING_LEFT 3301

#define ID_ANI_RACOON_MARIO_FLOATING_RIGHT 3402
#define ID_ANI_RACOON_MARIO_FLOATING_LEFT 3403

#define ID_ANI_RACOON_WARP 3231 


class CMario;
class CTail;

class MarioStateRacoon :
    public BaseMarioState
{
public:
    CTail* tail;

    MarioStateRacoon(CMario* mario);
    ~MarioStateRacoon();

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

    virtual void JumpUpdate(DWORD dt);
    virtual void PowerMeterUpdate(DWORD dt);
    virtual void AttackUpdate(DWORD dt);

    virtual void Render();

    float MAX_FLY_HEIGHT = 315;
};

