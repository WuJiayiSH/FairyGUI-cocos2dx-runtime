#include "FUILabel.h"
#include "BitmapFont.h"
#include "UIConfig.h"
#include "UIPackage.h"

NS_FGUI_BEGIN
USING_NS_CC;

namespace
{
    inline Color3B toGrayed(const Color3B& source)
    {
        Color3B c = source;
        c.r = c.g = c.b = c.r * 0.299f + c.g * 0.587f + c.b * 0.114f;
        return c;
    }

    Texture2D* _getTexture(Label* label)
    {
        Texture2D* texture = nullptr;

        if (FontAtlas* fontAtlas = label->getFontAtlas())
        {
            auto textures = fontAtlas->getTextures();
            if (!textures.empty())
            {
                texture = textures.begin()->second;
            }
        }

        return texture;
    }

    GLProgram* labelOutlineProgram = nullptr;
    const char* labelOutlineFrag = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 u_effectColor;

#ifdef GL_ES
uniform lowp int u_effectType; // 0: None (Draw text), 1: Outline, 2: Shadow
#else
uniform int u_effectType;
#endif

void main()
{
    vec4 sample = texture2D(CC_Texture0, v_texCoord);
    // fontAlpha == 1 means the area of solid text (without edge)
    // fontAlpha == 0 means the area outside text, including outline area
    // fontAlpha == (0, 1) means the edge of text
    float fontAlpha = sample.a;

    // outlineAlpha == 1 means the area of 'solid text' and 'solid outline'
    // outlineAlpha == 0 means the transparent area outside text and outline
    // outlineAlpha == (0, 1) means the edge of outline
    float outlineAlpha = sample.r;

    if (u_effectType == 0) // draw text
    {
        gl_FragColor = v_fragmentColor * vec4(u_effectColor.rgb, u_effectColor.a * fontAlpha);
    }
    else if (u_effectType == 1) // draw outline
    {
        // multipy (1.0 - fontAlpha) to make the inner edge of outline smoother and make the text itself transparent.
        gl_FragColor = vec4(u_effectColor.rgb, v_fragmentColor.a * u_effectColor.a * outlineAlpha * (1.0 - fontAlpha));
    }
    else // draw shadow
    {
        gl_FragColor = vec4(u_effectColor.rgb, v_fragmentColor.a * u_effectColor.a * fontAlpha);
    }
}
)";

}

FUILabel::FUILabel() : _fontSize(-1),
_bmFontCanTint(false),
_textFormat(new TextFormat()),
_grayed(false)
{
}

FUILabel::~FUILabel()
{
    delete _textFormat;
}

void FUILabel::setText(const std::string& value)
{
    if (_fontSize < 0)
        applyTextFormat();

    setString(value);
}

void FUILabel::applyTextFormat()
{
    if (_fontSize < 0 /**first time**/ || _fontName != _textFormat->face)
    {
        _fontName = _textFormat->face;
        Label::LabelType oldType = _currentLabelType;

        if (_fontName.find("ui://") != -1)
        {
            setBMFontFilePath(_fontName);
        }
        else
        {
            bool ttf = false;
            const std::string& fontName = UIConfig::getRealFontName(_fontName, &ttf);
            if (ttf)
            {
                _fontConfig.fontFilePath = fontName;
                _fontConfig.fontSize = _textFormat->fontSize;
                setTTFConfig(_fontConfig);
            }
            else
            {
                setSystemFontName(fontName);
            }

            if (oldType == LabelType::BMFONT)
                setTextColor((Color4B)_textFormat->color);
        }
    }

    if (_fontSize != _textFormat->fontSize)
    {
        _fontSize = _textFormat->fontSize;
        if (_currentLabelType == LabelType::STRING_TEXTURE)
        {
            setSystemFontSize(_fontSize);
        }
        else if (_currentLabelType == LabelType::BMFONT)
        {
            setBMFontSize(_fontSize);
        }
        else
        {
            _fontConfig.fontSize = _fontSize;
            setTTFConfig(_fontConfig);
        }
    }

    // See: https://github.com/fairygui/FairyGUI-cocos2dx/issues/29
    if (_currentLabelType != LabelType::BMFONT)
        setTextColor((Color4B)_textFormat->color);
    else if (_bmFontCanTint)
        setColor(_textFormat->color);

    if (_textFormat->underline)
        enableUnderline();
    else
        disableEffect(LabelEffect::UNDERLINE);

    if (_textFormat->italics)
        enableItalics();
    //else //Cant call this, cocos will do setRotationSkew(0)!
    //    disableEffect(LabelEffect::ITALICS);

    if (_textFormat->bold && _currentLabelType != LabelType::STRING_TEXTURE)
        enableBold();
    else
        disableEffect(LabelEffect::BOLD);

    setLineSpacing(_textFormat->lineSpacing);
    setHorizontalAlignment(_textFormat->align);
    setVerticalAlignment(_textFormat->verticalAlign);

    if (_textFormat->hasEffect(TextFormat::OUTLINE))
        enableOutline((Color4B)(_grayed ? toGrayed(_textFormat->outlineColor) : _textFormat->outlineColor), _textFormat->outlineSize);
    else
        disableEffect(LabelEffect::OUTLINE);

    if (_textFormat->hasEffect(TextFormat::SHADOW))
        enableShadow((Color4B)(_grayed ? toGrayed(_textFormat->shadowColor) : _textFormat->shadowColor), _textFormat->shadowOffset);
    else if (!_textFormat->bold)
        disableEffect(LabelEffect::SHADOW);
}

