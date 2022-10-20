#include "TextObj.h"

TextObj::TextObj()
{
}

TextObj::~TextObj()
{
}

void TextObj::Draw(RenderWindow& window)
{
	window.draw(text);
}

void TextObj::SetFont(Font& font)
{
	text.setFont(font);
}

void TextObj::SetText(string str)
{
	text.setString(str);
}

void TextObj::SetPos(const Vector2f& pos)
{
	text.setPosition(pos);
}

const string& TextObj::GetText() const
{
	return text.getString();
}
