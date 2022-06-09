#include "lua_fairygui.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "FairyGUI.h"
#include "utils/html/HtmlElement.h"
#include "utils/html/HtmlObject.h"
#include "display/FUISprite.h"

static void margin_to_luaval(lua_State* L, const fairygui::Margin& _margin)
{
	lua_newtable(L);
	lua_pushstring(L, "left");
	lua_pushnumber(L, (lua_Number)_margin.left);
	lua_rawset(L, -3);
	lua_pushstring(L, "top");
	lua_pushnumber(L, (lua_Number)_margin.top);
	lua_rawset(L, -3);
	lua_pushstring(L, "right");
	lua_pushnumber(L, (lua_Number)_margin.right);
	lua_rawset(L, -3);
	lua_pushstring(L, "bottom");
	lua_pushnumber(L, (lua_Number)_margin.bottom);
	lua_rawset(L, -3);
}

static bool luaval_to_margin(lua_State* L, int lo, fairygui::Margin* outValue, const char* funcName)
{
	bool ok = true;

	tolua_Error tolua_err;
	if (!tolua_istable(L, lo, 0, &tolua_err)) {
#if COCOS2D_DEBUG >=1
		luaval_to_native_err(L, "#ferror:", &tolua_err, funcName);
#endif
		ok = false;
	}

	if (ok) {
		lua_pushstring(L, "left");
		lua_gettable(L, lo);
		outValue->left = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "top");
		lua_gettable(L, lo);
		outValue->top = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "right");
		lua_gettable(L, lo);
		outValue->right = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "bottom");
		lua_gettable(L, lo);
		outValue->bottom = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return ok;
}

static bool luaval_to_array_of_float(lua_State* L, int lo, float **floats, int *count, const char* funcName)
{
	if (NULL == L)
		return false;

	bool ok = true;
	tolua_Error tolua_err;

	if (!tolua_istable(L, lo, 0, &tolua_err)) {
#if COCOS2D_DEBUG >=1
		luaval_to_native_err(L,"#ferror:", &tolua_err, funcName);
#endif
		ok = false;
	}

	if (ok) {
		size_t len = lua_objlen(L, lo);
		if (len > 0) {
			float* array = (float*) new float[len];
			if (NULL == array)
				return false;
			for (size_t i = 0; i < len; ++i) {
				lua_rawgeti(L, lo, i + 1); // +1
				array[i] = (float)lua_tonumber(L, -1);
				lua_pop(L, 1); // -1
			}
			*count = (int)len;
			*floats = array;
		}
	}
	return ok;
}

static int lua_fairygui_InputEvent_getTouchId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getTouchId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getTouchId();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getTouchId",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getTouchId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getProcessor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getProcessor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::InputProcessor* ret = cobj->getProcessor();
		object_to_luaval<fairygui::InputProcessor>(tolua_S, "fairygui.InputProcessor",(fairygui::InputProcessor*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getProcessor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getProcessor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getButton(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getButton'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getButton();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getButton",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getButton'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_isDoubleClick(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_isDoubleClick'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->isDoubleClick();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:isDoubleClick",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_isDoubleClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getTouch(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getTouch'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Touch* ret = cobj->getTouch();
		object_to_luaval<cocos2d::Touch>(tolua_S, "cc.Touch",(cocos2d::Touch*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getTouch",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getTouch'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_isAltDown(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_isAltDown'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isAltDown();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:isAltDown",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_isAltDown'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getTarget(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getTarget'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getTarget();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getTarget",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getTarget'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const int ret = cobj->getX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const int ret = cobj->getY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getMouseWheelDelta(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getMouseWheelDelta'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getMouseWheelDelta();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getMouseWheelDelta",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getMouseWheelDelta'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_isShiftDown(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_isShiftDown'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isShiftDown();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:isShiftDown",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_isShiftDown'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_isCtrlDown(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_isCtrlDown'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isCtrlDown();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:isCtrlDown",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_isCtrlDown'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getKeyCode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getKeyCode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getKeyCode();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getKeyCode",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getKeyCode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_InputEvent_getPosition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputEvent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputEvent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputEvent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputEvent_getPosition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vec2& ret = cobj->getPosition();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputEvent:getPosition",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputEvent_getPosition'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_InputEvent(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.InputEvent");
	tolua_cclass(tolua_S,"InputEvent","fairygui.InputEvent","",nullptr);

	tolua_beginmodule(tolua_S,"InputEvent");
	tolua_function(tolua_S,"getTouchId",lua_fairygui_InputEvent_getTouchId);
	tolua_function(tolua_S,"getProcessor",lua_fairygui_InputEvent_getProcessor);
	tolua_function(tolua_S,"getButton",lua_fairygui_InputEvent_getButton);
	tolua_function(tolua_S,"isDoubleClick",lua_fairygui_InputEvent_isDoubleClick);
	tolua_function(tolua_S,"getTouch",lua_fairygui_InputEvent_getTouch);
	tolua_function(tolua_S,"isAltDown",lua_fairygui_InputEvent_isAltDown);
	tolua_function(tolua_S,"getTarget",lua_fairygui_InputEvent_getTarget);
	tolua_function(tolua_S,"getX",lua_fairygui_InputEvent_getX);
	tolua_function(tolua_S,"getY",lua_fairygui_InputEvent_getY);
	tolua_function(tolua_S,"getMouseWheelDelta",lua_fairygui_InputEvent_getMouseWheelDelta);
	tolua_function(tolua_S,"isShiftDown",lua_fairygui_InputEvent_isShiftDown);
	tolua_function(tolua_S,"isCtrlDown",lua_fairygui_InputEvent_isCtrlDown);
	tolua_function(tolua_S,"getKeyCode",lua_fairygui_InputEvent_getKeyCode);
	tolua_function(tolua_S,"getPosition",lua_fairygui_InputEvent_getPosition);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::InputEvent).name();
	g_luaType[typeName] = "fairygui.InputEvent";
	g_typeCast["InputEvent"] = "fairygui.InputEvent";
	return 1;
}

static int lua_fairygui_EventContext_preventDefault(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_preventDefault'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->preventDefault();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:preventDefault",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_preventDefault'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_isDefaultPrevented(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_isDefaultPrevented'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isDefaultPrevented();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:isDefaultPrevented",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_isDefaultPrevented'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_getType(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_getType'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getType();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:getType",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_getType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_getSender(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_getSender'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Ref* ret = cobj->getSender();
		object_to_luaval<cocos2d::Ref>(tolua_S, "cc.Ref",(cocos2d::Ref*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:getSender",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_getSender'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_uncaptureTouch(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_uncaptureTouch'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->uncaptureTouch();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:uncaptureTouch",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_uncaptureTouch'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_getInput(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_getInput'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::InputEvent* ret = cobj->getInput();
		object_to_luaval<fairygui::InputEvent>(tolua_S, "fairygui.InputEvent",(fairygui::InputEvent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:getInput",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_getInput'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_captureTouch(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_captureTouch'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->captureTouch();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:captureTouch",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_captureTouch'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_stopPropagation(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_stopPropagation'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->stopPropagation();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:stopPropagation",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_stopPropagation'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_getData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_getData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Ref *ret = (cocos2d::Ref *)cobj->getData();
		object_to_luaval<cocos2d::Ref>(tolua_S, "cc.Ref", ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:getData",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_getData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_EventContext_getDataValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.EventContext",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_EventContext_getDataValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Value &ret = cobj->getDataValue();
		ccvalue_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.EventContext:getDataValue",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_EventContext_getDataValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_EventContext(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.EventContext");
	tolua_cclass(tolua_S,"EventContext","fairygui.EventContext","",nullptr);

	tolua_beginmodule(tolua_S,"EventContext");
	tolua_function(tolua_S,"preventDefault",lua_fairygui_EventContext_preventDefault);
	tolua_function(tolua_S,"isDefaultPrevented",lua_fairygui_EventContext_isDefaultPrevented);
	tolua_function(tolua_S,"getType",lua_fairygui_EventContext_getType);
	tolua_function(tolua_S,"getSender",lua_fairygui_EventContext_getSender);
	tolua_function(tolua_S,"uncaptureTouch",lua_fairygui_EventContext_uncaptureTouch);
	tolua_function(tolua_S,"getInput",lua_fairygui_EventContext_getInput);
	tolua_function(tolua_S,"captureTouch",lua_fairygui_EventContext_captureTouch);
	tolua_function(tolua_S,"stopPropagation",lua_fairygui_EventContext_stopPropagation);
	tolua_function(tolua_S,"getData",lua_fairygui_EventContext_getData);
	tolua_function(tolua_S,"getDataValue",lua_fairygui_EventContext_getDataValue);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::EventContext).name();
	g_luaType[typeName] = "fairygui.EventContext";
	g_typeCast["EventContext"] = "fairygui.EventContext";
	return 1;
}

static int lua_fairygui_UIEventDispatcher_isDispatchingEvent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIEventDispatcher* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIEventDispatcher",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIEventDispatcher*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_isDispatchingEvent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;

		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:isDispatchingEvent");
		if(!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_isDispatchingEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->isDispatchingEvent(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIEventDispatcher:isDispatchingEvent",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIEventDispatcher_isDispatchingEvent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIEventDispatcher_removeEventListeners(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIEventDispatcher* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIEventDispatcher",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIEventDispatcher*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_removeEventListeners'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->removeEventListeners();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIEventDispatcher:removeEventListeners",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIEventDispatcher_removeEventListeners'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIEventDispatcher_dispatchEvent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIEventDispatcher* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIEventDispatcher",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIEventDispatcher*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_dispatchEvent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:dispatchEvent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_dispatchEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->dispatchEvent(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	if (argc == 2) {
		int arg0;
		cocos2d::Ref *arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:dispatchEvent");
		ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 3, "cc.Ref", &arg1, "fairygui.UIEventDispatcher:dispatchEvent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_dispatchEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->dispatchEvent(arg0, (void *)arg1);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	if (argc == 3) {
		int arg0;
		cocos2d::Ref *arg1;
		cocos2d::Value arg2;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:dispatchEvent");
		ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 3, "cc.Ref", &arg1, "fairygui.UIEventDispatcher:dispatchEvent");
		ok &= luaval_to_ccvalue(tolua_S, 4, &arg2, "fairygui.UIEventDispatcher:dispatchEvent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_dispatchEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->dispatchEvent(arg0, (void *)arg1, arg2);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIEventDispatcher:dispatchEvent",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIEventDispatcher_dispatchEvent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIEventDispatcher_bubbleEvent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIEventDispatcher* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIEventDispatcher",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIEventDispatcher*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_bubbleEvent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:bubbleEvent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_bubbleEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->bubbleEvent(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	if (argc == 2) {
		int arg0;
		cocos2d::Ref *arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:bubbleEvent");
		ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 3, "cc.Ref", &arg1, "fairygui.UIEventDispatcher:bubbleEvent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_bubbleEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->bubbleEvent(arg0, (void *)arg1);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	if (argc == 3) {
		int arg0;
		cocos2d::Ref *arg1;
		cocos2d::Value arg2;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.UIEventDispatcher:bubbleEvent");
		ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 3, "cc.Ref", &arg1, "fairygui.UIEventDispatcher:bubbleEvent");
		ok &= luaval_to_ccvalue(tolua_S, 4, &arg2, "fairygui.UIEventDispatcher:bubbleEvent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIEventDispatcher_bubbleEvent'", nullptr);
			return 0;
		}
		bool ret = cobj->bubbleEvent(arg0, (void *)arg1, arg2);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIEventDispatcher:bubbleEvent",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIEventDispatcher_bubbleEvent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIEventDispatcher_addEventListener(lua_State* L)
{
	int argc = 0;
	fairygui::UIEventDispatcher* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fairygui.UIEventDispatcher", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = static_cast<fairygui::UIEventDispatcher*>(tolua_tousertype(L, 1, 0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == cobj) {
		tolua_error(L, "invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_addEventListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;
	if (2 == argc) {
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "fairygui.UIEventDispatcher:addEventListener");
		if (!ok) {
			tolua_error(L, "invalid arguments in function 'lua_fairygui_UIEventDispatcher_addEventListener'", nullptr);
			return 0;
		}
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 3, "LUA_FUNCTION", 0, &tolua_err)) {
			goto tolua_lerror;
		}
#endif
		LUA_FUNCTION handler = (toluafix_ref_function(L, 3, 0));

		cobj->addEventListener(eventType, [=](fairygui::EventContext* context) {
				auto stack = LuaEngine::getInstance()->getLuaStack();
				object_to_luaval<fairygui::EventContext>(stack->getLuaState(), "fairygui.EventContext", context);
				stack->executeFunctionByHandler(handler, 1);
				});
		ScriptHandlerMgr::getInstance()->addObjectHandler((void*)cobj, handler, (ScriptHandlerMgr::HandlerType)eventType);// fake a HandlerType
		return 0;
	}
	if (3 == argc) {
		int eventType;
		int tag;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "fairygui.UIEventDispatcher:addEventListener");
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 3, "LUA_FUNCTION", 0, &tolua_err)) {
			goto tolua_lerror;
		}
#endif
		LUA_FUNCTION handler = (toluafix_ref_function(L, 3, 0));
		ok &= luaval_to_int32(L, 4, (int *)&tag, "fairygui.UIEventDispatcher:addEventListener");
		if (!ok) {
			tolua_error(L, "invalid arguments in function 'lua_fairygui_UIEventDispatcher_addEventListener'", nullptr);
			return 0;
		}

		cobj->addEventListener(eventType, [=](fairygui::EventContext* context) {
				auto stack = LuaEngine::getInstance()->getLuaStack();
				object_to_luaval<fairygui::EventContext>(stack->getLuaState(), "fairygui.EventContext", context);
				stack->executeFunctionByHandler(handler, 1);
				}, fairygui::EventTag(tag));
		ScriptHandlerMgr::getInstance()->addObjectHandler((void*)cobj, handler, (ScriptHandlerMgr::HandlerType)eventType);// fake a HandlerType
		return 0;
	}

	luaL_error(L, "'addEventListener' function of UIEventDispatcher has wrong number of arguments: %d, was expecting 2~3\n", argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L, "#ferror in function 'addEventListener'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIEventDispatcher_removeEventListener(lua_State* L)
{
	int argc = 0;
	fairygui::UIEventDispatcher *cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fairygui.UIEventDispatcher", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = static_cast<fairygui::UIEventDispatcher*>(tolua_tousertype(L, 1, 0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == cobj) {
		tolua_error(L, "invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_removeEventListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;
	if (1 == argc) {
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "fairygui.UIEventDispatcher:removeEventListener");
		if (!ok) {
			tolua_error(L, "invalid arguments in function 'lua_fairygui_UIEventDispatcher_removeEventListener'", nullptr);
			return 0;
		}

		cobj->removeEventListener(eventType);
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)eventType);// fake a HandlerType
		return 0;
	}
	if (2 == argc) {
		int eventType;
		int tag;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "fairygui.UIEventDispatcher:removeEventListener");
		ok &= luaval_to_int32(L, 3, (int *)&tag, "fairygui.UIEventDispatcher:removeEventListener");
		if (!ok) {
			tolua_error(L, "invalid arguments in function 'lua_fairygui_UIEventDispatcher_removeEventListener'", nullptr);
			return 0;
		}

		cobj->removeEventListener(eventType, fairygui::EventTag(tag));
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)eventType);// fake a HandlerType
		return 0;
	}

	luaL_error(L, "'removeEventListener' function of UIEventDispatcher has wrong number of arguments: %d, was expecting 1~2\n", argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L, "#ferror in function 'removeEventListener'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIEventDispatcher_hasEventListener(lua_State* L)
{
	int argc = 0;
	fairygui::UIEventDispatcher *cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fairygui.UIEventDispatcher", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = static_cast<fairygui::UIEventDispatcher*>(tolua_tousertype(L, 1, 0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == cobj) {
		tolua_error(L, "invalid 'cobj' in function 'lua_fairygui_UIEventDispatcher_hasEventListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;
	if (1 == argc) {
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "fairygui.UIEventDispatcher:hasEventListener");
		if (!ok) {
			tolua_error(L, "invalid arguments in function 'lua_fairygui_UIEventDispatcher_hasEventListener'", nullptr);
			return 0;
		}

		lua_pushboolean(L, cobj->hasEventListener(eventType));
		return 1;
	}
	if (2 == argc) {
		int eventType;
		int tag;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "fairygui.UIEventDispatcher:hasEventListener");
		ok &= luaval_to_int32(L, 3, (int *)&tag, "fairygui.UIEventDispatcher:hasEventListener");
		if (!ok) {
			tolua_error(L, "invalid arguments in function 'lua_fairygui_UIEventDispatcher_hasEventListener'", nullptr);
			return 0;
		}

		lua_pushboolean(L, cobj->hasEventListener(eventType, fairygui::EventTag(tag)));
		return 1;
	}

	luaL_error(L, "'hasEventListener' function of UIEventDispatcher has wrong number of arguments: %d, was expecting 1~2\n", argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L, "#ferror in function 'lua_fairygui_UIEventDispatcher_hasEventListener'.", &tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_UIEventDispatcher(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.UIEventDispatcher");
	tolua_cclass(tolua_S,"UIEventDispatcher","fairygui.UIEventDispatcher","cc.Ref",nullptr);

	tolua_beginmodule(tolua_S,"UIEventDispatcher");
	tolua_function(tolua_S,"isDispatchingEvent",lua_fairygui_UIEventDispatcher_isDispatchingEvent);
	tolua_function(tolua_S,"removeEventListeners",lua_fairygui_UIEventDispatcher_removeEventListeners);
	tolua_function(tolua_S,"dispatchEvent",lua_fairygui_UIEventDispatcher_dispatchEvent);
	tolua_function(tolua_S,"bubbleEvent",lua_fairygui_UIEventDispatcher_bubbleEvent);
	tolua_function(tolua_S,"addEventListener",lua_fairygui_UIEventDispatcher_addEventListener);
	tolua_function(tolua_S,"removeEventListener",lua_fairygui_UIEventDispatcher_removeEventListener);
	tolua_function(tolua_S,"hasEventListener",lua_fairygui_UIEventDispatcher_hasEventListener);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::UIEventDispatcher).name();
	g_luaType[typeName] = "fairygui.UIEventDispatcher";
	g_typeCast["UIEventDispatcher"] = "fairygui.UIEventDispatcher";
	return 1;
}

static int lua_fairygui_GController_getPageIndexById(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPageIndexById'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GController:getPageIndexById");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_getPageIndexById'", nullptr);
			return 0;
		}
		int ret = cobj->getPageIndexById(arg0);
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPageIndexById",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPageIndexById'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_setSelectedPage(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_setSelectedPage'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GController:setSelectedPage");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_setSelectedPage'", nullptr);
			return 0;
		}
		cobj->setSelectedPage(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:setSelectedPage",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_setSelectedPage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getPrevisousIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPrevisousIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getPrevisousIndex();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPrevisousIndex",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPrevisousIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_setSelectedIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_setSelectedIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc >= 1 && argc <= 2) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GController:setSelectedIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_setSelectedIndex'", nullptr);
			return 0;
		}
		if (argc > 1) {
			bool arg1;
			ok &= luaval_to_boolean(tolua_S, 3, &arg1, "fairygui.GController:setSelectedIndex");
			if (!ok) {
				tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_setSelectedIndex'", nullptr);
				return 0;
			}
			cobj->setSelectedIndex(arg0, arg1);
		} else {
			cobj->setSelectedIndex(arg0);
		}
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting 1~2\n", "fairygui.GController:setSelectedIndex",argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_setSelectedIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getPageId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPageId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GController:getPageId");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_getPageId'", nullptr);
			return 0;
		}
		const std::string& ret = cobj->getPageId(arg0);
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPageId",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPageId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getSelectedPageId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getSelectedPageId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getSelectedPageId();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getSelectedPageId",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getSelectedPageId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_setParent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_setParent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GComponent* arg0;
		ok &= luaval_to_object<fairygui::GComponent>(tolua_S, 2, "fairygui.GComponent",&arg0, "fairygui.GController:setParent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_setParent'", nullptr);
			return 0;
		}
		cobj->setParent(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:setParent",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_setParent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_runActions(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_runActions'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->runActions();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:runActions",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_runActions'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_setOppositePageId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_setOppositePageId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GController:setOppositePageId");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_setOppositePageId'", nullptr);
			return 0;
		}
		cobj->setOppositePageId(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:setOppositePageId",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_setOppositePageId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_hasPage(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_hasPage'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GController:hasPage");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_hasPage'", nullptr);
			return 0;
		}
		bool ret = cobj->hasPage(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:hasPage",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_hasPage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getPageNameById(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPageNameById'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GController:getPageNameById");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_getPageNameById'", nullptr);
			return 0;
		}
		const std::string& ret = cobj->getPageNameById(arg0);
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPageNameById",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPageNameById'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getPreviousPageId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPreviousPageId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getPreviousPageId();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPreviousPageId",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPreviousPageId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getPageCount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPageCount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getPageCount();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPageCount",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPageCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getSelectedIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getSelectedIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getSelectedIndex();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getSelectedIndex",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getSelectedIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getPreviousPage(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getPreviousPage'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getPreviousPage();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getPreviousPage",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getPreviousPage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_setSelectedPageId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_setSelectedPageId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GController:setSelectedPageId");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GController_setSelectedPageId'", nullptr);
			return 0;
		}
		cobj->setSelectedPageId(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:setSelectedPageId",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_setSelectedPageId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getSelectedPage(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getSelectedPage'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getSelectedPage();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getSelectedPage",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getSelectedPage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GController_getParent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GController* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GController",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GController*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GController_getParent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getParent();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GController:getParent",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GController_getParent'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GController(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GController");
	tolua_cclass(tolua_S,"GController","fairygui.GController","fairygui.UIEventDispatcher",nullptr);

	tolua_beginmodule(tolua_S,"GController");
	tolua_function(tolua_S,"getPageIndexById",lua_fairygui_GController_getPageIndexById);
	tolua_function(tolua_S,"setSelectedPage",lua_fairygui_GController_setSelectedPage);
	tolua_function(tolua_S,"getPrevisousIndex",lua_fairygui_GController_getPrevisousIndex);
	tolua_function(tolua_S,"setSelectedIndex",lua_fairygui_GController_setSelectedIndex);
	tolua_function(tolua_S,"getPageId",lua_fairygui_GController_getPageId);
	tolua_function(tolua_S,"getSelectedPageId",lua_fairygui_GController_getSelectedPageId);
	tolua_function(tolua_S,"setParent",lua_fairygui_GController_setParent);
	tolua_function(tolua_S,"runActions",lua_fairygui_GController_runActions);
	tolua_function(tolua_S,"setOppositePageId",lua_fairygui_GController_setOppositePageId);
	tolua_function(tolua_S,"hasPage",lua_fairygui_GController_hasPage);
	tolua_function(tolua_S,"getPageNameById",lua_fairygui_GController_getPageNameById);
	tolua_function(tolua_S,"getPreviousPageId",lua_fairygui_GController_getPreviousPageId);
	tolua_function(tolua_S,"getPageCount",lua_fairygui_GController_getPageCount);
	tolua_function(tolua_S,"getSelectedIndex",lua_fairygui_GController_getSelectedIndex);
	tolua_function(tolua_S,"getPreviousPage",lua_fairygui_GController_getPreviousPage);
	tolua_function(tolua_S,"setSelectedPageId",lua_fairygui_GController_setSelectedPageId);
	tolua_function(tolua_S,"getSelectedPage",lua_fairygui_GController_getSelectedPage);
	tolua_function(tolua_S,"getParent",lua_fairygui_GController_getParent);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GController).name();
	g_luaType[typeName] = "fairygui.GController";
	g_typeCast["GController"] = "fairygui.GController";
	return 1;
}

static int lua_fairygui_GObject_get_id(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_get_id'", nullptr);
		return 0;
	}
#endif

	tolua_pushstring(L, cobj->id.c_str());
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_get_id'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_get_name(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_get_name'", nullptr);
		return 0;
	}
#endif

	tolua_pushstring(L, cobj->name.c_str());
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_get_name'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_set_name(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_set_name'", nullptr);
		return 0;
	}
#endif
	cobj->name = tolua_tostring(L, 2, "");
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_set_name'.",&tolua_err);
	return 0;
#endif
}
static int lua_fairygui_GObject_get_sourceSize(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_get_sourceSize'", nullptr);
		return 0;
	}
#endif

	size_to_luaval(L, cobj->sourceSize);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_get_sourceSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_get_initSize(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_get_initSize'", nullptr);
		return 0;
	}
#endif

	size_to_luaval(L, cobj->initSize);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_get_initSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_get_minSize(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_get_minSize'", nullptr);
		return 0;
	}
#endif

	size_to_luaval(L, cobj->minSize);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_get_minSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_get_maxSize(lua_State* L)
{
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GObject_get_maxSize'", nullptr);
		return 0;
	}