bool FUILabel::setBMFontFilePath(const std::string& bmfontFilePath, const Vec2& imageOffset, float fontSize)
{
    BitmapFont* bmFont = (BitmapFont*)UIPackage::getItemAssetByURL(bmfontFilePath, PackageItemType::FONT);
    if (bmFont == nullptr)
    {
        reset();
        return false;
    }

    //assign the default fontSize
    if (std::abs(fontSize) < FLT_EPSILON)
    {
        float originalFontSize = bmFont->getOriginalFontSize();
        _bmFontSize = originalFontSize / CC_CONTENT_SCALE_FACTOR();
    }

    if (fontSize > 0.0f && bmFont->isResizable())
    {
        _bmFontSize = fontSize;
    }

    _bmFontPath = bmfontFilePath;
    _bmFontCanTint = bmFont->canTint();

    _currentLabelType = LabelType::BMFONT;
    setFontAtlas(bmFont->createFontAtlas());

    return true;
}

bool FUILabel::setBMFontFilePath(const std::string& bmfontFilePath, const Rect& imageRect, bool imageRotated, float fontSize)
{
    if (!setBMFontFilePath(bmfontFilePath, imageRect.origin, fontSize))
        return false;

    _bmRect = imageRect;
    _bmRotated = imageRotated;

    return true;
}

void FUILabel::setGrayed(bool value)
{
    if (_grayed != value)
    {
        _grayed = value;

        if (_currentLabelType != LabelType::BMFONT)
            setTextColor((Color4B)(_grayed ? toGrayed(_textFormat->color) : _textFormat->color));
        else if (_bmFontCanTint)
            setColor(_grayed ? toGrayed(_textFormat->color) : _textFormat->color);

        if (_textFormat->hasEffect(TextFormat::OUTLINE))
            enableOutline((Color4B)(_grayed ? toGrayed(_textFormat->outlineColor) : _textFormat->outlineColor), _textFormat->outlineSize);

        if (_textFormat->hasEffect(TextFormat::SHADOW))
            enableShadow((Color4B)(_grayed ? toGrayed(_textFormat->shadowColor) : _textFormat->shadowColor), _textFormat->shadowOffset);
    }
}

void FUILabel::updateBMFontScale()
{
    auto font = _fontAtlas->getFont();
    if (_currentLabelType == LabelType::BMFONT)
    {
        BitmapFont* bmFont = (BitmapFont*)font;
        float originalFontSize = bmFont->getOriginalFontSize();
        _bmfontScale = _bmFontSize * CC_CONTENT_SCALE_FACTOR() / originalFontSize;
    }
    else
    {
        _bmfontScale = 1.0f;
    }
}

