#ifndef __FUILABEL_H__
#define __FUILABEL_H__

#include "cocos2d.h"
#include "FairyGUIMacros.h"
#include "TextFormat.h"

NS_FGUI_BEGIN

class FUILabel : public cocos2d::Label
{
public:
    FUILabel();
    virtual ~FUILabel();

    CREATE_FUNC(FUILabel);

    const std::string& getText() const { return getString(); }
    void setText(const std::string& value);

    TextFormat* getTextFormat() const { return _textFormat; }
    void applyTextFormat();

    // NOTE: cocos2d-x doesn't support setting seperate color to underline,
    // and I really think what richtext need is to set text color as a whole for links
    // void setUnderlineColor(const cocos2d::Color3B& value);

    virtual bool setBMFontFilePath(const std::string& bmfontFilePath, const cocos2d::Vec2& imageOffset = cocos2d::Vec2::ZERO, float fontSize = 0) override;

    void setGrayed(bool value);

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
protected:
    /* 2d/CCLabel.h add `virtual` defore `void updateBMFontScale()` */
    virtual void updateBMFontScale() override;
    virtual void updateColor() override;
    virtual void updateShaderProgram() override;

    void onDraw(const cocos2d::Mat4& transform, bool transformUpdated);
private:
    void drawQuads(GLint type, const cocos2d::Color4F& color);
    TextFormat* _textFormat;
    std::string _fontName;
    int _fontSize;
    bool _bmFontCanTint;
    bool _grayed;
};

NS_FGUI_END

#endif