#endif

	size_to_luaval(L, cobj->maxSize);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GObject_get_maxSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getXMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getXMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushnumber(tolua_S, cobj->getXMin());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getXMin", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getXMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setXMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setXMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cobj->setXMin(float(lua_tonumber(tolua_S, 2)));
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setXMin", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setXMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getYMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getYMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushnumber(tolua_S, cobj->getYMin());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getYMin", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getYMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setYMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setYMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cobj->setYMin(float(lua_tonumber(tolua_S, 2)));
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setYMin", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setYMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_isPivotAsAnchor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_isPivotAsAnchor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushboolean(tolua_S, cobj->isPivotAsAnchor());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:isPivotAsAnchor", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_isPivotAsAnchor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getProp(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getProp'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Value value = cobj->getProp((fairygui::ObjectPropID)lua_tointeger(tolua_S, 2));
		ccvalue_to_luaval(tolua_S, value);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getProp", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getProp'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setProp(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setProp'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		cocos2d::Value value;
		luaval_to_ccvalue(tolua_S, 3, &value, "fairygui.GObject:setProp");
		cobj->setProp((fairygui::ObjectPropID)lua_tointeger(tolua_S, 2), value);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setProp", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setProp'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_startDrag(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_startDrag'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->startDrag();
		return 0;
	}
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GObject:startDrag");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_startDrag'", nullptr);
			return 0;
		}
		cobj->startDrag(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:startDrag",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_startDrag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getPackageItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getPackageItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::PackageItem* ret = cobj->getPackageItem();
		object_to_luaval<fairygui::PackageItem>(tolua_S, "fairygui.PackageItem",(fairygui::PackageItem*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getPackageItem",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getPackageItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setGroup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setGroup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GGroup* arg0;
		ok &= luaval_to_object<fairygui::GGroup>(tolua_S, 2, "fairygui.GGroup",&arg0, "fairygui.GObject:setGroup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setGroup'", nullptr);
			return 0;
		}
		cobj->setGroup(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setGroup",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setGroup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_isDraggable(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_isDraggable'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isDraggable();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:isDraggable",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_isDraggable'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_relations(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_relations'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::Relations* ret = cobj->relations();
		object_to_luaval<fairygui::Relations>(tolua_S, "fairygui.Relations",(fairygui::Relations*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:relations",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_relations'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_isTouchable(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_isTouchable'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isTouchable();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:isTouchable",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_isTouchable'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setScale(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setScale'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setScale");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GObject:setScale");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setScale'", nullptr);
			return 0;
		}
		cobj->setScale(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setScale",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getWidth(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getWidth'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getWidth();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getWidth",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getWidth'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_displayObject(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_displayObject'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Node* ret = cobj->displayObject();
		object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node",(cocos2d::Node*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:displayObject",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_displayObject'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Ref *arg0;
		ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 2, "cc.Ref", &arg0, "fairygui.GObject:setData");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setData'", nullptr);
			return 0;
		}
		cobj->setData((void *)arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setData",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Ref *ret = (cocos2d::Ref *)cobj->getData();
		object_to_luaval<cocos2d::Ref>(tolua_S, "cc.Ref", ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getData",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setCustomData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setCustomData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Value arg0;
		ok &= luaval_to_ccvalue(tolua_S, 2, &arg0, "fairygui.GObject:setCustomData");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setCustomData'", nullptr);
			return 0;
		}
		cobj->setCustomData(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setCustomData",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setCustomData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getCustomData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getCustomData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Value &ret = cobj->getCustomData();
		ccvalue_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getCustomData",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getCustomData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getRotation(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getRotation'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getRotation();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getRotation",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getRotation'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_stopDrag(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_stopDrag'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->stopDrag();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:stopDrag",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_stopDrag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getSortingOrder(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getSortingOrder'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getSortingOrder();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getSortingOrder",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getSortingOrder'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_isGrayed(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_isGrayed'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isGrayed();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:isGrayed",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_isGrayed'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setWidth(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setWidth'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setWidth");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setWidth'", nullptr);
			return 0;
		}
		cobj->setWidth(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setWidth",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setWidth'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getScale(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getScale'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vec2& ret = cobj->getScale();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getScale",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getResourceURL(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getResourceURL'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		std::string ret = cobj->getResourceURL();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getResourceURL",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getResourceURL'.",&tolua_err);
	return 0;
#endif
}

int lua_fairygui_GObject_isPixelSnapping(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_isPixelSnapping'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isPixelSnapping();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:isPixelSnapping",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_isPixelSnapping'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setTooltips(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setTooltips'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GObject:setTooltips");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setTooltips'", nullptr);
			return 0;
		}
		cobj->setTooltips(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setTooltips",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setTooltips'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_localToGlobal(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_localToGlobal'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Rect arg0;
		ok = luaval_to_rect(tolua_S, 2, &arg0, "fairygui.GObject:localToGlobal");
		if (ok) {
			cocos2d::Rect ret = cobj->localToGlobal(arg0);
			rect_to_luaval(tolua_S, ret);
			return 1;
		}
		cocos2d::Vec2 arg00;
		ok = luaval_to_vec2(tolua_S, 2, &arg00, "fairygui.GObject:localToGlobal");
		if (ok) {
			cocos2d::Vec2 ret = cobj->localToGlobal(arg00);
			vec2_to_luaval(tolua_S, ret);
			return 1;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_localToGlobal'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GObject:localToGlobal",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_localToGlobal'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getScaleY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getScaleY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getScaleY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getScaleY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getScaleY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getScaleX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getScaleX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getScaleX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getScaleX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getScaleX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_onStage(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_onStage'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->onStage();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:onStage",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_onStage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getHeight(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getHeight'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getHeight();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getHeight",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getHeight'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getGear(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getGear'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GObject:getGear");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_getGear'", nullptr);
			return 0;
		}
		fairygui::GearBase* ret = cobj->getGear(arg0);
		object_to_luaval<fairygui::GearBase>(tolua_S, "fairygui.GearBase",(fairygui::GearBase*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getGear",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getGear'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_globalToLocal(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_globalToLocal'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Rect arg0;
		ok = luaval_to_rect(tolua_S, 2, &arg0, "fairygui.GObject:globalToLocal");
		if (ok) {
			cocos2d::Rect ret = cobj->globalToLocal(arg0);
			rect_to_luaval(tolua_S, ret);
			return 1;
		}
		cocos2d::Vec2 arg00;
		ok = luaval_to_vec2(tolua_S, 2, &arg00, "fairygui.GObject:globalToLocal");
		if (ok) {
			cocos2d::Vec2 ret = cobj->globalToLocal(arg00);
			vec2_to_luaval(tolua_S, ret);
			return 1;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_globalToLocal'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GObject:globalToLocal",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_globalToLocal'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getGroup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getGroup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GGroup* ret = cobj->getGroup();
		object_to_luaval<fairygui::GGroup>(tolua_S, "fairygui.GGroup",(fairygui::GGroup*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getGroup",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getGroup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_removeFromParent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_removeFromParent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->removeFromParent();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:removeFromParent",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_removeFromParent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_constructFromResource(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_constructFromResource'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->constructFromResource();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:constructFromResource",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_constructFromResource'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setSortingOrder(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setSortingOrder'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GObject:setSortingOrder");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setSortingOrder'", nullptr);
			return 0;
		}
		cobj->setSortingOrder(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setSortingOrder",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setSortingOrder'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setAlpha(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setAlpha'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setAlpha");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setAlpha'", nullptr);
			return 0;
		}
		cobj->setAlpha(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setAlpha",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setAlpha'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Size& ret = cobj->getSize();
		size_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getSkewY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getSkewY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getSkewY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getSkewY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getSkewY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setRotation(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setRotation'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setRotation");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setRotation'", nullptr);
			return 0;
		}
		cobj->setRotation(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setRotation",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setRotation'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_addDisplayLock(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_addDisplayLock'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		unsigned int ret = cobj->addDisplayLock();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:addDisplayLock",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_addDisplayLock'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setPixelSnapping(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setPixelSnapping'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GObject:setPixelSnapping");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setPixelSnapping'", nullptr);
			return 0;
		}
		cobj->setPixelSnapping(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setPixelSnapping",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setPixelSnapping'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_removeRelation(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_removeRelation'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		fairygui::RelationType arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GObject:removeRelation");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GObject:removeRelation");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_removeRelation'", nullptr);
			return 0;
		}
		cobj->removeRelation(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:removeRelation",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_removeRelation'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setDraggable(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setDraggable'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GObject:setDraggable");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setDraggable'", nullptr);
			return 0;
		}
		cobj->setDraggable(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setDraggable",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setDraggable'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setIcon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setIcon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GObject:setIcon");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setIcon'", nullptr);
			return 0;
		}
		cobj->setIcon(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setIcon",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setIcon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setScaleY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setScaleY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setScaleY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setScaleY'", nullptr);
			return 0;
		}
		cobj->setScaleY(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setScaleY",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setScaleY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setScaleX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setScaleX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setScaleX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setScaleX'", nullptr);
			return 0;
		}
		cobj->setScaleX(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setScaleX",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setScaleX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getSkewX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getSkewX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getSkewX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getSkewX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getSkewX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_releaseDisplayLock(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_releaseDisplayLock'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		unsigned int arg0;
		ok &= luaval_to_uint32(tolua_S, 2,&arg0, "fairygui.GObject:releaseDisplayLock");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_releaseDisplayLock'", nullptr);
			return 0;
		}
		cobj->releaseDisplayLock(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:releaseDisplayLock",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_releaseDisplayLock'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setDragBounds(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setDragBounds'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Rect arg0;
		ok &= luaval_to_rect(tolua_S, 2, &arg0, "fairygui.GObject:setDragBounds");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setDragBounds'", nullptr);
			return 0;
		}
		cobj->setDragBounds(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setDragBounds",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setDragBounds'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_makeFullScreen(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_makeFullScreen'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->makeFullScreen();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:makeFullScreen",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_makeFullScreen'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_transformRect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_transformRect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		cocos2d::Rect arg0;
		fairygui::GObject* arg1;
		ok &= luaval_to_rect(tolua_S, 2, &arg0, "fairygui.GObject:transformRect");
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 3, "fairygui.GObject",&arg1, "fairygui.GObject:transformRect");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_transformRect'", nullptr);
			return 0;
		}
		cocos2d::Rect ret = cobj->transformRect(arg0, arg1);
		rect_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:transformRect",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_transformRect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setTouchable(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setTouchable'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GObject:setTouchable");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setTouchable'", nullptr);
			return 0;
		}
		cobj->setTouchable(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setTouchable",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setTouchable'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getAlpha(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getAlpha'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getAlpha();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getAlpha",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getAlpha'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setGrayed(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setGrayed'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GObject:setGrayed");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setGrayed'", nullptr);
			return 0;
		}
		cobj->setGrayed(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setGrayed",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setGrayed'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getText(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getText'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getText();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getText",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getText'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setPosition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setPosition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setPosition");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GObject:setPosition");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setPosition'", nullptr);
			return 0;
		}
		cobj->setPosition(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setPosition",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setPosition'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setX'", nullptr);
			return 0;
		}
		cobj->setX(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setX",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setY'", nullptr);
			return 0;
		}
		cobj->setY(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setY",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getPosition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getPosition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vec2& ret = cobj->getPosition();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getPosition",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getPosition'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getParent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getParent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getParent();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getParent",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getParent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setHeight(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setHeight'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setHeight");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setHeight'", nullptr);
			return 0;
		}
		cobj->setHeight(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setHeight",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setHeight'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getRoot(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getRoot'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GRoot* ret = cobj->getRoot();
		object_to_luaval<fairygui::GRoot>(tolua_S, "fairygui.GRoot",(fairygui::GRoot*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getRoot",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getRoot'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_treeNode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_treeNode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTreeNode* ret = cobj->treeNode();
		object_to_luaval<fairygui::GTreeNode>(tolua_S, "fairygui.GTreeNode",(fairygui::GTreeNode*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:treeNode",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_treeNode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_center(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_center'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->center();
		return 0;
	}
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GObject:center");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_center'", nullptr);
			return 0;
		}
		cobj->center(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:center",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_center'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getIcon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getIcon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getIcon();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getIcon",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getIcon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setVisible(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setVisible'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GObject:setVisible");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setVisible'", nullptr);
			return 0;
		}
		cobj->setVisible(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setVisible",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setVisible'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setText(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setText'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GObject:setText");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setText'", nullptr);
			return 0;
		}
		cobj->setText(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setText",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setText'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_isVisible(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_isVisible'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isVisible();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:isVisible",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_isVisible'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_hitTest(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_hitTest'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		cocos2d::Vec2 arg0;
		const cocos2d::Camera* arg1;
		ok &= luaval_to_vec2(tolua_S, 2, &arg0, "fairygui.GObject:hitTest");
		ok &= luaval_to_object<const cocos2d::Camera>(tolua_S, 3, "cc.Camera",&arg1, "fairygui.GObject:hitTest");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_hitTest'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->hitTest(arg0, arg1);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:hitTest",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_hitTest'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getDragBounds(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getDragBounds'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Rect* ret = cobj->getDragBounds();
		object_to_luaval<cocos2d::Rect>(tolua_S, "cc.Rect",(cocos2d::Rect*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getDragBounds",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getDragBounds'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_addRelation(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_addRelation'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		fairygui::RelationType arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GObject:addRelation");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GObject:addRelation");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_addRelation'", nullptr);
			return 0;
		}
		cobj->addRelation(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		fairygui::GObject* arg0;
		fairygui::RelationType arg1;
		bool arg2;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GObject:addRelation");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GObject:addRelation");
		ok &= luaval_to_boolean(tolua_S, 4,&arg2, "fairygui.GObject:addRelation");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_addRelation'", nullptr);
			return 0;
		}
		cobj->addRelation(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:addRelation",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_addRelation'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setPivot(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setPivot'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setPivot");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GObject:setPivot");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setPivot'", nullptr);
			return 0;
		}
		cobj->setPivot(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		double arg0;
		double arg1;
		bool arg2;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setPivot");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GObject:setPivot");
		ok &= luaval_to_boolean(tolua_S, 4,&arg2, "fairygui.GObject:setPivot");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setPivot'", nullptr);
			return 0;
		}
		cobj->setPivot(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setPivot",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setPivot'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_checkGearController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_checkGearController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		int arg0;
		fairygui::GController* arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GObject:checkGearController");
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 3, "fairygui.GController",&arg1, "fairygui.GObject:checkGearController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_checkGearController'", nullptr);
			return 0;
		}
		bool ret = cobj->checkGearController(arg0, arg1);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:checkGearController",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_checkGearController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getPivot(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getPivot'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vec2& ret = cobj->getPivot();
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getPivot",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getPivot'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setSkewX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setSkewX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setSkewX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setSkewX'", nullptr);
			return 0;
		}
		cobj->setSkewX(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setSkewX",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setSkewX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setSkewY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setSkewY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setSkewY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setSkewY'", nullptr);
			return 0;
		}
		cobj->setSkewY(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setSkewY",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setSkewY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_setSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_setSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setSize");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GObject:setSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setSize'", nullptr);
			return 0;
		}
		cobj->setSize(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		double arg0;
		double arg1;
		bool arg2;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GObject:setSize");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GObject:setSize");
		ok &= luaval_to_boolean(tolua_S, 4,&arg2, "fairygui.GObject:setSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObject_setSize'", nullptr);
			return 0;
		}
		cobj->setSize(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:setSize",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_setSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getTooltips(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObject_getTooltips'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getTooltips();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObject:getTooltips",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getTooltips'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::GObject* ret = fairygui::GObject::create();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GObject:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObject_getDraggingObject(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GObject",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::GObject* ret = fairygui::GObject::getDraggingObject();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GObject:getDraggingObject",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObject_getDraggingObject'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GObject(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GObject");
	tolua_cclass(tolua_S,"GObject","fairygui.GObject","fairygui.UIEventDispatcher",nullptr);

	tolua_beginmodule(tolua_S,"GObject");
	// variable
	tolua_variable(tolua_S,"id",lua_fairygui_GObject_get_id, nullptr);
	tolua_variable(tolua_S,"name",lua_fairygui_GObject_get_name, lua_fairygui_GObject_set_name);
	tolua_variable(tolua_S,"sourceSize",lua_fairygui_GObject_get_sourceSize, nullptr);
	tolua_variable(tolua_S,"initSize",lua_fairygui_GObject_get_initSize, nullptr);
	tolua_variable(tolua_S,"minSize",lua_fairygui_GObject_get_minSize, nullptr);
	tolua_variable(tolua_S,"maxSize",lua_fairygui_GObject_get_maxSize, nullptr);
	// function
	tolua_function(tolua_S,"getXMin",lua_fairygui_GObject_getXMin);
	tolua_function(tolua_S,"setXMin",lua_fairygui_GObject_setXMin);
	tolua_function(tolua_S,"getYMin",lua_fairygui_GObject_getYMin);
	tolua_function(tolua_S,"setYMin",lua_fairygui_GObject_setYMin);
	tolua_function(tolua_S,"isPivotAsAnchor",lua_fairygui_GObject_isPivotAsAnchor);
	tolua_function(tolua_S,"getProp",lua_fairygui_GObject_getProp);
	tolua_function(tolua_S,"setProp",lua_fairygui_GObject_setProp);
	tolua_function(tolua_S,"startDrag",lua_fairygui_GObject_startDrag);
	tolua_function(tolua_S,"getPackageItem",lua_fairygui_GObject_getPackageItem);
	tolua_function(tolua_S,"setGroup",lua_fairygui_GObject_setGroup);
	tolua_function(tolua_S,"isDraggable",lua_fairygui_GObject_isDraggable);
	tolua_function(tolua_S,"relations",lua_fairygui_GObject_relations);
	tolua_function(tolua_S,"isTouchable",lua_fairygui_GObject_isTouchable);
	tolua_function(tolua_S,"setScale",lua_fairygui_GObject_setScale);
	tolua_function(tolua_S,"getWidth",lua_fairygui_GObject_getWidth);
	tolua_function(tolua_S,"getY",lua_fairygui_GObject_getY);
	tolua_function(tolua_S,"displayObject",lua_fairygui_GObject_displayObject);
	tolua_function(tolua_S,"setData",lua_fairygui_GObject_setData);
	tolua_function(tolua_S,"getData",lua_fairygui_GObject_getData);
	tolua_function(tolua_S,"setCustomData",lua_fairygui_GObject_setCustomData);
	tolua_function(tolua_S,"getCustomData",lua_fairygui_GObject_getCustomData);
	tolua_function(tolua_S,"getRotation",lua_fairygui_GObject_getRotation);
	tolua_function(tolua_S,"stopDrag",lua_fairygui_GObject_stopDrag);
	tolua_function(tolua_S,"getSortingOrder",lua_fairygui_GObject_getSortingOrder);
	tolua_function(tolua_S,"isGrayed",lua_fairygui_GObject_isGrayed);
	tolua_function(tolua_S,"setWidth",lua_fairygui_GObject_setWidth);
	tolua_function(tolua_S,"getScale",lua_fairygui_GObject_getScale);
	tolua_function(tolua_S,"getResourceURL",lua_fairygui_GObject_getResourceURL);
	tolua_function(tolua_S,"isPixelSnapping",lua_fairygui_GObject_isPixelSnapping);
	tolua_function(tolua_S,"setTooltips",lua_fairygui_GObject_setTooltips);
	tolua_function(tolua_S,"localToGlobal",lua_fairygui_GObject_localToGlobal);
	tolua_function(tolua_S,"getScaleY",lua_fairygui_GObject_getScaleY);
	tolua_function(tolua_S,"getScaleX",lua_fairygui_GObject_getScaleX);
	tolua_function(tolua_S,"onStage",lua_fairygui_GObject_onStage);
	tolua_function(tolua_S,"getHeight",lua_fairygui_GObject_getHeight);
	tolua_function(tolua_S,"getGear",lua_fairygui_GObject_getGear);
	tolua_function(tolua_S,"globalToLocal",lua_fairygui_GObject_globalToLocal);
	tolua_function(tolua_S,"getGroup",lua_fairygui_GObject_getGroup);
	tolua_function(tolua_S,"removeFromParent",lua_fairygui_GObject_removeFromParent);
	tolua_function(tolua_S,"constructFromResource",lua_fairygui_GObject_constructFromResource);
	tolua_function(tolua_S,"setSortingOrder",lua_fairygui_GObject_setSortingOrder);
	tolua_function(tolua_S,"setAlpha",lua_fairygui_GObject_setAlpha);
	tolua_function(tolua_S,"getSize",lua_fairygui_GObject_getSize);
	tolua_function(tolua_S,"getSkewY",lua_fairygui_GObject_getSkewY);
	tolua_function(tolua_S,"setRotation",lua_fairygui_GObject_setRotation);
	tolua_function(tolua_S,"addDisplayLock",lua_fairygui_GObject_addDisplayLock);
	tolua_function(tolua_S,"setPixelSnapping",lua_fairygui_GObject_setPixelSnapping);
	tolua_function(tolua_S,"removeRelation",lua_fairygui_GObject_removeRelation);
	tolua_function(tolua_S,"setDraggable",lua_fairygui_GObject_setDraggable);
	tolua_function(tolua_S,"setIcon",lua_fairygui_GObject_setIcon);
	tolua_function(tolua_S,"setScaleY",lua_fairygui_GObject_setScaleY);
	tolua_function(tolua_S,"setScaleX",lua_fairygui_GObject_setScaleX);
	tolua_function(tolua_S,"getSkewX",lua_fairygui_GObject_getSkewX);
	tolua_function(tolua_S,"releaseDisplayLock",lua_fairygui_GObject_releaseDisplayLock);
	tolua_function(tolua_S,"setDragBounds",lua_fairygui_GObject_setDragBounds);
	tolua_function(tolua_S,"makeFullScreen",lua_fairygui_GObject_makeFullScreen);
	tolua_function(tolua_S,"transformRect",lua_fairygui_GObject_transformRect);
	tolua_function(tolua_S,"setTouchable",lua_fairygui_GObject_setTouchable);
	tolua_function(tolua_S,"getAlpha",lua_fairygui_GObject_getAlpha);
	tolua_function(tolua_S,"setGrayed",lua_fairygui_GObject_setGrayed);
	tolua_function(tolua_S,"getText",lua_fairygui_GObject_getText);
	tolua_function(tolua_S,"setPosition",lua_fairygui_GObject_setPosition);
	tolua_function(tolua_S,"setX",lua_fairygui_GObject_setX);
	tolua_function(tolua_S,"setY",lua_fairygui_GObject_setY);
	tolua_function(tolua_S,"getPosition",lua_fairygui_GObject_getPosition);
	tolua_function(tolua_S,"getParent",lua_fairygui_GObject_getParent);
	tolua_function(tolua_S,"setHeight",lua_fairygui_GObject_setHeight);
	tolua_function(tolua_S,"getX",lua_fairygui_GObject_getX);
	tolua_function(tolua_S,"getRoot",lua_fairygui_GObject_getRoot);
	tolua_function(tolua_S,"treeNode",lua_fairygui_GObject_treeNode);
	tolua_function(tolua_S,"center",lua_fairygui_GObject_center);
	tolua_function(tolua_S,"getIcon",lua_fairygui_GObject_getIcon);
	tolua_function(tolua_S,"setVisible",lua_fairygui_GObject_setVisible);
	tolua_function(tolua_S,"setText",lua_fairygui_GObject_setText);
	tolua_function(tolua_S,"isVisible",lua_fairygui_GObject_isVisible);
	tolua_function(tolua_S,"hitTest",lua_fairygui_GObject_hitTest);
	tolua_function(tolua_S,"getDragBounds",lua_fairygui_GObject_getDragBounds);
	tolua_function(tolua_S,"addRelation",lua_fairygui_GObject_addRelation);
	tolua_function(tolua_S,"setPivot",lua_fairygui_GObject_setPivot);
	tolua_function(tolua_S,"checkGearController",lua_fairygui_GObject_checkGearController);
	tolua_function(tolua_S,"getPivot",lua_fairygui_GObject_getPivot);
	tolua_function(tolua_S,"setSkewX",lua_fairygui_GObject_setSkewX);
	tolua_function(tolua_S,"setSkewY",lua_fairygui_GObject_setSkewY);
	tolua_function(tolua_S,"setSize",lua_fairygui_GObject_setSize);
	tolua_function(tolua_S,"getTooltips",lua_fairygui_GObject_getTooltips);
	tolua_function(tolua_S,"create", lua_fairygui_GObject_create);
	tolua_function(tolua_S,"getDraggingObject", lua_fairygui_GObject_getDraggingObject);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GObject).name();
	g_luaType[typeName] = "fairygui.GObject";
	g_typeCast["GObject"] = "fairygui.GObject";
	return 1;
}

