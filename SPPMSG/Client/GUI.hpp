#pragma once

#include "config.hpp"

using namespace sf;

// Some function defs
typedef void (*LoadFunc)();
typedef void (*CloseFunc)();
typedef void (*MousePressFunc)(Event::MouseButtonEvent);
typedef void (*TextFunc)(Event::TextEvent);
typedef void (*MouseMoveFunc)(Event::MouseMoveEvent);
typedef void (*ButtonFunc)(int);
typedef void (*DrawFunc)(RenderWindow&);

void updateGui(RenderWindow& wnd);

struct Button
{
    string label;
    Vector2f pos;
    Vector2f size;
    bool enabled;
    bool mouseOver;

    void draw(RenderWindow& wnd);
};
struct TextBox
{
    string text;
    Vector2f pos;
    Vector2f size;
    float caretPos;

    void draw(RenderWindow& wnd);
};

class GUI
{
    // Controls
public:
    vector<Button> buttons;
    vector<TextBox> textBoxes;
    void addButton(Button& button);
    void addTextBox(TextBox textbox);

    static void loadFunc();
    static void closeFunc();
    static void mousePressFunc(Event::MouseButtonEvent embe);
    static void textFunc(Event::TextEvent ete);
    static void mouseMoveFunc(Event::MouseMoveEvent emme);
    static void buttonFunc(int i);
    static void drawFunc(RenderWindow& rw);
    GUI(LoadFunc _1=loadFunc, ButtonFunc _2=closeFunc, DrawFunc _3=drawFunc, TextFunc _4=textFunc, CloseFunc _5=closeFunc, MousePressFunc _6=mousePressFunc, MouseMoveFunc _7=mouseMoveFunc);

    LoadFunc onLoad;
    ButtonFunc onButtonClicked;
    DrawFunc onDraw;
    TextFunc onTextEvent;
    CloseFunc onClose;
    MousePressFunc onMousePress;
    MouseMoveFunc onMouseMove;
};
