#include <sdktools>

// --- SDK variables ---------------------------------------------------------------
Handle hGameConf;
Handle hAttachWeapon;

stock InitGiveWeapon()
{
    hGameConf = LoadGameConfigFile("tf2.vis");
    StartPrepSDKCall(SDKCall_Player);
    PrepSDKCall_SetFromConf(hGameConf, SDKConf_Signature, "CTFPlayer::PickupWeaponFromOther");
    PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
    hAttachWeapon = EndPrepSDKCall();
}

stock GiveWeapon(client, index)
{
    decl Float:origin[3];
    GetClientEyePosition(client, origin);
    origin[2] += 50;
    
    new ent = CreateEntityByName("tf_dropped_weapon");
    SetEntProp(ent, Prop_Send, "m_iItemDefinitionIndex", index);
    SetEntProp(ent, Prop_Send, "m_bInitialized", 1);
    TeleportEntity(ent, origin, NULL_VECTOR, NULL_VECTOR);
    DispatchSpawn(ent);
    SetEntProp(ent, Prop_Send, "m_iItemDefinitionIndex", index);
    SetEntProp(ent, Prop_Send, "m_bInitialized", 1);
    TeleportEntity(ent, origin, NULL_VECTOR, NULL_VECTOR);
    
    SDKCall(hAttachWeapon, client, ent);
    AcceptEntityInput(ent, "Kill");
}