static int lua_fairygui_UIPackage_getName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIPackage* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIPackage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIPackage_getName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getName();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIPackage:getName",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getId(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIPackage* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIPackage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIPackage_getId'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getId();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIPackage:getId",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getId'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getItemByName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIPackage* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIPackage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIPackage_getItemByName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:getItemByName");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_getItemByName'", nullptr);
			return 0;
		}
		fairygui::PackageItem* ret = cobj->getItemByName(arg0);
		object_to_luaval<fairygui::PackageItem>(tolua_S, "fairygui.PackageItem",(fairygui::PackageItem*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIPackage:getItemByName",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getItemByName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::UIPackage* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::UIPackage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_UIPackage_getItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:getItem");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_getItem'", nullptr);
			return 0;
		}
		fairygui::PackageItem* ret = cobj->getItem(arg0);
		object_to_luaval<fairygui::PackageItem>(tolua_S, "fairygui.PackageItem",(fairygui::PackageItem*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.UIPackage:getItem",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_createObjectFromURL(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:createObjectFromURL");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_createObjectFromURL'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = fairygui::UIPackage::createObjectFromURL(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:createObjectFromURL",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_createObjectFromURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_removePackage(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:removePackage");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_removePackage'", nullptr);
			return 0;
		}
		fairygui::UIPackage::removePackage(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:removePackage",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_removePackage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getItemURL(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:getItemURL");
		ok &= luaval_to_std_string(tolua_S, 3,&arg1, "fairygui.UIPackage:getItemURL");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_getItemURL'", nullptr);
			return 0;
		}
		std::string ret = fairygui::UIPackage::getItemURL(arg0, arg1);
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:getItemURL",argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getItemURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_normalizeURL(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:normalizeURL");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_normalizeURL'", nullptr);
			return 0;
		}
		std::string ret = fairygui::UIPackage::normalizeURL(arg0);
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:normalizeURL",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_normalizeURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_removeAllPackages(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::UIPackage::removeAllPackages();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:removeAllPackages",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_removeAllPackages'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_createObject(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:createObject");
		ok &= luaval_to_std_string(tolua_S, 3,&arg1, "fairygui.UIPackage:createObject");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_createObject'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = fairygui::UIPackage::createObject(arg0, arg1);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:createObject",argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_createObject'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getByName(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:getByName");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_getByName'", nullptr);
			return 0;
		}
		fairygui::UIPackage* ret = fairygui::UIPackage::getByName(arg0);
		object_to_luaval<fairygui::UIPackage>(tolua_S, "fairygui.UIPackage",(fairygui::UIPackage*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:getByName",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getByName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_addPackage(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:addPackage");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_addPackage'", nullptr);
			return 0;
		}
		fairygui::UIPackage* ret = fairygui::UIPackage::addPackage(arg0);
		object_to_luaval<fairygui::UIPackage>(tolua_S, "fairygui.UIPackage",(fairygui::UIPackage*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:addPackage",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_addPackage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getById(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:getById");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_getById'", nullptr);
			return 0;
		}
		fairygui::UIPackage* ret = fairygui::UIPackage::getById(arg0);
		object_to_luaval<fairygui::UIPackage>(tolua_S, "fairygui.UIPackage",(fairygui::UIPackage*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:getById",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getById'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getItemByURL(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.UIPackage:getItemByURL");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIPackage_getItemByURL'", nullptr);
			return 0;
		}
		fairygui::PackageItem* ret = fairygui::UIPackage::getItemByURL(arg0);
		object_to_luaval<fairygui::PackageItem>(tolua_S, "fairygui.PackageItem",(fairygui::PackageItem*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:getItemByURL",argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getItemByURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIPackage_getEmptyTexture(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIPackage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		cocos2d::Texture2D* ret = fairygui::UIPackage::getEmptyTexture();
		object_to_luaval<cocos2d::Texture2D>(tolua_S, "cc.Texture2D",(cocos2d::Texture2D*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIPackage:getEmptyTexture",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIPackage_getEmptyTexture'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_UIPackage(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.UIPackage");
	tolua_cclass(tolua_S,"UIPackage","fairygui.UIPackage","",nullptr);

	tolua_beginmodule(tolua_S,"UIPackage");
	tolua_function(tolua_S,"getName",lua_fairygui_UIPackage_getName);
	tolua_function(tolua_S,"getId",lua_fairygui_UIPackage_getId);
	tolua_function(tolua_S,"getItemByName",lua_fairygui_UIPackage_getItemByName);
	tolua_function(tolua_S,"getItem",lua_fairygui_UIPackage_getItem);
	tolua_function(tolua_S,"createObjectFromURL", lua_fairygui_UIPackage_createObjectFromURL);
	tolua_function(tolua_S,"removePackage", lua_fairygui_UIPackage_removePackage);
	tolua_function(tolua_S,"getItemURL", lua_fairygui_UIPackage_getItemURL);
	tolua_function(tolua_S,"normalizeURL", lua_fairygui_UIPackage_normalizeURL);
	tolua_function(tolua_S,"removeAllPackages", lua_fairygui_UIPackage_removeAllPackages);
	tolua_function(tolua_S,"createObject", lua_fairygui_UIPackage_createObject);
	tolua_function(tolua_S,"getByName", lua_fairygui_UIPackage_getByName);
	tolua_function(tolua_S,"addPackage", lua_fairygui_UIPackage_addPackage);
	tolua_function(tolua_S,"getById", lua_fairygui_UIPackage_getById);
	tolua_function(tolua_S,"getItemByURL", lua_fairygui_UIPackage_getItemByURL);
	tolua_function(tolua_S,"getEmptyTexture", lua_fairygui_UIPackage_getEmptyTexture);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::UIPackage).name();
	g_luaType[typeName] = "fairygui.UIPackage";
	g_typeCast["UIPackage"] = "fairygui.UIPackage";
	return 1;
}

static int lua_fairygui_GImage_setColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GImage* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GImage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GImage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GImage_setColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &=luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GImage:setColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GImage_setColor'", nullptr);
			return 0;
		}
		cobj->setColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GImage:setColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GImage_setColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GImage_getColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GImage* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GImage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GImage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GImage_getColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GImage:getColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GImage_getColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GImage_getFlip(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GImage* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GImage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GImage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GImage_getFlip'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getFlip();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GImage:getFlip",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GImage_getFlip'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GImage_setFlip(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GImage* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GImage",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GImage*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GImage_setFlip'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::FlipType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GImage:setFlip");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GImage_setFlip'", nullptr);
			return 0;
		}
		cobj->setFlip(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GImage:setFlip",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GImage_setFlip'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GImage_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GImage",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::GImage* ret = fairygui::GImage::create();
		object_to_luaval<fairygui::GImage>(tolua_S, "fairygui.GImage",(fairygui::GImage*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GImage:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GImage_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GImage(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GImage");
	tolua_cclass(tolua_S,"GImage","fairygui.GImage","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GImage");
	tolua_function(tolua_S,"setColor",lua_fairygui_GImage_setColor);
	tolua_function(tolua_S,"getColor",lua_fairygui_GImage_getColor);
	tolua_function(tolua_S,"getFlip",lua_fairygui_GImage_getFlip);
	tolua_function(tolua_S,"setFlip",lua_fairygui_GImage_setFlip);
	tolua_function(tolua_S,"create", lua_fairygui_GImage_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GImage).name();
	g_luaType[typeName] = "fairygui.GImage";
	g_typeCast["GImage"] = "fairygui.GImage";
	return 1;
}

static int lua_fairygui_GMovieClip_getFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_getFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getFrame();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:getFrame",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_getFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_setColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_setColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GMovieClip:setColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setColor'", nullptr);
			return 0;
		}
		cobj->setColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:setColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_setColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_getColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_getColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:getColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_getColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_getFlip(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_getFlip'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getFlip();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:getFlip",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_getFlip'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_setFlip(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_setFlip'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::FlipType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setFlip");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setFlip'", nullptr);
			return 0;
		}
		cobj->setFlip(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:setFlip",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_setFlip'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_setPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_setPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GMovieClip:setPlaying");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setPlaying'", nullptr);
			return 0;
		}
		cobj->setPlaying(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:setPlaying",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_setPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_setPlaySettings(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_setPlaySettings'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->setPlaySettings();
		return 0;
	}
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setPlaySettings");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setPlaySettings'", nullptr);
			return 0;
		}
		cobj->setPlaySettings(arg0);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		int arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GMovieClip:setPlaySettings");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setPlaySettings'", nullptr);
			return 0;
		}
		cobj->setPlaySettings(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		int arg0;
		int arg1;
		int arg2;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GMovieClip:setPlaySettings");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setPlaySettings'", nullptr);
			return 0;
		}
		cobj->setPlaySettings(arg0, arg1, arg2);
		return 0;
	}
	if (argc == 4) {
		int arg0;
		int arg1;
		int arg2;
		int arg3;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "fairygui.GMovieClip:setPlaySettings");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setPlaySettings'", nullptr);
			return 0;
		}
		cobj->setPlaySettings(arg0, arg1, arg2, arg3);
		return 0;
	}
	if (argc == 5) {
		int arg0;
		int arg1;
		int arg2;
		int arg3;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GMovieClip:setPlaySettings");
		ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3, "fairygui.GMovieClip:setPlaySettings");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 6, 0));
		if(!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setPlaySettings'", nullptr);
			return 0;
		}
		cobj->setPlaySettings(arg0, arg1, arg2, arg3, [=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:setPlaySettings",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_setPlaySettings'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_isPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_isPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isPlaying();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:isPlaying",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_isPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_setFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_setFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GMovieClip:setFrame");
		if(!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GMovieClip_setFrame'", nullptr);
			return 0;
		}
		cobj->setFrame(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:setFrame",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_setFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_reverse(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GMovieClip* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GMovieClip*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GMovieClip_reverse'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->reverse();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GMovieClip:reverse",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_reverse'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GMovieClip_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GMovieClip",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GMovieClip* ret = fairygui::GMovieClip::create();
		object_to_luaval<fairygui::GMovieClip>(tolua_S, "fairygui.GMovieClip",(fairygui::GMovieClip*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GMovieClip:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GMovieClip_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GMovieClip(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GMovieClip");
	tolua_cclass(tolua_S,"GMovieClip","fairygui.GMovieClip","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GMovieClip");
	tolua_function(tolua_S,"getFrame",lua_fairygui_GMovieClip_getFrame);
	tolua_function(tolua_S,"setColor",lua_fairygui_GMovieClip_setColor);
	tolua_function(tolua_S,"getColor",lua_fairygui_GMovieClip_getColor);
	tolua_function(tolua_S,"getFlip",lua_fairygui_GMovieClip_getFlip);
	tolua_function(tolua_S,"setFlip",lua_fairygui_GMovieClip_setFlip);
	tolua_function(tolua_S,"setPlaying",lua_fairygui_GMovieClip_setPlaying);
	tolua_function(tolua_S,"setPlaySettings",lua_fairygui_GMovieClip_setPlaySettings);
	tolua_function(tolua_S,"isPlaying",lua_fairygui_GMovieClip_isPlaying);
	tolua_function(tolua_S,"setFrame",lua_fairygui_GMovieClip_setFrame);
	tolua_function(tolua_S,"reverse", lua_fairygui_GMovieClip_reverse);
	tolua_function(tolua_S,"create", lua_fairygui_GMovieClip_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GMovieClip).name();
	g_luaType[typeName] = "fairygui.GMovieClip";
	g_typeCast["GMovieClip"] = "fairygui.GMovieClip";
	return 1;
}

static int lua_fairygui_GTextField_setAutoSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setAutoSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::AutoSizeType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GTextField:setAutoSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setAutoSize'", nullptr);
			return 0;
		}
		cobj->setAutoSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setAutoSize",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setAutoSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getFontSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getFontSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		float ret = cobj->getFontSize();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getFontSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getFontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GTextField:setColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setColor'", nullptr);
			return 0;
		}
		cobj->setColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setSingleLine(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setSingleLine'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GTextField:setSingleLine");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setSingleLine'", nullptr);
			return 0;
		}
		cobj->setSingleLine(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setSingleLine",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setSingleLine'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_isUBBEnabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_isUBBEnabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isUBBEnabled();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:isUBBEnabled",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_isUBBEnabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setUBBEnabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setUBBEnabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GTextField:setUBBEnabled");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setUBBEnabled'", nullptr);
			return 0;
		}
		cobj->setUBBEnabled(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setUBBEnabled",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setUBBEnabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getAutoSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getAutoSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getAutoSize();
		lua_pushinteger(tolua_S,ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getAutoSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getAutoSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setFontSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setFontSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GTextField:setFontSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setFontSize'", nullptr);
			return 0;
		}
		cobj->setFontSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setFontSize",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setFontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_applyTextFormat(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_applyTextFormat'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->applyTextFormat();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:applyTextFormat",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_applyTextFormat'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getTextFormat(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getTextFormat'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::TextFormat* ret = cobj->getTextFormat();
		object_to_luaval<fairygui::TextFormat>(tolua_S, "fairygui.TextFormat",(fairygui::TextFormat*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getTextFormat",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getTextFormat'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_isSingleLine(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_isSingleLine'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isSingleLine();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:isSingleLine",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_isSingleLine'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getTextSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getTextSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Size& ret = cobj->getTextSize();
		size_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getTextSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getTextSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getOutlineColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getOutlineColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Color3B ret = cobj->getOutlineColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getOutlineColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getOutlineColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setOutlineColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setOutlineColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		if (luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GTextField:setOutlineColor")) {
			cobj->setOutlineColor(arg0);
			return 0;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setOutlineColor'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setOutlineColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setOutlineColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_getTemplateVars(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_getTemplateVars'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::ValueMap *ret = cobj->getTemplateVars();
		ccvaluemap_to_luaval(tolua_S, *ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:getTemplateVars",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_getTemplateVars'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setTemplateVars(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setTemplateVars'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::ValueMap arg0;
		if (luaval_to_ccvaluemap(tolua_S, 2, &arg0, "fairygui.GTextField:setTemplateVars")) {
			cobj->setTemplateVars(&arg0);
			return 0;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setTemplateVars'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setTemplateVars",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setTemplateVars'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_setVar(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_setVar'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		cocos2d::Value arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "fairygui.GTextField:setVar");
		ok &= luaval_to_ccvalue(tolua_S, 3, &arg1, "fairygui.GTextField:setVar");
		if (ok) {
			cobj->setVar(arg0, arg1);
			return 0;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextField_setVar'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:setVar",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_setVar'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextField_flushVars(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextField_flushVars'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->flushVars();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextField:flushVars",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextField_flushVars'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GTextField(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GTextField");
	tolua_cclass(tolua_S,"GTextField","fairygui.GTextField","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GTextField");
	tolua_function(tolua_S,"setAutoSize",lua_fairygui_GTextField_setAutoSize);
	tolua_function(tolua_S,"getFontSize",lua_fairygui_GTextField_getFontSize);
	tolua_function(tolua_S,"setColor",lua_fairygui_GTextField_setColor);
	tolua_function(tolua_S,"setSingleLine",lua_fairygui_GTextField_setSingleLine);
	tolua_function(tolua_S,"isUBBEnabled",lua_fairygui_GTextField_isUBBEnabled);
	tolua_function(tolua_S,"getColor",lua_fairygui_GTextField_getColor);
	tolua_function(tolua_S,"setUBBEnabled",lua_fairygui_GTextField_setUBBEnabled);
	tolua_function(tolua_S,"getAutoSize",lua_fairygui_GTextField_getAutoSize);
	tolua_function(tolua_S,"setFontSize",lua_fairygui_GTextField_setFontSize);
	tolua_function(tolua_S,"applyTextFormat",lua_fairygui_GTextField_applyTextFormat);
	tolua_function(tolua_S,"getTextFormat",lua_fairygui_GTextField_getTextFormat);
	tolua_function(tolua_S,"isSingleLine",lua_fairygui_GTextField_isSingleLine);
	tolua_function(tolua_S,"getTextSize",lua_fairygui_GTextField_getTextSize);
	tolua_function(tolua_S,"getOutlineColor",lua_fairygui_GTextField_getOutlineColor);
	tolua_function(tolua_S,"setOutlineColor",lua_fairygui_GTextField_setOutlineColor);
	tolua_function(tolua_S,"getTemplateVars",lua_fairygui_GTextField_getTemplateVars);
	tolua_function(tolua_S,"setTemplateVars",lua_fairygui_GTextField_setTemplateVars);
	tolua_function(tolua_S,"setVar",lua_fairygui_GTextField_setVar);
	tolua_function(tolua_S,"flushVars",lua_fairygui_GTextField_flushVars);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GTextField).name();
	g_luaType[typeName] = "fairygui.GTextField";
	g_typeCast["GTextField"] = "fairygui.GTextField";
	return 1;
}

static int lua_fairygui_GRichTextField_getControl(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRichTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRichTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRichTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRichTextField_getControl'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string name = tolua_tocppstring(tolua_S, 2, "");
		fairygui::HtmlObject *ret = cobj->getControl(name);
		object_to_luaval<fairygui::HtmlObject>(tolua_S, "fairygui.HtmlObject", ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRichTextField:getControl", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRichTextField_getControl'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRichTextField_getControls(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRichTextField* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRichTextField",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRichTextField*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRichTextField_getControls'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::vector<fairygui::HtmlObject*>& ret = cobj->getControls();
		lua_newtable(tolua_S);
		int index = 1;
		for (fairygui::HtmlObject *obj : ret) {
			lua_pushnumber(tolua_S, index);
			object_to_luaval<fairygui::HtmlObject>(tolua_S, "fairygui.HtmlObject", obj);
			lua_rawset(tolua_S, -3);
			++index;
		}
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRichTextField:getControls", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRichTextField_getControls'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRichTextField_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GRichTextField",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GRichTextField* ret = fairygui::GRichTextField::create();
		object_to_luaval<fairygui::GRichTextField>(tolua_S, "fairygui.GRichTextField",(fairygui::GRichTextField*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GRichTextField:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRichTextField_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GRichTextField(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GRichTextField");
	tolua_cclass(tolua_S,"GRichTextField","fairygui.GRichTextField","fairygui.GTextField",nullptr);

	tolua_beginmodule(tolua_S,"GRichTextField");
	tolua_function(tolua_S,"getControl", lua_fairygui_GRichTextField_getControl);
	tolua_function(tolua_S,"getControls", lua_fairygui_GRichTextField_getControls);
	tolua_function(tolua_S,"create", lua_fairygui_GRichTextField_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GRichTextField).name();
	g_luaType[typeName] = "fairygui.GRichTextField";
	g_typeCast["GRichTextField"] = "fairygui.GRichTextField";
	return 1;
}

static int lua_fairygui_GTextInput_setKeyboardType(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextInput_setKeyboardType'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GTextInput:setKeyboardType");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextInput_setKeyboardType'", nullptr);
			return 0;
		}
		cobj->setKeyboardType(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextInput:setKeyboardType",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextInput_setKeyboardType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextInput_setPrompt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextInput_setPrompt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GTextInput:setPrompt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextInput_setPrompt'", nullptr);
			return 0;
		}
		cobj->setPrompt(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextInput:setPrompt",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextInput_setPrompt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextInput_setMaxLength(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextInput_setMaxLength'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GTextInput:setMaxLength");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextInput_setMaxLength'", nullptr);
			return 0;
		}
		cobj->setMaxLength(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextInput:setMaxLength",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextInput_setMaxLength'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextInput_setRestrict(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextInput_setRestrict'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GTextInput:setRestrict");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextInput_setRestrict'", nullptr);
			return 0;
		}
		cobj->setRestrict(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextInput:setRestrict",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextInput_setRestrict'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextInput_setPassword(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTextInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTextInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTextInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTextInput_setPassword'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GTextInput:setPassword");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GTextInput_setPassword'", nullptr);
			return 0;
		}
		cobj->setPassword(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GTextInput:setPassword",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextInput_setPassword'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTextInput_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GTextInput",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GTextInput* ret = fairygui::GTextInput::create();
		object_to_luaval<fairygui::GTextInput>(tolua_S, "fairygui.GTextInput",(fairygui::GTextInput*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GTextInput:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTextInput_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GTextInput(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GTextInput");
	tolua_cclass(tolua_S,"GTextInput","fairygui.GTextInput","fairygui.GTextField",nullptr);

	tolua_beginmodule(tolua_S,"GTextInput");
	tolua_function(tolua_S,"setKeyboardType",lua_fairygui_GTextInput_setKeyboardType);
	tolua_function(tolua_S,"setPrompt",lua_fairygui_GTextInput_setPrompt);
	tolua_function(tolua_S,"setMaxLength",lua_fairygui_GTextInput_setMaxLength);
	tolua_function(tolua_S,"setRestrict",lua_fairygui_GTextInput_setRestrict);
	tolua_function(tolua_S,"setPassword",lua_fairygui_GTextInput_setPassword);
	tolua_function(tolua_S,"create", lua_fairygui_GTextInput_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GTextInput).name();
	g_luaType[typeName] = "fairygui.GTextInput";
	g_typeCast["GTextInput"] = "fairygui.GTextInput";
	return 1;
}
static int  lua_fairygui_GGraph_setPolygonPoints(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.GGraph", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_GGraph_setPolygonPoints'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Vec2* arg1 = nullptr;
		int arg2;
		bool ok = luaval_to_array_of_vec2(tolua_S, 2, &arg1, &arg2, "fairygui.GGraph:setPolygonPoints");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_GGraph_setPolygonPoints'", nullptr);
			CC_SAFE_DELETE_ARRAY(arg1);
			return 0;
		}
		cobj->setPolygonPoints(arg1, arg2);
		CC_SAFE_DELETE_ARRAY(arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:setPolygonPoints", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_GGraph_setPolygonPoints'.", &tolua_err);
	return 0;
#endif
}
static int lua_fairygui_GGraph_getColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_getColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:getColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_getColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_setColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_setColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GGraph:setColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGraph_setColor'", nullptr);
			return 0;
		}
		cobj->setColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:setColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_setColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_drawPolygon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_drawPolygon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 4) {
		int arg0;
		cocos2d::Color4F arg1;
		cocos2d::Color4F arg2;
		cocos2d::Vec2* arg3 = nullptr;
		int arg4;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GGraph:drawPolygon");
		ok &= luaval_to_color4f(tolua_S, 3, &arg1, "fairygui.GGraph:drawPolygon");
		ok &= luaval_to_color4f(tolua_S, 4, &arg2, "fairygui.GGraph:drawPolygon");
		ok &= luaval_to_array_of_vec2(tolua_S, 5, &arg3, &arg4, "fairygui.GGraph:drawPolygon");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGraph_drawPolygon'", nullptr);
			CC_SAFE_DELETE_ARRAY(arg3);
			return 0;
		}
		cobj->drawPolygon(arg0, arg1, arg2, arg3, arg4);
		CC_SAFE_DELETE_ARRAY(arg3);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:drawPolygon", argc, 4);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_drawPolygon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_drawRegularPolygon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_drawRegularPolygon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc >= 4) {
		int arg0;
		cocos2d::Color4F arg1;
		cocos2d::Color4F arg2;
		int arg3;
		float arg4 = 0;
		float* arg5 = nullptr;
		int arg6 = 0;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GGraph:drawRegularPolygon");
		ok &= luaval_to_color4f(tolua_S, 3, &arg1, "fairygui.GGraph:drawRegularPolygon");
		ok &= luaval_to_color4f(tolua_S, 4, &arg2, "fairygui.GGraph:drawRegularPolygon");
		ok &= luaval_to_int32(tolua_S, 5, &arg3, "fairygui.GGraph:drawRegularPolygon");
		if (argc >= 5) {
			ok &= luaval_to_float(tolua_S, 6, &arg4, "fairygui.GGraph:drawRegularPolygon");
		}
		if (argc == 6) {
			ok &= luaval_to_array_of_float(tolua_S, 7, &arg5, &arg6, "fairygui.GGraph:drawRegularPolygon");
		}
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGraph_drawRegularPolygon'", nullptr);
			CC_SAFE_DELETE_ARRAY(arg5);
			return 0;
		}
		cobj->drawRegularPolygon(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
		CC_SAFE_DELETE_ARRAY(arg5);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting 4~6 \n", "fairygui.GGraph:drawRegularPolygon", argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_drawRegularPolygon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_drawEllipse(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_drawEllipse'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 5) {
		double arg0;
		double arg1;
		int arg2;
		cocos2d::Color4F arg3;
		cocos2d::Color4F arg4;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GGraph:drawEllipse");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GGraph:drawEllipse");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GGraph:drawEllipse");
		ok &=luaval_to_color4f(tolua_S, 5, &arg3, "fairygui.GGraph:drawEllipse");
		ok &=luaval_to_color4f(tolua_S, 6, &arg4, "fairygui.GGraph:drawEllipse");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGraph_drawEllipse'", nullptr);
			return 0;
		}
		cobj->drawEllipse(arg0, arg1, arg2, arg3, arg4);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:drawEllipse",argc, 5);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_drawEllipse'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_isEmpty(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_isEmpty'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isEmpty();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:isEmpty",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_isEmpty'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_drawRect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGraph* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGraph*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGraph_drawRect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 5) {
		double arg0;
		double arg1;
		int arg2;
		cocos2d::Color4F arg3;
		cocos2d::Color4F arg4;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GGraph:drawRect");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GGraph:drawRect");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GGraph:drawRect");
		ok &=luaval_to_color4f(tolua_S, 5, &arg3, "fairygui.GGraph:drawRect");
		ok &=luaval_to_color4f(tolua_S, 6, &arg4, "fairygui.GGraph:drawRect");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGraph_drawRect'", nullptr);
			return 0;
		}
		cobj->drawRect(arg0, arg1, arg2, arg3, arg4);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGraph:drawRect",argc, 5);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_drawRect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGraph_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GGraph",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GGraph* ret = fairygui::GGraph::create();
		object_to_luaval<fairygui::GGraph>(tolua_S, "fairygui.GGraph",(fairygui::GGraph*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GGraph:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGraph_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GGraph(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GGraph");
	tolua_cclass(tolua_S,"GGraph","fairygui.GGraph","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GGraph");
	tolua_function(tolua_S,"setPolygonPoints",lua_fairygui_GGraph_setPolygonPoints);
	tolua_function(tolua_S,"getColor",lua_fairygui_GGraph_getColor);
	tolua_function(tolua_S,"setColor",lua_fairygui_GGraph_setColor);
	tolua_function(tolua_S,"drawPolygon",lua_fairygui_GGraph_drawPolygon);
	tolua_function(tolua_S,"drawRegularPolygon",lua_fairygui_GGraph_drawRegularPolygon);
	tolua_function(tolua_S,"drawEllipse",lua_fairygui_GGraph_drawEllipse);
	tolua_function(tolua_S,"isEmpty",lua_fairygui_GGraph_isEmpty);
	tolua_function(tolua_S,"drawRect",lua_fairygui_GGraph_drawRect);
	tolua_function(tolua_S,"create", lua_fairygui_GGraph_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GGraph).name();
	g_luaType[typeName] = "fairygui.GGraph";
	g_typeCast["GGraph"] = "fairygui.GGraph";
	return 1;
}

static int lua_fairygui_GLoader_getURL(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getURL'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getURL();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getURL",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_isPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_isPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isPlaying();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:isPlaying",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_isPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GLoader:setColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setColor'", nullptr);
			return 0;
		}
		cobj->setColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::TextHAlignment arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader:setAlign");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setAlign'", nullptr);
			return 0;
		}
		cobj->setAlign(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setAlign",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader:setPlaying");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setPlaying'", nullptr);
			return 0;
		}
		cobj->setPlaying(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setPlaying",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setURL(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setURL'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GLoader:setURL");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setURL'", nullptr);
			return 0;
		}
		cobj->setURL(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setURL",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader:setFrame");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setFrame'", nullptr);
			return 0;
		}
		cobj->setFrame(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setFrame",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getFill(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getFill'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getFill();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getFill",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getFill'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setAutoSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setAutoSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader:setAutoSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setAutoSize'", nullptr);
			return 0;
		}
		cobj->setAutoSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setAutoSize",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setAutoSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getAutoSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getAutoSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->getAutoSize();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getAutoSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getAutoSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getContentSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getContentSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Size& ret = cobj->getContentSize();
		size_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getContentSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getContentSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setVerticalAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setVerticalAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::TextVAlignment arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader:setVerticalAlign");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setVerticalAlign'", nullptr);
			return 0;
		}
		cobj->setVerticalAlign(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setVerticalAlign",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setVerticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getAlign();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getAlign",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setFill(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setFill'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::LoaderFillType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader:setFill");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setFill'", nullptr);
			return 0;
		}
		cobj->setFill(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setFill",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setFill'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getVerticalAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getVerticalAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getVerticalAlign();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getVerticalAlign",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getVerticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getFrame();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getFrame",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_isShrinkOnly(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_isShrinkOnly'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		tolua_pushboolean(tolua_S, cobj->isShrinkOnly());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:isShrinkOnly",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_isShrinkOnly'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setShrinkOnly(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setShrinkOnly'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader:setShrinkOnly");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setShrinkOnly'", nullptr);
			return 0;
		}
		cobj->setShrinkOnly(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setShrinkOnly",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setShrinkOnly'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getFillMethod(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getFillMethod'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, (lua_Integer)cobj->getFillMethod());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getFillMethod",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getFillMethod'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setFillMethod(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setFillMethod'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,&arg0, "fairygui.GLoader:setFillMethod");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setFillMethod'", nullptr);
			return 0;
		}
		cobj->setFillMethod((fairygui::FillMethod)arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setFillMethod",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setFillMethod'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getFillOrigin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getFillOrigin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, (lua_Integer)cobj->getFillOrigin());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getFillOrigin",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getFillOrigin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setFillOrigin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setFillOrigin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,&arg0, "fairygui.GLoader:setFillMethod");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setFillOrigin'", nullptr);
			return 0;
		}
		cobj->setFillOrigin((fairygui::FillOrigin)arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setFillOrigin",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setFillOrigin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_isFillClockwise(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_isFillClockwise'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushboolean(tolua_S, cobj->isFillClockwise());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:isFillClockwise",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_isFillClockwise'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setFillClockwise(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setFillClockwise'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader:setFillClockwise");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setFillClockwise'", nullptr);
			return 0;
		}
		cobj->setFillClockwise(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setFillClockwise",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setFillClockwise'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getFillAmount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getFillAmount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushnumber(tolua_S, cobj->getFillAmount());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getFillAmount",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getFillAmount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_setFillAmount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_setFillAmount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		float arg0;
		ok &= luaval_to_float(tolua_S, 2,&arg0, "fairygui.GLoader:setFillAmount");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader_setFillAmount'", nullptr);
			return 0;
		}
		cobj->setFillAmount(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:setFillAmount",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_setFillAmount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_getComponent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader_getComponent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getComponent();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader:getComponent",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_getComponent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GLoader",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GLoader* ret = fairygui::GLoader::create();
		object_to_luaval<fairygui::GLoader>(tolua_S, "fairygui.GLoader",(fairygui::GLoader*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GLoader:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GLoader(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GLoader");
	tolua_cclass(tolua_S,"GLoader","fairygui.GLoader","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GLoader");
	tolua_function(tolua_S,"getURL",lua_fairygui_GLoader_getURL);
	tolua_function(tolua_S,"isPlaying",lua_fairygui_GLoader_isPlaying);
	tolua_function(tolua_S,"setColor",lua_fairygui_GLoader_setColor);
	tolua_function(tolua_S,"setAlign",lua_fairygui_GLoader_setAlign);
	tolua_function(tolua_S,"setPlaying",lua_fairygui_GLoader_setPlaying);
	tolua_function(tolua_S,"getColor",lua_fairygui_GLoader_getColor);
	tolua_function(tolua_S,"setURL",lua_fairygui_GLoader_setURL);
	tolua_function(tolua_S,"setFrame",lua_fairygui_GLoader_setFrame);
	tolua_function(tolua_S,"getFill",lua_fairygui_GLoader_getFill);
	tolua_function(tolua_S,"setAutoSize",lua_fairygui_GLoader_setAutoSize);
	tolua_function(tolua_S,"getAutoSize",lua_fairygui_GLoader_getAutoSize);
	tolua_function(tolua_S,"getContentSize",lua_fairygui_GLoader_getContentSize);
	tolua_function(tolua_S,"setVerticalAlign",lua_fairygui_GLoader_setVerticalAlign);
	tolua_function(tolua_S,"getAlign",lua_fairygui_GLoader_getAlign);
	tolua_function(tolua_S,"setFill",lua_fairygui_GLoader_setFill);
	tolua_function(tolua_S,"getVerticalAlign",lua_fairygui_GLoader_getVerticalAlign);
	tolua_function(tolua_S,"getFrame",lua_fairygui_GLoader_getFrame);
	tolua_function(tolua_S,"isShrinkOnly",lua_fairygui_GLoader_isShrinkOnly);
	tolua_function(tolua_S,"setShrinkOnly",lua_fairygui_GLoader_setShrinkOnly);
	tolua_function(tolua_S,"getFillMethod",lua_fairygui_GLoader_getFillMethod);
	tolua_function(tolua_S,"setFillMethod",lua_fairygui_GLoader_setFillMethod);
	tolua_function(tolua_S,"getFillOrigin",lua_fairygui_GLoader_getFillOrigin);
	tolua_function(tolua_S,"setFillOrigin",lua_fairygui_GLoader_setFillOrigin);
	tolua_function(tolua_S,"isFillClockwise",lua_fairygui_GLoader_isFillClockwise);
	tolua_function(tolua_S,"setFillClockwise",lua_fairygui_GLoader_setFillClockwise);
	tolua_function(tolua_S,"getFillAmount",lua_fairygui_GLoader_getFillAmount);
	tolua_function(tolua_S,"setFillAmount",lua_fairygui_GLoader_setFillAmount);
	tolua_function(tolua_S,"getComponent",lua_fairygui_GLoader_getComponent);
	tolua_function(tolua_S,"create", lua_fairygui_GLoader_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GLoader).name();
	g_luaType[typeName] = "fairygui.GLoader";
	g_typeCast["GLoader"] = "fairygui.GLoader";
	return 1;
}

static int lua_fairygui_GLoader3D_getURL(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getURL'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getURL();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getURL",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setURL(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setURL'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GLoader3D:setURL");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setURL'", nullptr);
			return 0;
		}
		cobj->setURL(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setURL",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setURL'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getAlign();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getAlign",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::TextHAlignment arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader3D:setAlign");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setAlign'", nullptr);
			return 0;
		}
		cobj->setAlign(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setAlign",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getVerticalAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getVerticalAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getVerticalAlign();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getVerticalAlign",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getVerticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setVerticalAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setVerticalAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::TextVAlignment arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader3D:setVerticalAlign");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setVerticalAlign'", nullptr);
			return 0;
		}
		cobj->setVerticalAlign(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setVerticalAlign",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setVerticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getAutoSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getAutoSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->getAutoSize();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getAutoSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getAutoSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setAutoSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setAutoSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader3D:setAutoSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setAutoSize'", nullptr);
			return 0;
		}
		cobj->setAutoSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setAutoSize",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setAutoSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getFill(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getFill'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getFill();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getFill",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getFill'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setFill(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setFill'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::LoaderFillType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader3D:setFill");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setFill'", nullptr);
			return 0;
		}
		cobj->setFill(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setFill",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setFill'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_isShrinkOnly(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_isShrinkOnly'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		tolua_pushboolean(tolua_S, cobj->isShrinkOnly());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:isShrinkOnly",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_isShrinkOnly'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setShrinkOnly(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setShrinkOnly'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader3D:setShrinkOnly");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setShrinkOnly'", nullptr);
			return 0;
		}
		cobj->setShrinkOnly(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setShrinkOnly",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setShrinkOnly'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getContent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getContent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Node *ret = cobj->getContent();
		object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node",(cocos2d::Node*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getContent",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getContent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setContent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setContent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Node* arg0;
		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "fairygui.GLoader3D:setContent");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setContent'", nullptr);
			return 0;
		}
		cobj->setContent(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setContent",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setContent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GLoader3D:setColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setColor'", nullptr);
			return 0;
		}
		cobj->setColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_isPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_isPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isPlaying();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:isPlaying",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_isPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader3D:setPlaying");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setPlaying'", nullptr);
			return 0;
		}
		cobj->setPlaying(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setPlaying",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getFrame();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getFrame",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GLoader3D:setFrame");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setFrame'", nullptr);
			return 0;
		}
		cobj->setFrame(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setFrame",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getAnimationName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getAnimationName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getAnimationName();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getAnimationName",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getAnimationName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setAnimationName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setAnimationName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GLoader3D:setAnimationName");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setAnimationName'", nullptr);
			return 0;
		}
		cobj->setAnimationName(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setAnimationName",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setAnimationName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getSkinName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getSkinName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getSkinName();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getSkinName",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getSkinName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setSkinName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setSkinName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GLoader3D:setSkinName");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setSkinName'", nullptr);
			return 0;
		}
		cobj->setSkinName(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setSkinName",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setSkinName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_getLoop(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_getLoop'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->getLoop();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:getLoop",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_getLoop'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_setLoop(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLoader3D* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLoader3D*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLoader3D_setLoop'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GLoader3D:setLoop");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLoader3D_setLoop'", nullptr);
			return 0;
		}
		cobj->setLoop(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLoader3D:setLoop",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_setLoop'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLoader3D_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GLoader3D",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GLoader3D* ret = fairygui::GLoader3D::create();
		object_to_luaval<fairygui::GLoader3D>(tolua_S, "fairygui.GLoader3D",(fairygui::GLoader3D*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GLoader3D:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLoader3D_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GLoader3D(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GLoader3D");
	tolua_cclass(tolua_S,"GLoader3D","fairygui.GLoader3D","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GLoader3D");
	tolua_function(tolua_S,"getURL",lua_fairygui_GLoader3D_getURL);
	tolua_function(tolua_S,"setURL",lua_fairygui_GLoader3D_setURL);
	tolua_function(tolua_S,"getAlign",lua_fairygui_GLoader3D_getAlign);
	tolua_function(tolua_S,"setAlign",lua_fairygui_GLoader3D_setAlign);
	tolua_function(tolua_S,"getVerticalAlign",lua_fairygui_GLoader3D_getVerticalAlign);
	tolua_function(tolua_S,"setVerticalAlign",lua_fairygui_GLoader3D_setVerticalAlign);
	tolua_function(tolua_S,"getAutoSize",lua_fairygui_GLoader3D_getAutoSize);
	tolua_function(tolua_S,"setAutoSize",lua_fairygui_GLoader3D_setAutoSize);
	tolua_function(tolua_S,"getFill",lua_fairygui_GLoader3D_getFill);
	tolua_function(tolua_S,"setFill",lua_fairygui_GLoader3D_setFill);
	tolua_function(tolua_S,"isShrinkOnly",lua_fairygui_GLoader3D_isShrinkOnly);
	tolua_function(tolua_S,"setShrinkOnly",lua_fairygui_GLoader3D_setShrinkOnly);
	tolua_function(tolua_S,"getContent",lua_fairygui_GLoader3D_getContent);
	tolua_function(tolua_S,"setContent",lua_fairygui_GLoader3D_setContent);
	tolua_function(tolua_S,"getColor",lua_fairygui_GLoader3D_getColor);
	tolua_function(tolua_S,"setColor",lua_fairygui_GLoader3D_setColor);
	tolua_function(tolua_S,"isPlaying",lua_fairygui_GLoader3D_isPlaying);
	tolua_function(tolua_S,"setPlaying",lua_fairygui_GLoader3D_setPlaying);
	tolua_function(tolua_S,"getFrame",lua_fairygui_GLoader3D_getFrame);
	tolua_function(tolua_S,"setFrame",lua_fairygui_GLoader3D_setFrame);
	tolua_function(tolua_S,"getAnimationName",lua_fairygui_GLoader3D_getAnimationName);
	tolua_function(tolua_S,"setAnimationName",lua_fairygui_GLoader3D_setAnimationName);
	tolua_function(tolua_S,"getSkinName",lua_fairygui_GLoader3D_getSkinName);
	tolua_function(tolua_S,"setSkinName",lua_fairygui_GLoader3D_setSkinName);
	tolua_function(tolua_S,"getLoop",lua_fairygui_GLoader3D_getLoop);
	tolua_function(tolua_S,"setLoop",lua_fairygui_GLoader3D_setLoop);
	tolua_function(tolua_S,"create", lua_fairygui_GLoader3D_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GLoader3D).name();
	g_luaType[typeName] = "fairygui.GLoader3D";
	g_typeCast["GLoader3D"] = "fairygui.GLoader3D";
	return 1;
}

static int lua_fairygui_GGroup_getColumnGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_getColumnGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getColumnGap();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:getColumnGap",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_getColumnGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_resizeChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_resizeChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GGroup:resizeChildren");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GGroup:resizeChildren");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_resizeChildren'", nullptr);
			return 0;
		}
		cobj->resizeChildren(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:resizeChildren",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_resizeChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setLayout(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setLayout'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GroupLayoutType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GGroup:setLayout");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setLayout'", nullptr);
			return 0;
		}
		cobj->setLayout(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setLayout",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setLayout'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setColumnGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setColumnGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GGroup:setColumnGap");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setColumnGap'", nullptr);
			return 0;
		}
		cobj->setColumnGap(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setColumnGap",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setColumnGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setBoundsChangedFlag(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setBoundsChangedFlag'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->setBoundsChangedFlag();
		return 0;
	}
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GGroup:setBoundsChangedFlag");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setBoundsChangedFlag'", nullptr);
			return 0;
		}
		cobj->setBoundsChangedFlag(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setBoundsChangedFlag",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setBoundsChangedFlag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_moveChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_moveChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GGroup:moveChildren");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GGroup:moveChildren");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_moveChildren'", nullptr);
			return 0;
		}
		cobj->moveChildren(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:moveChildren",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_moveChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_getLineGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_getLineGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getLineGap();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:getLineGap",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_getLineGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setLineGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setLineGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GGroup:setLineGap");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setLineGap'", nullptr);
			return 0;
		}
		cobj->setLineGap(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setLineGap",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setLineGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_getLayout(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_getLayout'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getLayout();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:getLayout",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_getLayout'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_isExcludeInvisibles(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_isExcludeInvisibles'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushboolean(tolua_S,(int)cobj->isExcludeInvisibles());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:isExcludeInvisibles",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_isExcludeInvisibles'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setExcludeInvisibles(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setExcludeInvisibles'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.GGroup:setExcludeInvisibles");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setExcludeInvisibles'", nullptr);
			return 0;
		}
		cobj->setExcludeInvisibles(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setExcludeInvisibles",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setExcludeInvisibles'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_isAutoSizeDisabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_isAutoSizeDisabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushboolean(tolua_S,(int)cobj->isAutoSizeDisabled());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:isAutoSizeDisabled",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_isAutoSizeDisabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setAutoSizeDisabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setAutoSizeDisabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.GGroup:setAutoSizeDisabled");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setAutoSizeDisabled'", nullptr);
			return 0;
		}
		cobj->setAutoSizeDisabled(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setAutoSizeDisabled",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setAutoSizeDisabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_getMainGridIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_getMainGridIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, cobj->getMainGridIndex());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:getMainGridIndex",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_getMainGridIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setMainGridIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setMainGridIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GGroup:setMainGridIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setMainGridIndex'", nullptr);
			return 0;
		}
		cobj->setMainGridIndex(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setMainGridIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setMainGridIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_getMainGridMinSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_getMainGridMinSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, cobj->getMainGridMinSize());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:getMainGridMinSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_getMainGridMinSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_setMainGridMinSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GGroup* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GGroup*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GGroup_setMainGridMinSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GGroup:setMainGridMinSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GGroup_setMainGridMinSize'", nullptr);
			return 0;
		}
		cobj->setMainGridMinSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GGroup:setMainGridMinSize",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_setMainGridMinSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GGroup_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GGroup",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GGroup* ret = fairygui::GGroup::create();
		object_to_luaval<fairygui::GGroup>(tolua_S, "fairygui.GGroup",(fairygui::GGroup*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GGroup:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GGroup_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GGroup(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GGroup");
	tolua_cclass(tolua_S,"GGroup","fairygui.GGroup","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GGroup");
	tolua_function(tolua_S,"getColumnGap",lua_fairygui_GGroup_getColumnGap);
	tolua_function(tolua_S,"resizeChildren",lua_fairygui_GGroup_resizeChildren);
	tolua_function(tolua_S,"setLayout",lua_fairygui_GGroup_setLayout);
	tolua_function(tolua_S,"setColumnGap",lua_fairygui_GGroup_setColumnGap);
	tolua_function(tolua_S,"setBoundsChangedFlag",lua_fairygui_GGroup_setBoundsChangedFlag);
	tolua_function(tolua_S,"moveChildren",lua_fairygui_GGroup_moveChildren);
	tolua_function(tolua_S,"getLineGap",lua_fairygui_GGroup_getLineGap);
	tolua_function(tolua_S,"setLineGap",lua_fairygui_GGroup_setLineGap);
	tolua_function(tolua_S,"getLayout",lua_fairygui_GGroup_getLayout);
	tolua_function(tolua_S,"isExcludeInvisibles",lua_fairygui_GGroup_isExcludeInvisibles);
	tolua_function(tolua_S,"setExcludeInvisibles",lua_fairygui_GGroup_setExcludeInvisibles);
	tolua_function(tolua_S,"isAutoSizeDisabled",lua_fairygui_GGroup_isAutoSizeDisabled);
	tolua_function(tolua_S,"setAutoSizeDisabled",lua_fairygui_GGroup_setAutoSizeDisabled);
	tolua_function(tolua_S,"getMainGridIndex",lua_fairygui_GGroup_getMainGridIndex);
	tolua_function(tolua_S,"setMainGridIndex",lua_fairygui_GGroup_setMainGridIndex);
	tolua_function(tolua_S,"getMainGridMinSize",lua_fairygui_GGroup_getMainGridMinSize);
	tolua_function(tolua_S,"setMainGridMinSize",lua_fairygui_GGroup_setMainGridMinSize);
	tolua_function(tolua_S,"create", lua_fairygui_GGroup_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GGroup).name();
	g_luaType[typeName] = "fairygui.GGroup";
	g_typeCast["GGroup"] = "fairygui.GGroup";
	return 1;
}

static int lua_fairygui_Transition_setValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_setValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		cocos2d::ValueVector arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.Transition:setValue");
		ok &= luaval_to_ccvaluevector(tolua_S, 3, &arg1, "fairygui.Transition:setValue");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_setValue'", nullptr);
			return 0;
		}
		cobj->setValue(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:setValue",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_setValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_changePlayTimes(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_changePlayTimes'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Transition:changePlayTimes");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_changePlayTimes'", nullptr);
			return 0;
		}
		cobj->changePlayTimes(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:changePlayTimes",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_changePlayTimes'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_onOwnerRemovedFromStage(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_onOwnerRemovedFromStage'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->onOwnerRemovedFromStage();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:onOwnerRemovedFromStage",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_onOwnerRemovedFromStage'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_setTarget(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_setTarget'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		fairygui::GObject* arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.Transition:setTarget");
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 3, "fairygui.GObject",&arg1, "fairygui.Transition:setTarget");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_setTarget'", nullptr);
			return 0;
		}
		cobj->setTarget(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:setTarget",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_setTarget'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_clearHooks(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_clearHooks'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->clearHooks();// not remove removeObjectAllHandlers
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:clearHooks",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_clearHooks'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_stop(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_stop'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->stop();// not remove removeObjectAllHandlers
		return 0;
	}
	if (argc == 2) {
		bool arg0;
		bool arg1;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.Transition:stop");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.Transition:stop");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_stop'", nullptr);
			return 0;
		}
		cobj->stop(arg0, arg1);// not remove removeObjectAllHandlers
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.Transition:stop",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_stop'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_getOwner(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_getOwner'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getOwner();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:getOwner",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_getOwner'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_setHook(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_setHook'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.Transition:setHook");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 3, 0));
		if(!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_setHook'", nullptr);
			return 0;
		}
		cobj->setHook(arg0, [=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:setHook",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_setHook'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_setDuration(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_setDuration'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		double arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.Transition:setDuration");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:setDuration");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_setDuration'", nullptr);
			return 0;
		}
		cobj->setDuration(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:setDuration",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_setDuration'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_isPlaying(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_isPlaying'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isPlaying();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:isPlaying",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_isPlaying'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_setAutoPlay(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_setAutoPlay'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 3) {
		bool arg0;
		int arg1;
		double arg2;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.Transition:setAutoPlay");
		ok &= luaval_to_int32(tolua_S, 3,&arg1, "fairygui.Transition:setAutoPlay");
		ok &= luaval_to_number(tolua_S, 4,&arg2, "fairygui.Transition:setAutoPlay");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_setAutoPlay'", nullptr);
			return 0;
		}
		cobj->setAutoPlay(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:setAutoPlay",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_setAutoPlay'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_updateFromRelations(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_updateFromRelations'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 3) {
		std::string arg0;
		double arg1;
		double arg2;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.Transition:updateFromRelations");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:updateFromRelations");
		ok &= luaval_to_number(tolua_S, 4,&arg2, "fairygui.Transition:updateFromRelations");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_updateFromRelations'", nullptr);
			return 0;
		}
		cobj->updateFromRelations(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:updateFromRelations",argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_updateFromRelations'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_playReverse(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_playReverse'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->playReverse();
		return 0;
	}
	if (argc == 1) {
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 2, 0));
		if (0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_playReverse'", nullptr);
			return 0;
		}
		cobj->playReverse([=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		double arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Transition:playReverse");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:playReverse");
		cobj->playReverse(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		int arg0;
		double arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Transition:playReverse");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:playReverse");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 4, 0));
		if (!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_playReverse'", nullptr);
			return 0;
		}
		cobj->playReverse(arg0, arg1, [=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.Transition:playReverse",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_playReverse'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_play(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_play'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->play();
		return 0;
	}
	if (argc == 1) {
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 2, 0));
		if (0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_play'", nullptr);
			return 0;
		}
		cobj->play([=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		double arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:play");
		cobj->play(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		int arg0;
		double arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:play");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 4, 0));
		if (!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_play'", nullptr);
			return 0;
		}
		cobj->play(arg0, arg1, [=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	if (argc == 4) {
		int arg0;
		double arg1;
		double arg2;
		double arg3;
		ok &= luaval_to_int32(tolua_S, 2,&arg0, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 4,&arg2, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 5,&arg3, "fairygui.Transition:play");
		cobj->play(arg0, arg1, arg2, arg3);
		return 0;
	}
	if (argc == 5) {
		int arg0;
		double arg1;
		double arg2;
		double arg3;
		ok &= luaval_to_int32(tolua_S, 2,&arg0, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 4,&arg2, "fairygui.Transition:play");
		ok &= luaval_to_number(tolua_S, 5,&arg3, "fairygui.Transition:play");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 6, 0));
		if (!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_play'", nullptr);
			return 0;
		}
		cobj->play(arg0, arg1, arg2, arg3, [=]() {
				LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 0);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.Transition:play",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_play'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_setTimeScale(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_setTimeScale'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.Transition:setTimeScale");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Transition_setTimeScale'", nullptr);
			return 0;
		}
		cobj->setTimeScale(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:setTimeScale",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_setTimeScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Transition_getTimeScale(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Transition* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Transition",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Transition*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Transition_getTimeScale'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushnumber(tolua_S, lua_Number(cobj->getTimeScale()));
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Transition:getTimeScale",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Transition_getTimeScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_Transition(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.Transition");
	tolua_cclass(tolua_S,"Transition","fairygui.Transition","cc.Ref",nullptr);

	tolua_beginmodule(tolua_S,"Transition");
	tolua_function(tolua_S,"setValue",lua_fairygui_Transition_setValue);
	tolua_function(tolua_S,"changePlayTimes",lua_fairygui_Transition_changePlayTimes);
	tolua_function(tolua_S,"onOwnerRemovedFromStage",lua_fairygui_Transition_onOwnerRemovedFromStage);
	tolua_function(tolua_S,"setTarget",lua_fairygui_Transition_setTarget);
	tolua_function(tolua_S,"clearHooks",lua_fairygui_Transition_clearHooks);
	tolua_function(tolua_S,"stop",lua_fairygui_Transition_stop);
	tolua_function(tolua_S,"getOwner",lua_fairygui_Transition_getOwner);
	tolua_function(tolua_S,"setHook",lua_fairygui_Transition_setHook);
	tolua_function(tolua_S,"setDuration",lua_fairygui_Transition_setDuration);
	tolua_function(tolua_S,"isPlaying",lua_fairygui_Transition_isPlaying);
	tolua_function(tolua_S,"setAutoPlay",lua_fairygui_Transition_setAutoPlay);
	tolua_function(tolua_S,"updateFromRelations",lua_fairygui_Transition_updateFromRelations);
	tolua_function(tolua_S,"playReverse",lua_fairygui_Transition_playReverse);
	tolua_function(tolua_S,"play",lua_fairygui_Transition_play);
	tolua_function(tolua_S,"setTimeScale",lua_fairygui_Transition_setTimeScale);
	tolua_function(tolua_S,"getTimeScale",lua_fairygui_Transition_getTimeScale);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::Transition).name();
	g_luaType[typeName] = "fairygui.Transition";
	g_typeCast["Transition"] = "fairygui.Transition";
	return 1;
}

static int lua_fairygui_ScrollPane_setBouncebackEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setBouncebackEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:setBouncebackEffect");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setBouncebackEffect'", nullptr);
			return 0;
		}
		cobj->setBouncebackEffect(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setBouncebackEffect",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setBouncebackEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getHzScrollBar(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getHzScrollBar'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GScrollBar* ret = cobj->getHzScrollBar();
		object_to_luaval<fairygui::GScrollBar>(tolua_S, "fairygui.GScrollBar",(fairygui::GScrollBar*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getHzScrollBar",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getHzScrollBar'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollToView(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollToView'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Rect arg0;
		ok = luaval_to_rect(tolua_S, 2, &arg0, "fairygui.ScrollPane:scrollToView");
		if (ok) {
			cobj->scrollToView(arg0);
			return 0;
		}

		fairygui::GObject* arg00;
		ok = luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg00, "fairygui.ScrollPane:scrollToView");
		if (ok) {
			cobj->scrollToView(arg00);
			return 0;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollToView'", nullptr);
		return 0;
	}
	if (argc == 2) {
		cocos2d::Rect arg0;
		ok = luaval_to_rect(tolua_S, 2, &arg0, "fairygui.ScrollPane:scrollToView");
		bool arg1;
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:scrollToView");
		if (ok) {
			cobj->scrollToView(arg0, arg1);
			return 0;
		}

		fairygui::GObject* arg00;
		ok = luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg00, "fairygui.ScrollPane:scrollToView");
		bool arg11;
		ok &= luaval_to_boolean(tolua_S, 3,&arg11, "fairygui.ScrollPane:scrollToView");
		if (ok) {
			cobj->scrollToView(arg00, arg11);
			return 0;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollToView'", nullptr);
		return 0;
	}
	if (argc == 3) {
		cocos2d::Rect arg0;
		ok = luaval_to_rect(tolua_S, 2, &arg0, "fairygui.ScrollPane:scrollToView");
		bool arg1;
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:scrollToView");
		bool arg2;
		ok &= luaval_to_boolean(tolua_S, 4,&arg2, "fairygui.ScrollPane:scrollToView");
		if (ok) {
			cobj->scrollToView(arg0, arg1, arg2);
			return 0;
		}

		fairygui::GObject* arg00;
		ok = luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg00, "fairygui.ScrollPane:scrollToView");
		bool arg11;
		ok &= luaval_to_boolean(tolua_S, 3,&arg11, "fairygui.ScrollPane:scrollToView");
		bool arg22;
		ok &= luaval_to_boolean(tolua_S, 4,&arg22, "fairygui.ScrollPane:scrollToView");
		if (ok) {
			cobj->scrollToView(arg00, arg11, arg22);
			return 0;
		}
		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollToView'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.ScrollPane:scrollToView",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollToView'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getDecelerationRate(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getDecelerationRate'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getDecelerationRate();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getDecelerationRate",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getDecelerationRate'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPercX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPercX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getPercX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPercX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPercX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPosY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPosY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getPosY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPosY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPosY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getFooter(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getFooter'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getFooter();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getFooter",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getFooter'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPosX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPosX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPosX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPosX'", nullptr);
			return 0;
		}
		cobj->setPosX(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPosX");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:setPosX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPosX'", nullptr);
			return 0;
		}
		cobj->setPosX(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPosX",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPosX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPosX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPosX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getPosX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPosX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPosX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPageY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPageY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.ScrollPane:setPageY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPageY'", nullptr);
			return 0;
		}
		cobj->setPageY(arg0);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		bool arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.ScrollPane:setPageY");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:setPageY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPageY'", nullptr);
			return 0;
		}
		cobj->setPageY(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPageY",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPageY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollTop(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollTop'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->scrollTop();
		return 0;
	}
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollTop");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollTop'", nullptr);
			return 0;
		}
		cobj->scrollTop(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:scrollTop",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollTop'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isSnapToItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isSnapToItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isSnapToItem();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isSnapToItem",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isSnapToItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isTouchEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isTouchEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isTouchEffect();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isTouchEffect",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isTouchEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setScrollStep(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setScrollStep'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setScrollStep");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setScrollStep'", nullptr);
			return 0;
		}
		cobj->setScrollStep(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setScrollStep",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setScrollStep'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollBottom(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollBottom'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->scrollBottom();
		return 0;
	}
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollBottom");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollBottom'", nullptr);
			return 0;
		}
		cobj->scrollBottom(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:scrollBottom",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollBottom'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isChildInView(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isChildInView'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.ScrollPane:isChildInView");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_isChildInView'", nullptr);
			return 0;
		}
		bool ret = cobj->isChildInView(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isChildInView",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isChildInView'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isPageMode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isPageMode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isPageMode();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isPageMode",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isPageMode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setDecelerationRate(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setDecelerationRate'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setDecelerationRate");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setDecelerationRate'", nullptr);
			return 0;
		}
		cobj->setDecelerationRate(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setDecelerationRate",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setDecelerationRate'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getScrollingPosY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getScrollingPosY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getScrollingPosY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getScrollingPosY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getScrollingPosY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getScrollingPosX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getScrollingPosX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getScrollingPosX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getScrollingPosX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getScrollingPosX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getContentSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getContentSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Size& ret = cobj->getContentSize();
		size_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getContentSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getContentSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isRightMost(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isRightMost'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isRightMost();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isRightMost",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isRightMost'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPageController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPageController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.ScrollPane:setPageController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPageController'", nullptr);
			return 0;
		}
		cobj->setPageController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPageController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPageController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_lockHeader(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_lockHeader'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.ScrollPane:lockHeader");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_lockHeader'", nullptr);
			return 0;
		}
		cobj->lockHeader(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:lockHeader",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_lockHeader'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_lockFooter(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_lockFooter'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.ScrollPane:lockFooter");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_lockFooter'", nullptr);
			return 0;
		}
		cobj->lockFooter(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:lockFooter",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_lockFooter'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollDown(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollDown'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->scrollDown();
		return 0;
	}
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollDown");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollDown'", nullptr);
			return 0;
		}
		cobj->scrollDown(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollDown");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:scrollDown");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollDown'", nullptr);
			return 0;
		}
		cobj->scrollDown(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:scrollDown",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollDown'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setSnapToItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setSnapToItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:setSnapToItem");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setSnapToItem'", nullptr);
			return 0;
		}
		cobj->setSnapToItem(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setSnapToItem",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setSnapToItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getScrollStep(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getScrollStep'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getScrollStep();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getScrollStep",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getScrollStep'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPageX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPageX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getPageX();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPageX",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPageX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollLeft(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollLeft'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->scrollLeft();
		return 0;
	}
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollLeft");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollLeft'", nullptr);
			return 0;
		}
		cobj->scrollLeft(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollLeft");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:scrollLeft");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollLeft'", nullptr);
			return 0;
		}
		cobj->scrollLeft(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:scrollLeft",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollLeft'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getViewSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getViewSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Size& ret = cobj->getViewSize();
		size_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getViewSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getViewSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setInertiaDisabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setInertiaDisabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:setInertiaDisabled");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setInertiaDisabled'", nullptr);
			return 0;
		}
		cobj->setInertiaDisabled(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setInertiaDisabled",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setInertiaDisabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPageController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPageController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GController* ret = cobj->getPageController();
		object_to_luaval<fairygui::GController>(tolua_S, "fairygui.GController",(fairygui::GController*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPageController",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPageController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPageX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPageX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.ScrollPane:setPageX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPageX'", nullptr);
			return 0;
		}
		cobj->setPageX(arg0);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		bool arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.ScrollPane:setPageX");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:setPageX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPageX'", nullptr);
			return 0;
		}
		cobj->setPageX(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPageX",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPageX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollUp(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollUp'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->scrollUp();
		return 0;
	}
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollUp");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollUp'", nullptr);
			return 0;
		}
		cobj->scrollUp(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollUp");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:scrollUp");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollUp'", nullptr);
			return 0;
		}
		cobj->scrollUp(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:scrollUp",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollUp'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isMouseWheelEnabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isMouseWheelEnabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isMouseWheelEnabled();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isMouseWheelEnabled",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isMouseWheelEnabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPageMode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPageMode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPageMode");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPageMode'", nullptr);
			return 0;
		}
		cobj->setPageMode(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPageMode",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPageMode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setTouchEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setTouchEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:setTouchEffect");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setTouchEffect'", nullptr);
			return 0;
		}
		cobj->setTouchEffect(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setTouchEffect",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setTouchEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setMouseWheelEnabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setMouseWheelEnabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.ScrollPane:setMouseWheelEnabled");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setMouseWheelEnabled'", nullptr);
			return 0;
		}
		cobj->setMouseWheelEnabled(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setMouseWheelEnabled",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setMouseWheelEnabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isInertiaDisabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isInertiaDisabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isInertiaDisabled();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isInertiaDisabled",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isInertiaDisabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPercX(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPercX'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPercX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPercX'", nullptr);
			return 0;
		}
		cobj->setPercX(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPercX");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:setPercX");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPercX'", nullptr);
			return 0;
		}
		cobj->setPercX(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPercX",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPercX'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPercY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPercY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPercY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPercY'", nullptr);
			return 0;
		}
		cobj->setPercY(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPercY");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:setPercY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPercY'", nullptr);
			return 0;
		}
		cobj->setPercY(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPercY",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPercY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_cancelDragging(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_cancelDragging'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->cancelDragging();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:cancelDragging",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_cancelDragging'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPercY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPercY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getPercY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPercY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPercY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_scrollRight(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_scrollRight'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->scrollRight();
		return 0;
	}
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollRight");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollRight'", nullptr);
			return 0;
		}
		cobj->scrollRight(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:scrollRight");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:scrollRight");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_scrollRight'", nullptr);
			return 0;
		}
		cobj->scrollRight(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:scrollRight",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_scrollRight'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getHeader(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getHeader'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getHeader();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getHeader",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getHeader'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getVtScrollBar(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getVtScrollBar'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GScrollBar* ret = cobj->getVtScrollBar();
		object_to_luaval<fairygui::GScrollBar>(tolua_S, "fairygui.GScrollBar",(fairygui::GScrollBar*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getVtScrollBar",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getVtScrollBar'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getOwner(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getOwner'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getOwner();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getOwner",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getOwner'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isBottomMost(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isBottomMost'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isBottomMost();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isBottomMost",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isBottomMost'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getPageY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_getPageY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getPageY();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:getPageY",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getPageY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_isBouncebackEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_isBouncebackEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isBouncebackEffect();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:isBouncebackEffect",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_isBouncebackEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_setPosY(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::ScrollPane* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::ScrollPane*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_ScrollPane_setPosY'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPosY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPosY'", nullptr);
			return 0;
		}
		cobj->setPosY(arg0);
		return 0;
	}
	if (argc == 2) {
		double arg0;
		bool arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.ScrollPane:setPosY");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.ScrollPane:setPosY");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_ScrollPane_setPosY'", nullptr);
			return 0;
		}
		cobj->setPosY(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.ScrollPane:setPosY",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_setPosY'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_ScrollPane_getDraggingPane(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.ScrollPane",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::ScrollPane* ret = fairygui::ScrollPane::getDraggingPane();
		object_to_luaval<fairygui::ScrollPane>(tolua_S, "fairygui.ScrollPane",(fairygui::ScrollPane*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.ScrollPane:getDraggingPane",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_ScrollPane_getDraggingPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_ScrollPane(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.ScrollPane");
	tolua_cclass(tolua_S,"ScrollPane","fairygui.ScrollPane","cc.Ref",nullptr);

	tolua_beginmodule(tolua_S,"ScrollPane");
	tolua_function(tolua_S,"setBouncebackEffect",lua_fairygui_ScrollPane_setBouncebackEffect);
	tolua_function(tolua_S,"getHzScrollBar",lua_fairygui_ScrollPane_getHzScrollBar);
	tolua_function(tolua_S,"scrollToView",lua_fairygui_ScrollPane_scrollToView);
	tolua_function(tolua_S,"getDecelerationRate",lua_fairygui_ScrollPane_getDecelerationRate);
	tolua_function(tolua_S,"getPercX",lua_fairygui_ScrollPane_getPercX);
	tolua_function(tolua_S,"getPosY",lua_fairygui_ScrollPane_getPosY);
	tolua_function(tolua_S,"getFooter",lua_fairygui_ScrollPane_getFooter);
	tolua_function(tolua_S,"setPosX",lua_fairygui_ScrollPane_setPosX);
	tolua_function(tolua_S,"getPosX",lua_fairygui_ScrollPane_getPosX);
	tolua_function(tolua_S,"setPageY",lua_fairygui_ScrollPane_setPageY);
	tolua_function(tolua_S,"scrollTop",lua_fairygui_ScrollPane_scrollTop);
	tolua_function(tolua_S,"isSnapToItem",lua_fairygui_ScrollPane_isSnapToItem);
	tolua_function(tolua_S,"isTouchEffect",lua_fairygui_ScrollPane_isTouchEffect);
	tolua_function(tolua_S,"setScrollStep",lua_fairygui_ScrollPane_setScrollStep);
	tolua_function(tolua_S,"scrollBottom",lua_fairygui_ScrollPane_scrollBottom);
	tolua_function(tolua_S,"isChildInView",lua_fairygui_ScrollPane_isChildInView);
	tolua_function(tolua_S,"isPageMode",lua_fairygui_ScrollPane_isPageMode);
	tolua_function(tolua_S,"setDecelerationRate",lua_fairygui_ScrollPane_setDecelerationRate);
	tolua_function(tolua_S,"getScrollingPosY",lua_fairygui_ScrollPane_getScrollingPosY);
	tolua_function(tolua_S,"getScrollingPosX",lua_fairygui_ScrollPane_getScrollingPosX);
	tolua_function(tolua_S,"getContentSize",lua_fairygui_ScrollPane_getContentSize);
	tolua_function(tolua_S,"isRightMost",lua_fairygui_ScrollPane_isRightMost);
	tolua_function(tolua_S,"setPageController",lua_fairygui_ScrollPane_setPageController);
	tolua_function(tolua_S,"lockHeader",lua_fairygui_ScrollPane_lockHeader);
	tolua_function(tolua_S,"lockFooter",lua_fairygui_ScrollPane_lockFooter);
	tolua_function(tolua_S,"scrollDown",lua_fairygui_ScrollPane_scrollDown);
	tolua_function(tolua_S,"setSnapToItem",lua_fairygui_ScrollPane_setSnapToItem);
	tolua_function(tolua_S,"getScrollStep",lua_fairygui_ScrollPane_getScrollStep);
	tolua_function(tolua_S,"getPageX",lua_fairygui_ScrollPane_getPageX);
	tolua_function(tolua_S,"scrollLeft",lua_fairygui_ScrollPane_scrollLeft);
	tolua_function(tolua_S,"getViewSize",lua_fairygui_ScrollPane_getViewSize);
	tolua_function(tolua_S,"setInertiaDisabled",lua_fairygui_ScrollPane_setInertiaDisabled);
	tolua_function(tolua_S,"getPageController",lua_fairygui_ScrollPane_getPageController);
	tolua_function(tolua_S,"setPageX",lua_fairygui_ScrollPane_setPageX);
	tolua_function(tolua_S,"scrollUp",lua_fairygui_ScrollPane_scrollUp);
	tolua_function(tolua_S,"isMouseWheelEnabled",lua_fairygui_ScrollPane_isMouseWheelEnabled);
	tolua_function(tolua_S,"setPageMode",lua_fairygui_ScrollPane_setPageMode);
	tolua_function(tolua_S,"setTouchEffect",lua_fairygui_ScrollPane_setTouchEffect);
	tolua_function(tolua_S,"setMouseWheelEnabled",lua_fairygui_ScrollPane_setMouseWheelEnabled);
	tolua_function(tolua_S,"isInertiaDisabled",lua_fairygui_ScrollPane_isInertiaDisabled);
	tolua_function(tolua_S,"setPercX",lua_fairygui_ScrollPane_setPercX);
	tolua_function(tolua_S,"setPercY",lua_fairygui_ScrollPane_setPercY);
	tolua_function(tolua_S,"cancelDragging",lua_fairygui_ScrollPane_cancelDragging);
	tolua_function(tolua_S,"getPercY",lua_fairygui_ScrollPane_getPercY);
	tolua_function(tolua_S,"scrollRight",lua_fairygui_ScrollPane_scrollRight);
	tolua_function(tolua_S,"getHeader",lua_fairygui_ScrollPane_getHeader);
	tolua_function(tolua_S,"getVtScrollBar",lua_fairygui_ScrollPane_getVtScrollBar);
	tolua_function(tolua_S,"getOwner",lua_fairygui_ScrollPane_getOwner);
	tolua_function(tolua_S,"isBottomMost",lua_fairygui_ScrollPane_isBottomMost);
	tolua_function(tolua_S,"getPageY",lua_fairygui_ScrollPane_getPageY);
	tolua_function(tolua_S,"isBouncebackEffect",lua_fairygui_ScrollPane_isBouncebackEffect);
	tolua_function(tolua_S,"setPosY",lua_fairygui_ScrollPane_setPosY);
	tolua_function(tolua_S,"getDraggingPane", lua_fairygui_ScrollPane_getDraggingPane);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::ScrollPane).name();
	g_luaType[typeName] = "fairygui.ScrollPane";
	g_typeCast["ScrollPane"] = "fairygui.ScrollPane";
	return 1;
}

