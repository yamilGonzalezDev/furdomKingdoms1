#ifndef DIRECTOR_HPP_INCLUDED
#define DIRECTOR_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <memory>
#include "player.hpp"
#include "enemy.hpp"
#include "scene.hpp"
#include "limits.hpp"
#include "fooDraw.hpp"
#include "colisions.hpp"
#include "sceneState.hpp"

enum class TransitionState
{
    NONE,
    FADINGOUT,
    LOADING,
    FADINGIN,
    GAMEOVER,
    PAUSE
};

class Director
{
    public:
        Director();
        ~Director();
        void run();
    private:
        const float WIDTH;
        const float HEIGHT;
        const float timeStep = 1.f/60.f;
        const float FLOORHEIGHT = 528.f;
        const float speed = 200.f;
        float transitionTime = 0.f, maxTransitionTime, cooldownElapsed;
        const int velocityIterations = 6;
        const int positionIterations = 2;
        int alpha = 255;
        bool drawEnemies, fadingIn, fadingOut, drawPlayer, changeScene, transitioning, gameOver;
        sf::RenderWindow window;
        sf::View view;
        sf::Text text, playerLevel, saveGame;
        sf::Font font, uiFont;
        sf::Clock clock;
        sf::Music music;
        sf::RectangleShape fadeRectangle;
        sf::Vector2f screenTopLeft;
        FooDraw fooDrawInstance;
        SceneState nextScene;
        TransitionState transitionState;

        sf::Texture containerText, hpBarText, heartText, hpContainerText;
        sf::Sprite container, hpBar, heart, hpContainer;

        b2World* world = nullptr;

        std::vector<Enemy*> enemies;
        std::unique_ptr<BoundsFactory> boundFactory;
        std::unique_ptr<EnemiesFactory> enemyFactory;
        Bounds* sensor = nullptr;
        Enemy* enemy = nullptr;
        Player* player = nullptr;
        Scene* currentScene = nullptr;
        Colision* colisionCheck = nullptr;

        void update(float);
        void updateScene(float);
        void gameEvents();
        void render();

        void initUI();
        void fadeIn(float);
        void fadeOut(float);
        void saveProgress();
        void initBarScene();
        void initCityScene();
        void initMenuScene();
        void initTestScene();
        void initHouseScene();
        void initWorld(float);
        void setScene(Scene*);
        void initCastleScene();
        void initForestScene();
        void initDungeonScene();
        void cleanScene(b2World*);
        void newSong(std::string);
        void loadFile();

        b2Vec2 getMousePositionInWorld(const sf::RenderWindow&);
};

#endif // DIRECTOR_HPP_INCLUDED
