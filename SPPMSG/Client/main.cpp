#include "config.hpp"

using namespace sf;
using namespace std;

bool running = true;
string logstr;
string chatString;
TcpSocket sck;
vector<string> issuedCommands;
int cmdIndex = -1;

void send()
{
    if(chatString == "\\\\")
    {
        running = false;
        sck.disconnect();
    }
    else
    {
        Socket::Status status1 = sck.send(chatString.data(), chatString.size());
        if(status1 == Socket::Disconnected)
            logstr += "Server closed!\n";
        else if(status1 == Socket::Error)
            logstr += "Unexpected error while sending data!\n";
    }
}

void networkLoop()
{
    while(running)
    {
        char packet2[1024];
        size_t x;

        if(sck.receive(&packet2, 1024, x) == Socket::Done)
        {
            cout << x << endl;
            string text;
            logstr += ((char*)packet2);
        }
        else
        {
            cout << "Error" << endl;
        }
    }
}

void loop()
{
    RenderWindow wnd(VideoMode(500, 500), "Pomemeon Client");
    wnd.setFramerateLimit(60);
    Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    View wndView(FloatRect(0.f, 0.f, 500.f, 500.f));
    wnd.setView(wndView);
    wnd.setKeyRepeatEnabled(true);
    View guiView(FloatRect(0.f, 0.f, 500.f, 500.f));

    while(running && wnd.isOpen())
    {
        Event e;
        while(wnd.pollEvent(e))
        {
            if(e.type == Event::Closed)
                running = false;
            if(e.type == Event::Resized)
            {
                wndView.reset(FloatRect(0.f, 0.f, e.size.width, e.size.height));
                guiView.reset(FloatRect(0.f, 0.f, e.size.width, e.size.height));
            }

            if(e.type == Event::KeyPressed && e.key.code == Keyboard::Return)
            {
                send();
                issuedCommands.push_back(chatString);
                chatString = "";
            }
            else if(e.type == Event::TextEntered)
            {
                if(e.text.unicode == '\b')
                    chatString.assign(chatString.substr(0,chatString.size() - 1));
                else if(e.text.unicode > 0x1F)
                    chatString.append({char(e.text.unicode & 0xFF)});

                cmdIndex = -1;
            }
            if(e.type == Event::KeyPressed && e.key.code == Keyboard::Up)
            {
                if(!issuedCommands.empty())
                {
                    if(cmdIndex == -1)
                    {
                        cmdIndex = issuedCommands.size()-1;
                        chatString = issuedCommands[cmdIndex];
                    }
                    else
                    {
                        if(cmdIndex != 0)
                            cmdIndex--;

                        chatString = issuedCommands[cmdIndex];
                    }
                }
            }
            if(e.type == Event::KeyPressed && e.key.code == Keyboard::Down)
            {
                if(!issuedCommands.empty())
                {
                    if(cmdIndex < issuedCommands.size()-1)
                    {
                        cmdIndex++;
                        chatString = issuedCommands[cmdIndex];
                    }
                    else
                    {
                        cmdIndex = -1;
                        chatString = "";
                    }
                }
            }
        }

        wndView = View(FloatRect(0.f, 0.f, wnd.getSize().x, wnd.getSize().y));

        wnd.clear(Color::White);
        wnd.setView(wndView);
        Text tx(logstr, font, 16);
        tx.setPosition(2, wnd.getView().getSize().y-tx.getLocalBounds().height-50.f);
        tx.setFillColor(Color::Black);
        wnd.draw(tx);

        wnd.setView(guiView);
        RectangleShape rs(Vector2f(wnd.getView().getSize().x, 50.f));
        rs.setPosition(0.f, wnd.getView().getSize().y-50.f);
        rs.setFillColor(Color(200,200,200));
        wnd.draw(rs);

        Text tx2(chatString, font, 20);
        tx2.setPosition(5, wnd.getView().getSize().y-50.f);
        tx2.setFillColor(Color::Black);
        tx2.setStyle(Text::Underlined);
        wnd.draw(tx2);

        RectangleShape rs2(Vector2f(3.f, 25.f));
        rs2.setPosition(tx2.findCharacterPos(chatString.size()));
        rs2.setFillColor(time(NULL) % 2 == 0 ? Color::Transparent : Color::Black);
        wnd.draw(rs2);

        wnd.display();
    }
}

int main(int argc, char* argv[])
{
    cout << "Starting SPPMsg Client..." << endl;
    string ip;

    cout << "Select IP to connect:" << endl;
    cin >> ip;

    bool attemptConnect = true;
    bool skipConnect = false;

    do
    {
        Socket::Status status;
        if(!skipConnect)
            status = sck.connect(ip, 12346);

        //sck.setBlocking(false);

        if(status != Socket::Done)
        {
            cout << "Error: " << ((status==Socket::Disconnected ? "Disconnected" :
            (status==Socket::Error ? ("Error" + to_string(errno)) : ("Unexpected error: "
            + to_string(status))))) << ". Do you want to try again? (Y/N)" << endl;

            string str;
            cin >> str;

            if(str == "Y")
            {
                skipConnect = false;
                continue;
            }
            else if(str == "N")
            {
                attemptConnect = false;
                running = false;
            }
            else
            {
                cout << "Invalid String!" << endl;
                skipConnect = true;
            }
        }
        else
            attemptConnect = false;
    } while(attemptConnect);

    Thread guiThread(loop);
    guiThread.launch();

    Thread networkThread(networkLoop);
    networkThread.launch();

    guiThread.wait();

    return 0;
}