static int lua_fairygui_GComponent_addChild(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_addChild'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:addChild");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_addChild'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->addChild(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:addChild",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_addChild'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getHitArea(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getHitArea'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::IHitTest* ret = cobj->getHitArea();
		object_to_luaval<fairygui::IHitTest>(tolua_S, "fairygui.IHitTest",(fairygui::IHitTest*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getHitArea",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getHitArea'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_applyAllControllers(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_applyAllControllers'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->applyAllControllers();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:applyAllControllers",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_applyAllControllers'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getOpaque(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getOpaque'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->getOpaque();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getOpaque",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getOpaque'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setChildrenRenderOrder(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setChildrenRenderOrder'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::ChildrenRenderOrder arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:setChildrenRenderOrder");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setChildrenRenderOrder'", nullptr);
			return 0;
		}
		cobj->setChildrenRenderOrder(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setChildrenRenderOrder",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setChildrenRenderOrder'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setOpaque(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setOpaque'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GComponent:setOpaque");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setOpaque'", nullptr);
			return 0;
		}
		cobj->setOpaque(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setOpaque",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setOpaque'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getViewWidth(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getViewWidth'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getViewWidth();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getViewWidth",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getViewWidth'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getTransitions(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getTransitions'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vector<fairygui::Transition *>& ret = cobj->getTransitions();
		ccvector_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getTransitions",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getTransitions'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getViewHeight(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getViewHeight'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getViewHeight();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getViewHeight",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getViewHeight'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_ensureBoundsCorrect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_ensureBoundsCorrect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->ensureBoundsCorrect();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:ensureBoundsCorrect",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_ensureBoundsCorrect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_isAncestorOf(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_isAncestorOf'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		const fairygui::GObject* arg0;
		ok &= luaval_to_object<const fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:isAncestorOf");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_isAncestorOf'", nullptr);
			return 0;
		}
		bool ret = cobj->isAncestorOf(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:isAncestorOf",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_isAncestorOf'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vector<fairygui::GObject *>& ret = cobj->getChildren();
		ccvector_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildren",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getMask(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getMask'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::Node* ret = cobj->getMask();
		object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node",(cocos2d::Node*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getMask",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getMask'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_adjustRadioGroupDepth(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_adjustRadioGroupDepth'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		fairygui::GController* arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:adjustRadioGroupDepth");
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 3, "fairygui.GController",&arg1, "fairygui.GComponent:adjustRadioGroupDepth");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_adjustRadioGroupDepth'", nullptr);
			return 0;
		}
		cobj->adjustRadioGroupDepth(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:adjustRadioGroupDepth",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_adjustRadioGroupDepth'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComponent:getController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getController'", nullptr);
			return 0;
		}
		fairygui::GController* ret = cobj->getController(arg0);
		object_to_luaval<fairygui::GController>(tolua_S, "fairygui.GController",(fairygui::GController*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setApexIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setApexIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:setApexIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setApexIndex'", nullptr);
			return 0;
		}
		cobj->setApexIndex(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setApexIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setApexIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getTransition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getTransition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComponent:getTransition");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getTransition'", nullptr);
			return 0;
		}
		fairygui::Transition* ret = cobj->getTransition(arg0);
		object_to_luaval<fairygui::Transition>(tolua_S, "fairygui.Transition",(fairygui::Transition*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getTransition",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getTransition'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getFirstChildInView(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getFirstChildInView'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getFirstChildInView();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getFirstChildInView",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getFirstChildInView'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_isChildInView(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_isChildInView'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:isChildInView");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_isChildInView'", nullptr);
			return 0;
		}
		bool ret = cobj->isChildInView(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:isChildInView",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_isChildInView'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setBoundsChangedFlag(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setBoundsChangedFlag'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->setBoundsChangedFlag();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setBoundsChangedFlag",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setBoundsChangedFlag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_addChildAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_addChildAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		int arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:addChildAt");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GComponent:addChildAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_addChildAt'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->addChildAt(arg0, arg1);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:addChildAt",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_addChildAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_removeChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_removeChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->removeChildren();
		return 0;
	}
	if (argc == 2) {
		int arg0;
		int arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:removeChildren");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GComponent:removeChildren");
		cobj->removeChildren(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GComponent:removeChildren",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_removeChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_removeChildAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_removeChildAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:removeChildAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_removeChildAt'", nullptr);
			return 0;
		}
		cobj->removeChildAt(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:removeChildAt",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_removeChildAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChild(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChild'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComponent:getChild");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getChild'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getChild(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChild",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChild'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_addController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_addController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.GComponent:addController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_addController'", nullptr);
			return 0;
		}
		cobj->addController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:addController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_addController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_childSortingOrderChanged(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_childSortingOrderChanged'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 3) {
		fairygui::GObject* arg0;
		int arg1;
		int arg2;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:childSortingOrderChanged");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GComponent:childSortingOrderChanged");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GComponent:childSortingOrderChanged");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_childSortingOrderChanged'", nullptr);
			return 0;
		}
		cobj->childSortingOrderChanged(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:childSortingOrderChanged",argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_childSortingOrderChanged'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_applyController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_applyController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.GComponent:applyController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_applyController'", nullptr);
			return 0;
		}
		cobj->applyController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:applyController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_applyController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getControllers(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getControllers'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Vector<fairygui::GController *>& ret = cobj->getControllers();
		ccvector_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getControllers",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getControllers'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setMask(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setMask'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Node* arg0;
		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "fairygui.GComponent:setMask");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setMask'", nullptr);
			return 0;
		}
		cobj->setMask(arg0);
		return 0;
	}
	if (argc == 2) {
		cocos2d::Node* arg0;
		bool arg1;
		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "fairygui.GComponent:setMask");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.GComponent:setMask");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setMask'", nullptr);
			return 0;
		}
		cobj->setMask(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setMask",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setMask'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setChildIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setChildIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		int arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:setChildIndex");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GComponent:setChildIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setChildIndex'", nullptr);
			return 0;
		}
		cobj->setChildIndex(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setChildIndex",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setChildIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setViewWidth(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setViewWidth'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GComponent:setViewWidth");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setViewWidth'", nullptr);
			return 0;
		}
		cobj->setViewWidth(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setViewWidth",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setViewWidth'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildById(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildById'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComponent:getChildById");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getChildById'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getChildById(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildById",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildById'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildByPath(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildByPath'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComponent:getChildByPath");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getChildByPath'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getChildByPath(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildByPath",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildByPath'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		const fairygui::GObject* arg0;
		ok &= luaval_to_object<const fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:getChildIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getChildIndex'", nullptr);
			return 0;
		}
		int ret = cobj->getChildIndex(arg0);
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getMargin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getMargin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const fairygui::Margin& ret = cobj->getMargin();
		margin_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getMargin",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getMargin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getControllerAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getControllerAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:getControllerAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getControllerAt'", nullptr);
			return 0;
		}
		fairygui::GController* ret = cobj->getControllerAt(arg0);
		object_to_luaval<fairygui::GController>(tolua_S, "fairygui.GController",(fairygui::GController*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getControllerAt",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getControllerAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:getChildAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getChildAt'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getChildAt(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildAt",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getApexIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getApexIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getApexIndex();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getApexIndex",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getApexIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setMargin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setMargin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::Margin arg0;
		ok &= luaval_to_margin(tolua_S, 2, &arg0, "fairygui.GComponent:setMargin");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setMargin'", nullptr);
			return 0;
		}
		cobj->setMargin(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setMargin",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setMargin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getScrollPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getScrollPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::ScrollPane* ret = cobj->getScrollPane();
		object_to_luaval<fairygui::ScrollPane>(tolua_S, "fairygui.ScrollPane",(fairygui::ScrollPane*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getScrollPane",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getScrollPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getTransitionAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getTransitionAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:getTransitionAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getTransitionAt'", nullptr);
			return 0;
		}
		fairygui::Transition* ret = cobj->getTransitionAt(arg0);
		object_to_luaval<fairygui::Transition>(tolua_S, "fairygui.Transition",(fairygui::Transition*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getTransitionAt",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getTransitionAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildInGroup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildInGroup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		const fairygui::GGroup* arg0;
		std::string arg1;
		ok &= luaval_to_object<const fairygui::GGroup>(tolua_S, 2, "fairygui.GGroup",&arg0, "fairygui.GComponent:getChildInGroup");
		ok &= luaval_to_std_string(tolua_S, 3,&arg1, "fairygui.GComponent:getChildInGroup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getChildInGroup'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getChildInGroup(arg0, arg1);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildInGroup",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildInGroup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_removeController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_removeController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.GComponent:removeController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_removeController'", nullptr);
			return 0;
		}
		cobj->removeController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:removeController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_removeController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getSnappingPosition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getSnappingPosition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Vec2 arg0;
		ok &= luaval_to_vec2(tolua_S, 2, &arg0, "fairygui.GComponent:getSnappingPosition");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_getSnappingPosition'", nullptr);
			return 0;
		}
		cocos2d::Vec2 ret = cobj->getSnappingPosition(arg0);
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getSnappingPosition",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getSnappingPosition'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setChildIndexBefore(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setChildIndexBefore'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		int arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:setChildIndexBefore");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GComponent:setChildIndexBefore");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setChildIndexBefore'", nullptr);
			return 0;
		}
		int ret = cobj->setChildIndexBefore(arg0, arg1);
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setChildIndexBefore",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setChildIndexBefore'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setHitArea(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setHitArea'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::IHitTest* arg0;
		ok &= luaval_to_object<fairygui::IHitTest>(tolua_S, 2, "fairygui.IHitTest",&arg0, "fairygui.GComponent:setHitArea");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setHitArea'", nullptr);
			return 0;
		}
		cobj->setHitArea(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setHitArea",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setHitArea'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_swapChildrenAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_swapChildrenAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		int arg0;
		int arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComponent:swapChildrenAt");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GComponent:swapChildrenAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_swapChildrenAt'", nullptr);
			return 0;
		}
		cobj->swapChildrenAt(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:swapChildrenAt",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_swapChildrenAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_numChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_numChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->numChildren();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:numChildren",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_numChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_removeChild(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_removeChild'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:removeChild");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_removeChild'", nullptr);
			return 0;
		}
		cobj->removeChild(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:removeChild",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_removeChild'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_swapChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_swapChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GObject* arg0;
		fairygui::GObject* arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:swapChildren");
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 3, "fairygui.GObject",&arg1, "fairygui.GComponent:swapChildren");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_swapChildren'", nullptr);
			return 0;
		}
		cobj->swapChildren(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:swapChildren",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_swapChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_childStateChanged(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_childStateChanged'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GComponent:childStateChanged");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_childStateChanged'", nullptr);
			return 0;
		}
		cobj->childStateChanged(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:childStateChanged",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_childStateChanged'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_getChildrenRenderOrder(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_getChildrenRenderOrder'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getChildrenRenderOrder();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:getChildrenRenderOrder",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_getChildrenRenderOrder'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_setViewHeight(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComponent_setViewHeight'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GComponent:setViewHeight");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComponent_setViewHeight'", nullptr);
			return 0;
		}
		cobj->setViewHeight(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComponent:setViewHeight",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_setViewHeight'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComponent_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GComponent",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GComponent* ret = fairygui::GComponent::create();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GComponent:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComponent_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GComponent(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GComponent");
	tolua_cclass(tolua_S,"GComponent","fairygui.GComponent","fairygui.GObject",nullptr);

	tolua_beginmodule(tolua_S,"GComponent");
	tolua_function(tolua_S,"addChild",lua_fairygui_GComponent_addChild);
	tolua_function(tolua_S,"getHitArea",lua_fairygui_GComponent_getHitArea);
	tolua_function(tolua_S,"applyAllControllers",lua_fairygui_GComponent_applyAllControllers);
	tolua_function(tolua_S,"getOpaque",lua_fairygui_GComponent_getOpaque);
	tolua_function(tolua_S,"setChildrenRenderOrder",lua_fairygui_GComponent_setChildrenRenderOrder);
	tolua_function(tolua_S,"setOpaque",lua_fairygui_GComponent_setOpaque);
	tolua_function(tolua_S,"getViewWidth",lua_fairygui_GComponent_getViewWidth);
	tolua_function(tolua_S,"getTransitions",lua_fairygui_GComponent_getTransitions);
	tolua_function(tolua_S,"getViewHeight",lua_fairygui_GComponent_getViewHeight);
	tolua_function(tolua_S,"ensureBoundsCorrect",lua_fairygui_GComponent_ensureBoundsCorrect);
	tolua_function(tolua_S,"isAncestorOf",lua_fairygui_GComponent_isAncestorOf);
	tolua_function(tolua_S,"getChildren",lua_fairygui_GComponent_getChildren);
	tolua_function(tolua_S,"getMask",lua_fairygui_GComponent_getMask);
	tolua_function(tolua_S,"adjustRadioGroupDepth",lua_fairygui_GComponent_adjustRadioGroupDepth);
	tolua_function(tolua_S,"getController",lua_fairygui_GComponent_getController);
	tolua_function(tolua_S,"setApexIndex",lua_fairygui_GComponent_setApexIndex);
	tolua_function(tolua_S,"getTransition",lua_fairygui_GComponent_getTransition);
	tolua_function(tolua_S,"getFirstChildInView",lua_fairygui_GComponent_getFirstChildInView);
	tolua_function(tolua_S,"isChildInView",lua_fairygui_GComponent_isChildInView);
	tolua_function(tolua_S,"setBoundsChangedFlag",lua_fairygui_GComponent_setBoundsChangedFlag);
	tolua_function(tolua_S,"addChildAt",lua_fairygui_GComponent_addChildAt);
	tolua_function(tolua_S,"removeChildren",lua_fairygui_GComponent_removeChildren);
	tolua_function(tolua_S,"removeChildAt",lua_fairygui_GComponent_removeChildAt);
	tolua_function(tolua_S,"getChild",lua_fairygui_GComponent_getChild);
	tolua_function(tolua_S,"addController",lua_fairygui_GComponent_addController);
	tolua_function(tolua_S,"childSortingOrderChanged",lua_fairygui_GComponent_childSortingOrderChanged);
	tolua_function(tolua_S,"applyController",lua_fairygui_GComponent_applyController);
	tolua_function(tolua_S,"getControllers",lua_fairygui_GComponent_getControllers);
	tolua_function(tolua_S,"setMask",lua_fairygui_GComponent_setMask);
	tolua_function(tolua_S,"setChildIndex",lua_fairygui_GComponent_setChildIndex);
	tolua_function(tolua_S,"setViewWidth",lua_fairygui_GComponent_setViewWidth);
	tolua_function(tolua_S,"getChildById",lua_fairygui_GComponent_getChildById);
	tolua_function(tolua_S,"getChildByPath",lua_fairygui_GComponent_getChildByPath);
	tolua_function(tolua_S,"getChildIndex",lua_fairygui_GComponent_getChildIndex);
	tolua_function(tolua_S,"getMargin",lua_fairygui_GComponent_getMargin);
	tolua_function(tolua_S,"getControllerAt",lua_fairygui_GComponent_getControllerAt);
	tolua_function(tolua_S,"getChildAt",lua_fairygui_GComponent_getChildAt);
	tolua_function(tolua_S,"getApexIndex",lua_fairygui_GComponent_getApexIndex);
	tolua_function(tolua_S,"setMargin",lua_fairygui_GComponent_setMargin);
	tolua_function(tolua_S,"getScrollPane",lua_fairygui_GComponent_getScrollPane);
	tolua_function(tolua_S,"getTransitionAt",lua_fairygui_GComponent_getTransitionAt);
	tolua_function(tolua_S,"getChildInGroup",lua_fairygui_GComponent_getChildInGroup);
	tolua_function(tolua_S,"removeController",lua_fairygui_GComponent_removeController);
	tolua_function(tolua_S,"getSnappingPosition",lua_fairygui_GComponent_getSnappingPosition);
	tolua_function(tolua_S,"setChildIndexBefore",lua_fairygui_GComponent_setChildIndexBefore);
	tolua_function(tolua_S,"setHitArea",lua_fairygui_GComponent_setHitArea);
	tolua_function(tolua_S,"swapChildrenAt",lua_fairygui_GComponent_swapChildrenAt);
	tolua_function(tolua_S,"numChildren",lua_fairygui_GComponent_numChildren);
	tolua_function(tolua_S,"removeChild",lua_fairygui_GComponent_removeChild);
	tolua_function(tolua_S,"swapChildren",lua_fairygui_GComponent_swapChildren);
	tolua_function(tolua_S,"childStateChanged",lua_fairygui_GComponent_childStateChanged);
	tolua_function(tolua_S,"getChildrenRenderOrder",lua_fairygui_GComponent_getChildrenRenderOrder);
	tolua_function(tolua_S,"setViewHeight",lua_fairygui_GComponent_setViewHeight);
	tolua_function(tolua_S,"create", lua_fairygui_GComponent_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GComponent).name();
	g_luaType[typeName] = "fairygui.GComponent";
	g_typeCast["GComponent"] = "fairygui.GComponent";
	return 1;
}

static int lua_fairygui_GButton_getRelatedController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_getRelatedController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GController* ret = cobj->getRelatedController();
		object_to_luaval<fairygui::GController>(tolua_S, "fairygui.GController",(fairygui::GController*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:getRelatedController",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_getRelatedController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setChangeStateOnClick(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setChangeStateOnClick'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GButton:setChangeStateOnClick");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setChangeStateOnClick'", nullptr);
			return 0;
		}
		cobj->setChangeStateOnClick(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setChangeStateOnClick",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setChangeStateOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setSelectedTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setSelectedTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GButton:setSelectedTitle");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setSelectedTitle'", nullptr);
			return 0;
		}
		cobj->setSelectedTitle(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setSelectedTitle",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setSelectedTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setSelected(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setSelected'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GButton:setSelected");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setSelected'", nullptr);
			return 0;
		}
		cobj->setSelected(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setSelected",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setSelected'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setRelatedController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setRelatedController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.GButton:setRelatedController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setRelatedController'", nullptr);
			return 0;
		}
		cobj->setRelatedController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setRelatedController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setRelatedController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GButton:setTitle");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setTitle'", nullptr);
			return 0;
		}
		cobj->setTitle(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setTitle",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_getTitleFontSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_getTitleFontSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getTitleFontSize();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:getTitleFontSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_getTitleFontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_getSelectedIcon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_getSelectedIcon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getSelectedIcon();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:getSelectedIcon",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_getSelectedIcon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_isSelected(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_isSelected'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isSelected();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:isSelected",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_isSelected'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_getTitleColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_getTitleColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B& ret = cobj->getTitleColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:getTitleColor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_getTitleColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_getSelectedTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_getSelectedTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getSelectedTitle();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:getSelectedTitle",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_getSelectedTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_getTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_getTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getTitle();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:getTitle",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_getTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setSelectedIcon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setSelectedIcon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GButton:setSelectedIcon");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setSelectedIcon'", nullptr);
			return 0;
		}
		cobj->setSelectedIcon(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setSelectedIcon",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setSelectedIcon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_isChangeStateOnClick(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_isChangeStateOnClick'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isChangeStateOnClick();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:isChangeStateOnClick",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_isChangeStateOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setTitleFontSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setTitleFontSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GButton:setTitleFontSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setTitleFontSize'", nullptr);
			return 0;
		}
		cobj->setTitleFontSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setTitleFontSize",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setTitleFontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_setTitleColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GButton* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GButton*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GButton_setTitleColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GButton:setTitleColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GButton_setTitleColor'", nullptr);
			return 0;
		}
		cobj->setTitleColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GButton:setTitleColor",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_setTitleColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GButton_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GButton",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GButton* ret = fairygui::GButton::create();
		object_to_luaval<fairygui::GButton>(tolua_S, "fairygui.GButton",(fairygui::GButton*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GButton:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GButton_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GButton(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GButton");
	tolua_cclass(tolua_S,"GButton","fairygui.GButton","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GButton");
	tolua_function(tolua_S,"getRelatedController",lua_fairygui_GButton_getRelatedController);
	tolua_function(tolua_S,"setChangeStateOnClick",lua_fairygui_GButton_setChangeStateOnClick);
	tolua_function(tolua_S,"setSelectedTitle",lua_fairygui_GButton_setSelectedTitle);
	tolua_function(tolua_S,"setSelected",lua_fairygui_GButton_setSelected);
	tolua_function(tolua_S,"setRelatedController",lua_fairygui_GButton_setRelatedController);
	tolua_function(tolua_S,"setTitle",lua_fairygui_GButton_setTitle);
	tolua_function(tolua_S,"getTitleFontSize",lua_fairygui_GButton_getTitleFontSize);
	tolua_function(tolua_S,"getSelectedIcon",lua_fairygui_GButton_getSelectedIcon);
	tolua_function(tolua_S,"isSelected",lua_fairygui_GButton_isSelected);
	tolua_function(tolua_S,"getTitleColor",lua_fairygui_GButton_getTitleColor);
	tolua_function(tolua_S,"getSelectedTitle",lua_fairygui_GButton_getSelectedTitle);
	tolua_function(tolua_S,"getTitle",lua_fairygui_GButton_getTitle);
	tolua_function(tolua_S,"setSelectedIcon",lua_fairygui_GButton_setSelectedIcon);
	tolua_function(tolua_S,"isChangeStateOnClick",lua_fairygui_GButton_isChangeStateOnClick);
	tolua_function(tolua_S,"setTitleFontSize",lua_fairygui_GButton_setTitleFontSize);
	tolua_function(tolua_S,"setTitleColor",lua_fairygui_GButton_setTitleColor);
	tolua_function(tolua_S,"create", lua_fairygui_GButton_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GButton).name();
	g_luaType[typeName] = "fairygui.GButton";
	g_typeCast["GButton"] = "fairygui.GButton";
	return 1;
}

static int lua_fairygui_GObjectPool_getObject(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObjectPool* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObjectPool",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObjectPool*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObjectPool_getObject'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GObjectPool:getObject");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObjectPool_getObject'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getObject(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObjectPool:getObject",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObjectPool_getObject'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GObjectPool_returnObject(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObjectPool* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GObjectPool",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GObjectPool*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GObjectPool_returnObject'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GObjectPool:returnObject");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GObjectPool_returnObject'", nullptr);
			return 0;
		}
		cobj->returnObject(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GObjectPool:returnObject",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GObjectPool_returnObject'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GObjectPool(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GObjectPool");
	tolua_cclass(tolua_S,"GObjectPool","fairygui.GObjectPool","",nullptr);

	tolua_beginmodule(tolua_S,"GObjectPool");
	tolua_function(tolua_S,"getObject",lua_fairygui_GObjectPool_getObject);
	tolua_function(tolua_S,"returnObject",lua_fairygui_GObjectPool_returnObject);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GObjectPool).name();
	g_luaType[typeName] = "fairygui.GObjectPool";
	g_typeCast["GObjectPool"] = "fairygui.GObjectPool";
	return 1;
}

