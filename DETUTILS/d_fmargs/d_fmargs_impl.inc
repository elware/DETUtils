/*
 * 
 * DETUtils
 *
 * Since 2019., DEntisT
 *
 *
 *
 * - Library which provides using format
 *   inside functions.
 *
 */

// Backwards compat.
new DET_g_FuncId;
#define DETUTILS_FMARGS_MAX_STRING 1024

// MAX_PLAYERS will be used to avoid collisions on bigger servers...
new DET_g_pClientMessage[MAX_PLAYERS][DETUTILS_FMARGS_MAX_STRING]; 
new DET_g_ClientMessage[DETUTILS_FMARGS_MAX_STRING];
new DET_g_TDTextString[DETUTILS_FMARGS_MAX_STRING];
new DET_g_pTDTextString[MAX_PLAYERS][DETUTILS_FMARGS_MAX_STRING];
new DET_g_RconCommand[DETUTILS_FMARGS_MAX_STRING];
new DET_g_3DLabelText[DETUTILS_FMARGS_MAX_STRING];
new DET_g_p3DLabelText[MAX_PLAYERS][DETUTILS_FMARGS_MAX_STRING];

// Natives:
native DET_n_Func1(playerid, color, const text[]) = SendClientMessage;
native DET_n_Func2(color, const message[]) = SendClientMessageToAll;
native DET_n_Func3(Text:text, string[]) = TextDrawSetString;
native DET_n_Func4(playerid, PlayerText:text, string[]) = PlayerTextDrawSetString;
native DET_n_Func5(command[]) = SendRconCommand;
native DET_n_Func6(Text3D:id, color, text[]) = Update3DTextLabelText;
native DET_n_Func7(playerid, PlayerText3D:id, color, text[]) = UpdatePlayer3DTextLabelText;

// Macro versions (compatible with JIT):
#define fmargs_SendClientMessage(%1,%2,%3) \
format(DET_g_pClientMessage[%1],DETUTILS_FMARGS_MAX_STRING,%3)&&DET_n_Func1(%1,%2,DET_g_pClientMessage[%1])
#define fmargs_SendClientMessageToAll(%1,%2) \
format(DET_g_ClientMessage,DETUTILS_FMARGS_MAX_STRING,%2)&&DET_n_Func2(%1,DET_g_ClientMessage)

#define fmargs_TextDrawSetString(%1,%2) \
format(DET_g_TDTextString,DETUTILS_FMARGS_MAX_STRING,%2)&&DET_n_Func3(%1,DET_g_TDTextString)
#define fmargs_PlayerTextDrawSetString(%0,%1,%2) \
format(DET_g_pTDTextString[%0],DETUTILS_FMARGS_MAX_STRING,%2)&&DET_n_Func4(%0,%1,DET_g_pTDTextString[%0])

#define fmargs_SendRconCommand(%0) \
format(DET_g_RconCommand,DETUTILS_FMARGS_MAX_STRING,%0)&&DET_n_Func5(DET_g_RconCommand)

#define fmargs_Update3DTextLabelText(%1,%2,%3) \
format(DET_g_3DLabelText,DETUTILS_FMARGS_MAX_STRING,%3)&&DET_n_Func6(%1,%2,DET_g_3DLabelText)
#define fmargs_UpdateP3DTextLabelText(%0,%1,%2,%3) \
format(DET_g_p3DLabelText[%0],DETUTILS_FMARGS_MAX_STRING,%3)&&DET_n_Func7(%1,%2,DET_g_p3DLabelText[%0])

// Function versions:

stock fmargs_GameTextForPlayer(playerid,const unformatted_text[],duration,style,@DETUTILSFMARGS_TAGS__:...)
{
    new time = duration;
    if(funcidx("OnPlayerScreenUpdate") != -1)
    {
        CallLocalFunction("OnPlayerScreenUpdate", "ii", playerid, 4);
        SetTimerEx("OnPlayerScreenUpdate", time, false, "ii", playerid, 5);
    }
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 3;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully called.", DET_g_FuncId);
    #endif
    static  
        args,
        start,
        end,
        formatted_str[144];
    #emit LOAD.S.pri 8
    #emit STOR.pri args

    if(args > 12)
    {
        #emit ADDR.pri unformatted_text
        #emit STOR.pri start

        for(end = start + (args - 12);end > start;end -= 4)
        {
            #emit LREF.pri end
            #emit PUSH.pri
        }
        #emit PUSH.S unformatted_text
        #emit PUSH.C 144
        #emit PUSH.C formatted_str
        #emit PUSH.C args
        #emit SYSREQ.C format

        GameTextForPlayer(playerid,formatted_str,duration,style);

        #emit LCTRL 5
        #emit SCTRL 4
        #emit RETN
    }
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 3;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully performed.", DET_g_FuncId);
    #endif
    return GameTextForPlayer(playerid,unformatted_text,duration,style);
}

