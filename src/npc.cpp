#include "npc.hpp"
#include <iostream>

void Barman::update(float deltaTime)
{
 elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 5;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Barman::render(sf::RenderWindow& window)
{
        window.draw(npcSprite);

}


Barman::Barman(float x, float y)
{

    if (!npcTexture.loadFromFile("Textures/Taverna/tabernero1.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

     animation[0] = {0, 0, 40, 48};
    animation[1] = {40, 0, 40, 48};
    animation[2] = {80, 0, 40, 48};
    animation[3] = {120, 0, 40, 48};
    animation[4] = {160, 0, 40, 48};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);

    currentFrame = 0;
    elapsedTime = 0.f;

}

void Marcianito::update(float deltaTime)
{
    elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 3;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}


void Marcianito::render(sf::RenderWindow& window)
{
    window.draw(npcSprite);
}

Marcianito::Marcianito(float x, float y)
{
    if (!npcTexture.loadFromFile("Textures/Taverna/marci.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 173, 115};
    animation[1] = {173, 0, 173, 115};
    animation[2] = {346, 0, 173, 115};

    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTexture(npcTexture);
    npcSprite.setTextureRect(animation[1]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(0.50f, 0.45f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Dog::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 4;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Dog::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Dog::Dog(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Npcs/dog.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Farmer::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 5;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Farmer::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Farmer::Farmer(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Npcs/farmer_01.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};
    animation[4] = {128, 0, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(1.5f,1.5f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void FarmerGirl::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 5;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void FarmerGirl::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

FarmerGirl::FarmerGirl(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Npcs/farmer_02.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};
    animation[4] = {128, 0, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(1.5f,1.5f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Bear::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 18;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Bear::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Bear::Bear(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Forest/MiniBear1.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};
    animation[4] = {128, 0, 32, 32};
    animation[5] = {160, 0, 32, 32};
    animation[6] = {192, 0, 32, 32};
    animation[7] = {224, 0, 32, 32};
    animation[8] = {256, 0, 32, 32};
    animation[9] = {288, 0, 32, 32};

    animation[10] = {0, 32, 32, 32};
    animation[11] = {32, 32, 32, 32};
    animation[12] = {64, 32, 32, 32};
    animation[13] = {96, 32, 32, 32};
    animation[14] = {128, 32, 32, 32};
    animation[15] = {160, 32, 32, 32};
    animation[16] = {192, 32, 32, 32};
    animation[17] = {224, 32, 32, 32};
    animation[18] = {256, 32, 32, 32};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(2.0f, 2.0f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Wolf::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 20;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Wolf::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Wolf::Wolf(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Forest/MiniWolf1.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};
    animation[4] = {128, 0, 32, 32};
    animation[5] = {160, 0, 32, 32};
    animation[6] = {192, 0, 32, 32};

    animation[7] = {0, 0, 32, 32};
    animation[8] = {32, 0, 32, 32};
    animation[9] = {64, 0, 32, 32};
    animation[10] = {96, 32, 32, 32};
    animation[11] = {128, 32, 32, 32};
    animation[12] = {160, 32, 32, 32};
    animation[13] = {192, 32, 32, 32};

    animation[14] = {0, 64, 32, 32};
    animation[15] = {32, 64, 32, 32};
    animation[16] = {64, 64, 32, 32};
    animation[17] = {96, 64, 32, 32};
    animation[18] = {128, 64, 32, 32};
    animation[19] = {160, 64, 32, 32};




    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(1.7f, 1.7f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Carpincho::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 30;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Carpincho::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Carpincho::Carpincho(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Forest/charlie.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 64, 60};
    animation[1] = {64, 0, 64, 60};
    animation[2] = {128, 0, 64, 60};
    animation[3] = {192, 0, 64, 60};
    animation[4] = {256, 0, 64, 60};
    animation[5] = {320, 0, 64, 60};
    animation[6] = {384, 0, 64, 60};
    animation[7] = {448, 0, 64, 60};
    animation[8] = {512, 0, 64, 60};

    animation[9] = {0, 64, 64, 60};
    animation[10] = {64, 64, 64, 60};
    animation[11] = {128, 64, 64, 60};
    animation[12] = {192, 64, 64, 60};
    animation[13] = {256, 64, 64, 60};
    animation[14] = {320, 64, 64, 60};
    animation[15] = {384, 64, 64, 60};
    animation[16] = {448, 64, 64, 60};
    animation[17] = {512, 64, 64, 60};

    animation[18] = {0, 128, 64, 60};
    animation[19] = {64, 128, 64, 60};
    animation[20] = {128, 128, 64, 60};
    animation[21] = {192, 128, 64, 60};
    animation[22] = {256, 128, 64, 60};
    animation[23] = {320, 128, 64, 60};
    animation[24] = {384, 128, 64, 60};
    animation[25] = {448, 128, 64, 60};
    animation[26] = {512, 128, 64, 60};

    animation[27] = {0, 192, 64, 60};
    animation[28] = {64, 192, 64, 60};
    animation[29] = {128, 192, 64, 60};

    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(0.8f, 0.8f);

    currentFrame = 0;
    elapsedTime = 0.f;
}

void Bunny::update(float deltaTime)
{
     elapsedTime += deltaTime;

    if(elapsedTime >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % 8;
        npcSprite.setTextureRect(animation[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Bunny::render(sf::RenderWindow& window)
{
     window.draw(npcSprite);
}

Bunny::Bunny(float x,float y)
{

    if (!npcTexture.loadFromFile("Textures/Forest/MiniBunny.png")) {
        std::cerr << "No se pudo cargar la textura del NPC Barman" << std::endl;
        return;
    }

    animation[0] = {0, 0, 32, 32};
    animation[1] = {32, 0, 32, 32};
    animation[2] = {64, 0, 32, 32};
    animation[3] = {96, 0, 32, 32};

    animation[4] = {0, 32, 32, 32};
    animation[5] = {32, 32, 32, 32};
    animation[6] = {64, 32, 32, 32};
    animation[7] = {96, 0, 32, 32};



    npcSprite.setTexture(npcTexture);
    npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2.f, npcSprite.getLocalBounds().height / 2.f);;
    npcSprite.setTextureRect(animation[0]);
    npcSprite.setPosition(x,y);
    npcSprite.setScale(1.7f, 1.7f);

    currentFrame = 0;
    elapsedTime = 0.f;
}