static int lua_fairygui_GList_selectAll(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_selectAll'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->selectAll();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:selectAll",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_selectAll'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getColumnGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getColumnGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getColumnGap();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getColumnGap",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getColumnGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getLineCount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getLineCount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getLineCount();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getLineCount",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getLineCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setVirtualAndLoop(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setVirtualAndLoop'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->setVirtualAndLoop();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setVirtualAndLoop",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setVirtualAndLoop'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setLayout(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setLayout'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::ListLayoutType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setLayout");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setLayout'", nullptr);
			return 0;
		}
		cobj->setLayout(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setLayout",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setLayout'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_refreshVirtualList(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_refreshVirtualList'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->refreshVirtualList();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:refreshVirtualList",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_refreshVirtualList'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_removeSelection(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_removeSelection'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:removeSelection");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_removeSelection'", nullptr);
			return 0;
		}
		cobj->removeSelection(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:removeSelection",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_removeSelection'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setColumnGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setColumnGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setColumnGap");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setColumnGap'", nullptr);
			return 0;
		}
		cobj->setColumnGap(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setColumnGap",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setColumnGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getColumnCount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getColumnCount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getColumnCount();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getColumnCount",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getColumnCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setVirtual(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setVirtual'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->setVirtual();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setVirtual",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setVirtual'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setNumItems(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setNumItems'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setNumItems");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setNumItems'", nullptr);
			return 0;
		}
		cobj->setNumItems(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setNumItems",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setNumItems'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setVerticalAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setVerticalAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::TextVAlignment arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setVerticalAlign");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setVerticalAlign'", nullptr);
			return 0;
		}
		cobj->setVerticalAlign(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setVerticalAlign",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setVerticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_resizeToFit(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_resizeToFit'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:resizeToFit");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_resizeToFit'", nullptr);
			return 0;
		}
		cobj->resizeToFit(arg0);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		int arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:resizeToFit");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GList:resizeToFit");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_resizeToFit'", nullptr);
			return 0;
		}
		cobj->resizeToFit(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GList:resizeToFit",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_resizeToFit'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setSelectionController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setSelectionController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.GList:setSelectionController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setSelectionController'", nullptr);
			return 0;
		}
		cobj->setSelectionController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setSelectionController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setSelectionController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_returnToPool(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_returnToPool'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GList:returnToPool");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_returnToPool'", nullptr);
			return 0;
		}
		cobj->returnToPool(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:returnToPool",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_returnToPool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_clearSelection(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_clearSelection'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->clearSelection();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:clearSelection",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_clearSelection'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getNumItems(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getNumItems'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getNumItems();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getNumItems",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getNumItems'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setColumnCount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setColumnCount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setColumnCount");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setColumnCount'", nullptr);
			return 0;
		}
		cobj->setColumnCount(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setColumnCount",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setColumnCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_removeChildToPoolAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_removeChildToPoolAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:removeChildToPoolAt");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_removeChildToPoolAt'", nullptr);
			return 0;
		}
		cobj->removeChildToPoolAt(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:removeChildToPoolAt",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_removeChildToPoolAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_addItemFromPool(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_addItemFromPool'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->addItemFromPool();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GList:addItemFromPool");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_addItemFromPool'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->addItemFromPool(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GList:addItemFromPool",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_addItemFromPool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_selectReverse(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_selectReverse'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->selectReverse();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:selectReverse",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_selectReverse'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setLineCount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setLineCount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setLineCount");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setLineCount'", nullptr);
			return 0;
		}
		cobj->setLineCount(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setLineCount",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setLineCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setAutoResizeItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setAutoResizeItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GList:setAutoResizeItem");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setAutoResizeItem'", nullptr);
			return 0;
		}
		cobj->setAutoResizeItem(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setAutoResizeItem",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setAutoResizeItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getVerticalAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getVerticalAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getVerticalAlign();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getVerticalAlign",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getVerticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_removeChildrenToPool(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_removeChildrenToPool'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->removeChildrenToPool();
		return 0;
	}
	if (argc == 2) {
		int arg0;
		int arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:removeChildrenToPool");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GList:removeChildrenToPool");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_removeChildrenToPool'", nullptr);
			return 0;
		}
		cobj->removeChildrenToPool(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GList:removeChildrenToPool",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_removeChildrenToPool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::TextHAlignment arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setAlign");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setAlign'", nullptr);
			return 0;
		}
		cobj->setAlign(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setAlign",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_isVirtual(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_isVirtual'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isVirtual();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:isVirtual",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_isVirtual'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getSelectedIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getSelectedIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getSelectedIndex();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getSelectedIndex",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getSelectedIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getSelection(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getSelection'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		std::vector<int> arg0;
		cobj->getSelection(arg0);
		ccvector_int_to_luaval(tolua_S, arg0);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getSelection",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getSelection'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_itemIndexToChildIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_itemIndexToChildIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:itemIndexToChildIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_itemIndexToChildIndex'", nullptr);
			return 0;
		}
		int ret = cobj->itemIndexToChildIndex(arg0);
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:itemIndexToChildIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_itemIndexToChildIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getItemPool(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getItemPool'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObjectPool* ret = cobj->getItemPool();
		object_to_luaval<fairygui::GObjectPool>(tolua_S, "fairygui.GObjectPool",(fairygui::GObjectPool*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getItemPool",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getItemPool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getSelectionController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getSelectionController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GController* ret = cobj->getSelectionController();
		object_to_luaval<fairygui::GController>(tolua_S, "fairygui.GController",(fairygui::GController*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getSelectionController",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getSelectionController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getLineGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getLineGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getLineGap();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getLineGap",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getLineGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_scrollToView(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_scrollToView'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:scrollToView");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_scrollToView'", nullptr);
			return 0;
		}
		cobj->scrollToView(arg0);
		return 0;
	}
	if (argc == 2) {
		int arg0;
		bool arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:scrollToView");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.GList:scrollToView");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_scrollToView'", nullptr);
			return 0;
		}
		cobj->scrollToView(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		int arg0;
		bool arg1;
		bool arg2;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:scrollToView");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.GList:scrollToView");
		ok &= luaval_to_boolean(tolua_S, 4,&arg2, "fairygui.GList:scrollToView");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_scrollToView'", nullptr);
			return 0;
		}
		cobj->scrollToView(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:scrollToView",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_scrollToView'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_handleArrowKey(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_handleArrowKey'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:handleArrowKey");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_handleArrowKey'", nullptr);
			return 0;
		}
		cobj->handleArrowKey(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:handleArrowKey",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_handleArrowKey'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getAlign(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getAlign'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getAlign();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getAlign",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getFromPool(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getFromPool'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getFromPool();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GList:getFromPool");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_getFromPool'", nullptr);
			return 0;
		}
		fairygui::GObject* ret = cobj->getFromPool(arg0);
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GList:getFromPool",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getFromPool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getDefaultItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getDefaultItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getDefaultItem();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getDefaultItem",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getDefaultItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getSelectionMode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getSelectionMode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getSelectionMode();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getSelectionMode",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getSelectionMode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setDefaultItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setDefaultItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GList:setDefaultItem");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setDefaultItem'", nullptr);
			return 0;
		}
		cobj->setDefaultItem(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setDefaultItem",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setDefaultItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setSelectedIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setSelectedIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setSelectedIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setSelectedIndex'", nullptr);
			return 0;
		}
		cobj->setSelectedIndex(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setSelectedIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setSelectedIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setSelectionMode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setSelectionMode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::ListSelectionMode arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setSelectionMode");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setSelectionMode'", nullptr);
			return 0;
		}
		cobj->setSelectionMode(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setSelectionMode",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setSelectionMode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_addSelection(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_addSelection'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		int arg0;
		bool arg1;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:addSelection");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.GList:addSelection");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_addSelection'", nullptr);
			return 0;
		}
		cobj->addSelection(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:addSelection",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_addSelection'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_childIndexToItemIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_childIndexToItemIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:childIndexToItemIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_childIndexToItemIndex'", nullptr);
			return 0;
		}
		int ret = cobj->childIndexToItemIndex(arg0);
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:childIndexToItemIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_childIndexToItemIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getAutoResizeItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getAutoResizeItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->getAutoResizeItem();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getAutoResizeItem",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getAutoResizeItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_setLineGap(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_setLineGap'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GList:setLineGap");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_setLineGap'", nullptr);
			return 0;
		}
		cobj->setLineGap(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:setLineGap",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_setLineGap'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_removeChildToPool(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_removeChildToPool'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GList:removeChildToPool");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GList_removeChildToPool'", nullptr);
			return 0;
		}
		cobj->removeChildToPool(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:removeChildToPool",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_removeChildToPool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_getLayout(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GList_getLayout'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getLayout();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GList:getLayout",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_getLayout'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GList* ret = fairygui::GList::create();
		object_to_luaval<fairygui::GList>(tolua_S, "fairygui.GList",(fairygui::GList*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GList:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GList_create'.",&tolua_err);
	return 0;
#endif
}

typedef enum {
	GLIST_ITEM_RENDERER,
	GLIST_ITEM_PROVIDER,
	GTREE_NODE_RENDERER, // GTree is expend from list
	GTREE_NODE_WILLEXPAND
} GList_HandlerType;

static int lua_fairygui_GList_get_itemRenderer(lua_State* L)
{
	fairygui::GList* cobj = nullptr;
	int refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_get_itemRenderer'", nullptr);
		return 0;
	}
#endif

	// GLIST_ITEM_RENDERER is a HandlerType
	refid = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)GLIST_ITEM_RENDERER);
	if (refid >= 0) {
		toluafix_get_function_by_refid(L, refid);
	} else {
		lua_pushnil(L);
	}
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_get_itemRenderer'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_set_itemRenderer(lua_State* L)
{
	fairygui::GList* cobj = nullptr;
	LUA_FUNCTION refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_set_itemRenderer'", nullptr);
		return 0;
	}
#endif

	if lua_isnil(L, 2) {
		cobj->itemRenderer = nullptr;
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)GLIST_ITEM_RENDERER);
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		goto tolua_lerror;
#endif

	refid = (toluafix_ref_function(L, 2, 0));
	cobj->itemRenderer = [=](int index, fairygui::GObject *gobject) {
		auto stack = LuaEngine::getInstance()->getLuaStack();
		lua_pushinteger(stack->getLuaState(), index);
		object_to_luaval<fairygui::GObject>(stack->getLuaState(), "fairygui.GObject", gobject);
		stack->executeFunctionByHandler(refid, 2);
	};
	// GLIST_ITEM_RENDERER is a HandlerType
	ScriptHandlerMgr::getInstance()->addObjectHandler((void*)cobj, refid, (ScriptHandlerMgr::HandlerType)GLIST_ITEM_RENDERER);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_set_itemRenderer'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_get_itemProvider(lua_State* L)
{
	fairygui::GList* cobj = nullptr;
	LUA_FUNCTION refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_get_itemProvider'", nullptr);
		return 0;
	}
#endif

	// GLIST_ITEM_PROVIDER is a HandlerType
	refid = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)GLIST_ITEM_PROVIDER);
	if (refid >= 0) {
		toluafix_get_function_by_refid(L, refid);
	} else {
		lua_pushnil(L);
	}
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_get_itemProvider'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_set_itemProvider(lua_State* L)
{
	fairygui::GList* cobj = nullptr;
	LUA_FUNCTION refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_set_itemProvider'", nullptr);
		return 0;
	}
#endif

	if lua_isnil(L, 2) {
		cobj->itemProvider = nullptr;
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)GLIST_ITEM_PROVIDER);
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		goto tolua_lerror;
#endif

	refid = (toluafix_ref_function(L, 2, 0));
	cobj->itemProvider = [=](int index) -> std::string {
		LuaEngine::getInstance()->getLuaStack()->pushFunctionByHandler(refid);
		lua_pushinteger(L, index);
		int error = lua_pcall(L, 1, 1, 0); // 1 pararm, 1 return
		if (error) {
			CCLOG("[LUA ERROR] %s", lua_tostring(L, -1));
			lua_pop(L, 1); // remove error message from stack
			return "";
		}
		std::string rtn = lua_tostring(L, -1);
		lua_pop(L, 1);
		return rtn;
	};
	// GLIST_ITEM_PROVIDER is a HandlerType
	ScriptHandlerMgr::getInstance()->addObjectHandler((void*)cobj, refid, (ScriptHandlerMgr::HandlerType)GLIST_ITEM_PROVIDER);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_set_itemProvider'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_get_scrollItemToViewOnClick(lua_State* L)
{
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_get_scrollItemToViewOnClick'", nullptr);
		return 0;
	}
#endif

	lua_pushboolean(L, cobj->scrollItemToViewOnClick);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_get_scrollItemToViewOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_set_scrollItemToViewOnClick(lua_State* L)
{
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_set_scrollItemToViewOnClick'", nullptr);
		return 0;
	}
#endif

	if lua_isboolean(L, 2) {
		cobj->scrollItemToViewOnClick = lua_toboolean(L, 2);
		return 0;
	}
	luaL_error(L, "fairygui.GList:scrollItemToViewOnClick value type MUST be Boolean!");

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_set_scrollItemToViewOnClick'.",&tolua_err);
#endif
	return 0;
}

static int lua_fairygui_GList_get_foldInvisibleItems(lua_State* L)
{
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_get_foldInvisibleItems'", nullptr);
		return 0;
	}
#endif

	lua_pushboolean(L, cobj->foldInvisibleItems);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_get_foldInvisibleItems'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GList_set_foldInvisibleItems(lua_State* L)
{
	fairygui::GList* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GList",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GList*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GList_set_foldInvisibleItems'", nullptr);
		return 0;
	}
#endif

	if lua_isboolean(L, 2) {
		cobj->foldInvisibleItems = lua_toboolean(L, 2);
		return 0;
	}
	luaL_error(L, "fairygui.GList:foldInvisibleItems value type MUST be Boolean!");

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GList_set_foldInvisibleItems'.",&tolua_err);
#endif
	return 0;
}

static int lua_register_fairygui_GList(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GList");
	tolua_cclass(tolua_S,"GList","fairygui.GList","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GList");
	// variable
	tolua_variable(tolua_S, "itemRenderer", lua_fairygui_GList_get_itemRenderer, lua_fairygui_GList_set_itemRenderer);
	tolua_variable(tolua_S, "itemProvider", lua_fairygui_GList_get_itemProvider, lua_fairygui_GList_set_itemProvider);
	tolua_variable(tolua_S, "scrollItemToViewOnClick", lua_fairygui_GList_get_scrollItemToViewOnClick, lua_fairygui_GList_set_scrollItemToViewOnClick);
	tolua_variable(tolua_S, "foldInvisibleItems", lua_fairygui_GList_get_foldInvisibleItems, lua_fairygui_GList_set_foldInvisibleItems);
	// function
	tolua_function(tolua_S,"selectAll",lua_fairygui_GList_selectAll);
	tolua_function(tolua_S,"getColumnGap",lua_fairygui_GList_getColumnGap);
	tolua_function(tolua_S,"getLineCount",lua_fairygui_GList_getLineCount);
	tolua_function(tolua_S,"setVirtualAndLoop",lua_fairygui_GList_setVirtualAndLoop);
	tolua_function(tolua_S,"setLayout",lua_fairygui_GList_setLayout);
	tolua_function(tolua_S,"refreshVirtualList",lua_fairygui_GList_refreshVirtualList);
	tolua_function(tolua_S,"removeSelection",lua_fairygui_GList_removeSelection);
	tolua_function(tolua_S,"setColumnGap",lua_fairygui_GList_setColumnGap);
	tolua_function(tolua_S,"getColumnCount",lua_fairygui_GList_getColumnCount);
	tolua_function(tolua_S,"setVirtual",lua_fairygui_GList_setVirtual);
	tolua_function(tolua_S,"setNumItems",lua_fairygui_GList_setNumItems);
	tolua_function(tolua_S,"setVerticalAlign",lua_fairygui_GList_setVerticalAlign);
	tolua_function(tolua_S,"resizeToFit",lua_fairygui_GList_resizeToFit);
	tolua_function(tolua_S,"setSelectionController",lua_fairygui_GList_setSelectionController);
	tolua_function(tolua_S,"returnToPool",lua_fairygui_GList_returnToPool);
	tolua_function(tolua_S,"clearSelection",lua_fairygui_GList_clearSelection);
	tolua_function(tolua_S,"getNumItems",lua_fairygui_GList_getNumItems);
	tolua_function(tolua_S,"setColumnCount",lua_fairygui_GList_setColumnCount);
	tolua_function(tolua_S,"removeChildToPoolAt",lua_fairygui_GList_removeChildToPoolAt);
	tolua_function(tolua_S,"addItemFromPool",lua_fairygui_GList_addItemFromPool);
	tolua_function(tolua_S,"selectReverse",lua_fairygui_GList_selectReverse);
	tolua_function(tolua_S,"setLineCount",lua_fairygui_GList_setLineCount);
	tolua_function(tolua_S,"setAutoResizeItem",lua_fairygui_GList_setAutoResizeItem);
	tolua_function(tolua_S,"getVerticalAlign",lua_fairygui_GList_getVerticalAlign);
	tolua_function(tolua_S,"removeChildrenToPool",lua_fairygui_GList_removeChildrenToPool);
	tolua_function(tolua_S,"setAlign",lua_fairygui_GList_setAlign);
	tolua_function(tolua_S,"isVirtual",lua_fairygui_GList_isVirtual);
	tolua_function(tolua_S,"getSelectedIndex",lua_fairygui_GList_getSelectedIndex);
	tolua_function(tolua_S,"getSelection",lua_fairygui_GList_getSelection);
	tolua_function(tolua_S,"itemIndexToChildIndex",lua_fairygui_GList_itemIndexToChildIndex);
	tolua_function(tolua_S,"getItemPool",lua_fairygui_GList_getItemPool);
	tolua_function(tolua_S,"getSelectionController",lua_fairygui_GList_getSelectionController);
	tolua_function(tolua_S,"getLineGap",lua_fairygui_GList_getLineGap);
	tolua_function(tolua_S,"scrollToView",lua_fairygui_GList_scrollToView);
	tolua_function(tolua_S,"handleArrowKey",lua_fairygui_GList_handleArrowKey);
	tolua_function(tolua_S,"getAlign",lua_fairygui_GList_getAlign);
	tolua_function(tolua_S,"getFromPool",lua_fairygui_GList_getFromPool);
	tolua_function(tolua_S,"getDefaultItem",lua_fairygui_GList_getDefaultItem);
	tolua_function(tolua_S,"getSelectionMode",lua_fairygui_GList_getSelectionMode);
	tolua_function(tolua_S,"setDefaultItem",lua_fairygui_GList_setDefaultItem);
	tolua_function(tolua_S,"setSelectedIndex",lua_fairygui_GList_setSelectedIndex);
	tolua_function(tolua_S,"setSelectionMode",lua_fairygui_GList_setSelectionMode);
	tolua_function(tolua_S,"addSelection",lua_fairygui_GList_addSelection);
	tolua_function(tolua_S,"childIndexToItemIndex",lua_fairygui_GList_childIndexToItemIndex);
	tolua_function(tolua_S,"getAutoResizeItem",lua_fairygui_GList_getAutoResizeItem);
	tolua_function(tolua_S,"setLineGap",lua_fairygui_GList_setLineGap);
	tolua_function(tolua_S,"removeChildToPool",lua_fairygui_GList_removeChildToPool);
	tolua_function(tolua_S,"getLayout",lua_fairygui_GList_getLayout);
	tolua_function(tolua_S,"create", lua_fairygui_GList_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GList).name();
	g_luaType[typeName] = "fairygui.GList";
	g_typeCast["GList"] = "fairygui.GList";
	return 1;
}

