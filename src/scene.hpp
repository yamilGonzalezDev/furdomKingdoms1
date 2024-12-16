#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include "observer.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "witch.hpp"
#include "npc.hpp"

class Scene : public Observer
{
    protected:
        const float WIDTH = 1366;
        const float HEIGHT = 768;
        float volume;
        bool changeVol;
        PlayerAnimations playerAnimations;
        SceneState nextScene;
    public:
        virtual ~Scene() = default;

        virtual void render(sf::RenderWindow&) = 0;

        virtual void update(sf::RenderWindow&, float) = 0;

        virtual void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) {};

        virtual void setEnemySprites(const std::vector<Enemy*>& enemiesReferences) {};

        virtual bool closeGame() { return false; };

        virtual bool gameOverBool() { return false; };

        virtual bool shouldTransition() { return false; };

        virtual SceneState nextSceneState() const = 0;

        virtual SceneState getCurrentScene() const = 0;
};

class MenuScene : public Scene
{
    public:
        MenuScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;

        bool loadFile();

        sf::Texture scrollText;
        sf::Sprite scroll;

        SceneState getCurrentScene() const override { return SceneState::MENU; };
    private:
        sf::Font font;
        sf::Text menuOptions[4];
        int currentOption = 0;
        sf::Music music;
        bool mainMenu, settingsMenu, closeGame;
};

class HouseScene : public Scene
{
    public:
        HouseScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;
        sf::Texture candleText, tableText, houseText, bedText, layerTexture1, layerTexture2, layerTexture3, layerTexture4, layerTexture5;
        sf::Sprite candle, table, house, bed, layer1, layer2, layer3, layer4, layer5;
        void startCinematic(bool);

        SceneState getCurrentScene() const override { return SceneState::HOUSE; };
    private:
        float parallaxFactor5 = .04f;
        float parallaxFactor4 = .08f;
        float parallaxFactor3 = .032f;
        float parallaxFactor2 = .018f;
        float parallaxFactor1 = .020f;
        bool transition = false;
};

class CityScene : public Scene
{
    public:
        CityScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture, cartelTexture, barrilTexture, farolasTexture, cajasTexture, carretaTexture, fozaTexture;
        sf::Sprite background, cartel, barril, barril2, farolas, farolas2, cajas, carreta, foza;

        std::vector<std::unique_ptr<Npc>> npcs;

        SceneState getCurrentScene() const override { return SceneState::CITY; };
    private:
        bool transition = false;
        bool sensorActive = false;

};

class ForestScene : public Scene
{
    public:
        ForestScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture, plantaTexture, fondoTexture, layer1Texture, layer2Texture, layer3Texture;
        sf::Sprite background, planta, layer1, layer2, layer3, fondo;

        float parallaxFactor3 = .032f;
        float parallaxFactor2 = .018f;
        float parallaxFactor1 = .020f;

        std::vector<std::unique_ptr<Npc>> npcs;
        void setEnemySprites(const std::vector<Enemy*>&) override;
        bool gameOverBool() override { return playerAnimations.getGameOver(); };

        SceneState getCurrentScene() const override { return SceneState::FOREST; };
    private:
        std::vector<Enemy*> enemyReferences;
        bool sensorActive = false;
        bool transition = false;
        sf::Music music;
};

class DungeonScene : public Scene
{
   public:
        DungeonScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool gameOverBool() override { return playerAnimations.getGameOver(); };
        bool shouldTransition() override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
        sf::Texture layerTexture;
        sf::Sprite layer;
        float parallaxFactor1 = .020f;
        void setEnemySprites(const std::vector<Enemy*>&) override;
        std::vector<std::unique_ptr<Npc>> npcs;

        SceneState getCurrentScene() const override { return SceneState::DUNGEON; };
    private:
        std::vector<Enemy*> enemyReferences;
        bool transition = false;
        bool sensorActive = false;
};

class CastleScene : public Scene
{
    public:
        CastleScene();

        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
        std::vector<std::unique_ptr<Npc>> npcs;

        SceneState getCurrentScene() const override { return SceneState::CASTLE; };
        private:
        bool transition = false;

};

class BarScene : public Scene
{
    public:
        BarScene();
        void sensorNotify(bool,SceneState)override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;
        sf::Texture barTexture, pisoTexture, mesaTexture, copaTexture, nubesTexture, arcadeTexture, castillosTexture;
        sf::Sprite bar, copa, piso, mesa, nubes, arcade, castillos;
        sf::Music music;
        std::vector<std::unique_ptr<Npc>> npcs;

        SceneState getCurrentScene() const override { return SceneState::BAR; };
    private:
        bool transition = false;
        bool sensorActive = false;
};

class TestScene : public Scene
{
    public:
        TestScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        void setEnemySprites(const std::vector<Enemy*>&) override;
        bool gameOverBool() override;
        bool shouldTransition() override;
        SceneState nextSceneState() const override;

        SceneState getCurrentScene() const override { return SceneState::TEST; };
    private:
        std::vector<Enemy*> enemyReferences;
        Witch theWitch = Witch(700.f, 700.f);
};

#endif // SCENE_HPP_INCLUDED
