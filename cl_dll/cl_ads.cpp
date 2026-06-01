// ===================================================
// GLOBAL STATE
// ===================================================
bool  g_bIronsight = false;
float g_flADSProgress = 0;
int   g_iActiveWeapon = 0;

Vector g_vecRecoil;


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
// INPUT (CL_CREATEMOVE - CLIENT SIDE)
// ===================================================
void CL_CreateMove(UserCmd *cmd)
{
    g_bIronsight = (cmd->buttons & IN_ATTACK2);

    if (cmd->buttons & IN_ATTACK)
    {
        g_vecRecoil.x += 2;
        g_vecRecoil.y += (rand() % 2 - 1);
    }
}


// ===================================================
// RECOIL DECAY (SMOOTH)
// ===================================================
void UpdateRecoil()
{
    g_vecRecoil.x = Lerp(g_vecRecoil.x, 0, 0.15f);
    g_vecRecoil.y = Lerp(g_vecRecoil.y, 0, 0.15f);
}


// ===================================================
// HIPFIRE (CENTER)
// ===================================================
Vector GetHipfire()
{
    return Vector(0, 0, 0);
}


// ===================================================
// YOUR MDL = ADS POSITIONS
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
// VIEWMODEL FINAL (ADS + RECOIL)
// ===================================================
Vector V_CalcViewModel()
{
    int weaponid = GetActiveWeaponID();

    Vector hip = GetHipfire();
    Vector ads = GetADSPosition(weaponid);

    Vector recoilFix = g_vecRecoil * -1;

    Vector result = Lerp(hip, ads, g_flADSProgress);

    return result + recoilFix;
}


// ===================================================
// FRAME UPDATE (CLIENT LOOP)
// ===================================================
void FrameUpdate(UserCmd *cmd)
{
    UpdateRecoil();

    float speed = 0.15f;

    if (g_bIronsight)
        g_flADSProgress += speed;
    else
        g_flADSProgress -= speed;

    if (g_flADSProgress > 1) g_flADSProgress = 1;
    if (g_flADSProgress < 0) g_flADSProgress = 0;
}