static int lua_fairygui_GComboBox_setValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComboBox:setValue");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComboBox_setValue'", nullptr);
			return 0;
		}
		cobj->setValue(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setValue",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_setSelectedIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setSelectedIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GComboBox:setSelectedIndex");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComboBox_setSelectedIndex'", nullptr);
			return 0;
		}
		cobj->setSelectedIndex(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setSelectedIndex",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setSelectedIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getItems(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getItems'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		std::vector<std::string>& ret = cobj->getItems();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getItems",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getItems'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getSelectedIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getSelectedIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getSelectedIndex();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getSelectedIndex",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getSelectedIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_setTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GComboBox:setTitle");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComboBox_setTitle'", nullptr);
			return 0;
		}
		cobj->setTitle(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setTitle",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_refresh(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_refresh'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->refresh();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:refresh",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_refresh'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getValue();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getValue",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getIcons(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getIcons'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		std::vector<std::string>& ret = cobj->getIcons();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getIcons",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getIcons'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& ret = cobj->getTitle();
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getTitle",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getSelectionController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getSelectionController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GController* ret = cobj->getSelectionController();
		object_to_luaval<fairygui::GController>(tolua_S, "fairygui.GController",(fairygui::GController*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getSelectionController",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getSelectionController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_getValues(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_getValues'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		std::vector<std::string>& ret = cobj->getValues();
		ccvector_std_string_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:getValues",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_getValues'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_setSelectionController(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setSelectionController'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GController* arg0;
		ok &= luaval_to_object<fairygui::GController>(tolua_S, 2, "fairygui.GController",&arg0, "fairygui.GComboBox:setSelectionController");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GComboBox_setSelectionController'", nullptr);
			return 0;
		}
		cobj->setSelectionController(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setSelectionController",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setSelectionController'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_setItems(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setItems'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::vector<std::string> strs;
		if (luaval_to_std_vector_string(tolua_S, 2, &strs, "fairygui.GComboBox:setItems")) {
			std::vector<std::string>& ret = cobj->getItems();
			ret.clear();
			ret.assign(strs.begin(), strs.end());
		}
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setItems",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setItems'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_setIcons(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setIcons'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::vector<std::string> strs;
		if (luaval_to_std_vector_string(tolua_S, 2, &strs, "fairygui.GComboBox:setIcons")) {
			std::vector<std::string>& ret = cobj->getIcons();
			ret.clear();
			ret.assign(strs.begin(), strs.end());
		}
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setIcons",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setIcons'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_setValues(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComboBox* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GComboBox*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GComboBox_setValues'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::vector<std::string> strs;
		if (luaval_to_std_vector_string(tolua_S, 2, &strs, "fairygui.GComboBox:setValues")) {
			std::vector<std::string>& ret = cobj->getValues();
			ret.clear();
			ret.assign(strs.begin(), strs.end());
		}
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GComboBox:setValues",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_setValues'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GComboBox_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GComboBox",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GComboBox* ret = fairygui::GComboBox::create();
		object_to_luaval<fairygui::GComboBox>(tolua_S, "fairygui.GComboBox",(fairygui::GComboBox*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GComboBox:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GComboBox_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GComboBox(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GComboBox");
	tolua_cclass(tolua_S,"GComboBox","fairygui.GComboBox","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GComboBox");
	tolua_function(tolua_S,"setValue",lua_fairygui_GComboBox_setValue);
	tolua_function(tolua_S,"setSelectedIndex",lua_fairygui_GComboBox_setSelectedIndex);
	tolua_function(tolua_S,"getItems",lua_fairygui_GComboBox_getItems);
	tolua_function(tolua_S,"getSelectedIndex",lua_fairygui_GComboBox_getSelectedIndex);
	tolua_function(tolua_S,"setTitle",lua_fairygui_GComboBox_setTitle);
	tolua_function(tolua_S,"refresh",lua_fairygui_GComboBox_refresh);
	tolua_function(tolua_S,"getValue",lua_fairygui_GComboBox_getValue);
	tolua_function(tolua_S,"getIcons",lua_fairygui_GComboBox_getIcons);
	tolua_function(tolua_S,"getTitle",lua_fairygui_GComboBox_getTitle);
	tolua_function(tolua_S,"getSelectionController",lua_fairygui_GComboBox_getSelectionController);
	tolua_function(tolua_S,"getValues",lua_fairygui_GComboBox_getValues);
	tolua_function(tolua_S,"setSelectionController",lua_fairygui_GComboBox_setSelectionController);
	tolua_function(tolua_S,"setItems",lua_fairygui_GComboBox_setItems);
	tolua_function(tolua_S,"setIcons",lua_fairygui_GComboBox_setIcons);
	tolua_function(tolua_S,"setValues",lua_fairygui_GComboBox_setValues);
	tolua_function(tolua_S,"create", lua_fairygui_GComboBox_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GComboBox).name();
	g_luaType[typeName] = "fairygui.GComboBox";
	g_typeCast["GComboBox"] = "fairygui.GComboBox";
	return 1;
}

static int lua_fairygui_GProgressBar_setValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_setValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GProgressBar:setValue");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GProgressBar_setValue'", nullptr);
			return 0;
		}
		cobj->setValue(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:setValue",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_setValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_setMax(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_setMax'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GProgressBar:setMax");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GProgressBar_setMax'", nullptr);
			return 0;
		}
		cobj->setMax(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:setMax",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_setMax'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_getValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_getValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getValue();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:getValue",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_getValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_getTitleType(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_getTitleType'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getTitleType();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:getTitleType",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_getTitleType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_tweenValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_tweenValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		double arg0;
		double arg1;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GProgressBar:tweenValue");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GProgressBar:tweenValue");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GProgressBar_tweenValue'", nullptr);
			return 0;
		}
		cobj->tweenValue(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:tweenValue",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_tweenValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_getMax(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_getMax'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getMax();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:getMax",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_getMax'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_setTitleType(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_setTitleType'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::ProgressTitleType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GProgressBar:setTitleType");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GProgressBar_setTitleType'", nullptr);
			return 0;
		}
		cobj->setTitleType(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:setTitleType",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_setTitleType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_setMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_setMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GProgressBar:setMin");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GProgressBar_setMin'", nullptr);
			return 0;
		}
		cobj->setMin(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:setMin",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_setMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_getMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GProgressBar* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GProgressBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GProgressBar_getMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getMin();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GProgressBar:getMin",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_getMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GProgressBar_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GProgressBar",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GProgressBar* ret = fairygui::GProgressBar::create();
		object_to_luaval<fairygui::GProgressBar>(tolua_S, "fairygui.GProgressBar",(fairygui::GProgressBar*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GProgressBar:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GProgressBar_create'.",&tolua_err);
	return 0;
#endif
}

int lua_register_fairygui_GProgressBar(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GProgressBar");
	tolua_cclass(tolua_S,"GProgressBar","fairygui.GProgressBar","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GProgressBar");
	tolua_function(tolua_S,"setTitleType",lua_fairygui_GProgressBar_setTitleType);
	tolua_function(tolua_S,"getTitleType",lua_fairygui_GProgressBar_getTitleType);
	tolua_function(tolua_S,"setMin",lua_fairygui_GProgressBar_setMin);
	tolua_function(tolua_S,"getMin",lua_fairygui_GProgressBar_getMin);
	tolua_function(tolua_S,"getMax",lua_fairygui_GProgressBar_getMax);
	tolua_function(tolua_S,"setMax",lua_fairygui_GProgressBar_setMax);
	tolua_function(tolua_S,"setValue",lua_fairygui_GProgressBar_setValue);
	tolua_function(tolua_S,"getValue",lua_fairygui_GProgressBar_getValue);
	tolua_function(tolua_S,"tweenValue",lua_fairygui_GProgressBar_tweenValue);
	tolua_function(tolua_S,"create", lua_fairygui_GProgressBar_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GProgressBar).name();
	g_luaType[typeName] = "fairygui.GProgressBar";
	g_typeCast["GProgressBar"] = "fairygui.GProgressBar";
	return 1;
}

static int lua_fairygui_GSlider_get_changeOnClick(lua_State* L)
{
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GSlider_get_changeOnClick'", nullptr);
		return 0;
	}
#endif

	lua_pushboolean(L, cobj->changeOnClick);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GSlider_get_changeOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_set_changeOnClick(lua_State* L)
{
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GSlider_set_changeOnClick'", nullptr);
		return 0;
	}
#endif

	cobj->changeOnClick = lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GSlider_set_changeOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_get_canDrag(lua_State* L)
{
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GSlider_get_canDrag'", nullptr);
		return 0;
	}
#endif

	lua_pushboolean(L, cobj->canDrag);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GSlider_get_canDrag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_set_canDrag(lua_State* L)
{
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GSlider_set_canDrag'", nullptr);
		return 0;
	}
#endif

	cobj->canDrag = lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GSlider_set_canDrag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_setValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_setValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GSlider:setValue");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GSlider_setValue'", nullptr);
			return 0;
		}
		cobj->setValue(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:setValue",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_setValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_setMax(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_setMax'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GSlider:setMax");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GSlider_setMax'", nullptr);
			return 0;
		}
		cobj->setMax(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:setMax",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_setMax'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_getValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_getValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getValue();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:getValue",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_getValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_getTitleType(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_getTitleType'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = (int)cobj->getTitleType();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:getTitleType",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_getTitleType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_getMax(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_getMax'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getMax();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:getMax",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_getMax'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_setTitleType(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_setTitleType'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::ProgressTitleType arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GSlider:setTitleType");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GSlider_setTitleType'", nullptr);
			return 0;
		}
		cobj->setTitleType(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:setTitleType",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_setTitleType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_getMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_getMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getMin();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:getMin",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_getMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_setMin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_setMin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GSlider:setMin");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GSlider_setMin'", nullptr);
			return 0;
		}
		cobj->setMin(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:setMin",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_setMin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_getWholeNumbers(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_getWholeNumbers'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->getWholeNumbers();
		tolua_pushboolean(tolua_S,(int)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:getWholeNumbers",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_getWholeNumbers'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_setWholeNumbers(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GSlider* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GSlider*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GSlider_setWholeNumbers'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.GSlider:setWholeNumbers");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GSlider_setWholeNumbers'", nullptr);
			return 0;
		}
		cobj->setWholeNumbers(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GSlider:setWholeNumbers",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_setWholeNumbers'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GSlider_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GSlider",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GSlider* ret = fairygui::GSlider::create();
		object_to_luaval<fairygui::GSlider>(tolua_S, "fairygui.GSlider",(fairygui::GSlider*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GSlider:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GSlider_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GSlider(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GSlider");
	tolua_cclass(tolua_S,"GSlider","fairygui.GSlider","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GSlider");
	// variable
	tolua_variable(tolua_S, "changeOnClick", lua_fairygui_GSlider_get_changeOnClick, lua_fairygui_GSlider_set_changeOnClick);
	tolua_variable(tolua_S, "canDrag", lua_fairygui_GSlider_get_canDrag, lua_fairygui_GSlider_set_canDrag);
	// function
	tolua_function(tolua_S,"getTitleType",lua_fairygui_GSlider_getTitleType);
	tolua_function(tolua_S,"setTitleType",lua_fairygui_GSlider_setTitleType);
	tolua_function(tolua_S,"getMin",lua_fairygui_GSlider_getMin);
	tolua_function(tolua_S,"setMin",lua_fairygui_GSlider_setMin);
	tolua_function(tolua_S,"getMax",lua_fairygui_GSlider_getMax);
	tolua_function(tolua_S,"setMax",lua_fairygui_GSlider_setMax);
	tolua_function(tolua_S,"getValue",lua_fairygui_GSlider_getValue);
	tolua_function(tolua_S,"setValue",lua_fairygui_GSlider_setValue);
	tolua_function(tolua_S,"getWholeNumbers",lua_fairygui_GSlider_getWholeNumbers);
	tolua_function(tolua_S,"setWholeNumbers",lua_fairygui_GSlider_setWholeNumbers);
	tolua_function(tolua_S,"create", lua_fairygui_GSlider_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GSlider).name();
	g_luaType[typeName] = "fairygui.GSlider";
	g_typeCast["GSlider"] = "fairygui.GSlider";
	return 1;
}

static int lua_fairygui_GScrollBar_setScrollPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GScrollBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GScrollBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GScrollBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GScrollBar_setScrollPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::ScrollPane* arg0;
		bool arg1;
		ok &= luaval_to_object<fairygui::ScrollPane>(tolua_S, 2, "fairygui.ScrollPane",&arg0, "fairygui.GScrollBar:setScrollPane");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.GScrollBar:setScrollPane");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GScrollBar_setScrollPane'", nullptr);
			return 0;
		}
		cobj->setScrollPane(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GScrollBar:setScrollPane",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GScrollBar_setScrollPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GScrollBar_setDisplayPerc(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GScrollBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GScrollBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GScrollBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GScrollBar_setDisplayPerc'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GScrollBar:setDisplayPerc");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GScrollBar_setDisplayPerc'", nullptr);
			return 0;
		}
		cobj->setDisplayPerc(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GScrollBar:setDisplayPerc",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GScrollBar_setDisplayPerc'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GScrollBar_getMinSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GScrollBar* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GScrollBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GScrollBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GScrollBar_getMinSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getMinSize();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GScrollBar:getMinSize",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GScrollBar_getMinSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GScrollBar_setScrollPerc(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GScrollBar* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GScrollBar",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GScrollBar*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GScrollBar_setScrollPerc'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GScrollBar:setScrollPerc");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GScrollBar_setScrollPerc'", nullptr);
			return 0;
		}
		cobj->setScrollPerc(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GScrollBar:setScrollPerc",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GScrollBar_setScrollPerc'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GScrollBar_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GScrollBar",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::GScrollBar* ret = fairygui::GScrollBar::create();
		object_to_luaval<fairygui::GScrollBar>(tolua_S, "fairygui.GScrollBar",(fairygui::GScrollBar*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GScrollBar:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GScrollBar_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GScrollBar(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GScrollBar");
	tolua_cclass(tolua_S,"GScrollBar","fairygui.GScrollBar","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GScrollBar");
	tolua_function(tolua_S,"setScrollPane",lua_fairygui_GScrollBar_setScrollPane);
	tolua_function(tolua_S,"setDisplayPerc",lua_fairygui_GScrollBar_setDisplayPerc);
	tolua_function(tolua_S,"getMinSize",lua_fairygui_GScrollBar_getMinSize);
	tolua_function(tolua_S,"setScrollPerc",lua_fairygui_GScrollBar_setScrollPerc);
	tolua_function(tolua_S,"create", lua_fairygui_GScrollBar_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GScrollBar).name();
	g_luaType[typeName] = "fairygui.GScrollBar";
	g_typeCast["GScrollBar"] = "fairygui.GScrollBar";
	return 1;
}

static int lua_fairygui_Window_getCloseButton(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_getCloseButton'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getCloseButton();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:getCloseButton",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_getCloseButton'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_getContentPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_getContentPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getContentPane();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:getContentPane",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_getContentPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_show(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_show'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->show();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:show",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_show'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_isTop(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_isTop'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isTop();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:isTop",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_isTop'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_hideImmediately(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_hideImmediately'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->hideImmediately();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:hideImmediately",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_hideImmediately'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_toggleStatus(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_toggleStatus'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->toggleStatus();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:toggleStatus",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_toggleStatus'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_hide(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_hide'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->hide();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:hide",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_hide'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_getFrame(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_getFrame'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getFrame();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:getFrame",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_getFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_isShowing(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_isShowing'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isShowing();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:isShowing",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_isShowing'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_closeModalWait(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_closeModalWait'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->closeModalWait();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Window:closeModalWait");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_closeModalWait'", nullptr);
			return 0;
		}
		bool ret = cobj->closeModalWait(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.Window:closeModalWait",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_closeModalWait'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_initWindow(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_initWindow'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->initWindow();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:initWindow",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_initWindow'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_setContentArea(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_setContentArea'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.Window:setContentArea");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_setContentArea'", nullptr);
			return 0;
		}
		cobj->setContentArea(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:setContentArea",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_setContentArea'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_setDragArea(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_setDragArea'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.Window:setDragArea");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_setDragArea'", nullptr);
			return 0;
		}
		cobj->setDragArea(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:setDragArea",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_setDragArea'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_setContentPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_setContentPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GComponent* arg0;
		ok &= luaval_to_object<fairygui::GComponent>(tolua_S, 2, "fairygui.GComponent",&arg0, "fairygui.Window:setContentPane");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_setContentPane'", nullptr);
			return 0;
		}
		cobj->setContentPane(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:setContentPane",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_setContentPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_isModal(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_isModal'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isModal();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:isModal",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_isModal'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_isBringToFrontOnClick(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_isBringToFrontOnClick'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isBringToFrontOnClick();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:isBringToFrontOnClick",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_isBringToFrontOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_getContentArea(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_getContentArea'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getContentArea();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:getContentArea",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_getContentArea'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_setBringToFrontOnClick(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_setBringToFrontOnClick'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.Window:setBringToFrontOnClick");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_setBringToFrontOnClick'", nullptr);
			return 0;
		}
		cobj->setBringToFrontOnClick(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:setBringToFrontOnClick",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_setBringToFrontOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_setModal(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_setModal'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.Window:setModal");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_setModal'", nullptr);
			return 0;
		}
		cobj->setModal(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:setModal",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_setModal'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_getModalWaitingPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_getModalWaitingPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getModalWaitingPane();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:getModalWaitingPane",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_getModalWaitingPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_getDragArea(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_getDragArea'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getDragArea();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:getDragArea",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_getDragArea'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_bringToFront(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_bringToFront'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->bringToFront();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:bringToFront",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_bringToFront'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_showModalWait(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_showModalWait'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->showModalWait();
		return 0;
	}
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.Window:showModalWait");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_showModalWait'", nullptr);
			return 0;
		}
		cobj->showModalWait(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.Window:showModalWait",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_showModalWait'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_setCloseButton(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_setCloseButton'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.Window:setCloseButton");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_setCloseButton'", nullptr);
			return 0;
		}
		cobj->setCloseButton(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:setCloseButton",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_setCloseButton'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_addUISource(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::Window* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::Window*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_Window_addUISource'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::IUISource* arg0;
		ok &= luaval_to_object<fairygui::IUISource>(tolua_S, 2, "fairygui.IUISource",&arg0, "fairygui.Window:addUISource");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_Window_addUISource'", nullptr);
			return 0;
		}
		cobj->addUISource(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.Window:addUISource",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_addUISource'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_Window_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.Window",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0){
		fairygui::Window* ret = fairygui::Window::create();
		object_to_luaval<fairygui::Window>(tolua_S, "fairygui.Window",(fairygui::Window*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.Window:create",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_Window_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_Window(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.Window");
	tolua_cclass(tolua_S,"Window","fairygui.Window","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"Window");
	tolua_function(tolua_S,"getCloseButton",lua_fairygui_Window_getCloseButton);
	tolua_function(tolua_S,"getContentPane",lua_fairygui_Window_getContentPane);
	tolua_function(tolua_S,"show",lua_fairygui_Window_show);
	tolua_function(tolua_S,"isTop",lua_fairygui_Window_isTop);
	tolua_function(tolua_S,"hideImmediately",lua_fairygui_Window_hideImmediately);
	tolua_function(tolua_S,"toggleStatus",lua_fairygui_Window_toggleStatus);
	tolua_function(tolua_S,"hide",lua_fairygui_Window_hide);
	tolua_function(tolua_S,"getFrame",lua_fairygui_Window_getFrame);
	tolua_function(tolua_S,"isShowing",lua_fairygui_Window_isShowing);
	tolua_function(tolua_S,"closeModalWait",lua_fairygui_Window_closeModalWait);
	tolua_function(tolua_S,"initWindow",lua_fairygui_Window_initWindow);
	tolua_function(tolua_S,"setContentArea",lua_fairygui_Window_setContentArea);
	tolua_function(tolua_S,"setDragArea",lua_fairygui_Window_setDragArea);
	tolua_function(tolua_S,"setContentPane",lua_fairygui_Window_setContentPane);
	tolua_function(tolua_S,"isModal",lua_fairygui_Window_isModal);
	tolua_function(tolua_S,"isBringToFrontOnClick",lua_fairygui_Window_isBringToFrontOnClick);
	tolua_function(tolua_S,"getContentArea",lua_fairygui_Window_getContentArea);
	tolua_function(tolua_S,"setBringToFrontOnClick",lua_fairygui_Window_setBringToFrontOnClick);
	tolua_function(tolua_S,"setModal",lua_fairygui_Window_setModal);
	tolua_function(tolua_S,"getModalWaitingPane",lua_fairygui_Window_getModalWaitingPane);
	tolua_function(tolua_S,"getDragArea",lua_fairygui_Window_getDragArea);
	tolua_function(tolua_S,"bringToFront",lua_fairygui_Window_bringToFront);
	tolua_function(tolua_S,"showModalWait",lua_fairygui_Window_showModalWait);
	tolua_function(tolua_S,"setCloseButton",lua_fairygui_Window_setCloseButton);
	tolua_function(tolua_S,"addUISource",lua_fairygui_Window_addUISource);
	tolua_function(tolua_S,"create", lua_fairygui_Window_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::Window).name();
	g_luaType[typeName] = "fairygui.Window";
	g_typeCast["Window"] = "fairygui.Window";
	return 1;
}

static int lua_fairygui_GRoot_closeAllWindows(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_closeAllWindows'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->closeAllWindows();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:closeAllWindows",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_closeAllWindows'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_hideTooltips(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_hideTooltips'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->hideTooltips();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:hideTooltips",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_hideTooltips'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_hasAnyPopup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_hasAnyPopup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->hasAnyPopup();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:hasAnyPopup",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_hasAnyPopup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getTopWindow(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getTopWindow'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::Window* ret = cobj->getTopWindow();
		object_to_luaval<fairygui::Window>(tolua_S, "fairygui.Window",(fairygui::Window*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getTopWindow",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getTopWindow'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_hidePopup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_hidePopup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->hidePopup();
		return 0;
	}
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:hidePopup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_hidePopup'", nullptr);
			return 0;
		}
		cobj->hidePopup(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GRoot:hidePopup",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_hidePopup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_closeAllExceptModals(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_closeAllExceptModals'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->closeAllExceptModals();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:closeAllExceptModals",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_closeAllExceptModals'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_showPopup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_showPopup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:showPopup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_showPopup'", nullptr);
			return 0;
		}
		cobj->showPopup(arg0);
		return 0;
	}
	if (argc == 3) {
		fairygui::GObject* arg0;
		fairygui::GObject* arg1;
		fairygui::PopupDirection arg2;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:showPopup");
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 3, "fairygui.GObject",&arg1, "fairygui.GRoot:showPopup");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GRoot:showPopup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_showPopup'", nullptr);
			return 0;
		}
		cobj->showPopup(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GRoot:showPopup",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_showPopup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_showTooltipsWin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_showTooltipsWin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:showTooltipsWin");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_showTooltipsWin'", nullptr);
			return 0;
		}
		cobj->showTooltipsWin(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:showTooltipsWin",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_showTooltipsWin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_closeModalWait(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_closeModalWait'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->closeModalWait();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:closeModalWait",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_closeModalWait'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getInputProcessor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getInputProcessor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::InputProcessor* ret = cobj->getInputProcessor();
		object_to_luaval<fairygui::InputProcessor>(tolua_S, "fairygui.InputProcessor",(fairygui::InputProcessor*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getInputProcessor",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getInputProcessor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getSoundVolumeScale(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getSoundVolumeScale'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		double ret = cobj->getSoundVolumeScale();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getSoundVolumeScale",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getSoundVolumeScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_togglePopup(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj){
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_togglePopup'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GObject* arg0;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:togglePopup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_togglePopup'", nullptr);
			return 0;
		}
		cobj->togglePopup(arg0);
		return 0;
	}
	if (argc == 3) {
		fairygui::GObject* arg0;
		fairygui::GObject* arg1;
		fairygui::PopupDirection arg2;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:togglePopup");
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 3, "fairygui.GObject",&arg1, "fairygui.GRoot:togglePopup");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GRoot:togglePopup");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_togglePopup'", nullptr);
			return 0;
		}
		cobj->togglePopup(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.GRoot:togglePopup",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_togglePopup'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_isSoundEnabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_isSoundEnabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isSoundEnabled();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:isSoundEnabled",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_isSoundEnabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_setSoundVolumeScale(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_setSoundVolumeScale'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		double arg0;
		ok &= luaval_to_number(tolua_S, 2,&arg0, "fairygui.GRoot:setSoundVolumeScale");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_setSoundVolumeScale'", nullptr);
			return 0;
		}
		cobj->setSoundVolumeScale(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:setSoundVolumeScale",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_setSoundVolumeScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getTouchTarget(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getTouchTarget'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getTouchTarget();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getTouchTarget",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getTouchTarget'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_hideWindowImmediately(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_hideWindowImmediately'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::Window* arg0;
		ok &= luaval_to_object<fairygui::Window>(tolua_S, 2, "fairygui.Window",&arg0, "fairygui.GRoot:hideWindowImmediately");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_hideWindowImmediately'", nullptr);
			return 0;
		}
		cobj->hideWindowImmediately(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:hideWindowImmediately",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_hideWindowImmediately'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getModalLayer(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getModalLayer'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GGraph* ret = cobj->getModalLayer();
		object_to_luaval<fairygui::GGraph>(tolua_S, "fairygui.GGraph",(fairygui::GGraph*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getModalLayer",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getModalLayer'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_playSound(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_playSound'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GRoot:playSound");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_playSound'", nullptr);
			return 0;
		}
		cobj->playSound(arg0);
		return 0;
	}
	if (argc == 2) {
		std::string arg0;
		double arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GRoot:playSound");
		ok &= luaval_to_number(tolua_S, 3,&arg1, "fairygui.GRoot:playSound");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_playSound'", nullptr);
			return 0;
		}
		cobj->playSound(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:playSound",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_playSound'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getModalWaitingPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getModalWaitingPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject* ret = cobj->getModalWaitingPane();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getModalWaitingPane",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getModalWaitingPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_setSoundEnabled(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_setSoundEnabled'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.GRoot:setSoundEnabled");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_setSoundEnabled'", nullptr);
			return 0;
		}
		cobj->setSoundEnabled(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:setSoundEnabled",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_setSoundEnabled'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_bringToFront(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_bringToFront'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::Window* arg0;
		ok &= luaval_to_object<fairygui::Window>(tolua_S, 2, "fairygui.Window",&arg0, "fairygui.GRoot:bringToFront");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_bringToFront'", nullptr);
			return 0;
		}
		cobj->bringToFront(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:bringToFront",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_bringToFront'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_hasModalWindow(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_hasModalWindow'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->hasModalWindow();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:hasModalWindow",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_hasModalWindow'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_showModalWait(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_showModalWait'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->showModalWait();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:showModalWait",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_showModalWait'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_isModalWaiting(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_isModalWaiting'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isModalWaiting();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:isModalWaiting",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_isModalWaiting'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getTouchPosition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getTouchPosition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.GRoot:getTouchPosition");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_getTouchPosition'", nullptr);
			return 0;
		}
		cocos2d::Vec2 ret = cobj->getTouchPosition(arg0);
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getTouchPosition",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getTouchPosition'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getPoupPosition(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_getPoupPosition'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 3) {
		fairygui::GObject* arg0;
		fairygui::GObject* arg1;
		fairygui::PopupDirection arg2;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.GRoot:getPoupPosition");
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 3, "fairygui.GObject",&arg1, "fairygui.GRoot:getPoupPosition");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.GRoot:getPoupPosition");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_getPoupPosition'", nullptr);
			return 0;
		}
		cocos2d::Vec2 ret = cobj->getPoupPosition(arg0, arg1, arg2);
		vec2_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:getPoupPosition",argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getPoupPosition'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_showWindow(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_showWindow'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::Window* arg0;
		ok &= luaval_to_object<fairygui::Window>(tolua_S, 2, "fairygui.Window",&arg0, "fairygui.GRoot:showWindow");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_showWindow'", nullptr);
			return 0;
		}
		cobj->showWindow(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:showWindow",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_showWindow'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_showTooltips(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_showTooltips'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.GRoot:showTooltips");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_showTooltips'", nullptr);
			return 0;
		}
		cobj->showTooltips(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:showTooltips",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_showTooltips'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_hideWindow(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GRoot* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GRoot*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GRoot_hideWindow'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::Window* arg0;
		ok &= luaval_to_object<fairygui::Window>(tolua_S, 2, "fairygui.Window",&arg0, "fairygui.GRoot:hideWindow");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_hideWindow'", nullptr);
			return 0;
		}
		cobj->hideWindow(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GRoot:hideWindow",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_hideWindow'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_create(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Scene* arg0;
		ok &= luaval_to_object<cocos2d::Scene>(tolua_S, 2, "cc.Scene",&arg0, "fairygui.GRoot:create");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_create'", nullptr);
			return 0;
		}
		fairygui::GRoot* ret = fairygui::GRoot::create(arg0);
		object_to_luaval<fairygui::GRoot>(tolua_S, "fairygui.GRoot",(fairygui::GRoot*)ret);
		return 1;
	}
	if (argc == 2) {
		cocos2d::Scene* arg0;
		int arg1;
		ok &= luaval_to_object<cocos2d::Scene>(tolua_S, 2, "cc.Scene",&arg0, "fairygui.GRoot:create");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.GRoot:create");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GRoot_create'", nullptr);
			return 0;
		}
		fairygui::GRoot* ret = fairygui::GRoot::create(arg0, arg1);
		object_to_luaval<fairygui::GRoot>(tolua_S, "fairygui.GRoot",(fairygui::GRoot*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GRoot:create",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GRoot_getInstance(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GRoot",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::GRoot* ret = fairygui::GRoot::getInstance();
		object_to_luaval<fairygui::GRoot>(tolua_S, "fairygui.GRoot",(fairygui::GRoot*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GRoot:getInstance",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GRoot_getInstance'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GRoot(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GRoot");
	tolua_cclass(tolua_S,"GRoot","fairygui.GRoot","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GRoot");
	tolua_function(tolua_S,"closeAllWindows",lua_fairygui_GRoot_closeAllWindows);
	tolua_function(tolua_S,"hideTooltips",lua_fairygui_GRoot_hideTooltips);
	tolua_function(tolua_S,"hasAnyPopup",lua_fairygui_GRoot_hasAnyPopup);
	tolua_function(tolua_S,"getTopWindow",lua_fairygui_GRoot_getTopWindow);
	tolua_function(tolua_S,"hidePopup",lua_fairygui_GRoot_hidePopup);
	tolua_function(tolua_S,"closeAllExceptModals",lua_fairygui_GRoot_closeAllExceptModals);
	tolua_function(tolua_S,"showPopup",lua_fairygui_GRoot_showPopup);
	tolua_function(tolua_S,"showTooltipsWin",lua_fairygui_GRoot_showTooltipsWin);
	tolua_function(tolua_S,"closeModalWait",lua_fairygui_GRoot_closeModalWait);
	tolua_function(tolua_S,"getInputProcessor",lua_fairygui_GRoot_getInputProcessor);
	tolua_function(tolua_S,"getSoundVolumeScale",lua_fairygui_GRoot_getSoundVolumeScale);
	tolua_function(tolua_S,"togglePopup",lua_fairygui_GRoot_togglePopup);
	tolua_function(tolua_S,"isSoundEnabled",lua_fairygui_GRoot_isSoundEnabled);
	tolua_function(tolua_S,"setSoundVolumeScale",lua_fairygui_GRoot_setSoundVolumeScale);
	tolua_function(tolua_S,"getTouchTarget",lua_fairygui_GRoot_getTouchTarget);
	tolua_function(tolua_S,"hideWindowImmediately",lua_fairygui_GRoot_hideWindowImmediately);
	tolua_function(tolua_S,"getModalLayer",lua_fairygui_GRoot_getModalLayer);
	tolua_function(tolua_S,"playSound",lua_fairygui_GRoot_playSound);
	tolua_function(tolua_S,"getModalWaitingPane",lua_fairygui_GRoot_getModalWaitingPane);
	tolua_function(tolua_S,"setSoundEnabled",lua_fairygui_GRoot_setSoundEnabled);
	tolua_function(tolua_S,"bringToFront",lua_fairygui_GRoot_bringToFront);
	tolua_function(tolua_S,"hasModalWindow",lua_fairygui_GRoot_hasModalWindow);
	tolua_function(tolua_S,"showModalWait",lua_fairygui_GRoot_showModalWait);
	tolua_function(tolua_S,"isModalWaiting",lua_fairygui_GRoot_isModalWaiting);
	tolua_function(tolua_S,"getTouchPosition",lua_fairygui_GRoot_getTouchPosition);
	tolua_function(tolua_S,"getPoupPosition",lua_fairygui_GRoot_getPoupPosition);
	tolua_function(tolua_S,"showWindow",lua_fairygui_GRoot_showWindow);
	tolua_function(tolua_S,"showTooltips",lua_fairygui_GRoot_showTooltips);
	tolua_function(tolua_S,"hideWindow",lua_fairygui_GRoot_hideWindow);
	tolua_function(tolua_S,"create", lua_fairygui_GRoot_create);
	tolua_function(tolua_S,"getInstance", lua_fairygui_GRoot_getInstance);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GRoot).name();
	g_luaType[typeName] = "fairygui.GRoot";
	g_typeCast["GRoot"] = "fairygui.GRoot";
	return 1;
}

static int lua_fairygui_PopupMenu_setItemGrayed(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_setItemGrayed'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		bool arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:setItemGrayed");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.PopupMenu:setItemGrayed");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_setItemGrayed'", nullptr);
			return 0;
		}
		cobj->setItemGrayed(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:setItemGrayed",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_setItemGrayed'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_getItemName(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_getItemName'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "fairygui.PopupMenu:getItemName");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_getItemName'", nullptr);
			return 0;
		}
		const std::string& ret = cobj->getItemName(arg0);
		lua_pushlstring(tolua_S,ret.c_str(),ret.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:getItemName",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_getItemName'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_clearItems(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_clearItems'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->clearItems();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:clearItems",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_clearItems'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_getList(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_getList'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GList* ret = cobj->getList();
		object_to_luaval<fairygui::GList>(tolua_S, "fairygui.GList",(fairygui::GList*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:getList",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_getList'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_removeItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_removeItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:removeItem");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_removeItem'", nullptr);
			return 0;
		}
		bool ret = cobj->removeItem(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:removeItem",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_removeItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_addItem(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_addItem'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:addItem");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 3, 0));
		if(!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_addItem'", nullptr);
			return 0;
		}
		fairygui::GButton* ret = cobj->addItem(arg0, [=](fairygui::EventContext* context) {
				auto stack = LuaEngine::getInstance()->getLuaStack();
				object_to_luaval<fairygui::EventContext>(stack->getLuaState(), "fairygui.EventContext", context);
				stack->executeFunctionByHandler(handler, 1);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		object_to_luaval<fairygui::GButton>(tolua_S, "fairygui.GButton",(fairygui::GButton*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:addItem",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_addItem'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_addSeperator(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_addSeperator'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->addSeperator();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:addSeperator",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_addSeperator'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_addItemAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_addItemAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 3) {
		std::string arg0;
		int arg1;
		std::function<void (fairygui::EventContext *)> arg2;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:addItemAt");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.PopupMenu:addItemAt");
		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 4, 0));
		if(!ok || 0 == handler) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_addItemAt'", nullptr);
			return 0;
		}
		fairygui::GButton* ret = cobj->addItemAt(arg0, arg1, [=](fairygui::EventContext* context) {
				auto stack = LuaEngine::getInstance()->getLuaStack();
				object_to_luaval<fairygui::EventContext>(stack->getLuaState(), "fairygui.EventContext", context);
				stack->executeFunctionByHandler(handler, 1);
				});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
		object_to_luaval<fairygui::GButton>(tolua_S, "fairygui.GButton",(fairygui::GButton*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:addItemAt",argc, 3);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_addItemAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_setItemText(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_setItemText'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:setItemText");
		ok &= luaval_to_std_string(tolua_S, 3,&arg1, "fairygui.PopupMenu:setItemText");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_setItemText'", nullptr);
			return 0;
		}
		cobj->setItemText(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:setItemText",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_setItemText'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_setItemChecked(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_setItemChecked'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		bool arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:setItemChecked");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.PopupMenu:setItemChecked");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_setItemChecked'", nullptr);
			return 0;
		}
		cobj->setItemChecked(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:setItemChecked",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_setItemChecked'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_show(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_show'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->show();
		return 0;
	}
	if (argc == 2) {
		fairygui::GObject* arg0;
		fairygui::PopupDirection arg1;
		ok &= luaval_to_object<fairygui::GObject>(tolua_S, 2, "fairygui.GObject",&arg0, "fairygui.PopupMenu:show");
		ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "fairygui.PopupMenu:show");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_show'", nullptr);
			return 0;
		}
		cobj->show(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "fairygui.PopupMenu:show",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_show'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_getContentPane(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_getContentPane'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent* ret = cobj->getContentPane();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent",(fairygui::GComponent*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:getContentPane",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_getContentPane'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_getItemCount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_getItemCount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int ret = cobj->getItemCount();
		tolua_pushnumber(tolua_S,(lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:getItemCount",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_getItemCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_setItemCheckable(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_setItemCheckable'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		bool arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:setItemCheckable");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.PopupMenu:setItemCheckable");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_setItemCheckable'", nullptr);
			return 0;
		}
		cobj->setItemCheckable(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:setItemCheckable",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_setItemCheckable'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_isItemChecked(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_isItemChecked'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:isItemChecked");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_isItemChecked'", nullptr);
			return 0;
		}
		bool ret = cobj->isItemChecked(arg0);
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:isItemChecked",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_isItemChecked'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_setItemVisible(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PopupMenu* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PopupMenu*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PopupMenu_setItemVisible'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string arg0;
		bool arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:setItemVisible");
		ok &= luaval_to_boolean(tolua_S, 3,&arg1, "fairygui.PopupMenu:setItemVisible");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_setItemVisible'", nullptr);
			return 0;
		}
		cobj->setItemVisible(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PopupMenu:setItemVisible",argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_setItemVisible'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PopupMenu_create(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.PopupMenu",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0){
		fairygui::PopupMenu* ret = fairygui::PopupMenu::create();
		object_to_luaval<fairygui::PopupMenu>(tolua_S, "fairygui.PopupMenu",(fairygui::PopupMenu*)ret);
		return 1;
	}
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.PopupMenu:create");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_PopupMenu_create'", nullptr);
			return 0;
		}
		fairygui::PopupMenu* ret = fairygui::PopupMenu::create(arg0);
		object_to_luaval<fairygui::PopupMenu>(tolua_S, "fairygui.PopupMenu",(fairygui::PopupMenu*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "fairygui.PopupMenu:create",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PopupMenu_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_PopupMenu(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.PopupMenu");
	tolua_cclass(tolua_S,"PopupMenu","fairygui.PopupMenu","cc.Ref",nullptr);

	tolua_beginmodule(tolua_S,"PopupMenu");
	tolua_function(tolua_S,"setItemGrayed",lua_fairygui_PopupMenu_setItemGrayed);
	tolua_function(tolua_S,"getItemName",lua_fairygui_PopupMenu_getItemName);
	tolua_function(tolua_S,"clearItems",lua_fairygui_PopupMenu_clearItems);
	tolua_function(tolua_S,"getList",lua_fairygui_PopupMenu_getList);
	tolua_function(tolua_S,"removeItem",lua_fairygui_PopupMenu_removeItem);
	tolua_function(tolua_S,"addItem",lua_fairygui_PopupMenu_addItem);
	tolua_function(tolua_S,"addSeperator",lua_fairygui_PopupMenu_addSeperator);
	tolua_function(tolua_S,"addItemAt",lua_fairygui_PopupMenu_addItemAt);
	tolua_function(tolua_S,"setItemText",lua_fairygui_PopupMenu_setItemText);
	tolua_function(tolua_S,"setItemChecked",lua_fairygui_PopupMenu_setItemChecked);
	tolua_function(tolua_S,"show",lua_fairygui_PopupMenu_show);
	tolua_function(tolua_S,"getContentPane",lua_fairygui_PopupMenu_getContentPane);
	tolua_function(tolua_S,"getItemCount",lua_fairygui_PopupMenu_getItemCount);
	tolua_function(tolua_S,"setItemCheckable",lua_fairygui_PopupMenu_setItemCheckable);
	tolua_function(tolua_S,"isItemChecked",lua_fairygui_PopupMenu_isItemChecked);
	tolua_function(tolua_S,"setItemVisible",lua_fairygui_PopupMenu_setItemVisible);
	tolua_function(tolua_S,"create", lua_fairygui_PopupMenu_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::PopupMenu).name();
	g_luaType[typeName] = "fairygui.PopupMenu";
	g_typeCast["PopupMenu"] = "fairygui.PopupMenu";
	return 1;
}

static int lua_fairygui_UIObjectFactory_newObject(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIObjectFactory",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1){
		fairygui::ObjectType arg0;
		ok = luaval_to_int32(tolua_S, 2, (int *)&arg0, "fairygui.UIObjectFactory:newObject");
		if (ok) {
			fairygui::GObject* ret = fairygui::UIObjectFactory::newObject(arg0);
			object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
			return 1;
		}

		fairygui::PackageItem* arg00;
		ok = luaval_to_object<fairygui::PackageItem>(tolua_S, 2, "fairygui.PackageItem",&arg00, "fairygui.UIObjectFactory:newObject");
		if (ok) {
			fairygui::GObject* ret = fairygui::UIObjectFactory::newObject(arg00);
			object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject",(fairygui::GObject*)ret);
			return 1;
		}

		tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIObjectFactory_newObject'", nullptr);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "fairygui.UIObjectFactory:newObject",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIObjectFactory_newObject'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIObjectFactory_setPackageItemExtension(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIObjectFactory",0,&tolua_err) ||
			!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
			!toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err))
	{
		goto tolua_lerror;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		std::string url;
		int handler;
		luaval_to_std_string(tolua_S, 2, &url, "fairygui.UIObjectFactory:setPackageItemExtension");
		handler = toluafix_ref_function(tolua_S, 3, 0);
		// typeid is a ref of static object
		ScriptHandlerMgr::getInstance()->addCustomHandler((void *)&typeid(fairygui::UIObjectFactory), handler);
		fairygui::UIObjectFactory::setPackageItemExtension(url, [=]() -> fairygui::GComponent * {
				LuaEngine::getInstance()->getLuaStack()->pushFunctionByHandler(handler);
				int error = lua_pcall(tolua_S, 0, 1, 0); // 1 pararm, 1 return
				if (error) {
				CCLOG("[LUA ERROR] %s", lua_tostring(tolua_S, -1));
				lua_pop(tolua_S, 1); // remove error message from stack
				return nullptr;
				}
				fairygui::GComponent *com = static_cast<fairygui::GComponent*>(tolua_tousertype(tolua_S, -1, 0));
				lua_pop(tolua_S, 1);
				return com;
				});
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "fairygui.UIObjectFactory:setPackageItemExtension",argc,2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIObjectFactory_setPackageItemExtension'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIObjectFactory_setLoaderExtension(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIObjectFactory",0,&tolua_err) ||
			!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err))
	{
		goto tolua_lerror;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int handler = toluafix_ref_function(tolua_S, 2, 0);
		// typeid is a ref of static object
		ScriptHandlerMgr::getInstance()->addCustomHandler((void *)&typeid(fairygui::UIObjectFactory), handler);
		fairygui::UIObjectFactory::setLoaderExtension([=]() -> fairygui::GLoader * {
				LuaEngine::getInstance()->getLuaStack()->pushFunctionByHandler(handler);
				int error = lua_pcall(tolua_S, 0, 1, 0); // 1 pararm, 1 return
				if (error) {
				CCLOG("[LUA ERROR] %s", lua_tostring(tolua_S, -1));
				lua_pop(tolua_S, 1); // remove error message from stack
				return nullptr;
				}
				fairygui::GLoader *loader = static_cast<fairygui::GLoader*>(tolua_tousertype(tolua_S, -1, 0));
				lua_pop(tolua_S, 1);
				return loader;
				});
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "fairygui.UIObjectFactory:setLoaderExtension",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIObjectFactory_setLoaderExtension'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_UIObjectFactory(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.UIObjectFactory");
	tolua_cclass(tolua_S,"UIObjectFactory","fairygui.UIObjectFactory","",nullptr);

	tolua_beginmodule(tolua_S,"UIObjectFactory");
	tolua_function(tolua_S,"newObject", lua_fairygui_UIObjectFactory_newObject);
	tolua_function(tolua_S,"setPackageItemExtension", lua_fairygui_UIObjectFactory_setPackageItemExtension);
	tolua_function(tolua_S,"setLoaderExtension", lua_fairygui_UIObjectFactory_setLoaderExtension);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::UIObjectFactory).name();
	g_luaType[typeName] = "fairygui.UIObjectFactory";
	g_typeCast["UIObjectFactory"] = "fairygui.UIObjectFactory";
	return 1;
}

static int lua_fairygui_DragDropManager_cancel(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::DragDropManager* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.DragDropManager",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::DragDropManager*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_DragDropManager_cancel'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->cancel();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.DragDropManager:cancel",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_DragDropManager_cancel'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_DragDropManager_isDragging(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::DragDropManager* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.DragDropManager",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::DragDropManager*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_DragDropManager_isDragging'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool ret = cobj->isDragging();
		tolua_pushboolean(tolua_S,(bool)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.DragDropManager:isDragging",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_DragDropManager_isDragging'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_DragDropManager_getAgent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::DragDropManager* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.DragDropManager",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::DragDropManager*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_DragDropManager_getAgent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GLoader* ret = cobj->getAgent();
		object_to_luaval<fairygui::GLoader>(tolua_S, "fairygui.GLoader",(fairygui::GLoader*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.DragDropManager:getAgent",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_DragDropManager_getAgent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_DragDropManager_startDrag(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::DragDropManager* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.DragDropManager",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::DragDropManager*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_DragDropManager_startDrag'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.DragDropManager:startDrag");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_DragDropManager_startDrag'", nullptr);
			return 0;
		}
		cobj->startDrag(arg0);
		return 0;
	}
	if (argc == 2) {
		std::string arg0;
		cocos2d::Value arg1;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.DragDropManager:startDrag");
		ok &= luaval_to_ccvalue(tolua_S, 3, &arg1, "fairygui.DragDropManager:startDrag");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_DragDropManager_startDrag'", nullptr);
			return 0;
		}
		cobj->startDrag(arg0, arg1);
		return 0;
	}
	if (argc == 3) {
		std::string arg0;
		cocos2d::Value arg1;
		int arg2;
		ok &= luaval_to_std_string(tolua_S, 2,&arg0, "fairygui.DragDropManager:startDrag");
		ok &= luaval_to_ccvalue(tolua_S, 3, &arg1, "fairygui.DragDropManager:startDrag");
		ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "fairygui.DragDropManager:startDrag");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_DragDropManager_startDrag'", nullptr);
			return 0;
		}
		cobj->startDrag(arg0, arg1, arg2);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.DragDropManager:startDrag",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_DragDropManager_startDrag'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_DragDropManager_getInstance(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.DragDropManager",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::DragDropManager* ret = fairygui::DragDropManager::getInstance();
		object_to_luaval<fairygui::DragDropManager>(tolua_S, "fairygui.DragDropManager",(fairygui::DragDropManager*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.DragDropManager:getInstance",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_DragDropManager_getInstance'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_DragDropManager_destroyInstance(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.DragDropManager",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::DragDropManager::destroyInstance();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.DragDropManager:destroyInstance",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_DragDropManager_destroyInstance'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_DragDropManager(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.DragDropManager");
	tolua_cclass(tolua_S,"DragDropManager","fairygui.DragDropManager","",nullptr);

	tolua_beginmodule(tolua_S,"DragDropManager");
	tolua_function(tolua_S,"cancel",lua_fairygui_DragDropManager_cancel);
	tolua_function(tolua_S,"isDragging",lua_fairygui_DragDropManager_isDragging);
	tolua_function(tolua_S,"getAgent",lua_fairygui_DragDropManager_getAgent);
	tolua_function(tolua_S,"startDrag",lua_fairygui_DragDropManager_startDrag);
	tolua_function(tolua_S,"getInstance", lua_fairygui_DragDropManager_getInstance);
	tolua_function(tolua_S,"destroyInstance", lua_fairygui_DragDropManager_destroyInstance);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::DragDropManager).name();
	g_luaType[typeName] = "fairygui.DragDropManager";
	g_typeCast["DragDropManager"] = "fairygui.DragDropManager";
	return 1;
}

typedef enum {
	UICONFIG_MUSIC
} UIConfig_HandlerType;

static int lua_fairygui_UIConfig_registerFont(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 2) {
		std::string arg0;
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "fairygui.UIConfig:registerFont");
		ok &= luaval_to_std_string(tolua_S, 3, &arg1, "fairygui.UIConfig:registerFont");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_UIConfig_registerFont'", nullptr);
			return 0;
		}
		fairygui::UIConfig::registerFont(arg0, arg1);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.UIConfig:registerFont", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_UIConfig_registerFont'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultFont(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultFont = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultFont'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_buttonSound(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::buttonSound = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_buttonSound'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_buttonSoundVolumeScale(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::buttonSoundVolumeScale = lua_tonumber(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_buttonSoundVolumeScale'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultScrollStep(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultScrollStep = lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultScrollStep'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultScrollDecelerationRate(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultScrollDecelerationRate = lua_tonumber(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultScrollDecelerationRate'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultScrollTouchEffect(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isboolean(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultScrollTouchEffect = lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultScrollTouchEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultScrollBounceEffect(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isboolean(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultScrollBounceEffect = lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultScrollBounceEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultScrollBarDisplay(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultScrollBarDisplay = (fairygui::ScrollBarDisplayType)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultScrollBarDisplay'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_verticalScrollBar(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::verticalScrollBar = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_verticalScrollBar'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_horizontalScrollBar(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::horizontalScrollBar = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_horizontalScrollBar'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_touchDragSensitivity(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::touchDragSensitivity = lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_touchDragSensitivity'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_clickDragSensitivity(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::clickDragSensitivity = lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_clickDragSensitivity'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_touchScrollSensitivity(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::touchScrollSensitivity = lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_touchScrollSensitivity'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_defaultComboBoxVisibleItemCount(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isnumber(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::defaultComboBoxVisibleItemCount = lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_defaultComboBoxVisibleItemCount'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_globalModalWaiting(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::globalModalWaiting = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_globalModalWaiting'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_modalLayerColor(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_istable(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	{
		Color4F colorf;
		luaval_to_color4f(L, 2, &colorf);
		fairygui::UIConfig::modalLayerColor = colorf;
	}
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_modalLayerColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_tooltipsWin(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::tooltipsWin = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_tooltipsWin'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_bringWindowToFrontOnClick(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isboolean(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::bringWindowToFrontOnClick = lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_bringWindowToFrontOnClick'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_windowModalWaiting(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::windowModalWaiting = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_windowModalWaiting'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_popupMenu(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::popupMenu = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_popupMenu'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_popupMenu_seperator(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::UIConfig::popupMenu_seperator = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_popupMenu_seperator'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_UIConfig_set_onMusicCallback(lua_State* L)
{
	LUA_FUNCTION refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(L,1,"fairygui.UIConfig",0,&tolua_err)) goto tolua_lerror;
#endif

	if lua_isnil(L, 2) {
		fairygui::UIConfig::onMusicCallback = nullptr;
		// cheat, use fairygui::UIConfig::registerFont address for ref object *_*
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)fairygui::UIConfig::registerFont, (ScriptHandlerMgr::HandlerType)UICONFIG_MUSIC);
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		goto tolua_lerror;
#endif

	refid = (toluafix_ref_function(L, 2, 0));
	fairygui::UIConfig::onMusicCallback = [=](const std::string &path) {
		LuaEngine::getInstance()->getLuaStack()->pushFunctionByHandler(refid);
		lua_pushlstring(L, path.c_str(), path.length());
		int error = lua_pcall(L, 1, 0, 0); // 1 pararm, 0 return
		if (error) {
			CCLOG("[LUA ERROR] %s", lua_tostring(L, -1));
			lua_pop(L, 1); // remove error message from stack
		}
	};
	ScriptHandlerMgr::getInstance()->addObjectHandler((void*)fairygui::UIConfig::registerFont, refid, (ScriptHandlerMgr::HandlerType)UICONFIG_MUSIC);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_UIConfig_set_onMusicCallback'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_UIConfig(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.UIConfig");
	tolua_cclass(tolua_S,"UIConfig","fairygui.UIConfig","",nullptr);

	tolua_beginmodule(tolua_S,"UIConfig");
	// variable
	tolua_variable(tolua_S, "defaultFont", nullptr, lua_fairygui_UIConfig_set_defaultFont);
	tolua_variable(tolua_S, "buttonSound", nullptr, lua_fairygui_UIConfig_set_buttonSound);
	tolua_variable(tolua_S, "buttonSoundVolumeScale", nullptr, lua_fairygui_UIConfig_set_buttonSoundVolumeScale);
	tolua_variable(tolua_S, "defaultScrollStep", nullptr, lua_fairygui_UIConfig_set_defaultScrollStep);
	tolua_variable(tolua_S, "defaultScrollDecelerationRate", nullptr, lua_fairygui_UIConfig_set_defaultScrollDecelerationRate);
	tolua_variable(tolua_S, "defaultScrollTouchEffect", nullptr, lua_fairygui_UIConfig_set_defaultScrollTouchEffect);
	tolua_variable(tolua_S, "defaultScrollBounceEffect", nullptr, lua_fairygui_UIConfig_set_defaultScrollBounceEffect);
	tolua_variable(tolua_S, "defaultScrollBarDisplay", nullptr, lua_fairygui_UIConfig_set_defaultScrollBarDisplay);
	tolua_variable(tolua_S, "verticalScrollBar", nullptr, lua_fairygui_UIConfig_set_verticalScrollBar);
	tolua_variable(tolua_S, "horizontalScrollBar", nullptr, lua_fairygui_UIConfig_set_horizontalScrollBar);
	tolua_variable(tolua_S, "touchDragSensitivity", nullptr, lua_fairygui_UIConfig_set_touchDragSensitivity);
	tolua_variable(tolua_S, "clickDragSensitivity", nullptr, lua_fairygui_UIConfig_set_clickDragSensitivity);
	tolua_variable(tolua_S, "touchScrollSensitivity", nullptr, lua_fairygui_UIConfig_set_touchScrollSensitivity);
	tolua_variable(tolua_S, "defaultComboBoxVisibleItemCount", nullptr, lua_fairygui_UIConfig_set_defaultComboBoxVisibleItemCount);
	tolua_variable(tolua_S, "globalModalWaiting", nullptr, lua_fairygui_UIConfig_set_globalModalWaiting);
	tolua_variable(tolua_S, "modalLayerColor", nullptr, lua_fairygui_UIConfig_set_modalLayerColor);
	tolua_variable(tolua_S, "tooltipsWin", nullptr, lua_fairygui_UIConfig_set_tooltipsWin);
	tolua_variable(tolua_S, "bringWindowToFrontOnClick", nullptr, lua_fairygui_UIConfig_set_bringWindowToFrontOnClick);
	tolua_variable(tolua_S, "windowModalWaiting", nullptr, lua_fairygui_UIConfig_set_windowModalWaiting);
	tolua_variable(tolua_S, "popupMenu", nullptr, lua_fairygui_UIConfig_set_popupMenu);
	tolua_variable(tolua_S, "popupMenu_seperator", nullptr, lua_fairygui_UIConfig_set_popupMenu_seperator);
	tolua_variable(tolua_S, "onMusicCallback", nullptr, lua_fairygui_UIConfig_set_onMusicCallback);
	// function
	tolua_function(tolua_S,"registerFont", lua_fairygui_UIConfig_registerFont);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::UIConfig).name();
	g_luaType[typeName] = "fairygui.UIConfig";
	g_typeCast["UIConfig"] = "fairygui.UIConfig";
	return 1;
}