void FUILabel::updateColor()
{
    if (_batchNodes.empty())
		return;

    if (_textFormat->hasEffect(TextFormat::GRADIENT))
    {
        Color4B color4bl(_textFormat->gradientColor[1].r, _textFormat->gradientColor[1].g, _textFormat->gradientColor[1].b, _displayedOpacity);
        Color4B color4br(_textFormat->gradientColor[3].r, _textFormat->gradientColor[3].g, _textFormat->gradientColor[3].b, _displayedOpacity);
        Color4B color4tl(_textFormat->gradientColor[0].r, _textFormat->gradientColor[0].g, _textFormat->gradientColor[0].b, _displayedOpacity);
        Color4B color4tr(_textFormat->gradientColor[2].r, _textFormat->gradientColor[2].g, _textFormat->gradientColor[2].b, _displayedOpacity);

        if (_isOpacityModifyRGB)
        {
            color4bl.r *= _displayedOpacity / 255.0f;
            color4bl.g *= _displayedOpacity / 255.0f;
            color4bl.b *= _displayedOpacity / 255.0f;

            color4br.r *= _displayedOpacity / 255.0f;
            color4br.g *= _displayedOpacity / 255.0f;
            color4br.b *= _displayedOpacity / 255.0f;

            color4tl.r *= _displayedOpacity / 255.0f;
            color4tl.g *= _displayedOpacity / 255.0f;
            color4tl.b *= _displayedOpacity / 255.0f;

            color4tr.r *= _displayedOpacity / 255.0f;
            color4tr.g *= _displayedOpacity / 255.0f;
            color4tr.b *= _displayedOpacity / 255.0f;
        }

        for (auto&& batchNode:_batchNodes)
        {
            TextureAtlas* textureAtlas = batchNode->getTextureAtlas();
            V3F_C4B_T2F_Quad* quads = textureAtlas->getQuads();
            ssize_t totalQuads = textureAtlas->getTotalQuads();

            for (int i = 0; i < totalQuads; i++)
            {
                quads[i].bl.colors = color4bl;
                quads[i].br.colors = color4br;
                quads[i].tl.colors = color4tl;
                quads[i].tr.colors = color4tr;
                textureAtlas->updateQuad(&quads[i], i);
            }
        }

        return;
    }
    
    Label::updateColor();
}

void FUILabel::updateShaderProgram()
{
    if (_currentLabelType == LabelType::TTF && (_textFormat->hasEffect(TextFormat::OUTLINE | TextFormat::SHADOW)))
    {
        if (!labelOutlineProgram)
            labelOutlineProgram = GLProgram::createWithByteArrays(ccLabel_vert, labelOutlineFrag);
        setGLProgramState(GLProgramState::getOrCreateWithGLProgram(labelOutlineProgram));

        _uniformEffectColor = glGetUniformLocation(getGLProgram()->getProgram(), "u_effectColor");
        _uniformEffectType = glGetUniformLocation(getGLProgram()->getProgram(), "u_effectType");

		return;
    }

    Label::updateShaderProgram();
}


void FUILabel::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (_batchNodes.empty() || _lengthOfString <= 0)
		return;

    if (_currentLabelType == LabelType::TTF && (_textFormat->hasEffect(TextFormat::OUTLINE | TextFormat::SHADOW)))
    {
        _customCommand.init(_globalZOrder, transform, flags);
        _customCommand.func = CC_CALLBACK_0(FUILabel::onDraw, this, transform, flags & FLAGS_TRANSFORM_DIRTY);
        renderer->addCommand(&_customCommand);
        return;
    }
    
    Label::draw(renderer, transform, flags);
}

void FUILabel::onDraw(const Mat4& transform, bool /*transformUpdated*/)
{
	GLProgram* program = getGLProgram();
    program->use();
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_shadowEnabled)
    {
        program->setUniformsForBuiltins(_shadowTransform);
        for (auto&& it : _letters)
			it.second->updateTransform();

        drawQuads(2, _boldEnabled ? _textColorF : _shadowColor4F);
    }

    program->setUniformsForBuiltins(transform);
    for (auto&& it : _letters)
		it.second->updateTransform();

    // draw outline of text
    if (_currLabelEffect == LabelEffect::OUTLINE)
		drawQuads(1, _effectColorF);

    // draw text without outline
    drawQuads(0, _textColorF);
}

void FUILabel::drawQuads(GLint type, const cocos2d::Color4F& color)
{
	GLProgram* program = getGLProgram();
    program->setUniformLocationWith1i(_uniformEffectType, type);
    program->setUniformLocationWith4f(_uniformEffectColor, color.r, color.g, color.b, color.a);

    for (auto&& batchNode : _batchNodes)
    {
        batchNode->getTextureAtlas()->drawQuads();
    }
}

// void FUILabel::setUnderlineColor(const cocos2d::Color3B& value)
// {
//     if (_textFormat->underline) {
//         enableUnderline(cocos2d::Color4B(value));
//     }
// }

NS_FGUI_END
