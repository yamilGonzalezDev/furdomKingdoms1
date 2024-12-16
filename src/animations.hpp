#ifndef ANIMATIONS_HPP_INCLUDED
#define ANIMATIONS_HPP_INCLUDED

struct Animation
{
    std::vector<sf::IntRect> frames;
    float frameDuration;

    Animation() {};

    Animation(std::vector<sf::IntRect> frames, float duration)  : frames(frames), frameDuration(duration){};
};

#endif // ANIMATIONS_HPP_INCLUDED