stock fmargs_GameTextForAll(const unformatted_text[], duration,style,@DETUTILSFMARGS_TAGS__:...)
{
    new time = duration;
    if(funcidx("OnPlayerScreenUpdate") != -1)
    {
        for(new i; i < MAX_PLAYERS; i++) if(IsPlayerConnected(i))
        {
            CallLocalFunction("OnPlayerScreenUpdate", "ii", i, 4);
            SetTimerEx("OnPlayerScreenUpdate", time, false, "ii", i, 5);
        }
    }
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 4;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully called.", DET_g_FuncId);
    #endif
    static  
        args,
        start,
        end,
        formatted_str[144];
    #emit LOAD.S.pri 8
    
    #emit STOR.pri args

    if(args > 12)
    {
        #emit ADDR.pri unformatted_text
        
        #emit STOR.pri start

        for(end = start + (args - 12);end > start;end -= 4)
        {
            #emit LREF.pri end
            #emit PUSH.pri
        }
        #emit PUSH.S unformatted_text
        #emit PUSH.C 144
        #emit PUSH.C formatted_str
        #emit PUSH.C args
        #emit SYSREQ.C format

        GameTextForAll(formatted_str,duration,style);

        #emit LCTRL 5
        #emit SCTRL 4
        #emit RETN
    }
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 4;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully performed.", DET_g_FuncId);
    #endif
    return GameTextForAll(unformatted_text,duration,style);
}

stock Text:fmargs_TextDrawCreate(Float:x,Float:y,const _StringArgs[],@DETUTILSFMARGS_TAGS__:...)
{
    new Text:textid;

    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 5;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully called.", DET_g_FuncId);
    #endif
    static  
        args,
        start,
        end,
        _unspecified_string[144]
    ;
    #emit LOAD.S.pri 8
    #emit STOR.pri args
 
    if(args > 12)
    {
        #emit ADDR.pri _StringArgs
        #emit STOR.pri start

        for
        (
            end = start + (args - 12);
            end > start;
            end -= 4
        )
        {
            #emit LREF.pri end
            #emit PUSH.pri
        }
        #emit PUSH.S _StringArgs
        #emit PUSH.C 144
        #emit PUSH.C _unspecified_string
        #emit PUSH.C args
        #emit SYSREQ.C format
 
        textid = TextDrawCreate(x,y,_unspecified_string);
        #if defined detutils_complete_error_boom
        g_TextDrawData[textid][Pos][0] = x;
        g_TextDrawData[textid][Pos][1] = y;
        if(g_TextDrawData[textid][Pos][0] != g_TextDrawData[textid][OldPos][0] ||
        g_TextDrawData[textid][Pos][1] != g_TextDrawData[textid][OldPos][1])
        {
            if(funcidx("OnPlayerScreenUpdate") != -1)
            {
                for(new i; i < MAX_PLAYERS; i++)
                {
                    CallLocalFunction("OnPlayerScreenUpdate", "ii", i, 6);
                }
            }
        }
        g_TextDrawData[textid][OldPos][0] = x;
        g_TextDrawData[textid][OldPos][1] = y;
        #endif
        #emit LCTRL 5
        #emit SCTRL 4
        #emit RETN
    }
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 5;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully performed.", DET_g_FuncId);
    #endif
    
    textid = TextDrawCreate(x,y,_StringArgs);
    g_TextDrawData[textid][Pos][0] = x;
    g_TextDrawData[textid][Pos][1] = y;
    if(g_TextDrawData[textid][Valid] == 1 && 
    g_TextDrawData[textid][Pos][0] != g_TextDrawData[textid][OldPos][0] ||
    g_TextDrawData[textid][Pos][1] != g_TextDrawData[textid][OldPos][1])
    {
        if(funcidx("OnPlayerScreenUpdate") != -1)
        {
            for(new i; i < MAX_PLAYERS; i++)
            {
                CallLocalFunction("OnPlayerScreenUpdate", "ii", i, 6);
            }
        }
    }
    g_TextDrawData[textid][OldPos][0] = x;
    g_TextDrawData[textid][OldPos][1] = y;
    g_TextDrawData[textid][Valid] = 1;
    return textid;
}

