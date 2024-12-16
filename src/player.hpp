#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <unordered_map>
#include "animations.hpp"

const float PPM = 30.f;

enum class PlayerState
{
    IDLE,
    RUNNING,
    JUMPING,
    FALLING,
    ATTACKING,
    DEATH
};

class Player
{
    public:
        Player();
        ~Player() {};
        b2Body* playerBody;
        void setHp(float);
        void gainExp(float);
        void takeDmg(float);
        void setIsMoving(bool);
        void setIsJumping(bool);
        void setIsOnGround(bool);
        void setCanBeDamaged(bool);
        void keyboardInput(b2World*);
        void timers(b2World*, float);
        void createPlayer(b2World*, float, float);

        int getLevel() const;

        float getHp();
        float dealDamage();

        b2Body* getBody();
        b2Vec2 getPos() const;
        sf::Vector2f getScale() const;
        sf::Vector2f getActualHp() const;
        PlayerState getPlayerState() const;
        bool getIsAlive() const { return isAlive; };
    private:
        int _level = 1;
        const float _baseExp = 100.f;
        const float _baseHp = 100.f;
        const float _baseDmg = 20.f;
        const float MOVE_SPEED = 10.f;
        const float _levelMultiplier = 1.2f;
        float _totalHp, _totalDmg, _currentHp, _exp, elapsedTime = 0.f, elapsedDamageTime = 0.f, cooldown = 0.38f, damageCooldown = 0.38f;
        bool isMoving, isJumping, isOnGround, isAttacking, canBeDamaged, isAlive;
        b2Vec2 velocity;
        b2Body* sword = nullptr;
        sf::Vector2f spriteScale = {1.5f, 1.5f};
        PlayerState currentState = PlayerState::IDLE;
        void levelUp();
        void switchState(PlayerState);
        void createAttackHitbox(b2World*);
};

class PlayerAnimations
{
    public:
        PlayerAnimations();
        void draw(sf::RenderWindow&);
        void update(float, b2Vec2, sf::Vector2f, PlayerState);
        bool getGameOver() { return gameOver; };
    private:
        const sf::Vector2i CHARACTER_SIZE = {50, 37};
        sf::Sprite sprite;
        sf::Texture texture;
        std::unordered_map<PlayerState, Animation> animations;
        Animation* currentAnimation = nullptr;
        PlayerState currentState = PlayerState::IDLE;
        int currentFrame = 0;
        float elapsedTime = 0.0f;
        void setAnimation(PlayerState);
        bool gameOver = false;
};

#endif // PLAYER_HPP_INCLUDED
