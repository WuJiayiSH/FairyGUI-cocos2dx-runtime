#include "lua_fairygui.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "FairyGUI.h"
#include "utils/html/HtmlElement.h"
#include "utils/html/HtmlObject.h"
#include "display/FUISprite.h"
#include "scripting/lua-bindings/manual/CCLuaBind.h"

namespace
{
    void margin_to_table(const fairygui::Margin &margin, cocos2d::bind::Table &table)
    {
        table.set("left", margin.left);
        table.set("top", margin.top);
        table.set("right", margin.right);
        table.set("bottom", margin.bottom);
    }

    fairygui::Margin table_to_margin(const cocos2d::bind::Table &table)
    {
        return fairygui::Margin(
            table.get("left", 0.0f),
            table.get("top", 0.0f),
            table.get("right", 0.0f),
            table.get("bottom", 0.0f));
    }

    void fairygui_UIEventDispatcher_dispatchEvent(fairygui::UIEventDispatcher &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            argv.rval(thisv.dispatchEvent(argv.get<int>(0)));
        else if (argc == 2)
            argv.rval(thisv.dispatchEvent(argv.get<int>(0), argv.get<cocos2d::Ref *>(1)));
        else if (argc == 3)
            argv.rval(thisv.dispatchEvent(argv.get<int>(0), argv.get<cocos2d::Ref *>(1), argv.get<cocos2d::Value>(2)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_UIEventDispatcher_bubbleEvent(fairygui::UIEventDispatcher &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            argv.rval(thisv.bubbleEvent(argv.get<int>(0)));
        else if (argc == 2)
            argv.rval(thisv.bubbleEvent(argv.get<int>(0), argv.get<cocos2d::Ref *>(1)));
        else if (argc == 3)
            argv.rval(thisv.bubbleEvent(argv.get<int>(0), argv.get<cocos2d::Ref *>(1), argv.get<cocos2d::Value>(2)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_UIEventDispatcher_addEventListener(fairygui::UIEventDispatcher &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 2)
            thisv.addEventListener(argv.get<int>(0), argv.get<fairygui::EventCallback>(1));
        else if (argc == 3)
            thisv.addEventListener(argv.get<int>(0), argv.get<fairygui::EventCallback>(1), fairygui::EventTag(argv.get<int>(2)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_UIEventDispatcher_removeEventListener(fairygui::UIEventDispatcher &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.removeEventListener(argv.get<int>(0));
        else if (argc == 2)
            thisv.removeEventListener(argv.get<int>(0), fairygui::EventTag(argv.get<int>(1)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_UIEventDispatcher_hasEventListener(fairygui::UIEventDispatcher &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            argv.rval(thisv.hasEventListener(argv.get<int>(0)));
        else if (argc == 2)
            argv.rval(thisv.hasEventListener(argv.get<int>(0), fairygui::EventTag(argv.get<int>(1))));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_startDrag(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.startDrag();
        else if (argc == 1)
            thisv.startDrag(argv.get<int>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_center(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.center();
        else if (argc == 1)
            thisv.center(argv.get<bool>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_addRelation(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 2)
            thisv.addRelation(argv.get<fairygui::GObject *>(0), argv.get<fairygui::RelationType>(1));
        else if (argc == 3)
            thisv.addRelation(argv.get<fairygui::GObject *>(0), argv.get<fairygui::RelationType>(1), argv.get<bool>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_setPivot(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 2)
            thisv.setPivot(argv.get<float>(0), argv.get<float>(1));
        else if (argc == 3)
            thisv.setPivot(argv.get<float>(0), argv.get<float>(1), argv.get<bool>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_setSize(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 2)
            thisv.setSize(argv.get<float>(0), argv.get<float>(1));
        else if (argc == 3)
            thisv.setSize(argv.get<float>(0), argv.get<float>(1), argv.get<bool>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_getData(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval((cocos2d::Ref *)thisv.getData());
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_setData(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setData(argv.get<cocos2d::Ref *>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    // As both cocos2d::Rect and cocos2d::Vec2 are table type, glue function cannot tell which overloading function is used.
    // The workaround is always call the function with a cocos2d::Rect, so it alway return a cocos2d::Rect.
    // Since cocos2d::Rect is convertable to cocos2d::Vec2, as they all own x and y, you can take the result as a cocos2d::Vec2.
    void fairygui_GObject_localToGlobal(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            argv.rval(thisv.localToGlobal(argv.get<cocos2d::Rect>(0)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_globalToLocal(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            argv.rval(thisv.globalToLocal(argv.get<cocos2d::Rect>(0)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GObject_setEnabled(fairygui::GObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
        {
            bool enabled = argv.get<bool>(0);
            thisv.setGrayed(!enabled);
            thisv.setTouchable(enabled);
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GMovieClip_setPlaySettings(fairygui::GMovieClip &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.setPlaySettings();
        else if (argc == 1)
            thisv.setPlaySettings(argv.get<int>(0));
        else if (argc == 2)
            thisv.setPlaySettings(argv.get<int>(0), argv.get<int>(1));
        else if (argc == 3)
            thisv.setPlaySettings(argv.get<int>(0), argv.get<int>(1), argv.get<int>(2));
        else if (argc == 4)
            thisv.setPlaySettings(argv.get<int>(0), argv.get<int>(1), argv.get<int>(2), argv.get<int>(3));
        else if (argc == 5)
            thisv.setPlaySettings(argv.get<int>(0), argv.get<int>(1), argv.get<int>(2), argv.get<int>(3), argv.get<std::function<void()>>(4));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GGroup_setBoundsChangedFlag(fairygui::GGroup &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.setBoundsChangedFlag();
        else if (argc == 1)
            thisv.setBoundsChangedFlag(argv.get<bool>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_Transition_stop(fairygui::Transition &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.stop();
        else if (argc == 2)
            thisv.stop(argv.get<bool>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_Transition_playReverse(fairygui::Transition &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.playReverse();
        else if (argc == 1)
            thisv.playReverse(argv.get<fairygui::Transition::PlayCompleteCallback>(0));
        else if (argc == 2)
            thisv.playReverse(argv.get<int>(0), argv.get<float>(1));
        else if (argc == 3)
            thisv.playReverse(argv.get<int>(0), argv.get<float>(1), argv.get<fairygui::Transition::PlayCompleteCallback>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_Transition_play(fairygui::Transition &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.play();
        else if (argc == 1)
            thisv.play(argv.get<fairygui::Transition::PlayCompleteCallback>(0));
        else if (argc == 2)
            thisv.play(argv.get<int>(0), argv.get<float>(1));
        else if (argc == 3)
            thisv.play(argv.get<int>(0), argv.get<float>(1), argv.get<fairygui::Transition::PlayCompleteCallback>(2));
        else if (argc == 4)
            thisv.play(argv.get<int>(0), argv.get<float>(1), argv.get<float>(2), argv.get<float>(3));
        else if (argc == 5)
            thisv.play(argv.get<int>(0), argv.get<float>(1), argv.get<float>(2), argv.get<float>(3), argv.get<fairygui::Transition::PlayCompleteCallback>(4));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_UIObjectFactory_newObject(std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
        {
            if (argv.is<fairygui::ObjectType>(0))
                argv.rval(fairygui::UIObjectFactory::newObject(argv.get<fairygui::ObjectType>(0)));
            else
                argv.rval(fairygui::UIObjectFactory::newObject(argv.get<fairygui::PackageItem *>(0)));
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollToView(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.scrollToView(argv.get<cocos2d::Rect>(0));
        else if (argc == 2)
            thisv.scrollToView(argv.get<cocos2d::Rect>(0), argv.get<bool>(1));
        else if (argc == 3)
            thisv.scrollToView(argv.get<cocos2d::Rect>(0), argv.get<bool>(1), argv.get<bool>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_setPosX(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setPosX(argv.get<float>(0));
        else if (argc == 2)
            thisv.setPosX(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_setPageY(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setPageY(argv.get<int>(0));
        else if (argc == 2)
            thisv.setPageY(argv.get<int>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollTop(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.scrollTop();
        else if (argc == 1)
            thisv.scrollTop(argv.get<bool>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollBottom(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.scrollBottom();
        else if (argc == 1)
            thisv.scrollBottom(argv.get<bool>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollDown(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.scrollDown();
        else if (argc == 1)
            thisv.scrollDown(argv.get<float>(0));
        else if (argc == 2)
            thisv.scrollDown(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollLeft(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.scrollLeft();
        else if (argc == 1)
            thisv.scrollLeft(argv.get<float>(0));
        else if (argc == 2)
            thisv.scrollLeft(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_setPageX(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setPageX(argv.get<int>(0));
        else if (argc == 2)
            thisv.setPageX(argv.get<int>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollUp(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.scrollUp();
        else if (argc == 1)
            thisv.scrollUp(argv.get<float>(0));
        else if (argc == 2)
            thisv.scrollUp(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_setPercX(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setPercX(argv.get<float>(0));
        else if (argc == 2)
            thisv.setPercX(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_setPercY(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setPercY(argv.get<float>(0));
        else if (argc == 2)
            thisv.setPercY(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_scrollRight(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.scrollRight();
        else if (argc == 1)
            thisv.scrollRight(argv.get<float>(0));
        else if (argc == 2)
            thisv.scrollRight(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_ScrollPane_setPosY(fairygui::ScrollPane &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setPosY(argv.get<float>(0));
        else if (argc == 2)
            thisv.setPosY(argv.get<float>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GComponent_removeChildren(fairygui::GComponent &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.removeChildren();
        else if (argc == 2)
            thisv.removeChildren(argv.get<int>(0), argv.get<int>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GComponent_setMask(fairygui::GComponent &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.setMask(argv.get<cocos2d::Node *>(0));
        else if (argc == 2)
            thisv.setMask(argv.get<cocos2d::Node *>(0), argv.get<bool>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GList_setVirtual(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.setVirtual();
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GList_resizeToFit(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.resizeToFit(argv.get<int>(0));
        else if (argc == 2)
            thisv.resizeToFit(argv.get<int>(0), argv.get<int>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GList_addItemFromPool(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval(thisv.addItemFromPool());
        else if (argc == 1)
            argv.rval(thisv.addItemFromPool(argv.get<std::string>(0)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GList_removeChildrenToPool(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.removeChildrenToPool();
        else if (argc == 2)
            thisv.removeChildrenToPool(argv.get<int>(0), argv.get<int>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GList_getSelection(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
        {
            std::vector<int> selection;
            thisv.getSelection(selection);
			argv.rval(selection);
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    

    void fairygui_GList_scrollToView(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.scrollToView(argv.get<int>(0));
        else if (argc == 2)
            thisv.scrollToView(argv.get<int>(0), argv.get<bool>(1));
        else if (argc == 3)
            thisv.scrollToView(argv.get<int>(0), argv.get<bool>(1), argv.get<bool>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GList_getFromPool(fairygui::GList &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval(thisv.getFromPool());
        else if (argc == 1)
            argv.rval(thisv.getFromPool(argv.get<std::string>(0)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GComboBox_setItems(fairygui::GComboBox &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
        {
            const std::vector<std::string> &arg0 = argv.get<std::vector<std::string>>(0);
            std::vector<std::string> &items = thisv.getItems();
            items.clear();
            items.assign(arg0.begin(), arg0.end());
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GComboBox_setIcons(fairygui::GComboBox &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
        {
            const std::vector<std::string> &arg0 = argv.get<std::vector<std::string>>(0);
            std::vector<std::string> &icons = thisv.getIcons();
            icons.clear();
            icons.assign(arg0.begin(), arg0.end());
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GComboBox_setValues(fairygui::GComboBox &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
        {
            const std::vector<std::string> &arg0 = argv.get<std::vector<std::string>>(0);
            std::vector<std::string> &values = thisv.getValues();
            values.clear();
            values.assign(arg0.begin(), arg0.end());
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_Window_closeModalWait(fairygui::Window &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval(thisv.closeModalWait());
        else if (argc == 1)
            argv.rval(thisv.closeModalWait(argv.get<int>(0)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_Window_showModalWait(fairygui::Window &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.showModalWait();
        else if (argc == 1)
            thisv.showModalWait(argv.get<int>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GRoot_hidePopup(fairygui::GRoot &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.hidePopup();
        else if (argc == 1)
            thisv.hidePopup(argv.get<fairygui::GObject *>(0));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GRoot_showPopup(fairygui::GRoot &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.showPopup(argv.get<fairygui::GObject *>(0));
        else if (argc == 3)
            thisv.showPopup(argv.get<fairygui::GObject *>(0), argv.get<fairygui::GObject *>(1), argv.get<fairygui::PopupDirection>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GRoot_togglePopup(fairygui::GRoot &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.togglePopup(argv.get<fairygui::GObject *>(0));
        else if (argc == 3)
            thisv.togglePopup(argv.get<fairygui::GObject *>(0), argv.get<fairygui::GObject *>(1), argv.get<fairygui::PopupDirection>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GRoot_playSound(fairygui::GRoot &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.playSound(argv.get<std::string>(0));
        else if (argc == 2)
            thisv.playSound(argv.get<std::string>(0), argv.get<float>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GRoot_create(std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            argv.rval(fairygui::GRoot::create(argv.get<cocos2d::Scene *>(0)));
        else if (argc == 2)
            argv.rval(fairygui::GRoot::create(argv.get<cocos2d::Scene *>(0), argv.get<int>(1)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_EventContext_getData(fairygui::EventContext &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval((cocos2d::Ref *)thisv.getData());
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_UIPackage_createObject(std::size_t argc, bind::Argv &argv)
    {
        if (argc == 2)
            argv.rval(fairygui::UIPackage::createObject(argv.get<std::string>(0), argv.get<std::string>(1)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_PopupMenu_show(fairygui::PopupMenu &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.show();
        else if (argc == 2)
            thisv.show(argv.get<fairygui::GObject *>(0), argv.get<fairygui::PopupDirection>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_PopupMenu_create(std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval(fairygui::PopupMenu::create());
        else if (argc == 1)
            argv.rval(fairygui::PopupMenu::create(argv.get<std::string>(0)));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_DragDropManager_startDrag(fairygui::DragDropManager &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
            thisv.startDrag(argv.get<std::string>(0));
        else if (argc == 2)
            thisv.startDrag(argv.get<std::string>(0), argv.get<cocos2d::Value>(1));
        else if (argc == 3)
            thisv.startDrag(argv.get<std::string>(0), argv.get<cocos2d::Value>(1), argv.get<int>(2));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_HtmlObject_getElementAttrs(fairygui::HtmlObject &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            argv.rval(thisv.getElement()->attrs);
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_HtmlObject_clearStaticPools(std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
        {
            fairygui::HtmlObject::objectPool.clear();
            fairygui::HtmlObject::loaderPool.clear();
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GTree_getSelectedNodes(fairygui::GTree &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
        {
            std::vector<fairygui::GTreeNode *> nodes;
            thisv.getSelectedNodes(nodes);
            argv.rval(nodes);
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_GTreeNode_removeChildren(fairygui::GTreeNode &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 0)
            thisv.removeChildren();
        else if (argc == 2)
            thisv.removeChildren(argv.get<int>(0), argv.get<int>(1));
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }

    void fairygui_TextFormat_setFormat(fairygui::TextFormat &thisv, std::size_t argc, bind::Argv &argv)
    {
        if (argc == 1)
        {
            fairygui::TextFormat *format = argv.get<fairygui::TextFormat *>(0);
            thisv.setFormat(*format);
        }
        else
            bind::reportError(CCBIND_INVALID_ARG_CNT);
    }
}

CCBIND_DEFINE_TABLE_TYPE(fairygui::Margin, table_to_margin, margin_to_table);

TOLUA_API int register_fairygui_manual(lua_State *tolua_S)
{
    if (bind::begin<fairygui::UIEventDispatcher>("fairygui.UIEventDispatcher", "cc.Ref"))
    {
        bind::function("isDispatchingEvent", CCBIND_FUNC(fairygui::UIEventDispatcher::isDispatchingEvent));
        bind::function("removeEventListeners", CCBIND_FUNC(fairygui::UIEventDispatcher::removeEventListeners));
        bind::function("dispatchEvent", CCBIND_OVERLOADED_FUNC(fairygui_UIEventDispatcher_dispatchEvent));
        bind::function("bubbleEvent", CCBIND_OVERLOADED_FUNC(fairygui_UIEventDispatcher_bubbleEvent));
        bind::function("addEventListener", CCBIND_OVERLOADED_FUNC(fairygui_UIEventDispatcher_addEventListener));
        bind::function("removeEventListener", CCBIND_OVERLOADED_FUNC(fairygui_UIEventDispatcher_removeEventListener));
        bind::function("hasEventListener", CCBIND_OVERLOADED_FUNC(fairygui_UIEventDispatcher_hasEventListener));
        bind::end();
    }

    if (bind::begin<fairygui::GObject>("fairygui.GObject", "fairygui.UIEventDispatcher"))
    {
        bind::variable("id", CCBIND_GETTER(fairygui::GObject::id), CCBIND_SETTER(fairygui::GObject::id));
        bind::variable("name", CCBIND_GETTER(fairygui::GObject::name), CCBIND_SETTER(fairygui::GObject::name));
        bind::variable("sourceSize", CCBIND_GETTER(fairygui::GObject::sourceSize), CCBIND_SETTER(fairygui::GObject::sourceSize));
        bind::variable("initSize", CCBIND_GETTER(fairygui::GObject::initSize), CCBIND_SETTER(fairygui::GObject::initSize));
        bind::variable("minSize", CCBIND_GETTER(fairygui::GObject::minSize), CCBIND_SETTER(fairygui::GObject::minSize));
        bind::variable("maxSize", CCBIND_GETTER(fairygui::GObject::maxSize), CCBIND_SETTER(fairygui::GObject::maxSize));

        bind::function("getXMin", CCBIND_FUNC(fairygui::GObject::getXMin));
        bind::function("setXMin", CCBIND_FUNC(fairygui::GObject::setXMin));
        bind::function("getYMin", CCBIND_FUNC(fairygui::GObject::getYMin));
        bind::function("setYMin", CCBIND_FUNC(fairygui::GObject::setYMin));
        bind::function("isPivotAsAnchor", CCBIND_FUNC(fairygui::GObject::isPivotAsAnchor));
        bind::function("getProp", CCBIND_FUNC(fairygui::GObject::getProp));
        bind::function("setProp", CCBIND_FUNC(fairygui::GObject::setProp));
        bind::function("startDrag", CCBIND_OVERLOADED_FUNC(fairygui_GObject_startDrag));
        bind::function("getPackageItem", CCBIND_FUNC(fairygui::GObject::getPackageItem));
        bind::function("setGroup", CCBIND_FUNC(fairygui::GObject::setGroup));
        bind::function("isDraggable", CCBIND_FUNC(fairygui::GObject::isDraggable));
        bind::function("relations", CCBIND_FUNC(fairygui::GObject::relations));
        bind::function("isTouchable", CCBIND_FUNC(fairygui::GObject::isTouchable));
        bind::function("setScale", CCBIND_FUNC(fairygui::GObject::setScale));
        bind::function("getWidth", CCBIND_FUNC(fairygui::GObject::getWidth));
        bind::function("getY", CCBIND_FUNC(fairygui::GObject::getY));
        bind::function("displayObject", CCBIND_FUNC(fairygui::GObject::displayObject));
        bind::function("setData", CCBIND_OVERLOADED_FUNC(fairygui_GObject_setData));
        bind::function("getData", CCBIND_OVERLOADED_FUNC(fairygui_GObject_getData));
        bind::function("setCustomData", CCBIND_FUNC(fairygui::GObject::setCustomData));
        bind::function("getCustomData", CCBIND_FUNC(fairygui::GObject::getCustomData));
        bind::function("getRotation", CCBIND_FUNC(fairygui::GObject::getRotation));
        bind::function("stopDrag", CCBIND_FUNC(fairygui::GObject::stopDrag));
        bind::function("getSortingOrder", CCBIND_FUNC(fairygui::GObject::getSortingOrder));
        bind::function("isGrayed", CCBIND_FUNC(fairygui::GObject::isGrayed));
        bind::function("setWidth", CCBIND_FUNC(fairygui::GObject::setWidth));
        bind::function("getScale", CCBIND_FUNC(fairygui::GObject::getScale));
        bind::function("getResourceURL", CCBIND_FUNC(fairygui::GObject::getResourceURL));
        bind::function("isPixelSnapping", CCBIND_FUNC(fairygui::GObject::isPixelSnapping));
        bind::function("setTooltips", CCBIND_FUNC(fairygui::GObject::setTooltips));
        bind::function("localToGlobal", CCBIND_OVERLOADED_FUNC(fairygui_GObject_localToGlobal));
        bind::function("getScaleY", CCBIND_FUNC(fairygui::GObject::getScaleY));
        bind::function("getScaleX", CCBIND_FUNC(fairygui::GObject::getScaleX));
        bind::function("onStage", CCBIND_FUNC(fairygui::GObject::onStage));
        bind::function("getHeight", CCBIND_FUNC(fairygui::GObject::getHeight));
        bind::function("getGear", CCBIND_FUNC(fairygui::GObject::getGear));
        bind::function("globalToLocal", CCBIND_OVERLOADED_FUNC(fairygui_GObject_globalToLocal));
        bind::function("getGroup", CCBIND_FUNC(fairygui::GObject::getGroup));
        bind::function("removeFromParent", CCBIND_FUNC(fairygui::GObject::removeFromParent));
        bind::function("constructFromResource", CCBIND_FUNC(fairygui::GObject::constructFromResource));
        bind::function("setSortingOrder", CCBIND_FUNC(fairygui::GObject::setSortingOrder));
        bind::function("setAlpha", CCBIND_FUNC(fairygui::GObject::setAlpha));
        bind::function("getSize", CCBIND_FUNC(fairygui::GObject::getSize));
        bind::function("getSkewY", CCBIND_FUNC(fairygui::GObject::getSkewY));
        bind::function("setRotation", CCBIND_FUNC(fairygui::GObject::setRotation));
        bind::function("addDisplayLock", CCBIND_FUNC(fairygui::GObject::addDisplayLock));
        bind::function("setPixelSnapping", CCBIND_FUNC(fairygui::GObject::setPixelSnapping));
        bind::function("removeRelation", CCBIND_FUNC(fairygui::GObject::removeRelation));
        bind::function("setDraggable", CCBIND_FUNC(fairygui::GObject::setDraggable));
        bind::function("setIcon", CCBIND_FUNC(fairygui::GObject::setIcon));
        bind::function("setScaleY", CCBIND_FUNC(fairygui::GObject::setScaleY));
        bind::function("setScaleX", CCBIND_FUNC(fairygui::GObject::setScaleX));
        bind::function("getSkewX", CCBIND_FUNC(fairygui::GObject::getSkewX));
        bind::function("releaseDisplayLock", CCBIND_FUNC(fairygui::GObject::releaseDisplayLock));
        bind::function("setDragBounds", CCBIND_FUNC(fairygui::GObject::setDragBounds));
        bind::function("makeFullScreen", CCBIND_FUNC(fairygui::GObject::makeFullScreen));
        bind::function("transformRect", CCBIND_FUNC(fairygui::GObject::transformRect));
        bind::function("setTouchable", CCBIND_FUNC(fairygui::GObject::setTouchable));
        bind::function("getAlpha", CCBIND_FUNC(fairygui::GObject::getAlpha));
        bind::function("setGrayed", CCBIND_FUNC(fairygui::GObject::setGrayed));
        bind::function("getText", CCBIND_FUNC(fairygui::GObject::getText));
        bind::function("setPosition", CCBIND_FUNC(fairygui::GObject::setPosition));
        bind::function("setX", CCBIND_FUNC(fairygui::GObject::setX));
        bind::function("setY", CCBIND_FUNC(fairygui::GObject::setY));
        bind::function("getPosition", CCBIND_FUNC(fairygui::GObject::getPosition));
        bind::function("getParent", CCBIND_FUNC(fairygui::GObject::getParent));
        bind::function("setHeight", CCBIND_FUNC(fairygui::GObject::setHeight));
        bind::function("getX", CCBIND_FUNC(fairygui::GObject::getX));
        bind::function("getRoot", CCBIND_FUNC(fairygui::GObject::getRoot));
        bind::function("treeNode", CCBIND_FUNC(fairygui::GObject::treeNode));
        bind::function("center", CCBIND_OVERLOADED_FUNC(fairygui_GObject_center));
        bind::function("getIcon", CCBIND_FUNC(fairygui::GObject::getIcon));
        bind::function("setVisible", CCBIND_FUNC(fairygui::GObject::setVisible));
        bind::function("setText", CCBIND_FUNC(fairygui::GObject::setText));
        bind::function("isVisible", CCBIND_FUNC(fairygui::GObject::isVisible));
        bind::function("hitTest", CCBIND_FUNC(fairygui::GObject::hitTest));
        bind::function("getDragBounds", CCBIND_FUNC(fairygui::GObject::getDragBounds));
        bind::function("addRelation", CCBIND_OVERLOADED_FUNC(fairygui_GObject_addRelation));
        bind::function("setPivot", CCBIND_OVERLOADED_FUNC(fairygui_GObject_setPivot));
        bind::function("checkGearController", CCBIND_FUNC(fairygui::GObject::checkGearController));
        bind::function("getPivot", CCBIND_FUNC(fairygui::GObject::getPivot));
        bind::function("setSkewX", CCBIND_FUNC(fairygui::GObject::setSkewX));
        bind::function("setSkewY", CCBIND_FUNC(fairygui::GObject::setSkewY));
        bind::function("setSize", CCBIND_OVERLOADED_FUNC(fairygui_GObject_setSize));
        bind::function("getTooltips", CCBIND_FUNC(fairygui::GObject::getTooltips));
        bind::function("create", CCBIND_SFUNC(fairygui::GObject::create));
        bind::function("getDraggingObject", CCBIND_SFUNC(fairygui::GObject::getDraggingObject));
        bind::function("setEnabled", CCBIND_OVERLOADED_FUNC(fairygui_GObject_setEnabled));
        bind::end();
    }

    if (bind::begin<fairygui::GImage>("fairygui.GImage", "fairygui.GObject"))
    {
        bind::function("setColor", CCBIND_FUNC(fairygui::GImage::setColor));
        bind::function("getColor", CCBIND_FUNC(fairygui::GImage::getColor));
        bind::function("getFlip", CCBIND_FUNC(fairygui::GImage::getFlip));
        bind::function("setFlip", CCBIND_FUNC(fairygui::GImage::setFlip));
        bind::function("create", CCBIND_SFUNC(fairygui::GImage::create));
        bind::end();
    }

    if (bind::begin<fairygui::GComponent>("fairygui.GComponent", "fairygui.GObject"))
    {
        bind::function("addChild", CCBIND_FUNC(fairygui::GComponent::addChild));
        bind::function("getHitArea", CCBIND_FUNC(fairygui::GComponent::getHitArea));
        bind::function("applyAllControllers", CCBIND_FUNC(fairygui::GComponent::applyAllControllers));
        bind::function("getOpaque", CCBIND_FUNC(fairygui::GComponent::getOpaque));
        bind::function("setChildrenRenderOrder", CCBIND_FUNC(fairygui::GComponent::setChildrenRenderOrder));
        bind::function("setOpaque", CCBIND_FUNC(fairygui::GComponent::setOpaque));
        bind::function("getViewWidth", CCBIND_FUNC(fairygui::GComponent::getViewWidth));
        bind::function("getTransitions", CCBIND_FUNC(fairygui::GComponent::getTransitions));
        bind::function("getViewHeight", CCBIND_FUNC(fairygui::GComponent::getViewHeight));
        bind::function("ensureBoundsCorrect", CCBIND_FUNC(fairygui::GComponent::ensureBoundsCorrect));
        bind::function("isAncestorOf", CCBIND_FUNC(fairygui::GComponent::isAncestorOf));
        bind::function("getChildren", CCBIND_FUNC(fairygui::GComponent::getChildren));
        bind::function("getMask", CCBIND_FUNC(fairygui::GComponent::getMask));
        bind::function("adjustRadioGroupDepth", CCBIND_FUNC(fairygui::GComponent::adjustRadioGroupDepth));
        bind::function("getController", CCBIND_FUNC(fairygui::GComponent::getController));
        bind::function("setApexIndex", CCBIND_FUNC(fairygui::GComponent::setApexIndex));
        bind::function("getTransition", CCBIND_FUNC(fairygui::GComponent::getTransition));
        bind::function("getFirstChildInView", CCBIND_FUNC(fairygui::GComponent::getFirstChildInView));
        bind::function("isChildInView", CCBIND_FUNC(fairygui::GComponent::isChildInView));
        bind::function("setBoundsChangedFlag", CCBIND_FUNC(fairygui::GComponent::setBoundsChangedFlag));
        bind::function("addChildAt", CCBIND_FUNC(fairygui::GComponent::addChildAt));
        bind::function("removeChildren", CCBIND_OVERLOADED_FUNC(fairygui_GComponent_removeChildren));
        bind::function("removeChildAt", CCBIND_FUNC(fairygui::GComponent::removeChildAt));
        bind::function("getChild", CCBIND_FUNC(fairygui::GComponent::getChild));
        bind::function("addController", CCBIND_FUNC(fairygui::GComponent::addController));
        bind::function("childSortingOrderChanged", CCBIND_FUNC(fairygui::GComponent::childSortingOrderChanged));
        bind::function("applyController", CCBIND_FUNC(fairygui::GComponent::applyController));
        bind::function("getControllers", CCBIND_FUNC(fairygui::GComponent::getControllers));
        bind::function("setMask", CCBIND_OVERLOADED_FUNC(fairygui_GComponent_setMask));
        bind::function("setChildIndex", CCBIND_FUNC(fairygui::GComponent::setChildIndex));
        bind::function("setViewWidth", CCBIND_FUNC(fairygui::GComponent::setViewWidth));
        bind::function("getChildById", CCBIND_FUNC(fairygui::GComponent::getChildById));
        bind::function("getChildByPath", CCBIND_FUNC(fairygui::GComponent::getChildByPath));
        bind::function("getChildIndex", CCBIND_FUNC(fairygui::GComponent::getChildIndex));
        bind::function("getMargin", CCBIND_FUNC(fairygui::GComponent::getMargin));
        bind::function("getControllerAt", CCBIND_FUNC(fairygui::GComponent::getControllerAt));
        bind::function("getChildAt", CCBIND_FUNC(fairygui::GComponent::getChildAt));
        bind::function("getApexIndex", CCBIND_FUNC(fairygui::GComponent::getApexIndex));
        bind::function("setMargin", CCBIND_FUNC(fairygui::GComponent::setMargin));
        bind::function("getScrollPane", CCBIND_FUNC(fairygui::GComponent::getScrollPane));
        bind::function("getTransitionAt", CCBIND_FUNC(fairygui::GComponent::getTransitionAt));
        bind::function("getChildInGroup", CCBIND_FUNC(fairygui::GComponent::getChildInGroup));
        bind::function("removeController", CCBIND_FUNC(fairygui::GComponent::removeController));
        bind::function("getSnappingPosition", CCBIND_FUNC(fairygui::GComponent::getSnappingPosition));
        bind::function("setChildIndexBefore", CCBIND_FUNC(fairygui::GComponent::setChildIndexBefore));
        bind::function("setHitArea", CCBIND_FUNC(fairygui::GComponent::setHitArea));
        bind::function("swapChildrenAt", CCBIND_FUNC(fairygui::GComponent::swapChildrenAt));
        bind::function("numChildren", CCBIND_FUNC(fairygui::GComponent::numChildren));
        bind::function("removeChild", CCBIND_FUNC(fairygui::GComponent::removeChild));
        bind::function("swapChildren", CCBIND_FUNC(fairygui::GComponent::swapChildren));
        bind::function("childStateChanged", CCBIND_FUNC(fairygui::GComponent::childStateChanged));
        bind::function("getChildrenRenderOrder", CCBIND_FUNC(fairygui::GComponent::getChildrenRenderOrder));
        bind::function("setViewHeight", CCBIND_FUNC(fairygui::GComponent::setViewHeight));
        bind::function("create", CCBIND_SFUNC(fairygui::GComponent::create));
        bind::end();
    }

    if (bind::begin<fairygui::GScrollBar>("fairygui.GScrollBar", "fairygui.GComponent"))
    {
        bind::function("setScrollPane", CCBIND_FUNC(fairygui::GScrollBar::setScrollPane));
        bind::function("setDisplayPerc", CCBIND_FUNC(fairygui::GScrollBar::setDisplayPerc));
        bind::function("getMinSize", CCBIND_FUNC(fairygui::GScrollBar::getMinSize));
        bind::function("setScrollPerc", CCBIND_FUNC(fairygui::GScrollBar::setScrollPerc));
        bind::function("create", CCBIND_SFUNC(fairygui::GScrollBar::create));
        bind::end();
    }

    if (bind::begin<fairygui::GList>("fairygui.GList", "fairygui.GComponent"))
    {
        // NOTE: A breaking API change: cannot read itemRenderer and itemProvider. Previous implementation has loophole as it doesn't expose cpp function to
        // underlying script, it only works when the function is assigned on script side.
        bind::variable("itemRenderer", nullptr, CCBIND_SETTER(fairygui::GList::itemRenderer));
        bind::variable("itemProvider", nullptr, CCBIND_SETTER(fairygui::GList::itemProvider));
        bind::variable("scrollItemToViewOnClick", CCBIND_GETTER(fairygui::GList::scrollItemToViewOnClick), CCBIND_SETTER(fairygui::GList::scrollItemToViewOnClick));
        bind::variable("foldInvisibleItems", CCBIND_GETTER(fairygui::GList::foldInvisibleItems), CCBIND_SETTER(fairygui::GList::foldInvisibleItems));

        bind::function("selectAll", CCBIND_FUNC(fairygui::GList::selectAll));
        bind::function("getColumnGap", CCBIND_FUNC(fairygui::GList::getColumnGap));
        bind::function("getLineCount", CCBIND_FUNC(fairygui::GList::getLineCount));
        bind::function("setVirtualAndLoop", CCBIND_FUNC(fairygui::GList::setVirtualAndLoop));
        bind::function("setLayout", CCBIND_FUNC(fairygui::GList::setLayout));
        bind::function("refreshVirtualList", CCBIND_FUNC(fairygui::GList::refreshVirtualList));
        bind::function("removeSelection", CCBIND_FUNC(fairygui::GList::removeSelection));
        bind::function("setColumnGap", CCBIND_FUNC(fairygui::GList::setColumnGap));
        bind::function("getColumnCount", CCBIND_FUNC(fairygui::GList::getColumnCount));
        bind::function("setVirtual", CCBIND_OVERLOADED_FUNC(fairygui_GList_setVirtual));
        bind::function("setNumItems", CCBIND_FUNC(fairygui::GList::setNumItems));
        bind::function("setVerticalAlign", CCBIND_FUNC(fairygui::GList::setVerticalAlign));
        bind::function("resizeToFit", CCBIND_OVERLOADED_FUNC(fairygui_GList_resizeToFit));
        bind::function("setSelectionController", CCBIND_FUNC(fairygui::GList::setSelectionController));
        bind::function("returnToPool", CCBIND_FUNC(fairygui::GList::returnToPool));
        bind::function("clearSelection", CCBIND_FUNC(fairygui::GList::clearSelection));
        bind::function("getNumItems", CCBIND_FUNC(fairygui::GList::getNumItems));
        bind::function("setColumnCount", CCBIND_FUNC(fairygui::GList::setColumnCount));
        bind::function("removeChildToPoolAt", CCBIND_FUNC(fairygui::GList::removeChildToPoolAt));
        bind::function("addItemFromPool", CCBIND_OVERLOADED_FUNC(fairygui_GList_addItemFromPool));
        bind::function("selectReverse", CCBIND_FUNC(fairygui::GList::selectReverse));
        bind::function("setLineCount", CCBIND_FUNC(fairygui::GList::setLineCount));
        bind::function("setAutoResizeItem", CCBIND_FUNC(fairygui::GList::setAutoResizeItem));
        bind::function("getVerticalAlign", CCBIND_FUNC(fairygui::GList::getVerticalAlign));
        bind::function("removeChildrenToPool", CCBIND_OVERLOADED_FUNC(fairygui_GList_removeChildrenToPool));
        bind::function("setAlign", CCBIND_FUNC(fairygui::GList::setAlign));
        bind::function("isVirtual", CCBIND_FUNC(fairygui::GList::isVirtual));
        bind::function("getSelectedIndex", CCBIND_FUNC(fairygui::GList::getSelectedIndex));
        bind::function("getSelection", CCBIND_OVERLOADED_FUNC(fairygui_GList_getSelection));
        bind::function("itemIndexToChildIndex", CCBIND_FUNC(fairygui::GList::itemIndexToChildIndex));
        bind::function("getItemPool", CCBIND_FUNC(fairygui::GList::getItemPool));
        bind::function("getSelectionController", CCBIND_FUNC(fairygui::GList::getSelectionController));
        bind::function("getLineGap", CCBIND_FUNC(fairygui::GList::getLineGap));
        bind::function("scrollToView", CCBIND_OVERLOADED_FUNC(fairygui_GList_scrollToView));
        bind::function("handleArrowKey", CCBIND_FUNC(fairygui::GList::handleArrowKey));
        bind::function("getAlign", CCBIND_FUNC(fairygui::GList::getAlign));
        bind::function("getFromPool", CCBIND_OVERLOADED_FUNC(fairygui_GList_getFromPool));
        bind::function("getDefaultItem", CCBIND_FUNC(fairygui::GList::getDefaultItem));
        bind::function("getSelectionMode", CCBIND_FUNC(fairygui::GList::getSelectionMode));
        bind::function("setDefaultItem", CCBIND_FUNC(fairygui::GList::setDefaultItem));
        bind::function("setSelectedIndex", CCBIND_FUNC(fairygui::GList::setSelectedIndex));
        bind::function("setSelectionMode", CCBIND_FUNC(fairygui::GList::setSelectionMode));
        bind::function("addSelection", CCBIND_FUNC(fairygui::GList::addSelection));
        bind::function("childIndexToItemIndex", CCBIND_FUNC(fairygui::GList::childIndexToItemIndex));
        bind::function("getAutoResizeItem", CCBIND_FUNC(fairygui::GList::getAutoResizeItem));
        bind::function("setLineGap", CCBIND_FUNC(fairygui::GList::setLineGap));
        bind::function("removeChildToPool", CCBIND_FUNC(fairygui::GList::removeChildToPool));
        bind::function("getLayout", CCBIND_FUNC(fairygui::GList::getLayout));
        bind::function("create", CCBIND_SFUNC(fairygui::GList::create));
        bind::end();
    }

    if (bind::begin<fairygui::GComboBox>("fairygui.GComboBox", "fairygui.GComponent"))
    {
        bind::function("setValue", CCBIND_FUNC(fairygui::GComboBox::setValue));
        bind::function("setSelectedIndex", CCBIND_FUNC(fairygui::GComboBox::setSelectedIndex));
        bind::function("getItems", CCBIND_FUNC(fairygui::GComboBox::getItems));
        bind::function("getSelectedIndex", CCBIND_FUNC(fairygui::GComboBox::getSelectedIndex));
        bind::function("setTitle", CCBIND_FUNC(fairygui::GComboBox::setTitle));
        bind::function("refresh", CCBIND_FUNC(fairygui::GComboBox::refresh));
        bind::function("getValue", CCBIND_FUNC(fairygui::GComboBox::getValue));
        bind::function("getIcons", CCBIND_FUNC(fairygui::GComboBox::getIcons));
        bind::function("getTitle", CCBIND_FUNC(fairygui::GComboBox::getTitle));
        bind::function("getSelectionController", CCBIND_FUNC(fairygui::GComboBox::getSelectionController));
        bind::function("getValues", CCBIND_FUNC(fairygui::GComboBox::getValues));
        bind::function("setSelectionController", CCBIND_FUNC(fairygui::GComboBox::setSelectionController));
        bind::function("setItems", CCBIND_OVERLOADED_FUNC(fairygui_GComboBox_setItems));
        bind::function("setIcons", CCBIND_OVERLOADED_FUNC(fairygui_GComboBox_setIcons));
        bind::function("setValues", CCBIND_OVERLOADED_FUNC(fairygui_GComboBox_setValues));
        bind::function("create", CCBIND_SFUNC(fairygui::GComboBox::create));
        bind::end();
    }

    if (bind::begin<fairygui::GButton>("fairygui.GButton", "fairygui.GComponent"))
    {
        bind::function("getRelatedController", CCBIND_FUNC(fairygui::GButton::getRelatedController));
        bind::function("setChangeStateOnClick", CCBIND_FUNC(fairygui::GButton::setChangeStateOnClick));
        bind::function("setSelectedTitle", CCBIND_FUNC(fairygui::GButton::setSelectedTitle));
        bind::function("setSelected", CCBIND_FUNC(fairygui::GButton::setSelected));
        bind::function("setRelatedController", CCBIND_FUNC(fairygui::GButton::setRelatedController));
        bind::function("setTitle", CCBIND_FUNC(fairygui::GButton::setTitle));
        bind::function("getTitleFontSize", CCBIND_FUNC(fairygui::GButton::getTitleFontSize));
        bind::function("getSelectedIcon", CCBIND_FUNC(fairygui::GButton::getSelectedIcon));
        bind::function("isSelected", CCBIND_FUNC(fairygui::GButton::isSelected));
        bind::function("getTitleColor", CCBIND_FUNC(fairygui::GButton::getTitleColor));
        bind::function("getSelectedTitle", CCBIND_FUNC(fairygui::GButton::getSelectedTitle));
        bind::function("getTitle", CCBIND_FUNC(fairygui::GButton::getTitle));
        bind::function("setSelectedIcon", CCBIND_FUNC(fairygui::GButton::setSelectedIcon));
        bind::function("isChangeStateOnClick", CCBIND_FUNC(fairygui::GButton::isChangeStateOnClick));
        bind::function("setTitleFontSize", CCBIND_FUNC(fairygui::GButton::setTitleFontSize));
        bind::function("setTitleColor", CCBIND_FUNC(fairygui::GButton::setTitleColor));
        bind::function("create", CCBIND_SFUNC(fairygui::GButton::create));
        bind::end();
    }

    if (bind::begin<fairygui::GGroup>("fairygui.GGroup", "fairygui.GObject"))
    {
        bind::function("getColumnGap", CCBIND_FUNC(fairygui::GGroup::getColumnGap));
        bind::function("resizeChildren", CCBIND_FUNC(fairygui::GGroup::resizeChildren));
        bind::function("setLayout", CCBIND_FUNC(fairygui::GGroup::setLayout));
        bind::function("setColumnGap", CCBIND_FUNC(fairygui::GGroup::setColumnGap));
        bind::function("setBoundsChangedFlag", CCBIND_OVERLOADED_FUNC(fairygui_GGroup_setBoundsChangedFlag));
        bind::function("moveChildren", CCBIND_FUNC(fairygui::GGroup::moveChildren));
        bind::function("getLineGap", CCBIND_FUNC(fairygui::GGroup::getLineGap));
        bind::function("setLineGap", CCBIND_FUNC(fairygui::GGroup::setLineGap));
        bind::function("getLayout", CCBIND_FUNC(fairygui::GGroup::getLayout));
        bind::function("isExcludeInvisibles", CCBIND_FUNC(fairygui::GGroup::isExcludeInvisibles));
        bind::function("setExcludeInvisibles", CCBIND_FUNC(fairygui::GGroup::setExcludeInvisibles));
        bind::function("isAutoSizeDisabled", CCBIND_FUNC(fairygui::GGroup::isAutoSizeDisabled));
        bind::function("setAutoSizeDisabled", CCBIND_FUNC(fairygui::GGroup::setAutoSizeDisabled));
        bind::function("getMainGridIndex", CCBIND_FUNC(fairygui::GGroup::getMainGridIndex));
        bind::function("setMainGridIndex", CCBIND_FUNC(fairygui::GGroup::setMainGridIndex));
        bind::function("getMainGridMinSize", CCBIND_FUNC(fairygui::GGroup::getMainGridMinSize));
        bind::function("setMainGridMinSize", CCBIND_FUNC(fairygui::GGroup::setMainGridMinSize));
        bind::function("create", CCBIND_SFUNC(fairygui::GGroup::create));
        bind::end();
    }

    if (bind::begin<fairygui::GMovieClip>("fairygui.GMovieClip", "fairygui.GObject"))
    {
        bind::function("getFrame", CCBIND_FUNC(fairygui::GMovieClip::getFrame));
        bind::function("setColor", CCBIND_FUNC(fairygui::GMovieClip::setColor));
        bind::function("getColor", CCBIND_FUNC(fairygui::GMovieClip::getColor));
        bind::function("getFlip", CCBIND_FUNC(fairygui::GMovieClip::getFlip));
        bind::function("setFlip", CCBIND_FUNC(fairygui::GMovieClip::setFlip));
        bind::function("setPlaying", CCBIND_FUNC(fairygui::GMovieClip::setPlaying));
        bind::function("setPlaySettings", CCBIND_OVERLOADED_FUNC(fairygui_GMovieClip_setPlaySettings));
        bind::function("isPlaying", CCBIND_FUNC(fairygui::GMovieClip::isPlaying));
        bind::function("setFrame", CCBIND_FUNC(fairygui::GMovieClip::setFrame));
        bind::function("reverse", CCBIND_FUNC(fairygui::GMovieClip::reverse));
        bind::function("create", CCBIND_SFUNC(fairygui::GMovieClip::create));
        bind::end();
    }

    if (bind::begin<fairygui::GTextField>("fairygui.GTextField", "fairygui.GObject"))
    {
        bind::function("setAutoSize", CCBIND_FUNC(fairygui::GTextField::setAutoSize));
        bind::function("getFontSize", CCBIND_FUNC(fairygui::GTextField::getFontSize));
        bind::function("setColor", CCBIND_FUNC(fairygui::GTextField::setColor));
        bind::function("setSingleLine", CCBIND_FUNC(fairygui::GTextField::setSingleLine));
        bind::function("isUBBEnabled", CCBIND_FUNC(fairygui::GTextField::isUBBEnabled));
        bind::function("getColor", CCBIND_FUNC(fairygui::GTextField::getColor));
        bind::function("setUBBEnabled", CCBIND_FUNC(fairygui::GTextField::setUBBEnabled));
        bind::function("getAutoSize", CCBIND_FUNC(fairygui::GTextField::getAutoSize));
        bind::function("setFontSize", CCBIND_FUNC(fairygui::GTextField::setFontSize));
        bind::function("applyTextFormat", CCBIND_FUNC(fairygui::GTextField::applyTextFormat));
        bind::function("getTextFormat", CCBIND_FUNC(fairygui::GTextField::getTextFormat));
        bind::function("isSingleLine", CCBIND_FUNC(fairygui::GTextField::isSingleLine));
        bind::function("getTextSize", CCBIND_FUNC(fairygui::GTextField::getTextSize));
        bind::function("getOutlineColor", CCBIND_FUNC(fairygui::GTextField::getOutlineColor));
        bind::function("setOutlineColor", CCBIND_FUNC(fairygui::GTextField::setOutlineColor));
        bind::function("getTemplateVars", CCBIND_FUNC(fairygui::GTextField::getTemplateVars));
        bind::function("setTemplateVars", CCBIND_FUNC(fairygui::GTextField::setTemplateVars));
        bind::function("setVar", CCBIND_FUNC(fairygui::GTextField::setVar));
        bind::function("flushVars", CCBIND_FUNC(fairygui::GTextField::flushVars));
        bind::end();
    }

    if (bind::begin<fairygui::GRichTextField>("fairygui.GRichTextField", "fairygui.GTextField"))
    {
        bind::function("getControl", CCBIND_FUNC(fairygui::GRichTextField::getControl));
        bind::function("getControls", CCBIND_FUNC(fairygui::GRichTextField::getControls));
        bind::function("create", CCBIND_SFUNC(fairygui::GRichTextField::create));
        bind::end();
    }

    if (bind::begin<fairygui::GRoot>("fairygui.GRoot", "fairygui.GComponent"))
    {
        bind::function("closeAllWindows", CCBIND_FUNC(fairygui::GRoot::closeAllWindows));
        bind::function("hideTooltips", CCBIND_FUNC(fairygui::GRoot::hideTooltips));
        bind::function("hasAnyPopup", CCBIND_FUNC(fairygui::GRoot::hasAnyPopup));
        bind::function("getTopWindow", CCBIND_FUNC(fairygui::GRoot::getTopWindow));
        bind::function("hidePopup", CCBIND_OVERLOADED_FUNC(fairygui_GRoot_hidePopup));
        bind::function("closeAllExceptModals", CCBIND_FUNC(fairygui::GRoot::closeAllExceptModals));
        bind::function("showPopup", CCBIND_OVERLOADED_FUNC(fairygui_GRoot_showPopup));
        bind::function("showTooltipsWin", CCBIND_FUNC(fairygui::GRoot::showTooltipsWin));
        bind::function("closeModalWait", CCBIND_FUNC(fairygui::GRoot::closeModalWait));
        bind::function("getInputProcessor", CCBIND_FUNC(fairygui::GRoot::getInputProcessor));
        bind::function("getSoundVolumeScale", CCBIND_FUNC(fairygui::GRoot::getSoundVolumeScale));
        bind::function("togglePopup", CCBIND_OVERLOADED_FUNC(fairygui_GRoot_togglePopup));
        bind::function("isSoundEnabled", CCBIND_FUNC(fairygui::GRoot::isSoundEnabled));
        bind::function("setSoundVolumeScale", CCBIND_FUNC(fairygui::GRoot::setSoundVolumeScale));
        bind::function("getTouchTarget", CCBIND_FUNC(fairygui::GRoot::getTouchTarget));
        bind::function("hideWindowImmediately", CCBIND_FUNC(fairygui::GRoot::hideWindowImmediately));
        bind::function("getModalLayer", CCBIND_FUNC(fairygui::GRoot::getModalLayer));
        bind::function("playSound", CCBIND_OVERLOADED_FUNC(fairygui_GRoot_playSound));
        bind::function("getModalWaitingPane", CCBIND_FUNC(fairygui::GRoot::getModalWaitingPane));
        bind::function("setSoundEnabled", CCBIND_FUNC(fairygui::GRoot::setSoundEnabled));
        bind::function("bringToFront", CCBIND_FUNC(fairygui::GRoot::bringToFront));
        bind::function("hasModalWindow", CCBIND_FUNC(fairygui::GRoot::hasModalWindow));
        bind::function("showModalWait", CCBIND_FUNC(fairygui::GRoot::showModalWait));
        bind::function("isModalWaiting", CCBIND_FUNC(fairygui::GRoot::isModalWaiting));
        bind::function("getTouchPosition", CCBIND_FUNC(fairygui::GRoot::getTouchPosition));
        bind::function("getPoupPosition", CCBIND_FUNC(fairygui::GRoot::getPoupPosition));
        bind::function("showWindow", CCBIND_FUNC(fairygui::GRoot::showWindow));
        bind::function("showTooltips", CCBIND_FUNC(fairygui::GRoot::showTooltips));
        bind::function("hideWindow", CCBIND_FUNC(fairygui::GRoot::hideWindow));
        bind::function("create", CCBIND_OVERLOADED_SFUNC(fairygui_GRoot_create));
        bind::function("getInstance", CCBIND_SFUNC(fairygui::GRoot::getInstance));
        bind::end();
    }

    if (bind::begin<fairygui::GGraph>("fairygui.GGraph", "fairygui.GObject"))
    {
        bind::function("setPolygonPoints", CCBIND_FUNC(fairygui::GGraph::setPolygonPoints));
        bind::function("getColor", CCBIND_FUNC(fairygui::GGraph::getColor));
        bind::function("setColor", CCBIND_FUNC(fairygui::GGraph::setColor));
        bind::function("drawPolygon", CCBIND_FUNC(fairygui::GGraph::drawPolygon));
        bind::function("drawRegularPolygon", CCBIND_FUNC(fairygui::GGraph::drawRegularPolygon));
        bind::function("drawEllipse", CCBIND_FUNC(fairygui::GGraph::drawEllipse));
        bind::function("isEmpty", CCBIND_FUNC(fairygui::GGraph::isEmpty));
        bind::function("drawRect", CCBIND_FUNC(fairygui::GGraph::drawRect));
        bind::function("create", CCBIND_SFUNC(fairygui::GGraph::create));
        bind::end();
    }

    if (bind::begin<fairygui::EventContext>("fairygui.EventContext"))
    {
        bind::function("preventDefault", CCBIND_FUNC(fairygui::EventContext::preventDefault));
        bind::function("isDefaultPrevented", CCBIND_FUNC(fairygui::EventContext::isDefaultPrevented));
        bind::function("getType", CCBIND_FUNC(fairygui::EventContext::getType));
        bind::function("getSender", CCBIND_FUNC(fairygui::EventContext::getSender));
        bind::function("uncaptureTouch", CCBIND_FUNC(fairygui::EventContext::uncaptureTouch));
        bind::function("getInput", CCBIND_FUNC(fairygui::EventContext::getInput));
        bind::function("captureTouch", CCBIND_FUNC(fairygui::EventContext::captureTouch));
        bind::function("stopPropagation", CCBIND_FUNC(fairygui::EventContext::stopPropagation));
        bind::function("getData", CCBIND_OVERLOADED_FUNC(fairygui_EventContext_getData));
        bind::function("getDataValue", CCBIND_FUNC(fairygui::EventContext::getDataValue));
        bind::end();
    }

    if (bind::begin<fairygui::UIPackage>("fairygui.UIPackage"))
    {
        bind::function("getName", CCBIND_FUNC(fairygui::UIPackage::getName));
        bind::function("getId", CCBIND_FUNC(fairygui::UIPackage::getId));
        bind::function("getItemByName", CCBIND_FUNC(fairygui::UIPackage::getItemByName));
        bind::function("getItem", CCBIND_FUNC(fairygui::UIPackage::getItem));
        bind::function("createObjectFromURL", CCBIND_SFUNC(fairygui::UIPackage::createObjectFromURL));
        bind::function("removePackage", CCBIND_SFUNC(fairygui::UIPackage::removePackage));
        bind::function("getItemURL", CCBIND_SFUNC(fairygui::UIPackage::getItemURL));
        bind::function("normalizeURL", CCBIND_SFUNC(fairygui::UIPackage::normalizeURL));
        bind::function("removeAllPackages", CCBIND_SFUNC(fairygui::UIPackage::removeAllPackages));
        bind::function("createObject", CCBIND_OVERLOADED_SFUNC(fairygui_UIPackage_createObject));
        bind::function("getByName", CCBIND_SFUNC(fairygui::UIPackage::getByName));
        bind::function("addPackage", CCBIND_SFUNC(fairygui::UIPackage::addPackage));
        bind::function("getById", CCBIND_SFUNC(fairygui::UIPackage::getById));
        bind::function("getItemByURL", CCBIND_SFUNC(fairygui::UIPackage::getItemByURL));
        bind::function("getEmptyTexture", CCBIND_SFUNC(fairygui::UIPackage::getEmptyTexture));
        bind::end();
    }

    if (bind::begin<fairygui::Transition>("fairygui.Transition", "cc.Ref"))
    {
        bind::function("setValue", CCBIND_FUNC(fairygui::Transition::setValue));
        bind::function("changePlayTimes", CCBIND_FUNC(fairygui::Transition::changePlayTimes));
        bind::function("onOwnerRemovedFromStage", CCBIND_FUNC(fairygui::Transition::onOwnerRemovedFromStage));
        bind::function("setTarget", CCBIND_FUNC(fairygui::Transition::setTarget));
        bind::function("clearHooks", CCBIND_FUNC(fairygui::Transition::clearHooks));
        bind::function("stop", CCBIND_OVERLOADED_FUNC(fairygui_Transition_stop));
        bind::function("getOwner", CCBIND_FUNC(fairygui::Transition::getOwner));
        bind::function("setHook", CCBIND_FUNC(fairygui::Transition::setHook));
        bind::function("setDuration", CCBIND_FUNC(fairygui::Transition::setDuration));
        bind::function("isPlaying", CCBIND_FUNC(fairygui::Transition::isPlaying));
        bind::function("setAutoPlay", CCBIND_FUNC(fairygui::Transition::setAutoPlay));
        bind::function("updateFromRelations", CCBIND_FUNC(fairygui::Transition::updateFromRelations));
        bind::function("playReverse", CCBIND_OVERLOADED_FUNC(fairygui_Transition_playReverse));
        bind::function("play", CCBIND_OVERLOADED_FUNC(fairygui_Transition_play));
        bind::function("setTimeScale", CCBIND_FUNC(fairygui::Transition::setTimeScale));
        bind::function("getTimeScale", CCBIND_FUNC(fairygui::Transition::getTimeScale));
        bind::end();
    }

    if (bind::begin<fairygui::InputEvent>("fairygui.InputEvent"))
    {
        bind::function("getTouchId", CCBIND_FUNC(fairygui::InputEvent::getTouchId));
        bind::function("getProcessor", CCBIND_FUNC(fairygui::InputEvent::getProcessor));
        bind::function("getButton", CCBIND_FUNC(fairygui::InputEvent::getButton));
        bind::function("isDoubleClick", CCBIND_FUNC(fairygui::InputEvent::isDoubleClick));
        bind::function("getTouch", CCBIND_FUNC(fairygui::InputEvent::getTouch));
        bind::function("isAltDown", CCBIND_FUNC(fairygui::InputEvent::isAltDown));
        bind::function("getTarget", CCBIND_FUNC(fairygui::InputEvent::getTarget));
        bind::function("getX", CCBIND_FUNC(fairygui::InputEvent::getX));
        bind::function("getY", CCBIND_FUNC(fairygui::InputEvent::getY));
        bind::function("getMouseWheelDelta", CCBIND_FUNC(fairygui::InputEvent::getMouseWheelDelta));
        bind::function("isShiftDown", CCBIND_FUNC(fairygui::InputEvent::isShiftDown));
        bind::function("isCtrlDown", CCBIND_FUNC(fairygui::InputEvent::isCtrlDown));
        bind::function("getKeyCode", CCBIND_FUNC(fairygui::InputEvent::getKeyCode));
        bind::function("getPosition", CCBIND_FUNC(fairygui::InputEvent::getPosition));
        bind::end();
    }

    if (bind::begin<fairygui::GController>("fairygui.GController", "fairygui.UIEventDispatcher"))
    {
        bind::function("getPageIndexById", CCBIND_FUNC(fairygui::GController::getPageIndexById));
        bind::function("setSelectedPage", CCBIND_FUNC(fairygui::GController::setSelectedPage));
        bind::function("getPrevisousIndex", CCBIND_FUNC(fairygui::GController::getPrevisousIndex));
        bind::function("setSelectedIndex", CCBIND_FUNC(fairygui::GController::setSelectedIndex));
        bind::function("getPageId", CCBIND_FUNC(fairygui::GController::getPageId));
        bind::function("getSelectedPageId", CCBIND_FUNC(fairygui::GController::getSelectedPageId));
        bind::function("setParent", CCBIND_FUNC(fairygui::GController::setParent));
        bind::function("runActions", CCBIND_FUNC(fairygui::GController::runActions));
        bind::function("setOppositePageId", CCBIND_FUNC(fairygui::GController::setOppositePageId));
        bind::function("hasPage", CCBIND_FUNC(fairygui::GController::hasPage));
        bind::function("getPageNameById", CCBIND_FUNC(fairygui::GController::getPageNameById));
        bind::function("getPreviousPageId", CCBIND_FUNC(fairygui::GController::getPreviousPageId));
        bind::function("getPageCount", CCBIND_FUNC(fairygui::GController::getPageCount));
        bind::function("getSelectedIndex", CCBIND_FUNC(fairygui::GController::getSelectedIndex));
        bind::function("getPreviousPage", CCBIND_FUNC(fairygui::GController::getPreviousPage));
        bind::function("setSelectedPageId", CCBIND_FUNC(fairygui::GController::setSelectedPageId));
        bind::function("getSelectedPage", CCBIND_FUNC(fairygui::GController::getSelectedPage));
        bind::function("getParent", CCBIND_FUNC(fairygui::GController::getParent));
        bind::end();
    }

    if (bind::begin<fairygui::UIObjectFactory>("fairygui.UIObjectFactory"))
    {
        bind::function("newObject", CCBIND_OVERLOADED_SFUNC(fairygui_UIObjectFactory_newObject));
        bind::function("setPackageItemExtension", CCBIND_SFUNC(fairygui::UIObjectFactory::setPackageItemExtension));
        bind::function("setLoaderExtension", CCBIND_SFUNC(fairygui::UIObjectFactory::setLoaderExtension));
        bind::end();
    }

    if (bind::begin<fairygui::PopupMenu>("fairygui.PopupMenu", "cc.Ref"))
    {
        bind::function("setItemGrayed", CCBIND_FUNC(fairygui::PopupMenu::setItemGrayed));
        bind::function("getItemName", CCBIND_FUNC(fairygui::PopupMenu::getItemName));
        bind::function("clearItems", CCBIND_FUNC(fairygui::PopupMenu::clearItems));
        bind::function("getList", CCBIND_FUNC(fairygui::PopupMenu::getList));
        bind::function("removeItem", CCBIND_FUNC(fairygui::PopupMenu::removeItem));
        bind::function("addItem", CCBIND_FUNC(fairygui::PopupMenu::addItem));
        bind::function("addSeperator", CCBIND_FUNC(fairygui::PopupMenu::addSeperator));
        bind::function("addItemAt", CCBIND_FUNC(fairygui::PopupMenu::addItemAt));
        bind::function("setItemText", CCBIND_FUNC(fairygui::PopupMenu::setItemText));
        bind::function("setItemChecked", CCBIND_FUNC(fairygui::PopupMenu::setItemChecked));
        bind::function("show", CCBIND_OVERLOADED_FUNC(fairygui_PopupMenu_show));
        bind::function("getContentPane", CCBIND_FUNC(fairygui::PopupMenu::getContentPane));
        bind::function("getItemCount", CCBIND_FUNC(fairygui::PopupMenu::getItemCount));
        bind::function("setItemCheckable", CCBIND_FUNC(fairygui::PopupMenu::setItemCheckable));
        bind::function("isItemChecked", CCBIND_FUNC(fairygui::PopupMenu::isItemChecked));
        bind::function("setItemVisible", CCBIND_FUNC(fairygui::PopupMenu::setItemVisible));
        bind::function("create", CCBIND_OVERLOADED_SFUNC(fairygui_PopupMenu_create));
        bind::end();
    }

    if (bind::begin<fairygui::GSlider>("fairygui.GSlider", "fairygui.GComponent"))
    {
        bind::variable("changeOnClick", CCBIND_GETTER(fairygui::GSlider::changeOnClick), CCBIND_SETTER(fairygui::GSlider::changeOnClick));
        bind::variable("canDrag", CCBIND_GETTER(fairygui::GSlider::canDrag), CCBIND_SETTER(fairygui::GSlider::canDrag));

        bind::function("getTitleType", CCBIND_FUNC(fairygui::GSlider::getTitleType));
        bind::function("setTitleType", CCBIND_FUNC(fairygui::GSlider::setTitleType));
        bind::function("getMin", CCBIND_FUNC(fairygui::GSlider::getMin));
        bind::function("setMin", CCBIND_FUNC(fairygui::GSlider::setMin));
        bind::function("getMax", CCBIND_FUNC(fairygui::GSlider::getMax));
        bind::function("setMax", CCBIND_FUNC(fairygui::GSlider::setMax));
        bind::function("getValue", CCBIND_FUNC(fairygui::GSlider::getValue));
        bind::function("setValue", CCBIND_FUNC(fairygui::GSlider::setValue));
        bind::function("getWholeNumbers", CCBIND_FUNC(fairygui::GSlider::getWholeNumbers));
        bind::function("setWholeNumbers", CCBIND_FUNC(fairygui::GSlider::setWholeNumbers));
        bind::function("create", CCBIND_SFUNC(fairygui::GSlider::create));
        bind::end();
    }

    if (bind::begin<fairygui::GTextInput>("fairygui.GTextInput", "fairygui.GTextField"))
    {
        bind::function("setKeyboardType", CCBIND_FUNC(fairygui::GTextInput::setKeyboardType));
        bind::function("setPrompt", CCBIND_FUNC(fairygui::GTextInput::setPrompt));
        bind::function("setMaxLength", CCBIND_FUNC(fairygui::GTextInput::setMaxLength));
        bind::function("setRestrict", CCBIND_FUNC(fairygui::GTextInput::setRestrict));
        bind::function("setPassword", CCBIND_FUNC(fairygui::GTextInput::setPassword));
        bind::function("create", CCBIND_SFUNC(fairygui::GTextInput::create));
        bind::end();
    }

    if (bind::begin<fairygui::Window>("fairygui.Window", "fairygui.GComponent"))
    {
        bind::function("getCloseButton", CCBIND_FUNC(fairygui::Window::getCloseButton));
        bind::function("getContentPane", CCBIND_FUNC(fairygui::Window::getContentPane));
        bind::function("show", CCBIND_FUNC(fairygui::Window::show));
        bind::function("isTop", CCBIND_FUNC(fairygui::Window::isTop));
        bind::function("hideImmediately", CCBIND_FUNC(fairygui::Window::hideImmediately));
        bind::function("toggleStatus", CCBIND_FUNC(fairygui::Window::toggleStatus));
        bind::function("hide", CCBIND_FUNC(fairygui::Window::hide));
        bind::function("getFrame", CCBIND_FUNC(fairygui::Window::getFrame));
        bind::function("isShowing", CCBIND_FUNC(fairygui::Window::isShowing));
        bind::function("closeModalWait", CCBIND_OVERLOADED_FUNC(fairygui_Window_closeModalWait));
        bind::function("initWindow", CCBIND_FUNC(fairygui::Window::initWindow));
        bind::function("setContentArea", CCBIND_FUNC(fairygui::Window::setContentArea));
        bind::function("setDragArea", CCBIND_FUNC(fairygui::Window::setDragArea));
        bind::function("setContentPane", CCBIND_FUNC(fairygui::Window::setContentPane));
        bind::function("isModal", CCBIND_FUNC(fairygui::Window::isModal));
        bind::function("isBringToFrontOnClick", CCBIND_FUNC(fairygui::Window::isBringToFrontOnClick));
        bind::function("getContentArea", CCBIND_FUNC(fairygui::Window::getContentArea));
        bind::function("setBringToFrontOnClick", CCBIND_FUNC(fairygui::Window::setBringToFrontOnClick));
        bind::function("setModal", CCBIND_FUNC(fairygui::Window::setModal));
        bind::function("getModalWaitingPane", CCBIND_FUNC(fairygui::Window::getModalWaitingPane));
        bind::function("getDragArea", CCBIND_FUNC(fairygui::Window::getDragArea));
        bind::function("bringToFront", CCBIND_FUNC(fairygui::Window::bringToFront));
        bind::function("showModalWait", CCBIND_OVERLOADED_FUNC(fairygui_Window_showModalWait));
        bind::function("setCloseButton", CCBIND_FUNC(fairygui::Window::setCloseButton));
        bind::function("addUISource", CCBIND_FUNC(fairygui::Window::addUISource));
        bind::function("create", CCBIND_SFUNC(fairygui::Window::create));
        bind::end();
    }

    if (bind::begin<fairygui::GLoader>("fairygui.GLoader", "fairygui.GObject"))
    {
        bind::function("getURL", CCBIND_FUNC(fairygui::GLoader::getURL));
        bind::function("isPlaying", CCBIND_FUNC(fairygui::GLoader::isPlaying));
        bind::function("setColor", CCBIND_FUNC(fairygui::GLoader::setColor));
        bind::function("setAlign", CCBIND_FUNC(fairygui::GLoader::setAlign));
        bind::function("setPlaying", CCBIND_FUNC(fairygui::GLoader::setPlaying));
        bind::function("getColor", CCBIND_FUNC(fairygui::GLoader::getColor));
        bind::function("setURL", CCBIND_FUNC(fairygui::GLoader::setURL));
        bind::function("setFrame", CCBIND_FUNC(fairygui::GLoader::setFrame));
        bind::function("getFill", CCBIND_FUNC(fairygui::GLoader::getFill));
        bind::function("setAutoSize", CCBIND_FUNC(fairygui::GLoader::setAutoSize));
        bind::function("getAutoSize", CCBIND_FUNC(fairygui::GLoader::getAutoSize));
        bind::function("getContentSize", CCBIND_FUNC(fairygui::GLoader::getContentSize));
        bind::function("setVerticalAlign", CCBIND_FUNC(fairygui::GLoader::setVerticalAlign));
        bind::function("getAlign", CCBIND_FUNC(fairygui::GLoader::getAlign));
        bind::function("setFill", CCBIND_FUNC(fairygui::GLoader::setFill));
        bind::function("getVerticalAlign", CCBIND_FUNC(fairygui::GLoader::getVerticalAlign));
        bind::function("getFrame", CCBIND_FUNC(fairygui::GLoader::getFrame));
        bind::function("isShrinkOnly", CCBIND_FUNC(fairygui::GLoader::isShrinkOnly));
        bind::function("setShrinkOnly", CCBIND_FUNC(fairygui::GLoader::setShrinkOnly));
        bind::function("getFillMethod", CCBIND_FUNC(fairygui::GLoader::getFillMethod));
        bind::function("setFillMethod", CCBIND_FUNC(fairygui::GLoader::setFillMethod));
        bind::function("getFillOrigin", CCBIND_FUNC(fairygui::GLoader::getFillOrigin));
        bind::function("setFillOrigin", CCBIND_FUNC(fairygui::GLoader::setFillOrigin));
        bind::function("isFillClockwise", CCBIND_FUNC(fairygui::GLoader::isFillClockwise));
        bind::function("setFillClockwise", CCBIND_FUNC(fairygui::GLoader::setFillClockwise));
        bind::function("getFillAmount", CCBIND_FUNC(fairygui::GLoader::getFillAmount));
        bind::function("setFillAmount", CCBIND_FUNC(fairygui::GLoader::setFillAmount));
        bind::function("getComponent", CCBIND_FUNC(fairygui::GLoader::getComponent));
        bind::function("create", CCBIND_SFUNC(fairygui::GLoader::create));
        bind::end();
    }

    if (bind::begin<fairygui::GLoader3D>("fairygui.GLoader3D", "fairygui.GObject"))
    {
        bind::function("getURL", CCBIND_FUNC(fairygui::GLoader3D::getURL));
        bind::function("setURL", CCBIND_FUNC(fairygui::GLoader3D::setURL));
        bind::function("getAlign", CCBIND_FUNC(fairygui::GLoader3D::getAlign));
        bind::function("setAlign", CCBIND_FUNC(fairygui::GLoader3D::setAlign));
        bind::function("getVerticalAlign", CCBIND_FUNC(fairygui::GLoader3D::getVerticalAlign));
        bind::function("setVerticalAlign", CCBIND_FUNC(fairygui::GLoader3D::setVerticalAlign));
        bind::function("getAutoSize", CCBIND_FUNC(fairygui::GLoader3D::getAutoSize));
        bind::function("setAutoSize", CCBIND_FUNC(fairygui::GLoader3D::setAutoSize));
        bind::function("getFill", CCBIND_FUNC(fairygui::GLoader3D::getFill));
        bind::function("setFill", CCBIND_FUNC(fairygui::GLoader3D::setFill));
        bind::function("isShrinkOnly", CCBIND_FUNC(fairygui::GLoader3D::isShrinkOnly));
        bind::function("setShrinkOnly", CCBIND_FUNC(fairygui::GLoader3D::setShrinkOnly));
        bind::function("getContent", CCBIND_FUNC(fairygui::GLoader3D::getContent));
        bind::function("setContent", CCBIND_FUNC(fairygui::GLoader3D::setContent));
        bind::function("getColor", CCBIND_FUNC(fairygui::GLoader3D::getColor));
        bind::function("setColor", CCBIND_FUNC(fairygui::GLoader3D::setColor));
        bind::function("isPlaying", CCBIND_FUNC(fairygui::GLoader3D::isPlaying));
        bind::function("setPlaying", CCBIND_FUNC(fairygui::GLoader3D::setPlaying));
        bind::function("getFrame", CCBIND_FUNC(fairygui::GLoader3D::getFrame));
        bind::function("setFrame", CCBIND_FUNC(fairygui::GLoader3D::setFrame));
        bind::function("getAnimationName", CCBIND_FUNC(fairygui::GLoader3D::getAnimationName));
        bind::function("setAnimationName", CCBIND_FUNC(fairygui::GLoader3D::setAnimationName));
        bind::function("getSkinName", CCBIND_FUNC(fairygui::GLoader3D::getSkinName));
        bind::function("setSkinName", CCBIND_FUNC(fairygui::GLoader3D::setSkinName));
        bind::function("getLoop", CCBIND_FUNC(fairygui::GLoader3D::getLoop));
        bind::function("setLoop", CCBIND_FUNC(fairygui::GLoader3D::setLoop));
        bind::function("create", CCBIND_SFUNC(fairygui::GLoader3D::create));
        bind::end();
    }

    if (bind::begin<fairygui::GObjectPool>("fairygui.GObjectPool"))
    {
        bind::function("getObject", CCBIND_FUNC(fairygui::GObjectPool::getObject));
        bind::function("returnObject", CCBIND_FUNC(fairygui::GObjectPool::returnObject));
        bind::end();
    }

    if (bind::begin<fairygui::ScrollPane>("fairygui.ScrollPane", "cc.Ref"))
    {
        bind::function("setBouncebackEffect", CCBIND_FUNC(fairygui::ScrollPane::setBouncebackEffect));
        bind::function("getHzScrollBar", CCBIND_FUNC(fairygui::ScrollPane::getHzScrollBar));
        bind::function("scrollToView", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollToView));
        bind::function("getDecelerationRate", CCBIND_FUNC(fairygui::ScrollPane::getDecelerationRate));
        bind::function("getPercX", CCBIND_FUNC(fairygui::ScrollPane::getPercX));
        bind::function("getPosY", CCBIND_FUNC(fairygui::ScrollPane::getPosY));
        bind::function("getFooter", CCBIND_FUNC(fairygui::ScrollPane::getFooter));
        bind::function("setPosX", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_setPosX));
        bind::function("getPosX", CCBIND_FUNC(fairygui::ScrollPane::getPosX));
        bind::function("setPageY", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_setPageY));
        bind::function("scrollTop", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollTop));
        bind::function("isSnapToItem", CCBIND_FUNC(fairygui::ScrollPane::isSnapToItem));
        bind::function("isTouchEffect", CCBIND_FUNC(fairygui::ScrollPane::isTouchEffect));
        bind::function("setScrollStep", CCBIND_FUNC(fairygui::ScrollPane::setScrollStep));
        bind::function("scrollBottom", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollBottom));
        bind::function("isChildInView", CCBIND_FUNC(fairygui::ScrollPane::isChildInView));
        bind::function("isPageMode", CCBIND_FUNC(fairygui::ScrollPane::isPageMode));
        bind::function("setDecelerationRate", CCBIND_FUNC(fairygui::ScrollPane::setDecelerationRate));
        bind::function("getScrollingPosY", CCBIND_FUNC(fairygui::ScrollPane::getScrollingPosY));
        bind::function("getScrollingPosX", CCBIND_FUNC(fairygui::ScrollPane::getScrollingPosX));
        bind::function("getContentSize", CCBIND_FUNC(fairygui::ScrollPane::getContentSize));
        bind::function("isRightMost", CCBIND_FUNC(fairygui::ScrollPane::isRightMost));
        bind::function("setPageController", CCBIND_FUNC(fairygui::ScrollPane::setPageController));
        bind::function("lockHeader", CCBIND_FUNC(fairygui::ScrollPane::lockHeader));
        bind::function("lockFooter", CCBIND_FUNC(fairygui::ScrollPane::lockFooter));
        bind::function("scrollDown", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollDown));
        bind::function("setSnapToItem", CCBIND_FUNC(fairygui::ScrollPane::setSnapToItem));
        bind::function("getScrollStep", CCBIND_FUNC(fairygui::ScrollPane::getScrollStep));
        bind::function("getPageX", CCBIND_FUNC(fairygui::ScrollPane::getPageX));
        bind::function("scrollLeft", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollLeft));
        bind::function("getViewSize", CCBIND_FUNC(fairygui::ScrollPane::getViewSize));
        bind::function("setInertiaDisabled", CCBIND_FUNC(fairygui::ScrollPane::setInertiaDisabled));
        bind::function("getPageController", CCBIND_FUNC(fairygui::ScrollPane::getPageController));
        bind::function("setPageX", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_setPageX));
        bind::function("scrollUp", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollUp));
        bind::function("isMouseWheelEnabled", CCBIND_FUNC(fairygui::ScrollPane::isMouseWheelEnabled));
        bind::function("setPageMode", CCBIND_FUNC(fairygui::ScrollPane::setPageMode));
        bind::function("setTouchEffect", CCBIND_FUNC(fairygui::ScrollPane::setTouchEffect));
        bind::function("setMouseWheelEnabled", CCBIND_FUNC(fairygui::ScrollPane::setMouseWheelEnabled));
        bind::function("isInertiaDisabled", CCBIND_FUNC(fairygui::ScrollPane::isInertiaDisabled));
        bind::function("setPercX", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_setPercX));
        bind::function("setPercY", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_setPercY));
        bind::function("cancelDragging", CCBIND_FUNC(fairygui::ScrollPane::cancelDragging));
        bind::function("getPercY", CCBIND_FUNC(fairygui::ScrollPane::getPercY));
        bind::function("scrollRight", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_scrollRight));
        bind::function("getHeader", CCBIND_FUNC(fairygui::ScrollPane::getHeader));
        bind::function("getVtScrollBar", CCBIND_FUNC(fairygui::ScrollPane::getVtScrollBar));
        bind::function("getOwner", CCBIND_FUNC(fairygui::ScrollPane::getOwner));
        bind::function("isBottomMost", CCBIND_FUNC(fairygui::ScrollPane::isBottomMost));
        bind::function("getPageY", CCBIND_FUNC(fairygui::ScrollPane::getPageY));
        bind::function("isBouncebackEffect", CCBIND_FUNC(fairygui::ScrollPane::isBouncebackEffect));
        bind::function("setPosY", CCBIND_OVERLOADED_FUNC(fairygui_ScrollPane_setPosY));
        bind::function("getDraggingPane", CCBIND_SFUNC(fairygui::ScrollPane::getDraggingPane));
        bind::end();
    }

    if (bind::begin<fairygui::GProgressBar>("fairygui.GProgressBar", "fairygui.GComponent"))
    {
        bind::function("setTitleType", CCBIND_FUNC(fairygui::GProgressBar::setTitleType));
        bind::function("getTitleType", CCBIND_FUNC(fairygui::GProgressBar::getTitleType));
        bind::function("setMin", CCBIND_FUNC(fairygui::GProgressBar::setMin));
        bind::function("getMin", CCBIND_FUNC(fairygui::GProgressBar::getMin));
        bind::function("getMax", CCBIND_FUNC(fairygui::GProgressBar::getMax));
        bind::function("setMax", CCBIND_FUNC(fairygui::GProgressBar::setMax));
        bind::function("setValue", CCBIND_FUNC(fairygui::GProgressBar::setValue));
        bind::function("getValue", CCBIND_FUNC(fairygui::GProgressBar::getValue));
        bind::function("tweenValue", CCBIND_FUNC(fairygui::GProgressBar::tweenValue));
        bind::function("create", CCBIND_SFUNC(fairygui::GProgressBar::create));
        bind::end();
    }

    if (bind::begin<fairygui::DragDropManager>("fairygui.DragDropManager"))
    {
        bind::function("cancel", CCBIND_FUNC(fairygui::DragDropManager::cancel));
        bind::function("isDragging", CCBIND_FUNC(fairygui::DragDropManager::isDragging));
        bind::function("getAgent", CCBIND_FUNC(fairygui::DragDropManager::getAgent));
        bind::function("startDrag", CCBIND_OVERLOADED_FUNC(fairygui_DragDropManager_startDrag));
        bind::function("getInstance", CCBIND_SFUNC(fairygui::DragDropManager::getInstance));
        bind::function("destroyInstance", CCBIND_SFUNC(fairygui::DragDropManager::destroyInstance));
        bind::end();
    }

    if (bind::begin<fairygui::UIConfig>("fairygui.UIConfig"))
    {
        bind::variable("defaultFont", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultFont));
        bind::variable("buttonSound", nullptr, CCBIND_SSETTER(fairygui::UIConfig::buttonSound));
        bind::variable("buttonSoundVolumeScale", nullptr, CCBIND_SSETTER(fairygui::UIConfig::buttonSoundVolumeScale));
        bind::variable("defaultScrollStep", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultScrollStep));
        bind::variable("defaultScrollDecelerationRate", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultScrollDecelerationRate));
        bind::variable("defaultScrollTouchEffect", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultScrollTouchEffect));
        bind::variable("defaultScrollBounceEffect", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultScrollBounceEffect));
        bind::variable("defaultScrollBarDisplay", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultScrollBarDisplay));
        bind::variable("verticalScrollBar", nullptr, CCBIND_SSETTER(fairygui::UIConfig::verticalScrollBar));
        bind::variable("horizontalScrollBar", nullptr, CCBIND_SSETTER(fairygui::UIConfig::horizontalScrollBar));
        bind::variable("touchDragSensitivity", nullptr, CCBIND_SSETTER(fairygui::UIConfig::touchDragSensitivity));
        bind::variable("clickDragSensitivity", nullptr, CCBIND_SSETTER(fairygui::UIConfig::clickDragSensitivity));
        bind::variable("touchScrollSensitivity", nullptr, CCBIND_SSETTER(fairygui::UIConfig::touchScrollSensitivity));
        bind::variable("defaultComboBoxVisibleItemCount", nullptr, CCBIND_SSETTER(fairygui::UIConfig::defaultComboBoxVisibleItemCount));
        bind::variable("globalModalWaiting", nullptr, CCBIND_SSETTER(fairygui::UIConfig::globalModalWaiting));
        bind::variable("modalLayerColor", nullptr, CCBIND_SSETTER(fairygui::UIConfig::modalLayerColor));
        bind::variable("tooltipsWin", nullptr, CCBIND_SSETTER(fairygui::UIConfig::tooltipsWin));
        bind::variable("bringWindowToFrontOnClick", nullptr, CCBIND_SSETTER(fairygui::UIConfig::bringWindowToFrontOnClick));
        bind::variable("windowModalWaiting", nullptr, CCBIND_SSETTER(fairygui::UIConfig::windowModalWaiting));
        bind::variable("popupMenu", nullptr, CCBIND_SSETTER(fairygui::UIConfig::popupMenu));
        bind::variable("popupMenu_seperator", nullptr, CCBIND_SSETTER(fairygui::UIConfig::popupMenu_seperator));
        bind::variable("onMusicCallback", nullptr, CCBIND_SSETTER(fairygui::UIConfig::onMusicCallback));

        bind::function("registerFont", CCBIND_SFUNC(fairygui::UIConfig::registerFont));
        bind::end();
    }

    if (bind::begin<fairygui::GLabel>("fairygui.GLabel", "fairygui.GComponent"))
    {
        bind::function("getTitle", CCBIND_FUNC(fairygui::GLabel::getTitle));
        bind::function("setTitle", CCBIND_FUNC(fairygui::GLabel::setTitle));
        bind::function("getTitleColor", CCBIND_FUNC(fairygui::GLabel::getTitleColor));
        bind::function("setTitleColor", CCBIND_FUNC(fairygui::GLabel::setTitleColor));
        bind::function("getTitleFontSize", CCBIND_FUNC(fairygui::GLabel::getTitleFontSize));
        bind::function("setTitleFontSize", CCBIND_FUNC(fairygui::GLabel::setTitleFontSize));
        bind::function("getTextField", CCBIND_FUNC(fairygui::GLabel::getTextField));
        bind::function("create", CCBIND_SFUNC(fairygui::GLabel::create));
        bind::end();
    }

    if (bind::begin<fairygui::FUIInput>("fairygui.FUIInput", "ccui.EditBox"))
    {
        bind::function("getTextFormat", CCBIND_FUNC(fairygui::FUIInput::getTextFormat));
        bind::function("applyTextFormat", CCBIND_FUNC(fairygui::FUIInput::applyTextFormat));
        bind::function("isSingleLine", CCBIND_FUNC(fairygui::FUIInput::isSingleLine));
        bind::function("setSingleLine", CCBIND_FUNC(fairygui::FUIInput::setSingleLine));
        bind::function("isPassword", CCBIND_FUNC(fairygui::FUIInput::isPassword));
        bind::function("setPassword", CCBIND_FUNC(fairygui::FUIInput::setPassword));
        bind::end();
    }

    if (bind::begin<fairygui::FUISprite>("fairygui.FUISprite", "cc.Sprite"))
    {
        bind::function("clearContent", CCBIND_FUNC(fairygui::FUISprite::clearContent));
        bind::function("setScale9Grid", CCBIND_FUNC(fairygui::FUISprite::setScale9Grid));
        bind::function("setGrayed", CCBIND_FUNC(fairygui::FUISprite::setGrayed));
        bind::function("getFillMethod", CCBIND_FUNC(fairygui::FUISprite::getFillMethod));
        bind::function("setFillMethod", CCBIND_FUNC(fairygui::FUISprite::setFillMethod));
        bind::function("getFillOrigin", CCBIND_FUNC(fairygui::FUISprite::getFillOrigin));
        bind::function("setFillOrigin", CCBIND_FUNC(fairygui::FUISprite::setFillOrigin));
        bind::function("isFillClockwise", CCBIND_FUNC(fairygui::FUISprite::isFillClockwise));
        bind::function("setFillClockwise", CCBIND_FUNC(fairygui::FUISprite::setFillClockwise));
        bind::function("isScaleByTile", CCBIND_FUNC(fairygui::FUISprite::isScaleByTile));
        bind::function("setScaleByTile", CCBIND_FUNC(fairygui::FUISprite::setScaleByTile));
        bind::end();
    }

    if (bind::begin<fairygui::GBasicTextField>("fairygui.GBasicTextField", "fairygui.GTextField"))
    {
        bind::end();
    }

    if (bind::begin<fairygui::InputProcessor>("fairygui.InputProcessor"))
    {
        bind::function("getRecentInput", CCBIND_FUNC(fairygui::InputProcessor::getRecentInput));
        bind::end();
    }

    if (bind::begin<fairygui::HtmlObject>("fairygui.HtmlObject"))
    {
        bind::variable("buttonResource", CCBIND_SGETTER(fairygui::HtmlObject::buttonResource), CCBIND_SSETTER(fairygui::HtmlObject::buttonResource));
        bind::variable("inputResource", CCBIND_SGETTER(fairygui::HtmlObject::inputResource), CCBIND_SSETTER(fairygui::HtmlObject::inputResource));
        bind::variable("selectResource", CCBIND_SGETTER(fairygui::HtmlObject::selectResource), CCBIND_SSETTER(fairygui::HtmlObject::selectResource));
        bind::variable("usePool", CCBIND_SGETTER(fairygui::HtmlObject::usePool), CCBIND_SSETTER(fairygui::HtmlObject::usePool));

        bind::function("getUI", CCBIND_FUNC(fairygui::HtmlObject::getUI));
        bind::function("getElementAttrs", CCBIND_OVERLOADED_FUNC(fairygui_HtmlObject_getElementAttrs));
        bind::function("clearStaticPools", CCBIND_OVERLOADED_SFUNC(fairygui_HtmlObject_clearStaticPools));
        bind::end();
    }

    if (bind::begin<fairygui::PackageItem>("fairygui.PackageItem"))
    {
        bind::variable("owner", CCBIND_GETTER(fairygui::PackageItem::owner), CCBIND_SETTER(fairygui::PackageItem::owner));
        bind::variable("type", CCBIND_GETTER(fairygui::PackageItem::type), CCBIND_SETTER(fairygui::PackageItem::type));
        bind::variable("objectType", CCBIND_GETTER(fairygui::PackageItem::objectType), CCBIND_SETTER(fairygui::PackageItem::objectType));
        bind::variable("id", CCBIND_GETTER(fairygui::PackageItem::id), CCBIND_SETTER(fairygui::PackageItem::id));
        bind::variable("name", CCBIND_GETTER(fairygui::PackageItem::name), CCBIND_SETTER(fairygui::PackageItem::name));
        bind::variable("width", CCBIND_GETTER(fairygui::PackageItem::width), CCBIND_SETTER(fairygui::PackageItem::width));
        bind::variable("height", CCBIND_GETTER(fairygui::PackageItem::height), CCBIND_SETTER(fairygui::PackageItem::height));
        bind::variable("file", CCBIND_GETTER(fairygui::PackageItem::file), CCBIND_SETTER(fairygui::PackageItem::file));
        bind::variable("spriteFrame", CCBIND_GETTER(fairygui::PackageItem::spriteFrame), CCBIND_SETTER(fairygui::PackageItem::spriteFrame));

        bind::function("load", CCBIND_FUNC(fairygui::PackageItem::load));
        bind::function("getBranch", CCBIND_FUNC(fairygui::PackageItem::getBranch));
        bind::function("getHighResolution", CCBIND_FUNC(fairygui::PackageItem::getHighResolution));
        bind::end();
    }

    if (bind::begin<fairygui::GTree>("fairygui.GTree", "fairygui.GList"))
    {
        bind::variable("treeNodeRender", nullptr, CCBIND_SETTER(fairygui::GTree::treeNodeRender));
        bind::variable("treeNodeWillExpand", nullptr, CCBIND_SETTER(fairygui::GTree::treeNodeWillExpand));

        bind::function("getIndent", CCBIND_FUNC(fairygui::GTree::getIndent));
        bind::function("setIndent", CCBIND_FUNC(fairygui::GTree::setIndent));
        bind::function("getClickToExpand", CCBIND_FUNC(fairygui::GTree::getClickToExpand));
        bind::function("setClickToExpand", CCBIND_FUNC(fairygui::GTree::setClickToExpand));
        bind::function("getRootNode", CCBIND_FUNC(fairygui::GTree::getRootNode));
        bind::function("getSelectedNode", CCBIND_FUNC(fairygui::GTree::getSelectedNode));
        bind::function("getSelectedNodes", CCBIND_OVERLOADED_FUNC(fairygui_GTree_getSelectedNodes));
        bind::function("selectNode", CCBIND_FUNC(fairygui::GTree::selectNode));
        bind::function("unselectNode", CCBIND_FUNC(fairygui::GTree::unselectNode));
        bind::function("expandAll", CCBIND_FUNC(fairygui::GTree::expandAll));
        bind::function("collapseAll", CCBIND_FUNC(fairygui::GTree::collapseAll));
        bind::function("create", CCBIND_SFUNC(fairygui::GTree::create));
        bind::end();
    }

    if (bind::begin<fairygui::GTreeNode>("fairygui.GTreeNode", "cc.Ref"))
    {
        bind::function("create", CCBIND_SFUNC(fairygui::GTreeNode::create));
        bind::function("getParent", CCBIND_FUNC(fairygui::GTreeNode::getParent));
        bind::function("getTree", CCBIND_FUNC(fairygui::GTreeNode::getTree));
        bind::function("getCell", CCBIND_FUNC(fairygui::GTreeNode::getCell));
        bind::function("getData", CCBIND_FUNC(fairygui::GTreeNode::getData));
        bind::function("setData", CCBIND_FUNC(fairygui::GTreeNode::setData));
        bind::function("isExpanded", CCBIND_FUNC(fairygui::GTreeNode::isExpanded));
        bind::function("setExpaned", CCBIND_FUNC(fairygui::GTreeNode::setExpaned));
        bind::function("isFolder", CCBIND_FUNC(fairygui::GTreeNode::isFolder));
        bind::function("getText", CCBIND_FUNC(fairygui::GTreeNode::getText));
        bind::function("setText", CCBIND_FUNC(fairygui::GTreeNode::setText));
        bind::function("getIcon", CCBIND_FUNC(fairygui::GTreeNode::getIcon));
        bind::function("setIcon", CCBIND_FUNC(fairygui::GTreeNode::setIcon));
        bind::function("addChild", CCBIND_FUNC(fairygui::GTreeNode::addChild));
        bind::function("addChildAt", CCBIND_FUNC(fairygui::GTreeNode::addChildAt));
        bind::function("removeChild", CCBIND_FUNC(fairygui::GTreeNode::removeChild));
        bind::function("removeChildAt", CCBIND_FUNC(fairygui::GTreeNode::removeChildAt));
        bind::function("removeChildren", CCBIND_OVERLOADED_FUNC(fairygui_GTreeNode_removeChildren));
        bind::function("getChildAt", CCBIND_FUNC(fairygui::GTreeNode::getChildAt));
        bind::function("getPrevSibling", CCBIND_FUNC(fairygui::GTreeNode::getPrevSibling));
        bind::function("getNextSibling", CCBIND_FUNC(fairygui::GTreeNode::getNextSibling));
        bind::function("getChildIndex", CCBIND_FUNC(fairygui::GTreeNode::getChildIndex));
        bind::function("setChildIndex", CCBIND_FUNC(fairygui::GTreeNode::setChildIndex));
        bind::function("setChildIndexBefore", CCBIND_FUNC(fairygui::GTreeNode::setChildIndexBefore));
        bind::function("swapChildren", CCBIND_FUNC(fairygui::GTreeNode::swapChildren));
        bind::function("swapChildrenAt", CCBIND_FUNC(fairygui::GTreeNode::swapChildrenAt));
        bind::function("numChildren", CCBIND_FUNC(fairygui::GTreeNode::numChildren));
        bind::end();
    }

    if (bind::begin<fairygui::TextFormat>("fairygui.TextFormat"))
    {
        bind::variable("face", nullptr, CCBIND_SETTER(fairygui::TextFormat::face));
        bind::variable("fontSize", nullptr, CCBIND_SETTER(fairygui::TextFormat::fontSize));
        bind::variable("color", nullptr, CCBIND_SETTER(fairygui::TextFormat::color));
        bind::variable("bold", nullptr, CCBIND_SETTER(fairygui::TextFormat::bold));
        bind::variable("italics", nullptr, CCBIND_SETTER(fairygui::TextFormat::italics));
        bind::variable("underline", nullptr, CCBIND_SETTER(fairygui::TextFormat::underline));
        bind::variable("lineSpacing", nullptr, CCBIND_SETTER(fairygui::TextFormat::lineSpacing));
        bind::variable("letterSpacing", nullptr, CCBIND_SETTER(fairygui::TextFormat::letterSpacing));
        bind::variable("align", nullptr, CCBIND_SETTER(fairygui::TextFormat::align));
        bind::variable("verticalAlign", nullptr, CCBIND_SETTER(fairygui::TextFormat::verticalAlign));
        bind::variable("outlineColor", nullptr, CCBIND_SETTER(fairygui::TextFormat::outlineColor));
        bind::variable("outlineSize", nullptr, CCBIND_SETTER(fairygui::TextFormat::outlineSize));
        bind::variable("shadowColor", nullptr, CCBIND_SETTER(fairygui::TextFormat::shadowColor));
        bind::variable("shadowOffset", nullptr, CCBIND_SETTER(fairygui::TextFormat::shadowOffset));
        bind::variable("shadowBlurRadius", nullptr, CCBIND_SETTER(fairygui::TextFormat::shadowBlurRadius));
        bind::variable("glowColor", nullptr, CCBIND_SETTER(fairygui::TextFormat::glowColor));

        bind::function("setFormat", CCBIND_OVERLOADED_FUNC(fairygui_TextFormat_setFormat));
        bind::function("enableEffect", CCBIND_FUNC(fairygui::TextFormat::enableEffect));
        bind::function("disableEffect", CCBIND_FUNC(fairygui::TextFormat::disableEffect));
        bind::function("hasEffect", CCBIND_FUNC(fairygui::TextFormat::hasEffect));
        bind::end();
    }

    // enums
    if (bind::begin<fairygui::UIEventType>("fairygui.UIEventType"))
    {
        bind::constant("Enter", CCBIND_CONSTANT(fairygui::UIEventType::Enter));
        bind::constant("Exit", CCBIND_CONSTANT(fairygui::UIEventType::Exit));
        bind::constant("Changed", CCBIND_CONSTANT(fairygui::UIEventType::Changed));
        bind::constant("Submit", CCBIND_CONSTANT(fairygui::UIEventType::Submit));
        bind::constant("TouchBegin", CCBIND_CONSTANT(fairygui::UIEventType::TouchBegin));
        bind::constant("TouchMove", CCBIND_CONSTANT(fairygui::UIEventType::TouchMove));
        bind::constant("TouchEnd", CCBIND_CONSTANT(fairygui::UIEventType::TouchEnd));
        bind::constant("Click", CCBIND_CONSTANT(fairygui::UIEventType::Click));
        bind::constant("RollOver", CCBIND_CONSTANT(fairygui::UIEventType::RollOver));
        bind::constant("RollOut", CCBIND_CONSTANT(fairygui::UIEventType::RollOut));
        bind::constant("MouseWheel", CCBIND_CONSTANT(fairygui::UIEventType::MouseWheel));
        bind::constant("RightClick", CCBIND_CONSTANT(fairygui::UIEventType::RightClick));
        bind::constant("MiddleClick", CCBIND_CONSTANT(fairygui::UIEventType::MiddleClick));
        bind::constant("PositionChange", CCBIND_CONSTANT(fairygui::UIEventType::PositionChange));
        bind::constant("SizeChange", CCBIND_CONSTANT(fairygui::UIEventType::SizeChange));
        bind::constant("KeyDown", CCBIND_CONSTANT(fairygui::UIEventType::KeyDown));
        bind::constant("KeyUp", CCBIND_CONSTANT(fairygui::UIEventType::KeyUp));
        bind::constant("Scroll", CCBIND_CONSTANT(fairygui::UIEventType::Scroll));
        bind::constant("ScrollEnd", CCBIND_CONSTANT(fairygui::UIEventType::ScrollEnd));
        bind::constant("PullDownRelease", CCBIND_CONSTANT(fairygui::UIEventType::PullDownRelease));
        bind::constant("PullUpRelease", CCBIND_CONSTANT(fairygui::UIEventType::PullUpRelease));
        bind::constant("ClickItem", CCBIND_CONSTANT(fairygui::UIEventType::ClickItem));
        bind::constant("ClickLink", CCBIND_CONSTANT(fairygui::UIEventType::ClickLink));
        bind::constant("ClickMenu", CCBIND_CONSTANT(fairygui::UIEventType::ClickMenu));
        bind::constant("RightClickItem", CCBIND_CONSTANT(fairygui::UIEventType::RightClickItem));
        bind::constant("DragStart", CCBIND_CONSTANT(fairygui::UIEventType::DragStart));
        bind::constant("DragMove", CCBIND_CONSTANT(fairygui::UIEventType::DragMove));
        bind::constant("DragEnd", CCBIND_CONSTANT(fairygui::UIEventType::DragEnd));
        bind::constant("Drop", CCBIND_CONSTANT(fairygui::UIEventType::Drop));
        bind::constant("GearStop", CCBIND_CONSTANT(fairygui::UIEventType::GearStop));
        bind::constant("OnInit", CCBIND_CONSTANT(fairygui::UIEventType::OnInit));
        bind::constant("OnShown", CCBIND_CONSTANT(fairygui::UIEventType::OnShown));
        bind::constant("OnHide", CCBIND_CONSTANT(fairygui::UIEventType::OnHide));
        bind::end();
    }

    if (bind::begin<fairygui::PackageItemType>("fairygui.PackageItemType"))
    {
        bind::constant("IMAGE", CCBIND_CONSTANT(fairygui::PackageItemType::IMAGE));
        bind::constant("MOVIECLIP", CCBIND_CONSTANT(fairygui::PackageItemType::MOVIECLIP));
        bind::constant("SOUND", CCBIND_CONSTANT(fairygui::PackageItemType::SOUND));
        bind::constant("COMPONENT", CCBIND_CONSTANT(fairygui::PackageItemType::COMPONENT));
        bind::constant("ATLAS", CCBIND_CONSTANT(fairygui::PackageItemType::ATLAS));
        bind::constant("FONT", CCBIND_CONSTANT(fairygui::PackageItemType::FONT));
        bind::constant("SWF", CCBIND_CONSTANT(fairygui::PackageItemType::SWF));
        bind::constant("MISC", CCBIND_CONSTANT(fairygui::PackageItemType::MISC));
        bind::constant("UNKNOWN", CCBIND_CONSTANT(fairygui::PackageItemType::UNKNOWN));
        bind::end();
    }

    if (bind::begin<fairygui::ObjectType>("fairygui.ObjectType"))
    {
        bind::constant("IMAGE", CCBIND_CONSTANT(fairygui::ObjectType::IMAGE));
        bind::constant("MOVIECLIP", CCBIND_CONSTANT(fairygui::ObjectType::MOVIECLIP));
        bind::constant("SWF", CCBIND_CONSTANT(fairygui::ObjectType::SWF));
        bind::constant("GRAPH", CCBIND_CONSTANT(fairygui::ObjectType::GRAPH));
        bind::constant("LOADER", CCBIND_CONSTANT(fairygui::ObjectType::LOADER));
        bind::constant("GROUP", CCBIND_CONSTANT(fairygui::ObjectType::GROUP));
        bind::constant("TEXT", CCBIND_CONSTANT(fairygui::ObjectType::TEXT));
        bind::constant("RICHTEXT", CCBIND_CONSTANT(fairygui::ObjectType::RICHTEXT));
        bind::constant("INPUTTEXT", CCBIND_CONSTANT(fairygui::ObjectType::INPUTTEXT));
        bind::constant("COMPONENT", CCBIND_CONSTANT(fairygui::ObjectType::COMPONENT));
        bind::constant("LIST", CCBIND_CONSTANT(fairygui::ObjectType::LIST));
        bind::constant("LABEL", CCBIND_CONSTANT(fairygui::ObjectType::LABEL));
        bind::constant("BUTTON", CCBIND_CONSTANT(fairygui::ObjectType::BUTTON));
        bind::constant("COMBOBOX", CCBIND_CONSTANT(fairygui::ObjectType::COMBOBOX));
        bind::constant("PROGRESSBAR", CCBIND_CONSTANT(fairygui::ObjectType::PROGRESSBAR));
        bind::constant("SLIDER", CCBIND_CONSTANT(fairygui::ObjectType::SLIDER));
        bind::constant("SCROLLBAR", CCBIND_CONSTANT(fairygui::ObjectType::SCROLLBAR));
        bind::constant("TREE", CCBIND_CONSTANT(fairygui::ObjectType::TREE));
        bind::end();
    }

    if (bind::begin<fairygui::ButtonMode>("fairygui.ButtonMode"))
    {
        bind::constant("COMMON", CCBIND_CONSTANT(fairygui::ButtonMode::COMMON));
        bind::constant("CHECK", CCBIND_CONSTANT(fairygui::ButtonMode::CHECK));
        bind::constant("RADIO", CCBIND_CONSTANT(fairygui::ButtonMode::RADIO));
        bind::end();
    }

    if (bind::begin<fairygui::ChildrenRenderOrder>("fairygui.ChildrenRenderOrder"))
    {
        bind::constant("ASCENT", CCBIND_CONSTANT(fairygui::ChildrenRenderOrder::ASCENT));
        bind::constant("DESCENT", CCBIND_CONSTANT(fairygui::ChildrenRenderOrder::DESCENT));
        bind::constant("ARCH", CCBIND_CONSTANT(fairygui::ChildrenRenderOrder::ARCH));
        bind::end();
    }

    if (bind::begin<fairygui::OverflowType>("fairygui.OverflowType"))
    {
        bind::constant("VISIBLE", CCBIND_CONSTANT(fairygui::OverflowType::VISIBLE));
        bind::constant("HIDDEN", CCBIND_CONSTANT(fairygui::OverflowType::HIDDEN));
        bind::constant("SCROLL", CCBIND_CONSTANT(fairygui::OverflowType::SCROLL));
        bind::end();
    }

    if (bind::begin<fairygui::ScrollType>("fairygui.ScrollType"))
    {
        bind::constant("HORIZONTAL", CCBIND_CONSTANT(fairygui::ScrollType::HORIZONTAL));
        bind::constant("VERTICAL", CCBIND_CONSTANT(fairygui::ScrollType::VERTICAL));
        bind::constant("BOTH", CCBIND_CONSTANT(fairygui::ScrollType::BOTH));
        bind::end();
    }

    if (bind::begin<fairygui::ScrollBarDisplayType>("fairygui.ScrollBarDisplayType"))
    {
        bind::constant("DEFAULT", CCBIND_CONSTANT(fairygui::ScrollBarDisplayType::DEFAULT));
        bind::constant("VISIBLE", CCBIND_CONSTANT(fairygui::ScrollBarDisplayType::VISIBLE));
        bind::constant("AUTO", CCBIND_CONSTANT(fairygui::ScrollBarDisplayType::AUTO));
        bind::constant("HIDDEN", CCBIND_CONSTANT(fairygui::ScrollBarDisplayType::HIDDEN));
        bind::end();
    }

    if (bind::begin<fairygui::LoaderFillType>("fairygui.LoaderFillType"))
    {
        bind::constant("NONE", CCBIND_CONSTANT(fairygui::LoaderFillType::NONE));
        bind::constant("SCALE", CCBIND_CONSTANT(fairygui::LoaderFillType::SCALE));
        bind::constant("SCALE_MATCH_HEIGHT", CCBIND_CONSTANT(fairygui::LoaderFillType::SCALE_MATCH_HEIGHT));
        bind::constant("SCALE_MATCH_WIDTH", CCBIND_CONSTANT(fairygui::LoaderFillType::SCALE_MATCH_WIDTH));
        bind::constant("SCALE_FREE", CCBIND_CONSTANT(fairygui::LoaderFillType::SCALE_FREE));
        bind::constant("SCALE_NO_BORDER", CCBIND_CONSTANT(fairygui::LoaderFillType::SCALE_NO_BORDER));
        bind::end();
    }

    if (bind::begin<fairygui::ProgressTitleType>("fairygui.ProgressTitleType"))
    {
        bind::constant("PERCENT", CCBIND_CONSTANT(fairygui::ProgressTitleType::PERCENT));
        bind::constant("VALUE_MAX", CCBIND_CONSTANT(fairygui::ProgressTitleType::VALUE_MAX));
        bind::constant("VALUE", CCBIND_CONSTANT(fairygui::ProgressTitleType::VALUE));
        bind::constant("MAX", CCBIND_CONSTANT(fairygui::ProgressTitleType::MAX));
        bind::end();
    }

    if (bind::begin<fairygui::ListLayoutType>("fairygui.ListLayoutType"))
    {
        bind::constant("SINGLE_COLUMN", CCBIND_CONSTANT(fairygui::ListLayoutType::SINGLE_COLUMN));
        bind::constant("SINGLE_ROW", CCBIND_CONSTANT(fairygui::ListLayoutType::SINGLE_ROW));
        bind::constant("FLOW_HORIZONTAL", CCBIND_CONSTANT(fairygui::ListLayoutType::FLOW_HORIZONTAL));
        bind::constant("FLOW_VERTICAL", CCBIND_CONSTANT(fairygui::ListLayoutType::FLOW_VERTICAL));
        bind::constant("PAGINATION", CCBIND_CONSTANT(fairygui::ListLayoutType::PAGINATION));
        bind::end();
    }

    if (bind::begin<fairygui::ListSelectionMode>("fairygui.ListSelectionMode"))
    {
        bind::constant("SINGLE", CCBIND_CONSTANT(fairygui::ListSelectionMode::SINGLE));
        bind::constant("MULTIPLE", CCBIND_CONSTANT(fairygui::ListSelectionMode::MULTIPLE));
        bind::constant("MULTIPLE_SINGLECLICK", CCBIND_CONSTANT(fairygui::ListSelectionMode::MULTIPLE_SINGLECLICK));
        bind::constant("NONE", CCBIND_CONSTANT(fairygui::ListSelectionMode::NONE));
        bind::end();
    }

    if (bind::begin<fairygui::GroupLayoutType>("fairygui.GroupLayoutType"))
    {
        bind::constant("NONE", CCBIND_CONSTANT(fairygui::GroupLayoutType::NONE));
        bind::constant("HORIZONTAL", CCBIND_CONSTANT(fairygui::GroupLayoutType::HORIZONTAL));
        bind::constant("VERTICAL", CCBIND_CONSTANT(fairygui::GroupLayoutType::VERTICAL));
        bind::end();
    }

    if (bind::begin<fairygui::PopupDirection>("fairygui.PopupDirection"))
    {
        bind::constant("AUTO", CCBIND_CONSTANT(fairygui::PopupDirection::AUTO));
        bind::constant("UP", CCBIND_CONSTANT(fairygui::PopupDirection::UP));
        bind::constant("DOWN", CCBIND_CONSTANT(fairygui::PopupDirection::DOWN));
        bind::end();
    }

    if (bind::begin<fairygui::AutoSizeType>("fairygui.AutoSizeType"))
    {
        bind::constant("NONE", CCBIND_CONSTANT(fairygui::AutoSizeType::NONE));
        bind::constant("BOTH", CCBIND_CONSTANT(fairygui::AutoSizeType::BOTH));
        bind::constant("HEIGHT", CCBIND_CONSTANT(fairygui::AutoSizeType::HEIGHT));
        bind::constant("SHRINK", CCBIND_CONSTANT(fairygui::AutoSizeType::SHRINK));
        bind::end();
    }

    if (bind::begin<fairygui::FlipType>("fairygui.FlipType"))
    {
        bind::constant("NONE", CCBIND_CONSTANT(fairygui::FlipType::NONE));
        bind::constant("HORIZONTAL", CCBIND_CONSTANT(fairygui::FlipType::HORIZONTAL));
        bind::constant("VERTICAL", CCBIND_CONSTANT(fairygui::FlipType::VERTICAL));
        bind::constant("BOTH", CCBIND_CONSTANT(fairygui::FlipType::BOTH));
        bind::end();
    }

    if (bind::begin<fairygui::TransitionActionType>("fairygui.TransitionActionType"))
    {
        bind::constant("XY", CCBIND_CONSTANT(fairygui::TransitionActionType::XY));
        bind::constant("Size", CCBIND_CONSTANT(fairygui::TransitionActionType::Size));
        bind::constant("Scale", CCBIND_CONSTANT(fairygui::TransitionActionType::Scale));
        bind::constant("Pivot", CCBIND_CONSTANT(fairygui::TransitionActionType::Pivot));
        bind::constant("Alpha", CCBIND_CONSTANT(fairygui::TransitionActionType::Alpha));
        bind::constant("Rotation", CCBIND_CONSTANT(fairygui::TransitionActionType::Rotation));
        bind::constant("Color", CCBIND_CONSTANT(fairygui::TransitionActionType::Color));
        bind::constant("Animation", CCBIND_CONSTANT(fairygui::TransitionActionType::Animation));
        bind::constant("Visible", CCBIND_CONSTANT(fairygui::TransitionActionType::Visible));
        bind::constant("Sound", CCBIND_CONSTANT(fairygui::TransitionActionType::Sound));
        bind::constant("Transition", CCBIND_CONSTANT(fairygui::TransitionActionType::Transition));
        bind::constant("Shake", CCBIND_CONSTANT(fairygui::TransitionActionType::Shake));
        bind::constant("ColorFilter", CCBIND_CONSTANT(fairygui::TransitionActionType::ColorFilter));
        bind::constant("Skew", CCBIND_CONSTANT(fairygui::TransitionActionType::Skew));
        bind::constant("Text", CCBIND_CONSTANT(fairygui::TransitionActionType::Text));
        bind::constant("Icon", CCBIND_CONSTANT(fairygui::TransitionActionType::Icon));
        bind::constant("Unknown", CCBIND_CONSTANT(fairygui::TransitionActionType::Unknown));
        bind::end();
    }

    if (bind::begin<fairygui::FillMethod>("fairygui.FillMethod"))
    {
        bind::constant("None", CCBIND_CONSTANT(fairygui::FillMethod::None));
        bind::constant("Horizontal", CCBIND_CONSTANT(fairygui::FillMethod::Horizontal));
        bind::constant("Vertical", CCBIND_CONSTANT(fairygui::FillMethod::Vertical));
        bind::constant("Radial90", CCBIND_CONSTANT(fairygui::FillMethod::Radial90));
        bind::constant("Radial180", CCBIND_CONSTANT(fairygui::FillMethod::Radial180));
        bind::constant("Radial360", CCBIND_CONSTANT(fairygui::FillMethod::Radial360));
        bind::end();
    }

    if (bind::begin<fairygui::FillOrigin>("fairygui.FillOrigin"))
    {
        bind::constant("Top", CCBIND_CONSTANT(fairygui::FillOrigin::Top));
        bind::constant("Bottom", CCBIND_CONSTANT(fairygui::FillOrigin::Bottom));
        bind::constant("Left", CCBIND_CONSTANT(fairygui::FillOrigin::Left));
        bind::constant("Right", CCBIND_CONSTANT(fairygui::FillOrigin::Right));
        bind::end();
    }

    if (bind::begin<fairygui::ObjectPropID>("fairygui.ObjectPropID"))
    {
        bind::constant("Text", CCBIND_CONSTANT(fairygui::ObjectPropID::Text));
        bind::constant("Icon", CCBIND_CONSTANT(fairygui::ObjectPropID::Icon));
        bind::constant("Color", CCBIND_CONSTANT(fairygui::ObjectPropID::Color));
        bind::constant("OutlineColor", CCBIND_CONSTANT(fairygui::ObjectPropID::OutlineColor));
        bind::constant("Playing", CCBIND_CONSTANT(fairygui::ObjectPropID::Playing));
        bind::constant("Frame", CCBIND_CONSTANT(fairygui::ObjectPropID::Frame));
        bind::constant("DeltaTime", CCBIND_CONSTANT(fairygui::ObjectPropID::DeltaTime));
        bind::constant("TimeScale", CCBIND_CONSTANT(fairygui::ObjectPropID::TimeScale));
        bind::constant("FontSize", CCBIND_CONSTANT(fairygui::ObjectPropID::FontSize));
        bind::constant("Selected", CCBIND_CONSTANT(fairygui::ObjectPropID::Selected));
        bind::end();
    }

    if (bind::begin<fairygui::TextFormat>("fairygui.TextFormat"))
    {
        bind::constant("OUTLINE", CCBIND_CONSTANT(fairygui::TextFormat::OUTLINE));
        bind::constant("SHADOW", CCBIND_CONSTANT(fairygui::TextFormat::SHADOW));
        bind::constant("GLOW", CCBIND_CONSTANT(fairygui::TextFormat::GLOW));
        bind::end();
    }

    return 1;
}