stock PlayerText:fmargs_CreatePlayerTextDraw(i,Float:x,Float:y,const _StringArgs[],@DETUTILSFMARGS_TAGS__:...)
{
    new PlayerText:textid;
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 6;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully called.", DET_g_FuncId);
    #endif
    static  
        args,
        start,
        end,
        _unspecified_string[144]
    ;
    #emit LOAD.S.pri 8
    #emit STOR.pri args
 
    if (
        args > 12
    )
    {
        #emit ADDR.pri _StringArgs
        #emit STOR.pri start
 
        for
        (
            end = start + (args - 12);
            end > start;
            end -= 4
        )
        {
            #emit LREF.pri end
            #emit PUSH.pri
        }
        #emit PUSH.S _StringArgs
        #emit PUSH.C 144
        #emit PUSH.C _unspecified_string
        #emit PUSH.C args
        #emit SYSREQ.C format
 
        textid = CreatePlayerTextDraw
        (
            i,
            x,
            y,
            _unspecified_string
        )
        ;
 
        #emit LCTRL 5
        #emit SCTRL 4
        #emit RETN
    }
    #if defined DETUTILS_DEBUG_MODE
    DET_g_FuncId = 6;
    printf("[DETUtils]: (debug) - fmargs :: Function id : %i successfully performed.", DET_g_FuncId);
    #endif
    
    textid = CreatePlayerTextDraw
        (
            i,
            x,
            y,
            _StringArgs
        )
    ;
    g_PlayerTextDrawData[textid][Pos][0] = x;
    g_PlayerTextDrawData[textid][Pos][1] = y;
    if(g_PlayerTextDrawData[textid][Valid] == 1 && 
    g_PlayerTextDrawData[textid][Pos][0] != g_PlayerTextDrawData[textid][OldPos][0] ||
    g_PlayerTextDrawData[textid][Pos][1] != g_PlayerTextDrawData[textid][OldPos][1])
    {
        if(funcidx("OnPlayerScreenUpdate") != -1)
        {
            for(new i; i < MAX_PLAYERS; i++)
            {
                CallLocalFunction("OnPlayerScreenUpdate", "ii", i, 7);
            }
        }
    }
    g_PlayerTextDrawData[textid][OldPos][0] = x;
    g_PlayerTextDrawData[textid][OldPos][1] = y;
    g_PlayerTextDrawData[textid][Valid] = 1;
    return textid;
}

// Hooks:

#if defined DETUTILS_FMARGS_FUNCREPLACE

#if defined _ALS_SendClientMessage
    #undef SendClientMessage
#else
    #define _ALS_SendClientMessage
#endif

#define SendClientMessage fmargs_SendClientMessage

#if defined _ALS_SendClientMessageToAll
    #undef SendClientMessageToAll
#else
    #define _ALS_SendClientMessageToAll
#endif

#define SendClientMessageToAll fmargs_SendClientMessageToAll

#if defined _ALS_GameTextForPlayer
    #undef GameTextForPlayer
#else
    #define _ALS_GameTextForPlayer
#endif

#define GameTextForPlayer fmargs_GameTextForPlayer

#if defined _ALS_GameTextForAll
    #undef GameTextForAll
#else
    #define _ALS_GameTextForAll
#endif

#define GameTextForAll fmargs_GameTextForAll

#if defined _ALS_TextDrawCreate
    #undef TextDrawCreate
#else
    #define _ALS_TextDrawCreate
#endif

#define TextDrawCreate fmargs_TextDrawCreate

#if defined _ALS_CreatePlayerTextDraw
    #undef CreatePlayerTextDraw
#else
    #define _ALS_CreatePlayerTextDraw
#endif

#define CreatePlayerTextDraw fmargs_CreatePlayerTextDraw

#if defined _ALS_TextDrawSetString
    #undef TextDrawSetString
#else
    #define _ALS_TextDrawSetString
#endif

#define TextDrawSetString fmargs_TextDrawSetString

#if defined _ALS_PlayerTextDrawSetString
    #undef PlayerTextDrawSetString
#else
    #define _ALS_PlayerTextDrawSetString
#endif

#define PlayerTextDrawSetString fmargs_PlayerTextDrawSetString

#if defined _ALS_SendRconCommand
    #undef SendRconCommand
#else
    #define _ALS_SendRconCommand
#endif

#define SendRconCommand fmargs_SendRconCommand

#if defined _ALS_Update3DTextLabelText
    #undef Update3DTextLabelText
#else
    #define _ALS_Update3DTextLabelText
#endif

#define Update3DTextLabelText fmargs_Update3DTextLabelText

#if defined _ALS_UpdatePlayer3DTextLabelTxt
    #undef UpdatePlayer3DTextLabelText
#else
    #define _ALS_UpdatePlayer3DTextLabelTxt
#endif

#define UpdatePlayer3DTextLabelText fmargs_UpdateP3DTextLabelText

#endif