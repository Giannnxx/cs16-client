// ===================================================
// REQUIRED HEADERS (FIXES ALL YOUR ERRORS)
// ===================================================
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weaponids.h"
#include "pm_shared.h"
#include "usercmd.h"
#include "mathlib.h"
#include "cdll_int.h"

#include <cstdlib>

// ===================================================
// GLOBAL STATE
// ===================================================
bool  g_bIronsight = false;
float g_flADSProgress = 0.0f;
int   g_iActiveWeapon = 0;

Vector g_vecRecoil(0, 0, 0);

// ===================================================
// WEAPON STATE
// ===================================================
void SetActiveWeapon(int id)
{
    g_iActiveWeapon = id;
}

int GetActiveWeaponID()
{
    return g_iActiveWeapon;
}

// ===================================================
// ADS WEAPONS ONLY
// ===================================================
bool UsesADS(int weaponid)
{
    switch (weaponid)
    {
        case WEAPON_AK47:
        case WEAPON_M4A1:
        case WEAPON_FAMAS:
        case WEAPON_GALIL:
        case WEAPON_AUG:
        case WEAPON_SG552:

        case WEAPON_MP5N:
        case WEAPON_UMP45:
        case WEAPON_MAC10:
        case WEAPON_TMP:
        case WEAPON_P90:

        case WEAPON_DEAGLE:
        case WEAPON_USP:
        case WEAPON_GLOCK:
        case WEAPON_FIVESEVEN:
            return true;
    }
    return false;
}

// ===================================================
// INPUT (CLIENT MOVE)
// ===================================================
void CL_CreateMove(usercmd_t *cmd)
{
    if (!cmd) return;

    g_bIronsight = (cmd->buttons & IN_ATTACK2);

    if (cmd->buttons & IN_ATTACK)
    {
        g_vecRecoil.x += 2.0f;
        g_vecRecoil.y += (rand() % 3 - 1);
    }
}

// ===================================================
// RECOIL DECAY
// ===================================================
void UpdateRecoil()
{
    float t = 0.15f;

    g_vecRecoil.x = g_vecRecoil.x + (0.0f - g_vecRecoil.x) * t;
    g_vecRecoil.y = g_vecRecoil.y + (0.0f - g_vecRecoil.y) * t;
}

// ===================================================
// HIP FIRE
// ===================================================
Vector GetHipfire()
{
    return Vector(0, 0, 0);
}

// ===================================================
// ADS POSITIONS
// ===================================================
Vector GetADSPosition(int weaponid)
{
    switch (weaponid)
    {
        case WEAPON_FIVESEVEN: return Vector(0, -0.1f, 3.9f);
        case WEAPON_GALIL:     return Vector(0, -2.65f, 2);
        case WEAPON_M249:      return Vector(0, -3, 2);
        case WEAPON_GLOCK:     return Vector(0, -0.5f, 4.4f);
        case WEAPON_MAC10:     return Vector(0, -2.5f, 2.2f);
        case WEAPON_TMP:       return Vector(0, -2.7f, 2.3f);
        case WEAPON_UMP45:     return Vector(0, -2.1f, 2);
        case WEAPON_XM1014:    return Vector(0, -2.95f, 1.8f);

        case WEAPON_AK47:      return Vector(0, -2.7f, 1.9f);
        case WEAPON_DEAGLE:    return Vector(0, -1.8f, 2.8f);
        case WEAPON_FAMAS:     return Vector(0, -3.7f, 1.4f);
        case WEAPON_M3:        return Vector(0, -3.35f, 2.25f);
        case WEAPON_M4A1:      return Vector(0, -3.13f, 0.7f);
        case WEAPON_MP5N:      return Vector(0, -3.7f, 1.9f);
        case WEAPON_P228:      return Vector(0, -0.88f, 3.73f);
        case WEAPON_P90:       return Vector(0, -3.38f, 1);
        case WEAPON_USP:       return Vector(0, -0.44f, 4.3f);

        default: return Vector(0, 0, 0);
    }
}

// ===================================================
// LERP
// ===================================================
Vector Lerp(Vector a, Vector b, float t)
{
    return a + (b - a) * t;
}

// ===================================================
// VIEWMODEL FINAL
// ===================================================
Vector V_CalcViewModel()
{
    int weaponid = GetActiveWeaponID();

    Vector hip = GetHipfire();
    Vector ads = GetADSPosition(weaponid);

    Vector recoilFix = g_vecRecoil * -1.0f;

    Vector result = Lerp(hip, ads, g_flADSProgress);

    return result + recoilFix;
}

// ===================================================
// FRAME UPDATE
// ===================================================
void FrameUpdate(usercmd_t *cmd)
{
    UpdateRecoil();

    float speed = 0.15f;

    if (g_bIronsight)
        g_flADSProgress += speed;
    else
        g_flADSProgress -= speed;

    if (g_flADSProgress > 1.0f) g_flADSProgress = 1.0f;
    if (g_flADSProgress < 0.0f) g_flADSProgress = 0.0f;
}
