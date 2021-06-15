#pragma once
#include "includes.h"

class Item
{
public:

    Item(const char* label, int type, int level) {
        this->label = label;
        this->min = 0;
        this->max = 1;
        this->value = 0;
        this->type = type;
        this->level = level;
    }

    Item(const char* label, int min, int max, int level) {
        this->label = label;
        this->min = min;
        this->max = max;
        this->value = min;
        this->type = 1;
        this->level = level;
    }

    void Display(char prefix) {

        if (type == 0)
            printf("%c %s %*s%*s\t[%s]\n", prefix, GetLevelPrefix(), 0, label, 25 - strlen(label), "", value ? "ON" : "OFF");

        if (type == 1)
            printf("%c %s %*s%*s\t<%03d>\n", prefix, GetLevelPrefix(), 0, label, 25 - strlen(label), "", value);

    }

    void Update(int val = 1) {

        if (type == 0)
            value = value ? 0 : 1;

        if (type == 1)
            if (value + val >= min && value + val <= max)
                value += val;

        if (type == 2)
            value = value ? 0 : 1;

    }

public:
    const char* label;
    int value;
    int type;
    int min, max;
    int level;

private:
    const char* GetLevelPrefix() {
        switch (level)
        {
        case 0:
            return "+";

        case 1:
            return "|--";

        default:
            break;
        }
    }

};

class Menu
{
public:
    Menu() {}

    Menu(const char* prompt) {
        this->prompt = prompt;
    }

    void AddSlideToggle(const char* label, int level) {
        items.emplace_back(label, 0, level);
    }

    void AddSlider(const char* label, int min, int max, int level) {
        items.emplace_back(label, min, max, level);
    }

    int GetIntemValue(const char* itemName) {
        for (int i = 0; i < items.size(); i++) {

            if (strcmp(itemName, items[i].label) == 0)
                return items[i].value;

        }
        return 0;
    }

private:
    const char* prompt;
    std::vector<Item> items;
    KEY_EVENT_RECORD key;
    int currentOption = 0;

private:
    void Render() {

        std::printf("%s\n", prompt);

        for (int i = 0; i < items.size(); i++)
            items.at(i).Display((currentOption == i) ? '>' : ' ');

    }

    void Update() {
        system("cls");
        Render();
    }

public:
    void Run() {

        Render();

        while (!GetAsyncKeyState(VK_HOME)) {

            if (GetAsyncKeyState(VK_UP)) {
                currentOption += (currentOption - 1 < 0) ? 0 : -1;
                Update();
            }
            if (GetAsyncKeyState(VK_DOWN)) {
                currentOption += (currentOption + 1 > items.size() - 1) ? 0 : 1;
                Update();
            }
            if (GetAsyncKeyState(VK_RIGHT)) {
                items.at(currentOption).Update();
                Update();
            }
            if (GetAsyncKeyState(VK_LEFT)) {
                items.at(currentOption).Update(-1);
                Update();
            }

            Sleep(100);

        }
    }
};