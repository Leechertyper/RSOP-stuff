/**************************************************************************
*                                                                        *
*                     Attachment Policy Workaround                       *
*                       Author: Mecha the Slag                           *
*                           Version: 2.0.0                               *
*                                                                        *
**************************************************************************/
#if defined _attachmentpolicy_included
    #endinput
#endif
#define _attachmentpolicy_included

#define EF_BONEMERGE            (1 << 0)
#define EF_BRIGHTLIGHT          (1 << 1)
#define EF_DIMLIGHT             (1 << 2)
#define EF_NOINTERP             (1 << 3)
#define EF_NOSHADOW             (1 << 4)
#define EF_NODRAW               (1 << 5)
#define EF_NORECEIVESHADOW      (1 << 6)
#define EF_BONEMERGE_FASTCULL   (1 << 7)
#define EF_ITEM_BLINK           (1 << 8)
#define EF_PARENT_ANIMATES      (1 << 9)

#define ATTACHMENTPOLICY_MODEL  "prop_dynamic"


new Handle:g_hAttachmentsPolicy = INVALID_HANDLE;

/** 
* Attaches a child to a parent.
* 
* @param child         Child entity. 
* @param parent        Parent entity. 
* @param variant       Attachment point. Empty for none. 
* @return              True if able to create attachment. False if fail.
*  
* On error/Errors:     If unable to create attachment.
*/ 
stock bool:CAttach(iChild, iParent, const String:strVariant[] = "", bool:bMaintainOffset = false, const String:strModel[]) {
    // if no attachment entity array, create one
    if (g_hAttachmentsPolicy == INVALID_HANDLE) g_hAttachmentsPolicy = CreateArray(2);
    
    // if any of the entities invovled are invalid, return false
    if (iChild <= 0 || !IsValidEdict(iChild)) return false;
    if (iParent <= 0 || !IsValidEdict(iParent)) return false;
    
    // get the current link (assuming there is one)
    new iLink = CGetLink(iChild);
    // if there is none, create eone!
    if (!CIsClassname(iLink, ATTACHMENTPOLICY_MODEL)) iLink = CAddLink(iChild, strModel);
    // if that shit didn't work, error out bro
    if (!CIsClassname(iLink, ATTACHMENTPOLICY_MODEL)) {
        decl String:strClassname[128];
        if (GetEdictClassname(iChild, strClassname, sizeof(strClassname))) ThrowError("Unable to create link for entity %s", strClassname);
        else ThrowError("Unable to create link for unknown entity");
        return false;
    }
    
    // lets set the parent's name so that we can attach to it
    decl String:strName[16];
    Format(strName, sizeof(strName), "target%i", iParent);
    DispatchKeyValue(iParent, "targetname", strName);
    
    // and now we set the link's parent to the same name
    DispatchKeyValue(iLink, "parentname", strName);
    
    // THE ACTUAL PARENTING OHH YEAHHH
    SetVariantString(strName);
    AcceptEntityInput(iLink, "SetParent", iLink, iLink, 0);
    
    // variant points or whatever. pretty cool and all
    if (!StrEqual(strVariant, "")) {
        LogMessage("SetParentAttachment");
        SetVariantString(strVariant);
        if (bMaintainOffset) AcceptEntityInput(iLink, "SetParentAttachmentMaintainOffset", iLink, iLink, 0);
        else AcceptEntityInput(iLink, "SetParentAttachment", iLink, iLink, 0);
    }
    
    LogMessage("CAttach Child %d -> Link %d -> Parent %d", iChild, iLink, iParent);
    return true;
}

/** 
* Detaches an entity from all its parents. Should be called before the entity is killed.
* 
* @param ent           Entity to detach. 
* @return              True if an attachment is destroyed. False if no attachment.
*  
* On error/Errors:     Never.
*/ 
stock CDetach(iChild) {
    if (g_hAttachmentsPolicy == INVALID_HANDLE) g_hAttachmentsPolicy = CreateArray(2);
    
    if (iChild <= 0 || !IsValidEdict(iChild)) return;
    
    new iLink = CGetLink(iChild);
    if (CIsClassname(iLink, ATTACHMENTPOLICY_MODEL)) {
        // Detach from parent
        AcceptEntityInput(iChild, "SetParent", -1, -1, 0);
        AcceptEntityInput(iLink, "SetParent", -1, -1, 0);
        
        // Kill link
        AcceptEntityInput(iLink, "kill");
    }
    
    for (new i = 0; i < GetArraySize(g_hAttachmentsPolicy); i++) {
        new iEntity = GetArrayCell(g_hAttachmentsPolicy, i);
        if (iChild == iEntity) RemoveFromArray(g_hAttachmentsPolicy, i);
    }
}

stock CGetLink(iChild) {
    for (new i = 0; i < GetArraySize(g_hAttachmentsPolicy); i++) {
        new iEntity = GetArrayCell(g_hAttachmentsPolicy, i);
        // If this is the child we want, let's get its link
        if (iChild == iEntity)
        {
            new iLink = GetArrayCell(g_hAttachmentsPolicy, i, 1);
            if (CIsClassname(iLink, ATTACHMENTPOLICY_MODEL)) return iLink;
        }
    }
    return -1;
}

stock CAddLink(iChild, const String:strModel[]) {
    // This attaches the child to the link
    
    new iLinkEntity = CreateEntityByName(ATTACHMENTPOLICY_MODEL);
    if (CIsClassname(iLinkEntity, ATTACHMENTPOLICY_MODEL)) {
        decl Float:vOrigin[3];
        decl Float:vRotation[3];
        GetEntPropVector(iChild, Prop_Send, "m_vecOrigin", vOrigin);
        GetEntPropVector(iChild, Prop_Send, "m_angRotation", vRotation);
        
        decl String:strNameLink[16]; 
        Format(strNameLink, sizeof(strNameLink), "target%i", iLinkEntity);
        DispatchKeyValue(iLinkEntity, "targetname", strNameLink);
        
        DispatchKeyValue(iLinkEntity, "classname", ATTACHMENTPOLICY_MODEL);
        //DispatchKeyValue(iLinkEntity, "spawnflags", "1");
        //DispatchKeyValue(iLinkEntity, "scale", "0");
        //DispatchKeyValue(iLinkEntity, "rendermode", "0");
        //DispatchKeyValue(iLinkEntity, "rendercolor", "255 255 255");
        
        DispatchKeyValue(iLinkEntity, "model", strModel);
        SetEntityModel(iLinkEntity, strModel);
        DispatchSpawn(iLinkEntity);
        SetEntProp(iLinkEntity, Prop_Send, "m_fEffects", EF_BONEMERGE|EF_BONEMERGE_FASTCULL|EF_NOSHADOW|EF_PARENT_ANIMATES);
        TeleportEntity(iLinkEntity, vOrigin, vRotation, NULL_VECTOR);
        
        SetVariantString(strNameLink);
        AcceptEntityInput(iChild, "SetParent", iChild, iChild, 0);
        
        new iIndex = PushArrayCell(g_hAttachmentsPolicy, iChild);
        SetArrayCell(g_hAttachmentsPolicy, iIndex, iLinkEntity, 1);
        
        
        LogMessage("CAddLink %s", strModel);
        return iLinkEntity;
    }
    return -1;
}

stock bool:CIsClassname(iEntity, String:strClassname[]) {
    if (iEntity <= 0) return false;
    if (!IsValidEdict(iEntity)) return false;
        
    decl String:strClassname2[32];
    GetEdictClassname(iEntity, strClassname2, sizeof(strClassname2));
    if (!StrEqual(strClassname, strClassname2, false)) return false;
    
    return true;
}