static int lua_fairygui_GLabel_getTitle(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_getTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& rtn = cobj->getTitle();
		lua_pushlstring(tolua_S, rtn.c_str(), rtn.length());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:getTitle",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_getTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_setTitle(lua_State* tolua_S)
{
	bool ok = true;
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_setTitle'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S,2,&arg0, "fairygui.GLabel:setTitle");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLabel_setTitle'", nullptr);
			return 0;
		}
		cobj->setTitle(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:setTitle",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_setTitle'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_getTitleColor(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_getTitleColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Color3B ret = cobj->getTitleColor();
		color3b_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:getTitleColor",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_getTitleColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_setTitleColor(lua_State* tolua_S)
{
	bool ok = true;
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_setTitleColor'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Color3B arg0;
		ok &=luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GLabel:setTitleColor");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLabel_setTitleColor'", nullptr);
			return 0;
		}
		cobj->setTitleColor(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:setTitleColor",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_setTitleColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_getTitleFontSize(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_getTitleFontSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, cobj->getTitleFontSize());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:getTitleFontSize",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_getTitleFontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_setTitleFontSize(lua_State* tolua_S)
{
	bool ok = true;
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_setTitleFontSize'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int arg0;
		ok &=luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GLabel:setTitleFontSize");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GLabel_setTitleFontSize'", nullptr);
			return 0;
		}
		cobj->setTitleFontSize(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:setTitleFontSize",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_setTitleFontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_getTextField(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GLabel* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GLabel*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GLabel_getTextField'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTextField *ret = cobj->getTextField();
		object_to_luaval<fairygui::GTextField>(tolua_S, "fairygui.GTextField",(fairygui::GTextField*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GLabel:getTextField",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_getTextField'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GLabel_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GLabel",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::GLabel *ret = fairygui::GLabel::create();
		object_to_luaval<fairygui::GLabel>(tolua_S, "fairygui.GLabel",(fairygui::GLabel*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.GLabel:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GLabel_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GLabel(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GLabel");
	tolua_cclass(tolua_S,"GLabel","fairygui.GLabel","fairygui.GComponent",nullptr);

	tolua_beginmodule(tolua_S,"GLabel");
	tolua_function(tolua_S,"getTitle",lua_fairygui_GLabel_getTitle);
	tolua_function(tolua_S,"setTitle",lua_fairygui_GLabel_setTitle);
	tolua_function(tolua_S,"getTitleColor",lua_fairygui_GLabel_getTitleColor);
	tolua_function(tolua_S,"setTitleColor",lua_fairygui_GLabel_setTitleColor);
	tolua_function(tolua_S,"getTitleFontSize",lua_fairygui_GLabel_getTitleFontSize);
	tolua_function(tolua_S,"setTitleFontSize",lua_fairygui_GLabel_setTitleFontSize);
	tolua_function(tolua_S,"getTextField",lua_fairygui_GLabel_getTextField);
	tolua_function(tolua_S,"create", lua_fairygui_GLabel_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GLabel).name();
	g_luaType[typeName] = "fairygui.GLabel";
	g_typeCast["GLabel"] = "fairygui.GLabel";
	return 1;
}

static int lua_fairygui_FUIInput_getTextFormat(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUIInput* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.FUIInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUIInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_FUIInput_getTextFormat'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::TextFormat* ret = cobj->getTextFormat();
		object_to_luaval<fairygui::TextFormat>(tolua_S, "fairygui.TextFormat",(fairygui::TextFormat*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUIInput:getTextFormat",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_FUIInput_getTextFormat'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUIInput_applyTextFormat(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUIInput* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.FUIInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUIInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_FUIInput_applyTextFormat'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->applyTextFormat();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUIInput:applyTextFormat",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_FUIInput_applyTextFormat'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUIInput_isSingleLine(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUIInput* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.FUIInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUIInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_FUIInput_isSingleLine'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushboolean(tolua_S, cobj->isSingleLine());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUIInput:isSingleLine",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_FUIInput_isSingleLine'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUIInput_setSingleLine(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUIInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.FUIInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUIInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_FUIInput_setSingleLine'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2,&arg0, "fairygui.FUIInput:setSingleLine");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_FUIInput_setSingleLine'", nullptr);
			return 0;
		}
		cobj->setSingleLine(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUIInput:setSingleLine",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_FUIInput_setSingleLine'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUIInput_isPassword(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUIInput* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.FUIInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUIInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_FUIInput_isPassword'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		lua_pushboolean(tolua_S, cobj->isPassword());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUIInput:isPassword",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_FUIInput_isPassword'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUIInput_setPassword(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUIInput* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.FUIInput",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUIInput*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_FUIInput_setPassword'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.FUIInput:setPassword");
		if (!ok) {
			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_FUIInput_setPassword'", nullptr);
			return 0;
		}
		cobj->setPassword(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUIInput:setPassword",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_FUIInput_setPassword'.",&tolua_err);
	return 0;
#endif
}

// FUIInput is return by GTextInput:displayObject()
static int lua_register_fairygui_FUIInput(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.FUIInput");
	tolua_cclass(tolua_S,"FUIInput","fairygui.FUIInput","ccui.EditBox",nullptr);

	tolua_beginmodule(tolua_S,"FUIInput");
	tolua_function(tolua_S,"getTextFormat",lua_fairygui_FUIInput_getTextFormat);
	tolua_function(tolua_S,"applyTextFormat",lua_fairygui_FUIInput_applyTextFormat);
	tolua_function(tolua_S,"isSingleLine",lua_fairygui_FUIInput_isSingleLine);
	tolua_function(tolua_S,"setSingleLine",lua_fairygui_FUIInput_setSingleLine);
	tolua_function(tolua_S,"isPassword",lua_fairygui_FUIInput_isPassword);
	tolua_function(tolua_S,"setPassword",lua_fairygui_FUIInput_setPassword);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::FUIInput).name();
	g_luaType[typeName] = "fairygui.FUIInput";
	g_typeCast["FUIInput"] = "fairygui.FUIInput";
	return 1;
}

static int lua_fairygui_FUISprite_clearContent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_clearContent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		cobj->clearContent();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:clearContent", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_clearContent'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setScale9Grid(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setScale9Grid'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		cocos2d::Rect arg0;
		ok = luaval_to_rect(tolua_S, 2, &arg0, "fairygui.FUISprite:setScale9Grid");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setScale9Grid'", nullptr);
			return 0;
		}
		cobj->setScale9Grid(&arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setScale9Grid", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setScale9Grid'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setGrayed(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setGrayed'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.FUISprite:setGrayed");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setGrayed'", nullptr);
			return 0;
		}
		cobj->setGrayed(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setGrayed", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setGrayed'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_getFillMethod(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_getFillMethod'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, (lua_Integer)cobj->getFillMethod());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:getFillMethod", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_getFillMethod'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setFillMethod(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setFillMethod'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.GLoader:setFillMethod");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setFillMethod'", nullptr);
			return 0;
		}
		cobj->setFillMethod((fairygui::FillMethod)arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setFillMethod", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setFillMethod'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_getFillOrigin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_getFillOrigin'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		lua_pushinteger(tolua_S, (lua_Integer)cobj->getFillOrigin());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:getFillOrigin", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_getFillOrigin'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setFillOrigin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setFillOrigin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		int arg0;
		ok &= luaval_to_int32(tolua_S, 2, &arg0, "fairygui.FUISprite:setFillMethod");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setFillOrigin'", nullptr);
			return 0;
		}
		cobj->setFillOrigin((fairygui::FillOrigin)arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setFillOrigin", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setFillOrigin'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_isFillClockwise(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_isFillClockwise'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		lua_pushboolean(tolua_S, cobj->isFillClockwise());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:isFillClockwise", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_isFillClockwise'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setFillClockwise(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setFillClockwise'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.FUISprite:setFillClockwise");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setFillClockwise'", nullptr);
			return 0;
		}
		cobj->setFillClockwise(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setFillClockwise", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setFillClockwise'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_getFillAmount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_getFillAmount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		lua_pushnumber(tolua_S, cobj->getFillAmount());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:getFillAmount", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_getFillAmount'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setFillAmount(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setFillAmount'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		float arg0;
		ok &= luaval_to_float(tolua_S, 2, &arg0, "fairygui.FUISprite:setFillAmount");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setFillAmount'", nullptr);
			return 0;
		}
		cobj->setFillAmount(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setFillAmount", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setFillAmount'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_isScaleByTile(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_isScaleByTile'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		lua_pushboolean(tolua_S, cobj->isScaleByTile());
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:isScaleByTile", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_isScaleByTile'.", &tolua_err);
	return 0;
#endif
}

static int lua_fairygui_FUISprite_setScaleByTile(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::FUISprite* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "fairygui.FUISprite", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::FUISprite*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_fairygui_FUISprite_setScaleByTile'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1) {
		bool arg0;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "fairygui.FUISprite:setScaleByTile");
		if (!ok) {
			tolua_error(tolua_S, "invalid arguments in function 'lua_fairygui_FUISprite_setScaleByTile'", nullptr);
			return 0;
		}
		cobj->setScaleByTile(arg0);
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.FUISprite:setScaleByTile", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_fairygui_FUISprite_setScaleByTile'.", &tolua_err);
	return 0;
#endif
}

// FUISprite is return by displayObject():getChildren()
static int lua_register_fairygui_FUISprite(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "fairygui.FUISprite");
	tolua_cclass(tolua_S, "FUISprite", "fairygui.FUISprite", "cc.Sprite", nullptr);

	tolua_beginmodule(tolua_S, "FUISprite");
	tolua_function(tolua_S, "clearConten", lua_fairygui_FUISprite_clearContent);
	tolua_function(tolua_S, "setScale9Grid", lua_fairygui_FUISprite_setScale9Grid);
	tolua_function(tolua_S, "setGrayed", lua_fairygui_FUISprite_setGrayed);
	tolua_function(tolua_S, "getFillMethod", lua_fairygui_FUISprite_getFillMethod);
	tolua_function(tolua_S, "setFillMethod", lua_fairygui_FUISprite_setFillMethod);
	tolua_function(tolua_S, "getFillOrigin", lua_fairygui_FUISprite_getFillOrigin);
	tolua_function(tolua_S, "setFillOrigin", lua_fairygui_FUISprite_setFillOrigin);
	tolua_function(tolua_S, "isFillClockwise", lua_fairygui_FUISprite_isFillClockwise);
	tolua_function(tolua_S, "setFillClockwise", lua_fairygui_FUISprite_setFillClockwise);
	tolua_function(tolua_S, "isScaleByTile", lua_fairygui_FUISprite_isScaleByTile);
	tolua_function(tolua_S, "setScaleByTile", lua_fairygui_FUISprite_setScaleByTile);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::FUISprite).name();
	g_luaType[typeName] = "fairygui.FUISprite";
	g_typeCast["FUISprite"] = "fairygui.FUISprite";
	return 1;
}

// static int lua_fairygui_GBasicTextField_setUnderlineColor(lua_State* tolua_S)
// {
// 	int argc = 0;
// 	fairygui::GBasicTextField* cobj = nullptr;
// 	bool ok = true;

// #if COCOS2D_DEBUG >= 1
// 	tolua_Error tolua_err;
// 	if (!tolua_isusertype(tolua_S,1,"fairygui.GBasicTextField",0,&tolua_err)) goto tolua_lerror;
// #endif
// 	cobj = (fairygui::GBasicTextField*)tolua_tousertype(tolua_S,1,0);
// #if COCOS2D_DEBUG >= 1
// 	if (!cobj) {
// 		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GBasicTextField_setUnderlineColor'", nullptr);
// 		return 0;
// 	}
// #endif

// 	argc = lua_gettop(tolua_S)-1;
// 	if (argc == 1) {
// 		cocos2d::Color3B arg0;
// 		ok &= luaval_to_color3b(tolua_S, 2, &arg0, "fairygui.GBasicTextField:setUnderlineColor");
// 		if (!ok) {
// 			tolua_error(tolua_S,"invalid arguments in function 'lua_fairygui_GBasicTextField_setUnderlineColor'", nullptr);
// 			return 0;
// 		}
// 		cobj->setUnderlineColor(arg0);
// 		return 0;
// 	}
// 	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.GBasicTextField:setUnderlineColor",argc,1);
// 	return 0;

// #if COCOS2D_DEBUG >= 1
// tolua_lerror:
// 	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GBasicTextField_setUnderlineColor'.",&tolua_err);
// 	return 0;
// #endif
// }

// GBasicTextField is extend from GTextField, and UIObjectFactory create this
static int lua_register_fairygui_GBasicTextField(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GBasicTextField");
	tolua_cclass(tolua_S,"GBasicTextField","fairygui.GBasicTextField","fairygui.GTextField",nullptr);

	tolua_beginmodule(tolua_S,"GBasicTextField");
	// tolua_function(tolua_S,"setUnderlineColor",lua_fairygui_GBasicTextField_setUnderlineColor);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GBasicTextField).name();
	g_luaType[typeName] = "fairygui.GBasicTextField";
	g_typeCast["GBasicTextField"] = "fairygui.GBasicTextField";
	return 1;
}

static int lua_fairygui_InputProcessor_getRecentInput(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::InputProcessor* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.InputProcessor",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::InputProcessor*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_InputProcessor_getRecentInput'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::InputEvent *ret = cobj->getRecentInput();
		object_to_luaval<fairygui::InputEvent>(tolua_S, "fairygui.InputEvent", ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.InputProcessor:getRecentInput",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_InputProcessor_getRecentInput'.",&tolua_err);
	return 0;
#endif
}

// InputProcessor is return by GRoot::getInputProcessor()
static int lua_register_fairygui_InputProcessor(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.InputProcessor");
	tolua_cclass(tolua_S,"InputProcessor","fairygui.InputProcessor","",nullptr);

	tolua_beginmodule(tolua_S,"InputProcessor");
	tolua_function(tolua_S,"getRecentInput",lua_fairygui_InputProcessor_getRecentInput);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::InputProcessor).name();
	g_luaType[typeName] = "fairygui.InputProcessor";
	g_typeCast["InputProcessor"] = "fairygui.InputProcessor";
	return 1;
}

static int lua_fairygui_HtmlObject_get_buttonResource(lua_State* L)
{
	lua_pushlstring(L, fairygui::HtmlObject::buttonResource.c_str(), fairygui::HtmlObject::buttonResource.size());
	return 1;
}

static int lua_fairygui_HtmlObject_set_buttonResource(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	if lua_isnil(L, 2) {
		fairygui::HtmlObject::buttonResource = "";
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::HtmlObject::buttonResource = tolua_tostring(L, 2, "");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_HtmlObject_set_buttonResource'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_HtmlObject_get_inputResource(lua_State* L)
{
	lua_pushlstring(L, fairygui::HtmlObject::inputResource.c_str(), fairygui::HtmlObject::inputResource.size());
	return 1;
}

static int lua_fairygui_HtmlObject_set_inputResource(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	if lua_isnil(L, 2) {
		fairygui::HtmlObject::inputResource = "";
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::HtmlObject::inputResource = tolua_tostring(L, 2, "");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_HtmlObject_set_inputResource'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_HtmlObject_get_selectResource(lua_State* L)
{
	lua_pushlstring(L, fairygui::HtmlObject::selectResource.c_str(), fairygui::HtmlObject::selectResource.size());
	return 1;
}

static int lua_fairygui_HtmlObject_set_selectResource(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

	if lua_isnil(L, 2) {
		fairygui::HtmlObject::selectResource = "";
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!tolua_isstring(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::HtmlObject::selectResource = tolua_tostring(L, 2, "");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_HtmlObject_set_selectResource'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_HtmlObject_get_usePool(lua_State* L)
{
	tolua_pushboolean(L, fairygui::HtmlObject::usePool);
	return 1;
}

static int lua_fairygui_HtmlObject_set_usePool(lua_State* L)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isboolean(L, 2, 0, &tolua_err))
		goto tolua_lerror;
#endif

	fairygui::HtmlObject::usePool = tolua_toboolean(L, 2, true);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_HtmlObject_set_usePool'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_HtmlObject_getUI(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::HtmlObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.HtmlObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::HtmlObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_HtmlObject_getUI'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GObject *ret = cobj->getUI();
		object_to_luaval<fairygui::GObject>(tolua_S, "fairygui.GObject", ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.HtmlObject:getUI",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_HtmlObject_getUI'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_HtmlObject_getElementAttrs(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::HtmlObject* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.HtmlObject",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::HtmlObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_HtmlObject_getElementAttrs'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cocos2d::ValueMap &ret = cobj->getElement()->attrs;
		ccvaluemap_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.HtmlObject:getElementAttrs",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_HtmlObject_getElementAttrs'.",&tolua_err);
	return 0;
#endif
}

// fairygui.HtmlObject:clearStaticPools()
static int lua_fairygui_HtmlObject_clearStaticPools(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.HtmlObject",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::HtmlObject::objectPool.clear();
		fairygui::HtmlObject::loaderPool.clear();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fairygui.HtmlObject:clearStaticPools",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_HtmlObject_clearStaticPools'.",&tolua_err);
	return 0;
#endif
}

// HtmlObject only need static public members
static int lua_register_fairygui_HtmlObject(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.HtmlObject");
	tolua_cclass(tolua_S,"HtmlObject","fairygui.HtmlObject","",nullptr);

	tolua_beginmodule(tolua_S,"HtmlObject");
	// variable
	tolua_variable(tolua_S, "buttonResource", lua_fairygui_HtmlObject_get_buttonResource, lua_fairygui_HtmlObject_set_buttonResource);
	tolua_variable(tolua_S, "inputResource", lua_fairygui_HtmlObject_get_inputResource, lua_fairygui_HtmlObject_set_inputResource);
	tolua_variable(tolua_S, "selectResource", lua_fairygui_HtmlObject_get_selectResource, lua_fairygui_HtmlObject_set_selectResource);
	tolua_variable(tolua_S, "usePool", lua_fairygui_HtmlObject_get_usePool, lua_fairygui_HtmlObject_set_usePool);
	// function
	tolua_function(tolua_S, "getUI", lua_fairygui_HtmlObject_getUI);
	tolua_function(tolua_S, "getElementAttrs", lua_fairygui_HtmlObject_getElementAttrs);
	tolua_function(tolua_S, "clearStaticPools", lua_fairygui_HtmlObject_clearStaticPools);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::HtmlObject).name();
	g_luaType[typeName] = "fairygui.HtmlObject";
	g_typeCast["HtmlObject"] = "fairygui.HtmlObject";
	return 1;
}

static int lua_fairygui_PackageItem_get_owner(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_owner'", nullptr);
		return 0;
	}
#endif

	object_to_luaval<fairygui::UIPackage>(L, "fairygui.PackageItem", cobj->owner);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_owner'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_type(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_type'", nullptr);
		return 0;
	}
#endif

	lua_pushinteger(L, (lua_Integer)cobj->type);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_type'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_objectType(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_objectType'", nullptr);
		return 0;
	}
#endif

	lua_pushinteger(L, (lua_Integer)cobj->objectType);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_objectType'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_id(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_id'", nullptr);
		return 0;
	}
#endif

	lua_pushlstring(L, cobj->id.c_str(), cobj->id.size());
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_id'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_name(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_name'", nullptr);
		return 0;
	}
#endif

	lua_pushlstring(L, cobj->name.c_str(), cobj->name.size());
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_name'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_width(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_width'", nullptr);
		return 0;
	}
#endif

	lua_pushinteger(L, (lua_Integer)cobj->width);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_width'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_height(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_height'", nullptr);
		return 0;
	}
#endif

	lua_pushinteger(L, (lua_Integer)cobj->height);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_height'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_file(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_file'", nullptr);
		return 0;
	}
#endif

	lua_pushlstring(L, cobj->file.c_str(), cobj->file.size());
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_file'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_get_spriteFrame(lua_State* L)
{
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(L, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_PackageItem_get_spriteFrame'", nullptr);
		return 0;
	}
#endif

	object_to_luaval<cocos2d::SpriteFrame>(L, "cc.SpriteFrame", cobj->spriteFrame);
	return 1;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_PackageItem_get_spriteFrame'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_load(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PackageItem_load'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->load();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PackageItem:load", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PackageItem_load'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_getBranch(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PackageItem_getBranch'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::PackageItem *ret = cobj->getBranch();
		object_to_luaval<fairygui::PackageItem>(tolua_S, "fairygui.PackageItem",(fairygui::PackageItem*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PackageItem:getBranch", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PackageItem_getBranch'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_PackageItem_getHighResolution(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::PackageItem* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.PackageItem",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::PackageItem*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_PackageItem_getHighResolution'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::PackageItem *ret = cobj->getHighResolution();
		object_to_luaval<fairygui::PackageItem>(tolua_S, "fairygui.PackageItem",(fairygui::PackageItem*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fairygui.PackageItem:getHighResolution", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_PackageItem_getHighResolution'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_PackageItem(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.PackageItem");
	tolua_cclass(tolua_S,"PackageItem","fairygui.PackageItem","",nullptr);

	tolua_beginmodule(tolua_S,"PackageItem");
	// variable
	tolua_variable(tolua_S, "owner", lua_fairygui_PackageItem_get_owner, nullptr);
	tolua_variable(tolua_S, "type", lua_fairygui_PackageItem_get_type, nullptr);
	tolua_variable(tolua_S, "objectType", lua_fairygui_PackageItem_get_objectType, nullptr);
	tolua_variable(tolua_S, "id", lua_fairygui_PackageItem_get_id, nullptr);
	tolua_variable(tolua_S, "name", lua_fairygui_PackageItem_get_name, nullptr);
	tolua_variable(tolua_S, "width", lua_fairygui_PackageItem_get_width, nullptr);
	tolua_variable(tolua_S, "height", lua_fairygui_PackageItem_get_height, nullptr);
	tolua_variable(tolua_S, "file", lua_fairygui_PackageItem_get_file, nullptr);
	tolua_variable(tolua_S, "spriteFrame", lua_fairygui_PackageItem_get_spriteFrame, nullptr);
	// function
	tolua_function(tolua_S, "load", lua_fairygui_PackageItem_load);
	tolua_function(tolua_S, "getBranch", lua_fairygui_PackageItem_getBranch);
	tolua_function(tolua_S, "getHighResolution", lua_fairygui_PackageItem_getHighResolution);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::PackageItem).name();
	g_luaType[typeName] = "fairygui.PackageItem";
	g_typeCast["PackageItem"] = "fairygui.PackageItem";
	return 1;
}

static int lua_fairygui_GTree_set_treeNodeRender(lua_State* L)
{
	fairygui::GTree* cobj = nullptr;
	LUA_FUNCTION refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GTree_set_treeNodeRender'", nullptr);
		return 0;
	}
#endif

	if lua_isnil(L, 2) {
		cobj->treeNodeRender = nullptr;
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)GTREE_NODE_RENDERER);
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		goto tolua_lerror;
#endif

	refid = (toluafix_ref_function(L, 2, 0));
	cobj->treeNodeRender = [=](fairygui::GTreeNode* node, fairygui::GComponent* obj) {
		auto stack = LuaEngine::getInstance()->getLuaStack();
		object_to_luaval<fairygui::GTreeNode>(stack->getLuaState(), "fairygui.GTreeNode", node);
		object_to_luaval<fairygui::GComponent>(stack->getLuaState(), "fairygui.GComponent", obj);
		stack->executeFunctionByHandler(refid, 2);
	};
	ScriptHandlerMgr::getInstance()->addObjectHandler((void*)cobj, refid, (ScriptHandlerMgr::HandlerType)GTREE_NODE_RENDERER);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GTree_set_treeNodeRender'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_set_treeNodeWillExpand(lua_State* L)
{
	fairygui::GTree* cobj = nullptr;
	LUA_FUNCTION refid = -1;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_GTree_set_treeNodeWillExpand'", nullptr);
		return 0;
	}
#endif

	if lua_isnil(L, 2) {
		cobj->treeNodeWillExpand = nullptr;
		ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)cobj, (ScriptHandlerMgr::HandlerType)GTREE_NODE_WILLEXPAND);
		return 0;
	}
#if COCOS2D_DEBUG >= 1
	if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		goto tolua_lerror;
#endif

	refid = (toluafix_ref_function(L, 2, 0));
	cobj->treeNodeWillExpand = [=](fairygui::GTreeNode* node, bool expand) {
		auto stack = LuaEngine::getInstance()->getLuaStack();
		object_to_luaval<fairygui::GTreeNode>(stack->getLuaState(), "fairygui.GTreeNode", node);
		lua_pushboolean(stack->getLuaState(), (int)expand);
		stack->executeFunctionByHandler(refid, 2);
	};
	ScriptHandlerMgr::getInstance()->addObjectHandler((void*)cobj, refid, (ScriptHandlerMgr::HandlerType)GTREE_NODE_WILLEXPAND);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_GTree_set_treeNodeWillExpand'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_getIndent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_getIndent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int rtn = cobj->getIndent();
		lua_pushinteger(tolua_S, rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:getIndent",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_getIndent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_setIndent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_setIndent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cobj->setIndent((int)lua_tointeger(tolua_S, 2));
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:setIndent",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_setIndent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_getClickToExpand(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_getClickToExpand'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int rtn = cobj->getClickToExpand();
		lua_pushinteger(tolua_S, rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:getClickToExpand",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_getClickToExpand'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_setClickToExpand(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_setClickToExpand'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cobj->setClickToExpand((int)lua_tointeger(tolua_S, 2));
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:setClickToExpand",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_setClickToExpand'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_getRootNode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_getRootNode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTreeNode* rtn = cobj->getRootNode();
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:getRootNode",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_getRootNode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_getSelectedNode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_getSelectedNode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTreeNode* rtn = cobj->getSelectedNode();
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:getSelectedNode",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_getSelectedNode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_getSelectedNodes(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_getSelectedNodes'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		std::vector<fairygui::GTreeNode*> rtn;
		cobj->getSelectedNodes(rtn);

		lua_newtable(tolua_S);
		int index = 1;
		for (const fairygui::GTreeNode* value : rtn)
		{
			lua_pushnumber(tolua_S, (lua_Number)index);
			object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)value);
			lua_rawset(tolua_S, -3);
			++index;
		}
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTree:getSelectedNodes",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_getSelectedNodes'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_selectNode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_selectNode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc > 0) {
		fairygui::GTreeNode *arg0;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui::GTreeNode",&arg0,"fairygui.GTree:selectNode");
		if (argc > 1) {
			bool arg1 = (bool)lua_toboolean(tolua_S,3);
			cobj->selectNode(arg0, arg1);
		} else {
			cobj->selectNode(arg0);
		}
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting 1 or 2 \n","fairygui.GTree:selectNode",argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_selectNode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_unselectNode(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_unselectNode'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GTreeNode *arg0;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui::GTreeNode",&arg0,"fairygui.GTree:unselectNode");
		cobj->unselectNode(arg0);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d\n","fairygui.GTree:unselectNode",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_unselectNode'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_expandAll(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_expandAll'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GTreeNode *arg0;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui::GTreeNode",&arg0,"fairygui.GTree:expandAll");
		cobj->expandAll(arg0);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d\n","fairygui.GTree:expandAll",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_expandAll'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_collapseAll(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTree* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTree*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTree_collapseAll'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GTreeNode *arg0;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui::GTreeNode",&arg0,"fairygui.GTree:collapseAll");
		cobj->collapseAll(arg0);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d\n","fairygui.GTree:collapseAll",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_collapseAll'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTree_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GTree",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0) {
		fairygui::GTree *ret = fairygui::GTree::create();
		object_to_luaval<fairygui::GTree>(tolua_S,"fairygui.GTree",(fairygui::GTree*)ret);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d\n ","fairygui.GTree:create",argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTree_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GTree(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GTree");
	tolua_cclass(tolua_S,"GTree","fairygui.GTree","fairygui.GList",nullptr);

	tolua_beginmodule(tolua_S,"GTree");
	// variable
	tolua_variable(tolua_S, "treeNodeRender", nullptr, lua_fairygui_GTree_set_treeNodeRender);
	tolua_variable(tolua_S, "treeNodeWillExpand", nullptr, lua_fairygui_GTree_set_treeNodeWillExpand);
	// function
	tolua_function(tolua_S,"getIndent",lua_fairygui_GTree_getIndent);
	tolua_function(tolua_S,"setIndent",lua_fairygui_GTree_setIndent);
	tolua_function(tolua_S,"getClickToExpand",lua_fairygui_GTree_getClickToExpand);
	tolua_function(tolua_S,"setClickToExpand",lua_fairygui_GTree_setClickToExpand);
	tolua_function(tolua_S,"getRootNode",lua_fairygui_GTree_getRootNode);
	tolua_function(tolua_S,"getSelectedNode",lua_fairygui_GTree_getSelectedNode);
	tolua_function(tolua_S,"getSelectedNodes",lua_fairygui_GTree_getSelectedNodes);
	tolua_function(tolua_S,"selectNode",lua_fairygui_GTree_selectNode);
	tolua_function(tolua_S,"unselectNode",lua_fairygui_GTree_unselectNode);
	tolua_function(tolua_S,"expandAll",lua_fairygui_GTree_expandAll);
	tolua_function(tolua_S,"collapseAll",lua_fairygui_GTree_collapseAll);
	tolua_function(tolua_S,"create", lua_fairygui_GTree_create);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GTree).name();
	g_luaType[typeName] = "fairygui.GTree";
	g_typeCast["GTree"] = "fairygui.GTree";
	return 1;
}

