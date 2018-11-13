#include "GUI.hpp"

void GUI::loadFunc()
{

}
void GUI::closeFunc()
{

}
void GUI::mousePressFunc(Event::MouseButtonEvent embe)
{

}
void GUI::textFunc(Event::TextEvent ete)
{

}
void GUI::mouseMoveFunc(Event::MouseMoveEvent emme)
{

}
void GUI::buttonFunc(int i)
{

}
void GUI::drawFunc(RenderWindow& rw)
{

}
void GUI::addButton(Button& button)
{

}
void GUI::addTextBox(TextBox textbox)
{

}
void updateGui(RenderWindow& wnd)
{

}
GUI::GUI(LoadFunc _1, ButtonFunc _2, DrawFunc _3, TextFunc _4, CloseFunc _5, MousePressFunc _6, MouseMoveFunc _7)
: onLoad(_1), onButtonClicked(_2), onDraw(_3), onTextEvent(_4), onClose(_5), onMousePress(_6), onMouseMove(_7)
{

}