static int lua_fairygui_GTreeNode_create(lua_State* tolua_S)
{
	int argc = 0;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc <= 2) {
		fairygui::GTreeNode *ret = nullptr;
		if (argc > 0) {
			bool isFolder = (bool)lua_toboolean(tolua_S, 2);
			if (argc > 1) {
				std::string resURL;
				luaval_to_std_string(tolua_S, 3, &resURL, "fairygui.GTreeNode:create");
				ret = fairygui::GTreeNode::create(isFolder, resURL);
			} else {
				ret = fairygui::GTreeNode::create(isFolder);
			}
		} else {
			ret = fairygui::GTreeNode::create();
		}
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)ret);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting 0~2\n ","fairygui.GTreeNode:create",argc);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_create'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getParent(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getParent'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTreeNode *node = cobj->getParent();
		object_to_luaval<fairygui::GTreeNode>(tolua_S, "fairygui.GTreeNode", node);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getParent",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getParent'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getTree(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getTree'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTree *tree = cobj->getTree();
		object_to_luaval<fairygui::GTree>(tolua_S, "fairygui.GTree", tree);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getTree",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getTree'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getCell(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getCell'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GComponent *com = cobj->getCell();
		object_to_luaval<fairygui::GComponent>(tolua_S, "fairygui.GComponent", com);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getCell",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getCell'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const cocos2d::Value& data = cobj->getData();
		ccvalue_to_luaval(tolua_S, data);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getData",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_setData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_setData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		cocos2d::Value data;
		luaval_to_ccvalue(tolua_S, 2, &data);
		cobj->setData(data);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:setData",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_setData'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_isExpanded(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_isExpanded'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool rtn = cobj->isExpanded();
		lua_pushboolean(tolua_S, (int)rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:isExpanded",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_isExpanded'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_setExpaned(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_setExpaned'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		bool expand = lua_toboolean(tolua_S, 2);
		cobj->setExpaned(expand);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:setExpaned",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_setExpaned'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_isFolder(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_isFolder'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		bool rtn = cobj->isFolder();
		lua_pushboolean(tolua_S, (int)rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:isFolder",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_isFolder'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getText(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getText'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& rtn = cobj->getText();
		lua_pushlstring(tolua_S, rtn.c_str(), rtn.length());
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getText",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getText'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_setText(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_setText'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string value;
		luaval_to_std_string(tolua_S, 2, &value, "fairygui.GTreeNode:setText");
		cobj->setText(value);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:setText",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_setText'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getIcon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getIcon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		const std::string& rtn = cobj->getIcon();
		lua_pushlstring(tolua_S, rtn.c_str(), rtn.length());
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getIcon",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getIcon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_setIcon(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_setIcon'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		std::string value;
		luaval_to_std_string(tolua_S, 2, &value, "fairygui.GTreeNode:setIcon");
		cobj->setIcon(value);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:setIcon",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_setIcon'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_addChild(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_addChild'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GTreeNode* child;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child,"fairygui.GTreeNode:addChild");
		child = cobj->addChild(child);
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)child);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:addChild",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_addChild'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_addChildAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_addChildAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GTreeNode* child;
		int index;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child,"fairygui.GTreeNode:addChildAt");
		index = (int)lua_tointeger(tolua_S, 3);
		child = cobj->addChildAt(child, index);
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)child);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:addChildAt",argc,2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_addChildAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_removeChild(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_removeChild'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GTreeNode* child;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child,"fairygui.GTreeNode:removeChild");
		cobj->removeChild(child);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:removeChild",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_removeChild'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_removeChildAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_removeChildAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int index = (int)lua_tointeger(tolua_S, 2);
		cobj->removeChildAt(index);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:removeChildAt",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_removeChildAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_removeChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_removeChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		cobj->removeChildren();
		return 0;
	} else if (argc == 2) {
		int beginIndex = (int)lua_tointeger(tolua_S, 2);
		int endIndex = (int)lua_tointeger(tolua_S, 3);
		cobj->removeChildren(beginIndex, endIndex);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting 0 or 2 \n","fairygui.GTreeNode:removeChildren",argc);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_removeChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getChildAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getChildAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int index = (int)lua_tointeger(tolua_S, 2);
		fairygui::GTreeNode *child = cobj->getChildAt(index);
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)child);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getChildAt",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getChildAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getPrevSibling(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getPrevSibling'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTreeNode *node = cobj->getPrevSibling();
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)node);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getPrevSibling",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getPrevSibling'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getNextSibling(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getNextSibling'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		fairygui::GTreeNode *node = cobj->getNextSibling();
		object_to_luaval<fairygui::GTreeNode>(tolua_S,"fairygui.GTreeNode",(fairygui::GTreeNode*)node);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getNextSibling",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getNextSibling'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_getChildIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_getChildIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::GTreeNode* child;
		int index;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child,"fairygui.GTreeNode:getChildIndex");
		index = cobj->getChildIndex(child);
		lua_pushinteger(tolua_S, (lua_Integer)index);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:getChildIndex",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_getChildIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_setChildIndex(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_setChildIndex'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GTreeNode* child;
		int index;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child,"fairygui.GTreeNode:setChildIndex");
		index = (int)lua_tointeger(tolua_S, 3);
		cobj->setChildIndex(child, index);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:setChildIndex",argc,2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_setChildIndex'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_setChildIndexBefore(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_setChildIndexBefore'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GTreeNode* child;
		int index;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child,"fairygui.GTreeNode:setChildIndexBefore");
		index = (int)lua_tointeger(tolua_S, 3);
		index = cobj->setChildIndexBefore(child, index);
		lua_pushinteger(tolua_S, (lua_Integer)index);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:setChildIndexBefore",argc,2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_setChildIndexBefore'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_swapChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_swapChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		fairygui::GTreeNode* child1;
		fairygui::GTreeNode* child2;
		luaval_to_object<fairygui::GTreeNode>(tolua_S,2,"fairygui.GTreeNode",&child1,"fairygui.GTreeNode:swapChildren");
		luaval_to_object<fairygui::GTreeNode>(tolua_S,3,"fairygui.GTreeNode",&child2,"fairygui.GTreeNode:swapChildren");
		cobj->swapChildren(child1, child2);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:swapChildren",argc,2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_swapChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_swapChildrenAt(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_swapChildrenAt'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 2) {
		int index1 = (int)lua_tointeger(tolua_S, 2);
		int index2 = (int)lua_tointeger(tolua_S, 3);
		cobj->swapChildrenAt(index1, index2);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:swapChildrenAt",argc,2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_swapChildrenAt'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_GTreeNode_numChildren(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GTreeNode* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.GTreeNode",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::GTreeNode*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_GTreeNode_numChildren'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) {
		int rtn = cobj->numChildren();
		lua_pushinteger(tolua_S, (lua_Integer)rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.GTreeNode:numChildren",argc,0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_GTreeNode_numChildren'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_GTreeNode(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.GTreeNode");
	tolua_cclass(tolua_S,"GTreeNode","fairygui.GTreeNode","cc.Ref",nullptr);

	tolua_beginmodule(tolua_S,"GTreeNode");
	tolua_function(tolua_S,"create", lua_fairygui_GTreeNode_create);
	tolua_function(tolua_S,"getParent",lua_fairygui_GTreeNode_getParent);
	tolua_function(tolua_S,"getTree",lua_fairygui_GTreeNode_getTree);
	tolua_function(tolua_S,"getCell",lua_fairygui_GTreeNode_getCell);
	tolua_function(tolua_S,"getData",lua_fairygui_GTreeNode_getData);
	tolua_function(tolua_S,"setData",lua_fairygui_GTreeNode_setData);
	tolua_function(tolua_S,"isExpanded",lua_fairygui_GTreeNode_isExpanded);
	tolua_function(tolua_S,"setExpaned",lua_fairygui_GTreeNode_setExpaned);
	tolua_function(tolua_S,"isFolder",lua_fairygui_GTreeNode_isFolder);
	tolua_function(tolua_S,"getText",lua_fairygui_GTreeNode_getText);
	tolua_function(tolua_S,"setText",lua_fairygui_GTreeNode_setText);
	tolua_function(tolua_S,"getIcon",lua_fairygui_GTreeNode_getIcon);
	tolua_function(tolua_S,"setIcon",lua_fairygui_GTreeNode_setIcon);
	tolua_function(tolua_S,"addChild",lua_fairygui_GTreeNode_addChild);
	tolua_function(tolua_S,"addChildAt",lua_fairygui_GTreeNode_addChildAt);
	tolua_function(tolua_S,"removeChild",lua_fairygui_GTreeNode_removeChild);
	tolua_function(tolua_S,"removeChildAt",lua_fairygui_GTreeNode_removeChildAt);
	tolua_function(tolua_S,"removeChildren",lua_fairygui_GTreeNode_removeChildren);
	tolua_function(tolua_S,"getChildAt",lua_fairygui_GTreeNode_getChildAt);
	tolua_function(tolua_S,"getPrevSibling",lua_fairygui_GTreeNode_getPrevSibling);
	tolua_function(tolua_S,"getNextSibling",lua_fairygui_GTreeNode_getNextSibling);
	tolua_function(tolua_S,"getChildIndex",lua_fairygui_GTreeNode_getChildIndex);
	tolua_function(tolua_S,"setChildIndex",lua_fairygui_GTreeNode_setChildIndex);
	tolua_function(tolua_S,"setChildIndexBefore",lua_fairygui_GTreeNode_setChildIndexBefore);
	tolua_function(tolua_S,"swapChildren",lua_fairygui_GTreeNode_swapChildren);
	tolua_function(tolua_S,"swapChildrenAt",lua_fairygui_GTreeNode_swapChildrenAt);
	tolua_function(tolua_S,"numChildren",lua_fairygui_GTreeNode_numChildren);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::GTreeNode).name();
	g_luaType[typeName] = "fairygui.GTreeNode";
	g_typeCast["GTreeNode"] = "fairygui.GTreeNode";
	return 1;
}

static int lua_fairygui_TextFormat_set_face(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_face'", nullptr);
		return 0;
	}
#endif

	cobj->face = lua_tostring(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_face'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_fontSize(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_fontSize'", nullptr);
		return 0;
	}
#endif

	cobj->fontSize = (float)lua_tonumber(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_fontSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_color(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_color'", nullptr);
		return 0;
	}
#endif

	luaval_to_color3b(L, 2, &cobj->color, "fairygui.TextFormat:color");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_color'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_bold(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_bold'", nullptr);
		return 0;
	}
#endif

	cobj->bold = (bool)lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_bold'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_italics(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_italics'", nullptr);
		return 0;
	}
#endif

	cobj->italics = (bool)lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_italics'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_underline(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_underline'", nullptr);
		return 0;
	}
#endif

	cobj->underline = (bool)lua_toboolean(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_underline'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_lineSpacing(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_lineSpacing'", nullptr);
		return 0;
	}
#endif

	cobj->lineSpacing = (int)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_lineSpacing'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_letterSpacing(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_letterSpacing'", nullptr);
		return 0;
	}
#endif

	cobj->letterSpacing = (int)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_letterSpacing'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_align(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_align'", nullptr);
		return 0;
	}
#endif

	cobj->align = (cocos2d::TextHAlignment)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_align'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_verticalAlign(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_verticalAlign'", nullptr);
		return 0;
	}
#endif

	cobj->verticalAlign = (cocos2d::TextVAlignment)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_verticalAlign'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_outlineColor(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_outlineColor'", nullptr);
		return 0;
	}
#endif

	luaval_to_color3b(L, 2, &cobj->outlineColor, "fairygui.TextFormat:outlineColor");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_outlineColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_outlineSize(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_outlineSize'", nullptr);
		return 0;
	}
#endif

	cobj->outlineSize = (int)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_outlineSize'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_shadowColor(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_shadowColor'", nullptr);
		return 0;
	}
#endif

	luaval_to_color3b(L, 2, &cobj->shadowColor, "fairygui.TextFormat:shadowColor");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_shadowColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_shadowOffset(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_shadowOffset'", nullptr);
		return 0;
	}
#endif

	luaval_to_size(L, 2, &cobj->shadowOffset);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_shadowOffset'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_shadowBlurRadius(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_shadowBlurRadius'", nullptr);
		return 0;
	}
#endif

	cobj->shadowBlurRadius = (int)lua_tointeger(L, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_shadowBlurRadius'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_set_glowColor(lua_State* L)
{
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(L,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(L,"invalid 'cobj' in function 'lua_fairygui_TextFormat_set_glowColor'", nullptr);
		return 0;
	}
#endif

	luaval_to_color3b(L, 2, &cobj->glowColor, "fairygui.TextFormat:glowColor");
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(L,"#ferror in function 'lua_fairygui_TextFormat_set_glowColor'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_setFormat(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_TextFormat_setFormat'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		fairygui::TextFormat *format;
		luaval_to_object<fairygui::TextFormat>(tolua_S,2,"fairygui.TextFormat",&format,"fairygui.TextFormat:setFormat");
		cobj->setFormat(*format);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.TextFormat:setFormat",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_TextFormat_setFormat'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_enableEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_TextFormat_enableEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int effectFlag = (int)lua_tointeger(tolua_S, 2);
		cobj->enableEffect(effectFlag);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.TextFormat:enableEffect",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_TextFormat_enableEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_disableEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_TextFormat_disableEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int effectFlag = (int)lua_tointeger(tolua_S, 2);
		cobj->disableEffect(effectFlag);
		return 0;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.TextFormat:disableEffect",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_TextFormat_disableEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_fairygui_TextFormat_hasEffect(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::TextFormat* cobj = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"fairygui.TextFormat",0,&tolua_err)) goto tolua_lerror;
#endif
	cobj = (fairygui::TextFormat*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
	if (!cobj) {
		tolua_error(tolua_S,"invalid 'cobj' in function 'lua_fairygui_TextFormat_hasEffect'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 1) {
		int effectFlag = (int)lua_tointeger(tolua_S, 2);
		bool rtn = cobj->hasEffect(effectFlag);
		lua_pushboolean(tolua_S, (int)rtn);
		return 1;
	}
	luaL_error(tolua_S,"%s has wrong number of arguments: %d, was expecting %d \n","fairygui.TextFormat:hasEffect",argc,1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_fairygui_TextFormat_hasEffect'.",&tolua_err);
	return 0;
#endif
}

static int lua_register_fairygui_TextFormat(lua_State* tolua_S)
{
	tolua_usertype(tolua_S,"fairygui.TextFormat");
	tolua_cclass(tolua_S,"TextFormat","fairygui.TextFormat","",nullptr);

	tolua_beginmodule(tolua_S,"TextFormat");
	// variable
	tolua_variable(tolua_S, "face", nullptr, lua_fairygui_TextFormat_set_face);
	tolua_variable(tolua_S, "fontSize", nullptr, lua_fairygui_TextFormat_set_fontSize);
	tolua_variable(tolua_S, "color", nullptr, lua_fairygui_TextFormat_set_color);
	tolua_variable(tolua_S, "bold", nullptr, lua_fairygui_TextFormat_set_bold);
	tolua_variable(tolua_S, "italics", nullptr, lua_fairygui_TextFormat_set_italics);
	tolua_variable(tolua_S, "underline", nullptr, lua_fairygui_TextFormat_set_underline);
	tolua_variable(tolua_S, "lineSpacing", nullptr, lua_fairygui_TextFormat_set_lineSpacing);
	tolua_variable(tolua_S, "letterSpacing", nullptr, lua_fairygui_TextFormat_set_letterSpacing);
	tolua_variable(tolua_S, "align", nullptr, lua_fairygui_TextFormat_set_align);
	tolua_variable(tolua_S, "verticalAlign", nullptr, lua_fairygui_TextFormat_set_verticalAlign);
	tolua_variable(tolua_S, "outlineColor", nullptr, lua_fairygui_TextFormat_set_outlineColor);
	tolua_variable(tolua_S, "outlineSize", nullptr, lua_fairygui_TextFormat_set_outlineSize);
	tolua_variable(tolua_S, "shadowColor", nullptr, lua_fairygui_TextFormat_set_shadowColor);
	tolua_variable(tolua_S, "shadowOffset", nullptr, lua_fairygui_TextFormat_set_shadowOffset);
	tolua_variable(tolua_S, "shadowBlurRadius", nullptr, lua_fairygui_TextFormat_set_shadowBlurRadius);
	tolua_variable(tolua_S, "glowColor", nullptr, lua_fairygui_TextFormat_set_glowColor);
	// function
	tolua_function(tolua_S,"setFormat",lua_fairygui_TextFormat_setFormat);
	tolua_function(tolua_S,"enableEffect",lua_fairygui_TextFormat_enableEffect);
	tolua_function(tolua_S,"disableEffect",lua_fairygui_TextFormat_disableEffect);
	tolua_function(tolua_S,"hasEffect",lua_fairygui_TextFormat_hasEffect);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(fairygui::TextFormat).name();
	g_luaType[typeName] = "fairygui.TextFormat";
	g_typeCast["TextFormat"] = "fairygui.TextFormat";
	return 1;
}

static int lua_register_fairygui_Constants(lua_State* tolua_S)
{
	tolua_module(tolua_S,"UIEventType", 0);
	tolua_beginmodule(tolua_S,"UIEventType");
	tolua_constant(tolua_S, "Enter", static_cast<int>(fairygui::UIEventType::Enter));
	tolua_constant(tolua_S, "Exit", static_cast<int>(fairygui::UIEventType::Exit));
	tolua_constant(tolua_S, "Changed", static_cast<int>(fairygui::UIEventType::Changed));
	tolua_constant(tolua_S, "Submit", static_cast<int>(fairygui::UIEventType::Submit));
	tolua_constant(tolua_S, "TouchBegin", static_cast<int>(fairygui::UIEventType::TouchBegin));
	tolua_constant(tolua_S, "TouchMove", static_cast<int>(fairygui::UIEventType::TouchMove));
	tolua_constant(tolua_S, "TouchEnd", static_cast<int>(fairygui::UIEventType::TouchEnd));
	tolua_constant(tolua_S, "Click", static_cast<int>(fairygui::UIEventType::Click));
	tolua_constant(tolua_S, "RollOver", static_cast<int>(fairygui::UIEventType::RollOver));
	tolua_constant(tolua_S, "RollOut", static_cast<int>(fairygui::UIEventType::RollOut));
	tolua_constant(tolua_S, "MouseWheel", static_cast<int>(fairygui::UIEventType::MouseWheel));
	tolua_constant(tolua_S, "RightClick", static_cast<int>(fairygui::UIEventType::RightClick));
	tolua_constant(tolua_S, "MiddleClick", static_cast<int>(fairygui::UIEventType::MiddleClick));
	tolua_constant(tolua_S, "PositionChange", static_cast<int>(fairygui::UIEventType::PositionChange));
	tolua_constant(tolua_S, "SizeChange", static_cast<int>(fairygui::UIEventType::SizeChange));
	tolua_constant(tolua_S, "KeyDown", static_cast<int>(fairygui::UIEventType::KeyDown));
	tolua_constant(tolua_S, "KeyUp", static_cast<int>(fairygui::UIEventType::KeyUp));
	tolua_constant(tolua_S, "Scroll", static_cast<int>(fairygui::UIEventType::Scroll));
	tolua_constant(tolua_S, "ScrollEnd", static_cast<int>(fairygui::UIEventType::ScrollEnd));
	tolua_constant(tolua_S, "PullDownRelease", static_cast<int>(fairygui::UIEventType::PullDownRelease));
	tolua_constant(tolua_S, "PullUpRelease", static_cast<int>(fairygui::UIEventType::PullUpRelease));
	tolua_constant(tolua_S, "ClickItem", static_cast<int>(fairygui::UIEventType::ClickItem));
	tolua_constant(tolua_S, "ClickLink", static_cast<int>(fairygui::UIEventType::ClickLink));
	tolua_constant(tolua_S, "ClickMenu", static_cast<int>(fairygui::UIEventType::ClickMenu));
	tolua_constant(tolua_S, "RightClickItem", static_cast<int>(fairygui::UIEventType::RightClickItem));
	tolua_constant(tolua_S, "DragStart", static_cast<int>(fairygui::UIEventType::DragStart));
	tolua_constant(tolua_S, "DragMove", static_cast<int>(fairygui::UIEventType::DragMove));
	tolua_constant(tolua_S, "DragEnd", static_cast<int>(fairygui::UIEventType::DragEnd));
	tolua_constant(tolua_S, "Drop", static_cast<int>(fairygui::UIEventType::Drop));
	tolua_constant(tolua_S, "GearStop", static_cast<int>(fairygui::UIEventType::GearStop));
	tolua_constant(tolua_S, "OnInit", static_cast<int>(fairygui::UIEventType::OnInit));
	tolua_constant(tolua_S, "OnShown", static_cast<int>(fairygui::UIEventType::OnShown));
	tolua_constant(tolua_S, "OnHide", static_cast<int>(fairygui::UIEventType::OnHide));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"PackageItemType", 0);
	tolua_beginmodule(tolua_S,"PackageItemType");
	tolua_constant(tolua_S, "IMAGE", static_cast<int>(fairygui::PackageItemType::IMAGE));
	tolua_constant(tolua_S, "MOVIECLIP", static_cast<int>(fairygui::PackageItemType::MOVIECLIP));
	tolua_constant(tolua_S, "SOUND", static_cast<int>(fairygui::PackageItemType::SOUND));
	tolua_constant(tolua_S, "COMPONENT", static_cast<int>(fairygui::PackageItemType::COMPONENT));
	tolua_constant(tolua_S, "ATLAS", static_cast<int>(fairygui::PackageItemType::ATLAS));
	tolua_constant(tolua_S, "FONT", static_cast<int>(fairygui::PackageItemType::FONT));
	tolua_constant(tolua_S, "SWF", static_cast<int>(fairygui::PackageItemType::SWF));
	tolua_constant(tolua_S, "MISC", static_cast<int>(fairygui::PackageItemType::MISC));
	tolua_constant(tolua_S, "UNKNOWN", static_cast<int>(fairygui::PackageItemType::UNKNOWN));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ObjectType", 0);
	tolua_beginmodule(tolua_S,"ObjectType");
	tolua_constant(tolua_S, "IMAGE", static_cast<int>(fairygui::ObjectType::IMAGE));
	tolua_constant(tolua_S, "MOVIECLIP", static_cast<int>(fairygui::ObjectType::MOVIECLIP));
	tolua_constant(tolua_S, "SWF", static_cast<int>(fairygui::ObjectType::SWF));
	tolua_constant(tolua_S, "GRAPH", static_cast<int>(fairygui::ObjectType::GRAPH));
	tolua_constant(tolua_S, "LOADER", static_cast<int>(fairygui::ObjectType::LOADER));
	tolua_constant(tolua_S, "GROUP", static_cast<int>(fairygui::ObjectType::GROUP));
	tolua_constant(tolua_S, "TEXT", static_cast<int>(fairygui::ObjectType::TEXT));
	tolua_constant(tolua_S, "RICHTEXT", static_cast<int>(fairygui::ObjectType::RICHTEXT));
	tolua_constant(tolua_S, "INPUTTEXT", static_cast<int>(fairygui::ObjectType::INPUTTEXT));
	tolua_constant(tolua_S, "COMPONENT", static_cast<int>(fairygui::ObjectType::COMPONENT));
	tolua_constant(tolua_S, "LIST", static_cast<int>(fairygui::ObjectType::LIST));
	tolua_constant(tolua_S, "LABEL", static_cast<int>(fairygui::ObjectType::LABEL));
	tolua_constant(tolua_S, "BUTTON", static_cast<int>(fairygui::ObjectType::BUTTON));
	tolua_constant(tolua_S, "COMBOBOX", static_cast<int>(fairygui::ObjectType::COMBOBOX));
	tolua_constant(tolua_S, "PROGRESSBAR", static_cast<int>(fairygui::ObjectType::PROGRESSBAR));
	tolua_constant(tolua_S, "SLIDER", static_cast<int>(fairygui::ObjectType::SLIDER));
	tolua_constant(tolua_S, "SCROLLBAR", static_cast<int>(fairygui::ObjectType::SCROLLBAR));
	tolua_constant(tolua_S, "TREE", static_cast<int>(fairygui::ObjectType::TREE));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ButtonMode", 0);
	tolua_beginmodule(tolua_S,"ButtonMode");
	tolua_constant(tolua_S, "COMMON", static_cast<int>(fairygui::ButtonMode::COMMON));
	tolua_constant(tolua_S, "CHECK", static_cast<int>(fairygui::ButtonMode::CHECK));
	tolua_constant(tolua_S, "RADIO", static_cast<int>(fairygui::ButtonMode::RADIO));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ChildrenRenderOrder", 0);
	tolua_beginmodule(tolua_S,"ChildrenRenderOrder");
	tolua_constant(tolua_S, "ASCENT", static_cast<int>(fairygui::ChildrenRenderOrder::ASCENT));
	tolua_constant(tolua_S, "DESCENT", static_cast<int>(fairygui::ChildrenRenderOrder::DESCENT));
	tolua_constant(tolua_S, "ARCH", static_cast<int>(fairygui::ChildrenRenderOrder::ARCH));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"OverflowType", 0);
	tolua_beginmodule(tolua_S,"OverflowType");
	tolua_constant(tolua_S, "VISIBLE", static_cast<int>(fairygui::OverflowType::VISIBLE));
	tolua_constant(tolua_S, "HIDDEN", static_cast<int>(fairygui::OverflowType::HIDDEN));
	tolua_constant(tolua_S, "SCROLL", static_cast<int>(fairygui::OverflowType::SCROLL));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ScrollType", 0);
	tolua_beginmodule(tolua_S,"ScrollType");
	tolua_constant(tolua_S, "HORIZONTAL", static_cast<int>(fairygui::ScrollType::HORIZONTAL));
	tolua_constant(tolua_S, "VERTICAL", static_cast<int>(fairygui::ScrollType::VERTICAL));
	tolua_constant(tolua_S, "BOTH", static_cast<int>(fairygui::ScrollType::BOTH));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ScrollBarDisplayType", 0);
	tolua_beginmodule(tolua_S,"ScrollBarDisplayType");
	tolua_constant(tolua_S, "DEFAULT", static_cast<int>(fairygui::ScrollBarDisplayType::DEFAULT));
	tolua_constant(tolua_S, "VISIBLE", static_cast<int>(fairygui::ScrollBarDisplayType::VISIBLE));
	tolua_constant(tolua_S, "AUTO", static_cast<int>(fairygui::ScrollBarDisplayType::AUTO));
	tolua_constant(tolua_S, "HIDDEN", static_cast<int>(fairygui::ScrollBarDisplayType::HIDDEN));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"LoaderFillType", 0);
	tolua_beginmodule(tolua_S,"LoaderFillType");
	tolua_constant(tolua_S, "NONE", static_cast<int>(fairygui::LoaderFillType::NONE));
	tolua_constant(tolua_S, "SCALE", static_cast<int>(fairygui::LoaderFillType::SCALE));
	tolua_constant(tolua_S, "SCALE_MATCH_HEIGHT", static_cast<int>(fairygui::LoaderFillType::SCALE_MATCH_HEIGHT));
	tolua_constant(tolua_S, "SCALE_MATCH_WIDTH", static_cast<int>(fairygui::LoaderFillType::SCALE_MATCH_WIDTH));
	tolua_constant(tolua_S, "SCALE_FREE", static_cast<int>(fairygui::LoaderFillType::SCALE_FREE));
	tolua_constant(tolua_S, "SCALE_NO_BORDER", static_cast<int>(fairygui::LoaderFillType::SCALE_NO_BORDER));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ProgressTitleType", 0);
	tolua_beginmodule(tolua_S,"ProgressTitleType");
	tolua_constant(tolua_S, "PERCENT", static_cast<int>(fairygui::ProgressTitleType::PERCENT));
	tolua_constant(tolua_S, "VALUE_MAX", static_cast<int>(fairygui::ProgressTitleType::VALUE_MAX));
	tolua_constant(tolua_S, "VALUE", static_cast<int>(fairygui::ProgressTitleType::VALUE));
	tolua_constant(tolua_S, "MAX", static_cast<int>(fairygui::ProgressTitleType::MAX));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ListLayoutType", 0);
	tolua_beginmodule(tolua_S,"ListLayoutType");
	tolua_constant(tolua_S, "SINGLE_COLUMN", static_cast<int>(fairygui::ListLayoutType::SINGLE_COLUMN));
	tolua_constant(tolua_S, "SINGLE_ROW", static_cast<int>(fairygui::ListLayoutType::SINGLE_ROW));
	tolua_constant(tolua_S, "FLOW_HORIZONTAL", static_cast<int>(fairygui::ListLayoutType::FLOW_HORIZONTAL));
	tolua_constant(tolua_S, "FLOW_VERTICAL", static_cast<int>(fairygui::ListLayoutType::FLOW_VERTICAL));
	tolua_constant(tolua_S, "PAGINATION", static_cast<int>(fairygui::ListLayoutType::PAGINATION));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ListSelectionMode", 0);
	tolua_beginmodule(tolua_S,"ListSelectionMode");
	tolua_constant(tolua_S, "SINGLE", static_cast<int>(fairygui::ListSelectionMode::SINGLE));
	tolua_constant(tolua_S, "MULTIPLE", static_cast<int>(fairygui::ListSelectionMode::MULTIPLE));
	tolua_constant(tolua_S, "MULTIPLE_SINGLECLICK", static_cast<int>(fairygui::ListSelectionMode::MULTIPLE_SINGLECLICK));
	tolua_constant(tolua_S, "NONE", static_cast<int>(fairygui::ListSelectionMode::NONE));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"GroupLayoutType", 0);
	tolua_beginmodule(tolua_S,"GroupLayoutType");
	tolua_constant(tolua_S, "NONE", static_cast<int>(fairygui::GroupLayoutType::NONE));
	tolua_constant(tolua_S, "HORIZONTAL", static_cast<int>(fairygui::GroupLayoutType::HORIZONTAL));
	tolua_constant(tolua_S, "VERTICAL", static_cast<int>(fairygui::GroupLayoutType::VERTICAL));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"PopupDirection", 0);
	tolua_beginmodule(tolua_S,"PopupDirection");
	tolua_constant(tolua_S, "AUTO", static_cast<int>(fairygui::PopupDirection::AUTO));
	tolua_constant(tolua_S, "UP", static_cast<int>(fairygui::PopupDirection::UP));
	tolua_constant(tolua_S, "DOWN", static_cast<int>(fairygui::PopupDirection::DOWN));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"AutoSizeType", 0);
	tolua_beginmodule(tolua_S,"AutoSizeType");
	tolua_constant(tolua_S, "NONE", static_cast<int>(fairygui::AutoSizeType::NONE));
	tolua_constant(tolua_S, "BOTH", static_cast<int>(fairygui::AutoSizeType::BOTH));
	tolua_constant(tolua_S, "HEIGHT", static_cast<int>(fairygui::AutoSizeType::HEIGHT));
	tolua_constant(tolua_S, "SHRINK", static_cast<int>(fairygui::AutoSizeType::SHRINK));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"FlipType", 0);
	tolua_beginmodule(tolua_S,"FlipType");
	tolua_constant(tolua_S, "NONE", static_cast<int>(fairygui::FlipType::NONE));
	tolua_constant(tolua_S, "HORIZONTAL", static_cast<int>(fairygui::FlipType::HORIZONTAL));
	tolua_constant(tolua_S, "VERTICAL", static_cast<int>(fairygui::FlipType::VERTICAL));
	tolua_constant(tolua_S, "BOTH", static_cast<int>(fairygui::FlipType::BOTH));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"TransitionActionType", 0);
	tolua_beginmodule(tolua_S,"TransitionActionType");
	tolua_constant(tolua_S, "XY", static_cast<int>(fairygui::TransitionActionType::XY));
	tolua_constant(tolua_S, "Size", static_cast<int>(fairygui::TransitionActionType::Size));
	tolua_constant(tolua_S, "Scale", static_cast<int>(fairygui::TransitionActionType::Scale));
	tolua_constant(tolua_S, "Pivot", static_cast<int>(fairygui::TransitionActionType::Pivot));
	tolua_constant(tolua_S, "Alpha", static_cast<int>(fairygui::TransitionActionType::Alpha));
	tolua_constant(tolua_S, "Rotation", static_cast<int>(fairygui::TransitionActionType::Rotation));
	tolua_constant(tolua_S, "Color", static_cast<int>(fairygui::TransitionActionType::Color));
	tolua_constant(tolua_S, "Animation", static_cast<int>(fairygui::TransitionActionType::Animation));
	tolua_constant(tolua_S, "Visible", static_cast<int>(fairygui::TransitionActionType::Visible));
	tolua_constant(tolua_S, "Sound", static_cast<int>(fairygui::TransitionActionType::Sound));
	tolua_constant(tolua_S, "Transition", static_cast<int>(fairygui::TransitionActionType::Transition));
	tolua_constant(tolua_S, "Shake", static_cast<int>(fairygui::TransitionActionType::Shake));
	tolua_constant(tolua_S, "ColorFilter", static_cast<int>(fairygui::TransitionActionType::ColorFilter));
	tolua_constant(tolua_S, "Skew", static_cast<int>(fairygui::TransitionActionType::Skew));
	tolua_constant(tolua_S, "Text", static_cast<int>(fairygui::TransitionActionType::Text));
	tolua_constant(tolua_S, "Icon", static_cast<int>(fairygui::TransitionActionType::Icon));
	tolua_constant(tolua_S, "Unknown", static_cast<int>(fairygui::TransitionActionType::Unknown));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"FillMethod", 0);
	tolua_beginmodule(tolua_S,"FillMethod");
	tolua_constant(tolua_S, "None", static_cast<int>(fairygui::FillMethod::None));
	tolua_constant(tolua_S, "Horizontal", static_cast<int>(fairygui::FillMethod::Horizontal));
	tolua_constant(tolua_S, "Vertical", static_cast<int>(fairygui::FillMethod::Vertical));
	tolua_constant(tolua_S, "Radial90", static_cast<int>(fairygui::FillMethod::Radial90));
	tolua_constant(tolua_S, "Radial180", static_cast<int>(fairygui::FillMethod::Radial180));
	tolua_constant(tolua_S, "Radial360", static_cast<int>(fairygui::FillMethod::Radial360));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"FillOrigin", 0);
	tolua_beginmodule(tolua_S,"FillOrigin");
	tolua_constant(tolua_S, "Top", static_cast<int>(fairygui::FillOrigin::Top));
	tolua_constant(tolua_S, "Bottom", static_cast<int>(fairygui::FillOrigin::Bottom));
	tolua_constant(tolua_S, "Left", static_cast<int>(fairygui::FillOrigin::Left));
	tolua_constant(tolua_S, "Right", static_cast<int>(fairygui::FillOrigin::Right));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"ObjectPropID", 0);
	tolua_beginmodule(tolua_S,"ObjectPropID");
	tolua_constant(tolua_S, "Text", static_cast<int>(fairygui::ObjectPropID::Text));
	tolua_constant(tolua_S, "Icon", static_cast<int>(fairygui::ObjectPropID::Icon));
	tolua_constant(tolua_S, "Color", static_cast<int>(fairygui::ObjectPropID::Color));
	tolua_constant(tolua_S, "OutlineColor", static_cast<int>(fairygui::ObjectPropID::OutlineColor));
	tolua_constant(tolua_S, "Playing", static_cast<int>(fairygui::ObjectPropID::Playing));
	tolua_constant(tolua_S, "Frame", static_cast<int>(fairygui::ObjectPropID::Frame));
	tolua_constant(tolua_S, "DeltaTime", static_cast<int>(fairygui::ObjectPropID::DeltaTime));
	tolua_constant(tolua_S, "TimeScale", static_cast<int>(fairygui::ObjectPropID::TimeScale));
	tolua_constant(tolua_S, "FontSize", static_cast<int>(fairygui::ObjectPropID::FontSize));
	tolua_constant(tolua_S, "Selected", static_cast<int>(fairygui::ObjectPropID::Selected));
	tolua_endmodule(tolua_S);

	tolua_module(tolua_S,"TextFormat", 0);
	tolua_beginmodule(tolua_S,"TextFormat");
	tolua_constant(tolua_S, "OUTLINE", static_cast<int>(fairygui::TextFormat::OUTLINE));
	tolua_constant(tolua_S, "SHADOW", static_cast<int>(fairygui::TextFormat::SHADOW));
	tolua_constant(tolua_S, "GLOW", static_cast<int>(fairygui::TextFormat::GLOW));
	tolua_endmodule(tolua_S);
	return 1;
}

TOLUA_API int register_fairygui_manual(lua_State* tolua_S)
{
	lua_getglobal(tolua_S, "_G");
	if (lua_istable(tolua_S, -1)) {
		tolua_open(tolua_S);

		tolua_module(tolua_S,"fairygui",0);
		tolua_beginmodule(tolua_S,"fairygui");

		lua_register_fairygui_UIEventDispatcher(tolua_S);
		lua_register_fairygui_GObject(tolua_S);
		lua_register_fairygui_GImage(tolua_S);
		lua_register_fairygui_GComponent(tolua_S);
		lua_register_fairygui_GScrollBar(tolua_S);
		lua_register_fairygui_GList(tolua_S);
		lua_register_fairygui_GComboBox(tolua_S);
		lua_register_fairygui_GButton(tolua_S);
		lua_register_fairygui_GGroup(tolua_S);
		lua_register_fairygui_GMovieClip(tolua_S);
		lua_register_fairygui_GTextField(tolua_S);
		lua_register_fairygui_GRichTextField(tolua_S);
		lua_register_fairygui_GRoot(tolua_S);
		lua_register_fairygui_GGraph(tolua_S);
		lua_register_fairygui_EventContext(tolua_S);
		lua_register_fairygui_UIPackage(tolua_S);
		lua_register_fairygui_Transition(tolua_S);
		lua_register_fairygui_InputEvent(tolua_S);
		lua_register_fairygui_GController(tolua_S);
		lua_register_fairygui_UIObjectFactory(tolua_S);
		lua_register_fairygui_PopupMenu(tolua_S);
		lua_register_fairygui_GSlider(tolua_S);
		lua_register_fairygui_GTextInput(tolua_S);
		lua_register_fairygui_Window(tolua_S);
		lua_register_fairygui_GLoader(tolua_S);
		lua_register_fairygui_GLoader3D(tolua_S);
		lua_register_fairygui_GObjectPool(tolua_S);
		lua_register_fairygui_ScrollPane(tolua_S);
		lua_register_fairygui_GProgressBar(tolua_S);
		lua_register_fairygui_DragDropManager(tolua_S);
		lua_register_fairygui_UIConfig(tolua_S);
		lua_register_fairygui_GLabel(tolua_S);
		lua_register_fairygui_FUIInput(tolua_S);
		lua_register_fairygui_FUISprite(tolua_S);
		lua_register_fairygui_GBasicTextField(tolua_S);
		lua_register_fairygui_InputProcessor(tolua_S);
		lua_register_fairygui_HtmlObject(tolua_S);
		lua_register_fairygui_PackageItem(tolua_S);
		lua_register_fairygui_GTree(tolua_S);
		lua_register_fairygui_GTreeNode(tolua_S);
		lua_register_fairygui_TextFormat(tolua_S);
		lua_register_fairygui_Constants(tolua_S);

		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S, 1);
	return 1;
}
